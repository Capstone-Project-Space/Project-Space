#include "filestorage.h"

#include <assert.h>
#include <memory.h>

#ifdef WIN32
#include <Windows.h>
#include <direct.h>
#endif

#include <src/engine/data_storage/buffer_utils.h>

std::vector<FileStorage*> FileStorage::storages;

FileStorage::FileStorage(const std::string& filename) : filename(filename) {
	storages.push_back(this);
}

size_t FileStorage::getCurrentSize() const {
	uint32_t size = 0;
	for (const auto& saveable : this->saveables) {
	   size += BufferUtils::GetStringSize(saveable->getIdentifier()) + sizeof(size_t) + saveable->getSize();
	}
	for (const auto& un : unloaded) {
	   size += BufferUtils::GetStringSize(un.identifier) + sizeof(size_t) + un.size;
	}
	return size;
}

void LogWin32Error() {
	uint32_t err = GetLastError();
	if (err == 0) {
	   fprintf(stderr, "Unknown Error.\n");
	} else {
	   LPSTR messageBuffer = NULL;
	   size_t size = FormatMessageA(
		  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		  NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &messageBuffer, 0, NULL
	   );
	   fprintf(stderr, "ERROR: %s", messageBuffer);
	   LocalFree(messageBuffer);
	}
}

int FileStorage::save(const std::string& filepath) const {
	HANDLE file = CreateFile((filepath + filename).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
	   LogWin32Error();
	   return -1;
	}

	size_t size = getCurrentSize();
	HANDLE mapping = CreateFileMapping(file, NULL, PAGE_READWRITE, (size & 0xFFFFFFFF00000000) >> 32, size & 0xFFFFFFFF, NULL);
	if (mapping == 0) {
	   LogWin32Error();
	   return -2;
	}
	void* start = MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (start == NULL) {
		LogWin32Error();
		return -3;
	}
	void* data = start;
	for (const auto& ptr : saveables) {
	   Saveable& saveable = *ptr;
	   BufferUtils::WriteString(&data, saveable.getIdentifier());
	   BufferUtils::WriteULong(&data, saveable.getSize());
	   saveable.save(data);
	   data = ((char*) data) + saveable.getSize();
	}
	for (const auto& un : unloaded) {
	   BufferUtils::WriteString(&data, un.identifier);
	   BufferUtils::WriteULong(&data, un.size);
	   memcpy(data, un.data, un.size);
	   data = ((char*)data) + un.size;
	}
	assert(start == (((char*)data) - size));
	FlushViewOfFile(start, size);
	UnmapViewOfFile(start);
	CloseHandle(mapping);
	CloseHandle(file);
	return 0;
}

int FileStorage::load(const std::string& filepath) {
	HANDLE file = CreateFile((filepath + filename).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
	   LogWin32Error();
	   return -1;
	}
	unsigned long highOrder;
	size_t size = GetFileSize(file, &highOrder);
	
	HANDLE mapping = CreateFileMapping(file, NULL, PAGE_READONLY, highOrder, size, NULL);
	if (mapping == 0) {
		LogWin32Error();
		return -2;
	}
	size &= ((size_t)highOrder) << 32;
	const void* start = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, size);
	if (start == NULL) {
		LogWin32Error();
		return -3;
	}
	const void* data = start;
	const void* end = ((char*) start) + size;
	unloaded.clear();
		
	while (data != end) {
		std::string identifier = BufferUtils::ReadString(&data);
		size_t size = BufferUtils::ReadULong(&data);
		const auto saveable = std::find_if(
			saveables.cbegin(), saveables.cend(),
			[identifier](std::shared_ptr<Saveable> s) {return s->getIdentifier() == identifier; }
		);
		if (saveable != saveables.cend()) {
			(*saveable)->load(data, size);
		} else {
			unloaded.emplace_back(identifier, data, size);
		}
		data = ((char*) data) + size;
	}
	FlushViewOfFile(start, size);
	UnmapViewOfFile(start);
	CloseHandle(mapping);
	CloseHandle(file);
	return 0;
}

void FileStorage::addSaveable(std::shared_ptr<Saveable> saveable) {
	auto it = unloaded.begin();
	for (; it != unloaded.end(); it++) {
		if (it->identifier == (*saveable).getIdentifier()) {
			(*saveable).load(it->data, it->size);
			unloaded.erase(it);
			break;
		}
	}
	saveables.push_back(saveable);
}

void FileStorage::removeSaveable(const std::string& identifier) {
	auto saveable = saveables.begin();
	for (; saveable != saveables.end(); saveable++) {
		if ((**saveable).getIdentifier() == identifier) {
			saveables.erase(saveable);
			return;
		}
	}
	auto unload = unloaded.begin();
	for (; unload != unloaded.end(); unload++) {
		if (unload->identifier == identifier) {
			unloaded.erase(unload);
			return;
		}
	}
}

void FileStorage::removeSaveable(std::shared_ptr<Saveable> saveable) {
	removeSaveable((*saveable).getIdentifier());
}


FileStorage::Unloaded::Unloaded(const std::string& identifier, const void* data, size_t size) : identifier(identifier), size(size) {
	void* newData = operator new[](size);
	std::memcpy(newData, data, size);
	this->data = newData;
}

FileStorage::Unloaded::~Unloaded() {
	delete[] this->data;
}

void DataStorage::SaveAll(const std::string& filepath) {
	for (auto ptr : FileStorage::storages) {
		ptr->save(filepath);
	}
}

void DataStorage::LoadAll(const std::string& filepath) {
	for (auto ptr : FileStorage::storages) {
		ptr->load(filepath);
	}
}

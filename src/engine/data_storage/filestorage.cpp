#include "filestorage.h"

#include <assert.h>
#include <memory.h>

#ifdef WIN32
#include <direct.h>
#endif

std::vector<FileStorage*> FileStorage::storages;

FileStorage::FileStorage(const std::string& filename) : filename(filename) {
	storages.push_back(this);
}

int FileStorage::save(const std::string& filepath) const {
	FILE* fp;
	std::string file = filepath + filename;
	int err = _mkdir(file.substr(0, file.find_last_of('/') + 1).c_str());
	if (err && errno != EEXIST) {
		perror("Unable to create directory for saving.");
		fprintf(stderr, "filepath: %s  %d\n", file.c_str(), err);
		return err;
	}
	err = fopen_s(&fp, file.c_str(), "wb");
	if (!fp) {
		perror("Unable to open file for saving.");
		fprintf(stderr, "filename: %s\n", file.c_str());
		return err;
	}

	size_t size;
	void* buffer = NULL;
	for (auto& ptr : saveables) {
		Saveable& saveable = *ptr;
		size = saveable.getIdentifier().size();
		fwrite(&size, sizeof(size_t), 1, fp);
		fwrite(saveable.getIdentifier().c_str(), 1, size, fp);

		// Write the saveables save data.
		size = saveable.getSize();
		void* temp = realloc(buffer, size);
		assert(temp && "Out of Memory.");
		buffer = temp;
		saveable.save(buffer);
		fwrite(&size, sizeof(size_t), 1, fp);
		fwrite(buffer, 1, size, fp);
	}
	// Write all of the unloaded data as well.
	for (auto& unload : unloaded) {
		size_t length = unload.identifier.size();
		fwrite(&length, sizeof(size_t), 1, fp);
		fwrite(unload.identifier.c_str(), 1, length, fp);
		fwrite(&unload.size, sizeof(size_t), 1, fp);
		fwrite(unload.data, 1, unload.size, fp);
	}
	fclose(fp);
	if (buffer) free(buffer);
	return 0;
}

int FileStorage::load(const std::string& filepath) {
	FILE* fp;
	errno_t err = fopen_s(&fp, (filepath + this->filename).c_str(), "rb");
	if (!fp) {
		perror("Unable to open file for loading.");
		fprintf(stderr, "filename: %s\n", (filepath + filename).c_str());
		return err;
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unloaded.clear();

	size_t size;
	void* buffer = NULL;
	char* identifier = NULL;
	while (fileSize) {
		// Read in the Saveable identifier.
		fread(&size, sizeof(size_t), 1, fp);
		void* temp = std::realloc(identifier, size+1);
		assert(temp && "Out of Memory.");
		identifier = (char*) temp;
		fread(identifier, 1, size, fp);
		((char*)identifier)[size] = 0;
		fileSize -= (size + sizeof(size_t));

		// Read in the Saveable load data;
		fread(&size, sizeof(size_t), 1, fp);
		temp = std::realloc(buffer, size);
		assert(temp && "Out of Memory.");
		buffer = temp;
		fread(buffer, 1, size, fp);
		fileSize -= (size + sizeof(size_t));

		bool found = false;
		// Attempt to find a Saveable in memory to load the data into.
		for (auto& ptr : saveables) {
			Saveable& saveable = *ptr;
			if (saveable.getIdentifier() == (char*)buffer) {
				saveable.load(buffer, size);
				found = true;
				break;
			}
		}

		// If it wasn't found store the data for later
		if (!found) {
			unloaded.emplace_back(std::string{ identifier }, buffer, size);
		}
	}
	fclose(fp);
	if (identifier) {
		std::free(identifier);
		std::free(buffer);
	}
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

#include "./../engine/data_storage/filestorage.h"

#include <assert.h>

#include "tests.h"
#include "./../engine/data_storage/buffer_utils.h"

struct DataTest1 : Saveable {
	int32_t count;
	int32_t capacity;

    DataTest1(const std::string& identifier, int32_t count, int32_t capacity) 
        : Saveable(identifier), count(count), capacity(capacity) {}

    virtual void save(void* buffer) const override {
        void* buf = buffer;
        BufferUtils::WriteInt(&buffer, count);
        BufferUtils::WriteInt(&buffer, capacity);
    }

    virtual void load(const void* buffer, size_t size) override {
        assert(size == sizeof(int) * 2 && "Incorrect size when loading.");
        this->count = BufferUtils::ReadInt(&buffer);
        this->capacity = BufferUtils::ReadInt(&buffer);
    }

    virtual size_t getSize() override {
        return sizeof(int32_t) * 2;
    }

};

struct DataTest2 : Saveable {
	std::string name;
    uint64_t value;

    DataTest2(const std::string& identifier, const std::string& name, uint64_t value)
        : Saveable(identifier), name(name), value(value) {}

    virtual void save(void* buffer) const override {
        void* buf = buffer;
        BufferUtils::WriteString(&buffer, name);
        BufferUtils::WriteULong(&buffer, value);
    }

    virtual void load(const void* buffer, size_t size) override {
        this->name = BufferUtils::ReadString(&buffer);
        this->value = BufferUtils::ReadULong(&buffer);
    }

    virtual size_t getSize() override {
        return name.size() + sizeof(uint64_t) + sizeof(size_t);
    }

};

TestResults TestFileStorage() {
	TestResults results{3};
	FileStorage storage{"test.data"};

    std::shared_ptr<Saveable> data1 = std::make_shared<DataTest1>(std::string{ "test1" }, 33, 2);
    std::shared_ptr<Saveable> data2 = std::make_shared<DataTest2>(std::string{ "test2" }, "health", 9834734);
    std::shared_ptr<Saveable> data3 = std::make_shared<DataTest2>(std::string{ "test3" }, "mana", 3453);
	
    storage.addSaveable(data1);
    storage.addSaveable(data2);
    storage.addSaveable(data3);

    storage.save("test/");

    storage.load("test/");
    
    DataTest1* test1 = (DataTest1*) data1.get();
    DataTest2* test2 = (DataTest2*) data2.get();
    DataTest2* test3 = (DataTest2*) data3.get();

    if (test1->count == 33 && test1->capacity == 2) results.success++;
    else results.failed++;

    if (test2->name == "health" && test2->value == 9834734) results.success++;
    else results.failed++;

    if (test3->name == "mana" && test3->value == 3453) results.success++;
    else results.failed++;

	return results;
}
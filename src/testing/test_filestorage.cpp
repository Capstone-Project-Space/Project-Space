#include "./../engine/data_storage/filestorage.h"

#include <assert.h>
#include <numeric>

#include "tests.h"
#include "test_filestorage.h"
#include "./../engine/data_storage/buffer_utils.h"

struct DataTestAllTypes : Saveable {
    struct Data {
        int8_t b;
        uint8_t ub;
        int16_t s;
        uint16_t us;
        int32_t i;
        uint32_t ui;
        int64_t l;
        uint64_t ul;
        float f;
        double d;
        char* cstr;
        std::string str;
        Data(int8_t b, uint8_t ub, int16_t s, uint16_t us, int32_t i, uint32_t ui, int64_t l, uint64_t ul, float f, double d, char* cstr, const std::string& str)
            : b(b), ub(ub), s(s), us(us), i(i), ui(ui), l(l), ul(ul), f(f), d(d), cstr(cstr), str(str)
        {}
    } data;

    DataTestAllTypes(const std::string& identifier, Data d)
        : Saveable(identifier), data(d) {}

    virtual void save(void* buffer) const override {
        BufferUtils::WriteByte(&buffer, data.b);
        BufferUtils::WriteUByte(&buffer, data.ub);
        BufferUtils::WriteShort(&buffer, data.s);
        BufferUtils::WriteUShort(&buffer, data.us);
        BufferUtils::WriteInt(&buffer, data.i);
        BufferUtils::WriteUInt(&buffer, data.ui);
        BufferUtils::WriteLong(&buffer, data.l);
        BufferUtils::WriteULong(&buffer, data.ul);
        BufferUtils::WriteFloat(&buffer, data.f);
        BufferUtils::WriteDouble(&buffer, data.d);
        BufferUtils::WriteString(&buffer, data.cstr);
        BufferUtils::WriteString(&buffer, data.str);

    }

    virtual void load(const void* buffer, size_t size) override {
        data.b = BufferUtils::ReadByte(&buffer);
        data.ub = BufferUtils::ReadUByte(&buffer);
        data.s = BufferUtils::ReadShort(&buffer);
        data.us = BufferUtils::ReadUShort(&buffer);
        data.i = BufferUtils::ReadInt(&buffer);
        data.ui = BufferUtils::ReadUInt(&buffer);
        data.l = BufferUtils::ReadLong(&buffer);
        data.f = BufferUtils::ReadFloat(&buffer);
        data.d = BufferUtils::ReadDouble(&buffer);
        data.ul = BufferUtils::ReadULong(&buffer);
        BufferUtils::ReadString(&buffer, data.cstr);
        data.str = BufferUtils::ReadString(&buffer);
    }

    virtual size_t getSize() override {
        return 2 + 4 + 8 + 16 + sizeof(float) + sizeof(double) + BufferUtils::GetStringSize(data.cstr) + BufferUtils::GetStringSize(data.str);
    }
};

struct DataTestStats : Saveable {
    struct Stat {
        std::string name;
        uint32_t value;
        Stat(const std::string& name, uint32_t value) : name(name), value(value) {}
    } stats[7];
    DataTestStats(const std::string& identifier, uint32_t mhp, uint32_t mmn, uint32_t str, uint32_t def, uint32_t mat, uint32_t mdf, uint32_t agi)
        : Saveable(identifier), stats{ { "max_health", mhp }, { "max_mana", mmn }, { "strength", str }, { "defense", def }, { "magic_strength", mat }, { "magic_defense", mdf }, { "agility", agi } } {}

    virtual void save(void* buffer) const override {
        for (const auto& [name, value] : stats) {
            BufferUtils::WriteString(&buffer, name);
            BufferUtils::WriteUInt(&buffer, value);
        }
    }

    virtual void load(const void* buffer, size_t size) override {
        for (auto& [name, value] : stats) {
            name = BufferUtils::ReadString(&buffer);
            value = BufferUtils::ReadUInt(&buffer);
        }
    }

    virtual size_t getSize() override {
        return std::accumulate(stats, stats + 7, 0llu, [](size_t v, Stat s) { return v + BufferUtils::GetStringSize(s.name) + sizeof(uint32_t); });
    }

};

TestResult FS_TestAllDataTypes() {
    TestResult result{ "All Data Types" };
    FileStorage storage{ "test_alldata_types.data" };
    std::shared_ptr<Saveable> saveable = std::make_shared<DataTestAllTypes>(
        std::string{ "all_data" }, DataTestAllTypes::Data(
            -128, 255, SHRT_MIN, USHRT_MAX, INT_MIN, UINT_MAX, LLONG_MIN, ULLONG_MAX,
            32.3634f, 6346436.543734, "Hello World", std::string{ "Welcome to the World" }
        )
    );
    DataTestAllTypes* data = ((DataTestAllTypes*) saveable.get());

    storage.addSaveable(saveable);

    storage.save("./test/");
    storage.load("./test/");
    if (remove("./test/test_alldata_types.data")) {
        perror("Unable to delete test_alldata_types save file.");
    }

    while (1) {
        if (data->data.b != -128) {
            result.msg = "int8_t was not written or read correctly.";
            break;
        }
        if (data->data.ub != 255) {
            result.msg = "uint8_t was not written or read correctly.";
            break;
        }
        if (data->data.s != SHRT_MIN) {
            result.msg = "int16_t was not written or read correctly.";
            break;
        }
        if (data->data.us != USHRT_MAX) {
            result.msg = "uint16_t was not written or read correctly.";
            break;
        }
        if (data->data.i != INT_MIN) {
            result.msg = "int32_t was not written or read correctly.";
            break;
        }
        if (data->data.ui != UINT_MAX) {
            result.msg = "uint32_t was not written or read correctly.";
            break;
        }
        if (data->data.l != LLONG_MIN) {
            result.msg = "int64_t was not written or read correctly.";
            break;
        }
        if (data->data.ul != ULLONG_MAX) {
            result.msg = "uint64_t was not written or read correctly.";
            break;
        }
        if (data->data.f != 32.3634f) {
            result.msg = "float was not written or read correctly.";
            break;
        }
        if (data->data.d != 6346436.543734) {
            result.msg = "double was not written or read correctlly.";
            break;
        }
        if (strncmp(data->data.cstr, "Hello World", 12)) {
            result.msg = "C-String was not written or read correctly.";
            break;
        }
        if (data->data.str != std::string{ "Welcome to the World" }) {
            result.msg = "std::string was not written or read correctly.";
            break;
        }
        result.succeeded = true;
        break;
    }
    return result;
}

TestResult FS_TestMultipleSaveables() {
    TestResult result{ "Multiple Saveables" };
    FileStorage storage{ "test_multiple_saveables.data" };
    std::shared_ptr<Saveable> character1 = std::make_shared<DataTestStats>(std::string{ "character1" }, 23, 11, 4, 6, 9, 12, 6);
    std::shared_ptr<Saveable> character2 = std::make_shared<DataTestStats>(std::string{ "character2" }, 223, 0, 23, 34, 14, 20, 15);
    DataTestStats* data1 = ((DataTestStats*)character1.get());
    DataTestStats* data2 = ((DataTestStats*)character2.get());

    storage.addSaveable(character1);
    storage.addSaveable(character2);

    storage.save("./test/");
    storage.load("./test/");
    if (remove("./test/test_multiple_saveables.data")) {
        perror("Unable to delete test_multiple_saveables save file.");
    }

    std::string statNames[] = { "max_health", "max_mana", "strength", "defense", "magic_strength", "magic_defense", "agility" };
    uint32_t stat1Values[] = { 23, 11, 4, 6, 9, 12, 6 };
    uint32_t stat2Values[] = { 223, 0, 23, 34, 14, 20, 15 };
    for (size_t i = 0; i < 7; i++) {
        const auto& [name, value] = data1->stats[i];
        if (name != statNames[i]) {
            result.msg = statNames[i] + "1 name was not written or read correctly.";
            return result;
        } else if (value != stat1Values[i]) {
            result.msg = statNames[i] + "1 value was not written or read correctly.";
            return result;
        }
    }
    for (size_t i = 0; i < 7; i++) {
        const auto& [name, value] = data2->stats[i];
        if (name != statNames[i]) {
            result.msg = statNames[i] + "2 name was not written or read correctly.";
            return result;
        }
        else if (value != stat2Values[i]) {
            result.msg = statNames[i] + "2 value was not written or read correctly.";
            return result;
        }
    }

    result.succeeded = true;
    return result;
}

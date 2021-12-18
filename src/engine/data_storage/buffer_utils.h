#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Functions in this namespace are used to populate a buffer with elements.
 * @param buffer All functions that populate or read from a buffer update the buffer pointer.
 */
namespace BufferUtils {
	int8_t ReadByte(const void** buffer);
	void WriteByte(void** buffer, int8_t byte);

	uint8_t ReadUByte(const void** buffer);
	void WriteUByte(void** buffer, uint8_t ubyte);

	int16_t ReadShort(const void** buffer);
	void WriteShort(void** buffer, int16_t s);

	uint16_t ReadUShort(const void** buffer);
	void WriteUShort(void** buffer, uint16_t us);

	int32_t ReadInt(const void** buffer);
	void WriteInt(void** buffer, int32_t i);

	uint32_t ReadUInt(const void** buffer);
	void WriteUInt(void** buffer, uint32_t ui);

	int64_t ReadLong(const void** buffer);
	void WriteLong(void** buffer, int64_t l);

	uint64_t ReadULong(const void** buffer);
	void WriteULong(void** buffer, uint64_t ul);

	float ReadFloat(const void** buffer);
	void WriteFloat(void** buffer, float f);

	double ReadDouble(const void** buffer);
	void WriteDouble(void** buffer, double d);

	std::string ReadString(const void** buffer);
	void WriteString(void** buffer, const std::string& string);

	void ReadString(const void** buffer, char* string);
	void WriteString(void** buffer, const char* string);

	/**
	 * @brief Get the size of a std::string when written into a buffer.
	 */
	size_t GetStringSize(const std::string& string);

	/**
	 * @brief Get the size of a C-String when written into a buffer.
	 */
	size_t GetStringSize(const char* string);

};

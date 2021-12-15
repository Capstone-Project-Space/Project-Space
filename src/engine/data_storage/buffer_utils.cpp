#pragma once

#include <cstdint>
#include <string>
#include <assert.h>
#include <memory.h>

#include "buffer_utils.h"

int8_t BufferUtils::ReadByte(const void** buffer) {
	int8_t b = *(*((int8_t**) buffer))++;
	return b;
}

void BufferUtils::WriteByte(void** buffer, int8_t b) {
	*(*((int8_t**) buffer))++ = b;
}

uint8_t BufferUtils::ReadUByte(const void** buffer) {
	uint8_t ub = *(*((uint8_t**) buffer))++;
	return ub;
}

void BufferUtils::WriteUByte(void** buffer, uint8_t ub) {
	*(*((uint8_t**) buffer))++ = ub;
}


int16_t BufferUtils::ReadShort(const void** buffer) {
	int16_t s = *(*((int16_t**) buffer))++;
	return s;
}

void BufferUtils::WriteShort(void** buffer, int16_t b) {
	*(*((int16_t**) buffer))++ = b;
}

uint16_t BufferUtils::ReadUShort(const void** buffer) {
	uint16_t ub = *(*((uint16_t**) buffer))++;
	return ub;
}

void BufferUtils::WriteUShort(void** buffer, uint16_t ub) {
	*(*((uint16_t**) buffer))++ = ub;
}


int32_t BufferUtils::ReadInt(const void** buffer) {
	int32_t s = *(*((int32_t**) buffer))++;
	return s;
}

void BufferUtils::WriteInt(void** buffer, int32_t b) {
	*(*((int32_t**) buffer))++ = b;
}

uint32_t BufferUtils::ReadUInt(const void** buffer) {
	uint32_t ub = *(*((uint32_t**) buffer))++;
	return ub;
}

void BufferUtils::WriteUInt(void** buffer, uint32_t ub) {
	*(*((uint32_t**) buffer))++ = ub;
}


int64_t BufferUtils::ReadLong(const void** buffer) {
	int64_t s = *(*((int64_t**) buffer))++;
	return s;
}

void BufferUtils::WriteLong(void** buffer, int64_t b) {
	*(*((int64_t**) buffer))++ = b;
}

uint64_t BufferUtils::ReadULong(const void** buffer) {
	uint64_t ub = *(*((uint64_t**) buffer))++;
	return ub;
}

void BufferUtils::WriteULong(void** buffer, uint64_t ub) {
	*(*((uint64_t**) buffer))++ = ub;
}


std::string BufferUtils::ReadString(const void** buffer) {
	size_t length = ((size_t*) *buffer)[0];
	char* string = (char*) malloc(length + 1);
	assert(string && "Out of memory.");
	std::memcpy(string, *((char**) buffer) + sizeof(size_t), length);
	string[length] = 0;
	*((char**) buffer) += length + sizeof(size_t);
	return string;
}

void BufferUtils::WriteString(void** buffer, const std::string& string) {
	((size_t**) buffer)[0][0] = string.size();
	std::memcpy((* ((char**)buffer)) + sizeof(size_t), string.c_str(), string.size());
	*((char**) buffer) += string.size() + sizeof(size_t);
}


void BufferUtils::ReadString(const void** buffer, char* string) {
	size_t length = ((size_t*) *buffer)[0];
	std::memcpy(string, *((char**) buffer) + sizeof(size_t), length);
	string[length] = 0;
	*((char**) buffer) += length + sizeof(size_t);
}

void BufferUtils::WriteString(void** buffer, const char* string) {
	size_t length = strlen(string);
	((size_t*) buffer)[0] = length;
	std::memcpy(*((char**) buffer) + sizeof(size_t), string, length);
	*((char**) buffer) += length + sizeof(size_t);
}

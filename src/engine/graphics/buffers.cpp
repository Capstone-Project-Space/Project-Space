#include "buffers.h"

#include <assert.h>

std::shared_ptr<VertexBuffer> VertexBuffer::CreateVertexBuffer(uint64_t size, void* data) {
	std::shared_ptr<VertexBuffer> buffer = std::shared_ptr<VertexBuffer>(new VertexBuffer(size));
	if (data) buffer->updateBuffer(size, data);
	return buffer;
}

std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(uint64_t size, uint32_t* data) {
	std::shared_ptr<IndexBuffer> buffer = std::shared_ptr<IndexBuffer>(new IndexBuffer(size));
	if (data) buffer->updateBuffer(size, data);
	return buffer;
}

VertexBuffer::VertexBuffer(uint64_t size) : size(size), data(nullptr) {
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &this->id);
}

void VertexBuffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, this->id); }

void VertexBuffer::updateBuffer(uint64_t size, void* data, uint64_t offset) {
	assert(size + offset >= size && size + offset >= offset);
	this->data = data;
	if (size + offset > this->size) {
		glDeleteBuffers(1, &this->id);
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
		glBufferData(GL_ARRAY_BUFFER, size + offset, nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, offset, this->data);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, ((uint8_t*) data) + offset);
}

void VertexBuffer::updateFromSeparateBuffer(uint64_t size, void* subdata, uint64_t offset) {
	assert(size + offset >= size && size + offset >= offset && size + offset <= this->size);
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}


IndexBuffer::IndexBuffer(uint64_t size) : size(size), count(size / sizeof(uint32_t)), data(nullptr) {
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &this->id);
}

void IndexBuffer::bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id); }

void IndexBuffer::updateBuffer(uint64_t size, uint32_t* data, uint64_t offset) {
	assert(size + offset >= size && size + offset >= offset);
	this->data = data;
	if (size + offset > this->size) {
		glDeleteBuffers(1, &this->id);
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size + offset, data, GL_DYNAMIC_DRAW);
	} else {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	}
}

void IndexBuffer::updateFromSeparateBuffer(uint64_t size, uint32_t* subdata, uint64_t offset) {
	assert(size + offset >= size && size + offset >= offset && size + offset <= this->size);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

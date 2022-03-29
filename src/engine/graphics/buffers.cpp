#include "buffers.h"

#include <assert.h>

#define LOG_GL_ERROR for (int glErrorGL = glGetError(); glErrorGL != 0;) { fprintf(stderr, "GLError: %d\n", glErrorGL); assert(false);}

std::shared_ptr<VertexBuffer> VertexBuffer::CreateVertexBuffer(uint64_t size, const void* data) {
	std::shared_ptr<VertexBuffer> buffer = std::shared_ptr<VertexBuffer>(new VertexBuffer(size));
	if (data) buffer->updateFromSeparateBuffer(size, data);
	return buffer;
}

std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(uint64_t size, const uint32_t* data) {
	std::shared_ptr<IndexBuffer> buffer = std::shared_ptr<IndexBuffer>(new IndexBuffer(size));
	if (data) buffer->updateFromSeparateBuffer(size, data);
	return buffer;
}

std::shared_ptr<UniformBuffer> UniformBuffer::CreateUniformBuffer(uint64_t count, uint64_t elemSize, const void* data) {
	static uint32_t nextBlockBinding = 0;
	std::shared_ptr<UniformBuffer> buffer = std::shared_ptr<UniformBuffer>(new UniformBuffer(nextBlockBinding, count, elemSize));
	nextBlockBinding++;
	return buffer;
}

VertexBuffer::VertexBuffer(uint64_t size) : size(size), data(nullptr) {
	glGenBuffers(1, &this->id);
	LOG_GL_ERROR;
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	LOG_GL_ERROR;
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	LOG_GL_ERROR;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	LOG_GL_ERROR;
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &this->id);
	LOG_GL_ERROR;
}

void VertexBuffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, this->id); LOG_GL_ERROR;}

void VertexBuffer::updateBuffer(uint64_t size, const void* data, uint64_t offset) {
	assert(size + offset >= size && size + offset >= offset);
	this->data = data;
	if (size + offset > this->size) {
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
		LOG_GL_ERROR;
		glBufferData(GL_ARRAY_BUFFER, size + offset, data, GL_DYNAMIC_DRAW);
		LOG_GL_ERROR;
		this->size = size;
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	LOG_GL_ERROR;
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, ((uint8_t*) data) + offset);
	LOG_GL_ERROR;
}

void VertexBuffer::updateFromSeparateBuffer(uint64_t size, const void* subdata, uint64_t offset) {
	assert(size + offset >= size && size + offset >= offset && size + offset <= this->size);
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	LOG_GL_ERROR;
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, subdata);
	LOG_GL_ERROR;
}


IndexBuffer::IndexBuffer(uint64_t size) : size(size), count(size / sizeof(uint32_t)), data(nullptr) {
	glGenBuffers(1, &this->id);
	LOG_GL_ERROR;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	LOG_GL_ERROR;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	LOG_GL_ERROR;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	LOG_GL_ERROR;
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &this->id);
	LOG_GL_ERROR;
}

void IndexBuffer::bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id); LOG_GL_ERROR; }

void IndexBuffer::updateBuffer(uint64_t size, const uint32_t* data, uint64_t offset) {
	assert(size + offset >= size && size + offset >= offset);
	this->data = data;
	if (size + offset > this->size) {
		glDeleteBuffers(1, &this->id);
		LOG_GL_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		LOG_GL_ERROR;
		glGenBuffers(1, &this->id);
		LOG_GL_ERROR;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		LOG_GL_ERROR;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size + offset, data, GL_DYNAMIC_DRAW);
		LOG_GL_ERROR;
		this->size = size;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	LOG_GL_ERROR;
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	LOG_GL_ERROR;
}

void IndexBuffer::updateFromSeparateBuffer(uint64_t size, const uint32_t* subdata, uint64_t offset) {
	assert(size + offset >= size && size + offset >= offset && size + offset <= this->size);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	LOG_GL_ERROR;
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, subdata);
	LOG_GL_ERROR;
}

UniformBuffer::UniformBuffer(uint32_t blockBinding, uint64_t count, uint64_t elemSize)
	: blockBinding(blockBinding), count(count), elemSize(elemSize), data(NULL) {
	glGenBuffers(1, &this->id);
	LOG_GL_ERROR;
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);
	LOG_GL_ERROR;
	glBufferData(GL_UNIFORM_BUFFER, count * elemSize, nullptr, GL_DYNAMIC_DRAW);
	LOG_GL_ERROR;
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	LOG_GL_ERROR;
	glBindBufferRange(GL_UNIFORM_BUFFER, blockBinding, this->id, 0, count * elemSize);
	LOG_GL_ERROR;
}

UniformBuffer::~UniformBuffer() {
	glDeleteBuffers(1, &this->id);
	LOG_GL_ERROR;
}

void UniformBuffer::updateBuffer(const void* data) {
	assert(data);
	this->data = data;
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);
	LOG_GL_ERROR;
	glBufferSubData(GL_UNIFORM_BUFFER, 0, this->count * this->elemSize, data);
	LOG_GL_ERROR;
	glBindBufferRange(GL_UNIFORM_BUFFER, blockBinding, this->id, 0, count * elemSize);
	LOG_GL_ERROR;
}

void UniformBuffer::bind() {
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);
}

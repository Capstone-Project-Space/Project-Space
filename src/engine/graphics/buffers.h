#pragma once

#include <stdint.h>
#include <memory>

#include <glad/glad.h>

class VertexBuffer {
public:
	std::shared_ptr<VertexBuffer> CreateVertexBuffer(uint64_t size, void* data=nullptr);

	~VertexBuffer();

	void bind();
	void updateBuffer(uint64_t size, void* data, uint64_t offset = 0);
	void updateFromSeparateBuffer(uint64_t size, void* subdata, uint64_t offset = 0);

private:
	uint32_t id;
	uint64_t size;
	void* data;
	VertexBuffer(uint64_t size);
};

class IndexBuffer {
public:
	std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint64_t size, uint32_t* data = nullptr);

	~IndexBuffer();

	void bind();
	void updateBuffer(uint64_t size, uint32_t* data, uint64_t offset = 0);
	void updateFromSeparateBuffer(uint64_t size, uint32_t* subdata, uint64_t offset = 0);

private:
	uint32_t id;
	uint64_t size, count;
	uint32_t* data;
	IndexBuffer(uint64_t size);
};

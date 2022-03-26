#pragma once

#include <stdint.h>
#include <memory>

#include <glad/glad.h>

class VertexBuffer {
public:
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(uint64_t size, const void* data=nullptr);

	~VertexBuffer();

	void bind();
	void updateBuffer(uint64_t size, const void* data, uint64_t offset = 0);
	void updateFromSeparateBuffer(uint64_t size, const void* subdata, uint64_t offset = 0);

	constexpr bool operator ==(const VertexBuffer& right) const {
		return this->id == right.id;
	}

private:
	uint32_t id;
	uint64_t size;
	const void* data;
	VertexBuffer(uint64_t size);
};

class IndexBuffer {
public:
	static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint64_t size, const uint32_t* data = nullptr);

	~IndexBuffer();

	void bind();
	void updateBuffer(uint64_t size, const uint32_t* data, uint64_t offset = 0);
	void updateFromSeparateBuffer(uint64_t size, const uint32_t* subdata, uint64_t offset = 0);

	inline uint64_t getCount() const { return count; }

private:
	uint32_t id;
	uint64_t size, count;
	const uint32_t* data;
	IndexBuffer(uint64_t size);
};

class UniformBuffer {
public:
	static std::shared_ptr<UniformBuffer> CreateUniformBuffer(uint64_t count, uint64_t elemSize, const void* data = nullptr);

	~UniformBuffer();

	void updateBuffer(const void* data);
	void bind();

	inline uint32_t getBlockBinding() const { return this->blockBinding; }

private:
	uint32_t id, blockBinding;
	uint64_t count, elemSize;
	const void* data;
	UniformBuffer(uint32_t blockBinding, uint64_t count, uint64_t elemSize);
};

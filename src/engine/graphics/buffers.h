#pragma once

#include <stdint.h>
#include <memory>

#include <glad/glad.h>

class VertexBuffer {
public:
	/**
	 * @brief Create a Vertex Buffer.
	 * 
	 * @param size The size of the buffer in bytes.
	 * @param data An optional initial buffer with `size` size.
	 * @return A std::shared_ptr of the VertexBuffer.
	 */
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(uint64_t size, const void* data=nullptr);

	~VertexBuffer();

	/**
	 * @brief Bind this VertexBuffer in the rendering context.
	 */
	void bind() const;

	/**
	 * @brief Update the buffer in a way which will increase the size if necessary.
	 * 
	 * @param size The size of the data buffer in bytes.
	 * @param data A pointer to the beginning of the buffer.
	 * @param offset The offset applied to data and to the VertexBuffer in bytes.
	 */
	void updateBuffer(uint64_t size, const void* data, uint64_t offset = 0);

	/**
	 * @brief Update the buffer in a way that will not increase the size if necessary.
	 * 
	 * @param size The size of the subdata buffer in bytes.
	 * @param subdata A pointer to the beginning of the buffer.
	 * @param offset The offset(in bytes) applied to the VertexBuffer.
	 */
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
	/**
	 * @brief Create an Index Buffer.
	 * 
	 * @param size The size of the buffer in bytes.
	 * @param data An optional initial buffer with `size` size.
	 * @return A std::shared_ptr of the IndexBuffer.
	 */
	static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint64_t size, const uint32_t* data = nullptr);

	~IndexBuffer();

	/**
	 * @brief Bind this IndexBuffer in the rendering context.
	 */
	void bind() const;

	/**
	 * @brief Update the buffer in a way which will increase the size if necessary.
	 * 
	 * @param size The size of the data buffer in bytes.
	 * @param data A pointer to the beginning of the buffer.
	 * @param offset The offset applied to data and to the VertexBuffer in bytes.
	 */
	void updateBuffer(uint64_t size, const uint32_t* data, uint64_t offset = 0);
	
	/**
	 * @brief Update the buffer in a way that will not increase the size if necessary.
	 * 
	 * @param size The size of the subdata buffer in bytes.
	 * @param subdata A pointer to the beginning of the buffer.
	 * @param offset The offset(in bytes) applied to the VertexBuffer.
	 */
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
	/**
	 * @brief Create an Index Buffer.
	 * 
	 * @param size The size of the buffer in bytes.
	 * @param data An optional initial buffer with `size` size.
	 * @return A std::shared_ptr of the IndexBuffer.
	 */
	static std::shared_ptr<UniformBuffer> CreateUniformBuffer(uint64_t count, uint64_t elemSize, const void* data = nullptr);

	~UniformBuffer();

	/**
	 * @brief Bind this UniformBuffer in the rendering context.
	 */
	void bind() const;

	/**
	 * @brief Update the entire uniform buffer.
	 * 
	 * @param data A pointer to the beginning of the new buffer.
	 */
	void updateBuffer(const void* data);

	inline uint32_t getBlockBinding() const { return this->blockBinding; }

private:
	uint32_t id, blockBinding;
	uint64_t count, elemSize;
	const void* data;
	UniformBuffer(uint32_t blockBinding, uint64_t count, uint64_t elemSize);
};

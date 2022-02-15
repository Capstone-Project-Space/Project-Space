#pragma once

#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

#include "buffers.h"

#include <glad/glad.h>

namespace ShaderDataType {
	enum class Type {
		FLOAT, FLOAT2, FLOAT3, FLOAT4,
		INT, INT2, INT3, INT4,
		BOOL, BOOL2, BOOL3, BOOL4,
		MAT3, MAT4
	};

	uint64_t GetSize(Type type);
	uint64_t GetCount(Type type);
}

struct LayoutElement {
	bool normalized;
	const std::string name;
	ShaderDataType::Type type;
	uint64_t offset;
	LayoutElement(const std::string& name, ShaderDataType::Type type, bool normalized = false);
};

class VertexLayout {
public:
	VertexLayout();
	VertexLayout(const std::initializer_list<LayoutElement>& elements);

	inline const uint32_t getStride() const { return stride; }

	inline const std::vector<LayoutElement>& getElements() const { return elements; }

	inline std::vector<LayoutElement>::iterator begin() { return elements.begin(); }
	inline std::vector<LayoutElement>::iterator end() { return elements.end(); }

	inline std::vector<LayoutElement>::const_iterator begin() const { return elements.cbegin(); }
	inline std::vector<LayoutElement>::const_iterator end() const { return elements.cend(); }

private:
	uint64_t stride;
	std::vector<LayoutElement> elements;
	void calculateOffsetAndStride();
};

class VertexArray {
public:
	static std::shared_ptr<VertexArray> CreateVertexArray(const VertexLayout& layout, const std::shared_ptr<VertexBuffer> buffer);

	~VertexArray();

	void bind();

private:
	uint32_t id;
	VertexArray();
};

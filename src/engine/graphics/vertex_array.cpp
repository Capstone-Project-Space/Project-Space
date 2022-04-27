#include "vertex_array.h"

#include <assert.h>

#define LOG_GL_ERROR for (int glErrorGL = glGetError(); glErrorGL != 0;) { fprintf(stderr, "GLError: %d\n", glErrorGL); assert(false);}

std::shared_ptr<VertexArray> VertexArray::CreateVertexArray(const VertexLayout& layout, const std::shared_ptr<VertexBuffer> buffer) {
	std::shared_ptr<VertexArray> vertArray = std::shared_ptr<VertexArray>(new VertexArray());
	if (buffer) vertArray->addVertexBuffer(layout, buffer);
	return vertArray;
}

uint64_t ShaderDataType::GetSize(Type type) {
	switch (type) {
		case Type::FLOAT: case Type::INT: case Type::BOOL:
			return 4;
		case Type::FLOAT2: case Type::INT2: case Type::BOOL2:
			return 8;
		case Type::FLOAT3: case Type::INT3: case Type::BOOL3:
			return 12;
		case Type::FLOAT4: case Type::INT4: case Type::BOOL4:
			return 16;
		case Type::MAT3:
			return 36;
		case Type::MAT4:
			return 64;
		default:
			throw "Unknown ShaderDataType::Type in GetSize";
	}
}

uint64_t ShaderDataType::GetCount(Type type) {
	switch (type) {
	case Type::FLOAT: case Type::INT: case Type::BOOL:
		return 1;
	case Type::FLOAT2: case Type::INT2: case Type::BOOL2:
		return 2;
	case Type::FLOAT3: case Type::INT3: case Type::BOOL3:
		return 3;
	case Type::FLOAT4: case Type::INT4: case Type::BOOL4:
		return 4;
	case Type::MAT3:
		return 9;
	case Type::MAT4:
		return 16;
	default:
		throw "Unknown ShaderDataType::Type in GetCount";
	}
}

LayoutElement::LayoutElement(const char* name, ShaderDataType::Type type, bool normalized)
	: type(type), normalized(normalized), offset(0) {}

VertexLayout::VertexLayout() : stride(0) {}
VertexLayout::VertexLayout(const std::initializer_list<LayoutElement>& elements)
	: elements(elements) {
	calculateOffsetAndStride();
}

void VertexLayout::calculateOffsetAndStride() {
	stride = 0;
	for (auto& element : elements) {
		element.offset = stride;
		stride += ShaderDataType::GetSize(element.type);
	}
}


VertexArray::VertexArray() {
	glGenVertexArrays(1, &this->id);
	LOG_GL_ERROR;
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &this->id);
	LOG_GL_ERROR;
}

void VertexArray::bind() {
	glBindVertexArray(this->id);
	LOG_GL_ERROR;
}

void VertexArray::addVertexBuffer(const VertexLayout& layout, const std::shared_ptr<VertexBuffer> vertices) {
	glBindVertexArray(this->id);
	LOG_GL_ERROR;
	vertices->bind();
	uint32_t index = 0;
	for (const auto& element : layout) {
		switch (element.type) {
		case ShaderDataType::Type::FLOAT:
		case ShaderDataType::Type::FLOAT2:
		case ShaderDataType::Type::FLOAT3:
		case ShaderDataType::Type::FLOAT4:
			glEnableVertexAttribArray(index);
			LOG_GL_ERROR;
			glVertexAttribPointer(
				index, ShaderDataType::GetCount(element.type), GL_FLOAT,
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(), (const void*)element.offset
			);
			LOG_GL_ERROR;
			index++;
			break;

		case ShaderDataType::Type::MAT3:
			for (uint64_t j = 0; j < 36; j += 12) {
				glEnableVertexAttribArray(index);
				LOG_GL_ERROR;
				glVertexAttribPointer(
					index, 3, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(), (const void*)(element.offset + j)
				);
				LOG_GL_ERROR;
				index++;
			}
			break;
		case ShaderDataType::Type::MAT4:
			for (uint64_t j = 0; j < 64; j += 16) {
				LOG_GL_ERROR;
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(
					index, 4, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(), (const void*)(element.offset + j)
				);
				LOG_GL_ERROR;
				index++;
			}
			break;
		default:
			glEnableVertexAttribArray(index);
			LOG_GL_ERROR;
			glVertexAttribIPointer(
				index, ShaderDataType::GetCount(element.type),
				GL_INT, layout.getStride(), (const void*)element.offset
			);
			LOG_GL_ERROR;
			index++;
			break;
		}
	}
}

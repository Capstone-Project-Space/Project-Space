#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <src/engine/graphics/buffers.h>
#include <src/engine/graphics/texture.h>

#include <glm/glm.hpp>

#pragma pack(1)
struct ModelVertex {
	glm::vec3 vertex;
	glm::vec2 uv;
	glm::vec3 normal;
	int texture;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float specularExponent;
	ModelVertex();
	ModelVertex(
		glm::vec3 vertex, glm::vec2 uv, glm::vec3 normal, int texture,
		glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float specExponent
	);
};
#pragma pack()

struct Material {
	glm::vec3 ambient{1.0f};
	glm::vec3 diffuse{ 1.0f };
	glm::vec3 specular{ 1.0f };
	float specExponent = 1.0f;
	float idxRefraction = 1.0f;
	std::shared_ptr<Texture> texture = nullptr;
};

constexpr uint64_t getVertexFloatSize() { return sizeof(ModelVertex) / sizeof(float); }

class Model {
public:
	static std::shared_ptr<Model> CreateModel(const std::string& filepath);

	~Model() = default;

	const inline std::shared_ptr<IndexBuffer> getIndices() const { return indexBuffer; }
	const inline std::shared_ptr<VertexBuffer> getVertices() const { return vertexBuffer; }

	void bind() const;

private:
	std::shared_ptr<IndexBuffer> indexBuffer;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::array<std::shared_ptr<Texture>, 32> textures = {nullptr};
	Model() = default;

	static const std::map<std::string, Material> CreateMaterial(const std::string& filepath);

};

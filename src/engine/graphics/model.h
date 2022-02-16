#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

struct ModelVertex {
	glm::vec3 vertex;
	glm::vec2 uv;
	glm::vec3 normal;
	ModelVertex();
	ModelVertex(const glm::vec3& vertex, const glm::vec3 normal, const glm::vec2& uv);
	ModelVertex(float vx, float vy, float vz, float nx, float ny, float nz, float s, float t);
};

class Model {
public:
	static std::shared_ptr<Model> CreateModel(const std::string& filepath);

	~Model();

private:
	uint64_t vertCount, indCount;
	ModelVertex* vertices;
	uint32_t* indices;
	Model(uint64_t vertSize, uint64_t indSize);
};

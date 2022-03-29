#pragma once

#include <array>

#include <src/engine/graphics/buffers.h>
#include <src/engine/graphics/vertex_array.h>
#include <src/engine/graphics/shader.h>
#include <src/engine/graphics/texture.h>
#include <src/engine/graphics/camera.h>

#include <glm/glm.hpp>

constexpr uint32_t MAX_QUADS = 8192;
constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;
constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;

#pragma pack(1)
struct QuadVertex {
	glm::vec2 position;
	glm::vec2 uv;
	uint32_t texture;
	glm::mat4 transform;
};

struct QuadData {
	QuadVertex vertices[4];
};
#pragma pack()

const glm::vec4 FULL_UV{ 0.0f, 0.0f, 1.0f, 1.0f };

class BatchRenderer {
public:
	BatchRenderer();

	void init();
	void destroy();

	void submitQuad(QuadData data, const std::shared_ptr<Texture> texture);
	void submitQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, float rotation = 0.0f);

	inline void submitQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, float rotation = 0.0f) {
		submitQuad(glm::vec3{ pos, 0.0f }, size, stpq, texture, rotation);
	}
	
	inline void submitQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation = 0.0f) {
		submitQuad(glm::vec3{ position, 0.0f }, size, FULL_UV, texture, rotation);
	}

	inline void submitQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec2& st, const glm::vec2& pq, const std::shared_ptr<Texture> texture, float rotation = 0.0f) {
		submitQuad(glm::vec3{ position, 0.0f }, size, glm::vec4(st, pq), texture, rotation);
	}

	inline void submitQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation = 0.0f) {
		submitQuad(position, size, FULL_UV, texture, rotation);
	}

	inline void submitQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec2& st, const glm::vec2& pq, const std::shared_ptr<Texture> texture, float rotation = 0.0f) {
		submitQuad(position, size, glm::vec4(st, pq), texture, rotation);
	}
	void draw();

	inline void setCamera(const Camera& camera) { this->camera = &camera; }

private:
	const Camera* camera;

	uint32_t count;
	QuadData* quads = nullptr;
	uint32_t* indexData = nullptr;

	uint32_t textureCount;
	std::array<std::shared_ptr<Texture>, 31> textures;

	std::shared_ptr<IndexBuffer> indices = nullptr;
	std::shared_ptr<VertexBuffer> vertices = nullptr;
	std::shared_ptr<VertexArray> vArray = nullptr;
	std::shared_ptr<ShaderProgram> program = nullptr;
};

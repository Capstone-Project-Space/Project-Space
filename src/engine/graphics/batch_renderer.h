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
	glm::vec4 tint;
	uint32_t texture;
	glm::mat4 transform;
};

struct QuadData {
	QuadVertex vertices[4];
};
#pragma pack()

constexpr glm::vec4 FULL_UV{ 0.0f, 0.0f, 1.0f, 1.0f };
constexpr glm::vec4 WHITE_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

class BatchRenderer {
public:
	BatchRenderer();

	/**
	 * @brief Initialize all of the graphics objects.
	 */
	void init();

	/**
	 * @brief Destroy all of the graphics objects.
	 */
	void destroy();

	void submitQuad(QuadData data, const std::shared_ptr<Texture> texture);

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param stpq The texture coordinates to take from the texture with 0, 0 being the bottom left.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	void submitQuad(
		const glm::vec3& pos, const glm::vec2& size, const glm::vec4& stpq,
		const std::shared_ptr<Texture> texture, const glm::vec4& color, float rotation = 0.0f
	);

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param stpq The texture coordinates to take from the texture with 0, 0 being the bottom left.
	 * @param texture The texture to apply to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, float rotation = 0.0f) {
		submitQuad(glm::vec3{ pos, 0.0f }, size, stpq, texture, WHITE_COLOR, rotation);
	}

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param stpq The texture coordinates to take from the texture with 0, 0 being the bottom left.
	 * @param texture The texture to apply to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, float rotation = 0.0f) {
		submitQuad(pos, size, stpq, texture, WHITE_COLOR, rotation);
	}

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation = 0.0f) {
		submitQuad(glm::vec3{ pos, 0.0f }, size, FULL_UV, texture, WHITE_COLOR, rotation);
	}

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation = 0.0f) {
		submitQuad(pos, size, FULL_UV, texture, WHITE_COLOR, rotation);
	}

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param stpq The texture coordinates to take from the texture with 0, 0 being the bottom left.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, const glm::vec4& tintColor, float rotation = 0.0f) {
		submitQuad(glm::vec3{ pos, 0.0f }, size, stpq, texture, tintColor, rotation);
	}

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, const glm::vec4& tintColor, float rotation = 0.0f) {
		submitQuad(glm::vec3{ pos, 0.0f }, size, FULL_UV, texture, tintColor, rotation);
	}

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, const glm::vec4& tintColor, float rotation = 0.0f) {
		submitQuad(pos, size, FULL_UV, texture, tintColor, rotation);
	}

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param stpq The texture coordinates to take from the texture with 0, 0 being the bottom left.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, const glm::vec3& tintColor, float rotation = 0.0f) {
		submitQuad(glm::vec3{ pos, 0.0f }, size, stpq, texture, glm::vec4{ tintColor, 1.0f }, rotation);
	}

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param stpq The texture coordinates to take from the texture with 0, 0 being the bottom left.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, const glm::vec3& tintColor, float rotation = 0.0f) {
		submitQuad(pos, size, stpq, texture, glm::vec4{ tintColor, 1.0f }, rotation);
	}

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, const glm::vec3& tintColor, float rotation = 0.0f) {
		submitQuad(glm::vec3{ pos, 0.0f }, size, FULL_UV, texture, glm::vec4{ tintColor, 1.0f }, rotation);
	}

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, const glm::vec3& tintColor, float rotation = 0.0f) {
		submitQuad(pos, size, FULL_UV, texture, glm::vec4{ tintColor, 1.0f }, rotation);
	}

	/**
	 * @brief Flush the contents of the quad buffer to the GPU and tell it to render.
	 */
	void draw();

	inline void setCamera(const Camera& camera) { this->camera = &camera; }

private:
	const Camera* camera;

	uint32_t count;
	QuadData* quads = nullptr;
	uint32_t* indexData = nullptr;

	uint32_t textureCount;
	std::array<std::shared_ptr<Texture>, 31> textures{0};

	std::shared_ptr<IndexBuffer> indices = nullptr;
	std::shared_ptr<VertexBuffer> vertices = nullptr;
	std::shared_ptr<VertexArray> vArray = nullptr;
	std::shared_ptr<ShaderProgram> program = nullptr;
};

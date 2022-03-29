#pragma once

#include <src/engine/graphics/buffers.h>
#include <src/engine/graphics/vertex_array.h>
#include <src/engine/graphics/shader.h>
#include <src/engine/graphics/camera.h>

#include <src/engine/io/event_receiver.h>

constexpr uint32_t TEXT_MAX_QUADS = 1024;
constexpr uint32_t TEXT_MAX_INDICES = TEXT_MAX_QUADS * 6;
constexpr uint32_t TEXT_MAX_VERTICES = TEXT_MAX_QUADS * 4;

#pragma pack(1)

struct TextVertex {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec4 color;
	uint32_t texture;
	glm::mat4 transform;
};

struct TextQuad {
	TextVertex vertices[4];
};

#pragma pack()

class TextRenderer : public EventReceiver {
public:
	TextRenderer(float width, float height);

	void init();
	void destroy();

	void submitText(const std::string& text, const glm::vec3& pos, const glm::vec4& color, float scale = 1.0f, float rotation = 0);
	
	inline void submitText(const std::string& text, const glm::vec3& pos, const glm::vec3& color, float scale = 1.0f, float rotation = 0) {
		submitText(text, pos, glm::vec4{ color, 1.0f }, scale, rotation);
	}

	inline void submitText(const std::string& text, const glm::vec2& pos, const glm::vec3& color, float scale = 1.0f, float rotation = 0) {
		submitText(text, glm::vec3{ pos, 0.0f }, color, scale, rotation);
	}

	inline void submitText(const std::string& text, const glm::vec2& pos, const glm::vec4& color, float scale = 1.0f, float rotation = 0) {
		submitText(text, glm::vec3{ pos, 0.0f }, color, scale, rotation);
	}

	void draw();

	virtual void onWindowResize(const float oldWidth, const float oldHeight, const float newWidth, const float newHeight) override;

private:
	Camera camera;

	uint32_t count;
	TextQuad* quads = nullptr;
	uint32_t* indexData = nullptr;

	uint32_t textureCount;
	std::array<uint32_t, 31> textures{0};

	std::shared_ptr<IndexBuffer> indices = nullptr;
	std::shared_ptr<VertexBuffer> vertices = nullptr;
	std::shared_ptr<VertexArray> vArray = nullptr;
	std::shared_ptr<ShaderProgram> program = nullptr;

	struct Character {
		uint32_t textureID = 0;
		glm::ivec2 size{10, 10};
		glm::ivec2 bearing{0, 0};
		uint32_t advance = 0;
	} fontCharacters[256];

};

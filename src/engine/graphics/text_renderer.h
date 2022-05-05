#pragma once

#include <src/engine/graphics/buffers.h>
#include <src/engine/graphics/vertex_array.h>
#include <src/engine/graphics/shader.h>
#include <src/engine/graphics/camera.h>
#include <src/engine/graphics/ui/layout.h>
#include <src/engine/graphics/font.h>

#include <src/engine/io/event_receiver.h>

constexpr uint32_t TEXT_MAX_QUADS = 1024;
constexpr uint32_t TEXT_MAX_INDICES = TEXT_MAX_QUADS * 6;
constexpr uint32_t TEXT_MAX_VERTICES = TEXT_MAX_QUADS * 4;

#pragma pack(1)

struct TextVertex {
	glm::vec3 pos;
	glm::vec2 uv;
	uint32_t font;
	glm::vec4 color;
	glm::mat4 transform;
};

struct TextQuad {
	TextVertex vertices[4];
};

#pragma pack()

class TextRenderer {
public:
	TextRenderer();

	/**
	 * @brief Initialize all of the graphics objects.
	 */
	void init();

	/**
	 * @brief Destroy all of the graphics objects.
	 */
	void destroy();

	/**
	 * @brief Submit a line of text for rendering.
	 * 
	 * @param text The text to be rendered.
	 * @param pos The position to render the text.
	 * @param color The color of the text when rendered.
	 * @param font The font to render the text with.
	 * @param gravity The gravity applied to the text(changes position).
	 * @param scale The scale(multiplier of size) of the text.
	 * @param rotation The rotation about the z axis.
	 */
	void submitText(const std::string& text, const glm::vec3& pos, const glm::vec4& color, const std::shared_ptr<Font> font, Gravity gravity, float scale, float rotation);
	
	/**
	 * @brief Submit a line of text for rendering.
	 * 
	 * @param text The text to be rendered.
	 * @param pos The position to render the text.
	 * @param color The color of the text when rendered.
	 * @param font The font to render the text with.
	 * @param gravity The gravity applied to the text(changes position).
	 * @param scale The scale(multiplier of size) of the text.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitText(const std::string& text, const glm::vec3& pos, const glm::vec3& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0) {
		submitText(text, pos, glm::vec4{ color, 1.0f }, font, gravity, scale, rotation);
	}

	/**
	 * @brief Submit a line of text for rendering.
	 * 
	 * @param text The text to be rendered.
	 * @param pos The position to render the text.
	 * @param color The color of the text when rendered.
	 * @param font The font to render the text with.
	 * @param gravity The gravity applied to the text(changes position).
	 * @param scale The scale(multiplier of size) of the text.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitText(const std::string& text, const glm::vec2& pos, const glm::vec3& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0) {
		submitText(text, glm::vec3{ pos, 0.0f }, color, font, gravity, scale, rotation);
	}

	/**
	 * @brief Submit a line of text for rendering.
	 * 
	 * @param text The text to be rendered.
	 * @param pos The position to render the text.
	 * @param color The color of the text when rendered.
	 * @param font The font to render the text with.
	 * @param gravity The gravity applied to the text(changes position).
	 * @param scale The scale(multiplier of size) of the text.
	 * @param rotation The rotation about the z axis.
	 */
	inline void submitText(const std::string& text, const glm::vec2& pos, const glm::vec4& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0) {
		submitText(text, glm::vec3{ pos, 0.0f }, color, font, gravity, scale, rotation);
	}

	/**
	 * @brief Flush the contents of the text buffer to the GPU and tell it to render.
	 */
	void draw();

	inline void setCamera(const Camera& camera) { this->camera = &camera; }

private:
	const Camera* camera;

	uint32_t count;
	TextQuad* quads = nullptr;
	uint32_t* indexData = nullptr;

	uint32_t fontCount = 0;
	std::array<std::shared_ptr<Font>, 32> fonts;

	std::shared_ptr<IndexBuffer> indices = nullptr;
	std::shared_ptr<VertexBuffer> vertices = nullptr;
	std::shared_ptr<VertexArray> vArray = nullptr;
	std::shared_ptr<ShaderProgram> program = nullptr;
};

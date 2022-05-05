#pragma once

#include <src/engine/graphics/batch_renderer.h>
#include <src/engine/graphics/model_renderer.h>
#include <src/engine/graphics/text_renderer.h>

struct Renderer {
private:
	static bool ModelRendererInitialized;
	static ModelRenderer ModelRenderer;
	
	static bool BatchRendererInitialized;
	static std::shared_ptr<Font> CurrentFont;
	static BatchRenderer BatchRenderer;
	static TextRenderer TextRenderer;

	enum class Mode { NONE, MODEL, BATCH };
	static Mode mode;

public:
	/**
	 * @brief Primes the Renderer for rendering 3D. Cannot be done at the same time as 2D.
	 * 
	 * @param camera The perspective camera to render with.
	 */
	static void Begin3DScene(const Camera& camera);

	/**
	 * @brief Submit a light source for this scene.
	 * 
	 * @param light The light source data to add to this scene.
	 */
	static void SubmitLightSource(const LightSource& light);

	/**
	 * @brief Submit a Model for this scene.
	 * 
	 * @param model The Model to be rendered.
	 * @param transform The transformation matrix to apply to this submission.
	 * @param color A tint color which will be multiplied by the models texture color.
	 */
	static void SubmitModel(const std::shared_ptr<Model> model, const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f });

	/**
	 * @brief Submit a Model for this scene.
	 * 
	 * @param model The model to be rendered.
	 * @param transform The transformation matrix to apply to this submission.
	 * @param program The ShaderProgram this model will use to render onto the screen.
	 * @param color A tint color which will be multipled by the models texture color.
	 */
	static void SubmitModel(const std::shared_ptr<Model> model, const glm::mat4& transform, const std::shared_ptr<ShaderProgram> program, const glm::vec4& color = glm::vec4{ 1.0f });

	/**
	 * @brief Ends this 3D scene and flush the 3D renderer.
	 */
	static void End3DScene();

	/**
	 * @brief Primes the Renderer for rendering 2D. Cannot be done at the same time as 3D.
	 * 
	 * @param camera The orthographic 
	 * @param font The default font to use during this 2D scene.
	 */
	static void Begin2DScene(const Camera& camera, const std::shared_ptr<Font> font = nullptr);

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation = 0.0f);

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation = 0.0f);

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, const glm::vec4& tintColor, float rotation = 0.0f);

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, const glm::vec4& tintColor, float rotation = 0.0f);

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, const glm::vec3& tintColor, float rotation = 0.0f);

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param texture The texture to apply to the quad.
	 * @param color The color which is multiplied to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, const glm::vec3& tintColor, float rotation = 0.0f);

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param stpq The texture coordinates to take from the texture with 0, 0 being the bottom left.
	 * @param texture The texture to apply to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, float rotation = 0.0f);

	/**
	 * @brief Submit a single Quad for rendering.
	 * 
	 * @param pos The position of the center of the quad.
	 * @param size The width and height of the quad.
	 * @param stpq The texture coordinates to take from the texture with 0, 0 being the bottom left.
	 * @param texture The texture to apply to the quad.
	 * @param rotation The rotation about the z axis.
	 */
	static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, float rotation = 0.0f);

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
	static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, const glm::vec4& tintColor, float rotation = 0.0f);

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
	static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, const glm::vec4& tintColor, float rotation = 0.0f);

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
	static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, const glm::vec3& tintColor, float rotation = 0.0f);

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
	static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, const glm::vec3& tintColor, float rotation = 0.0f);

	/**
	 * @brief Change the default font.
	 * 
	 * @param font The font to change to.
	 */
	inline static void ChangeFont(const std::shared_ptr<Font> font) {
		assert(Renderer::mode == Mode::BATCH && "Renderer did not begin 2D scene.");
		Renderer::CurrentFont = font;
	}

	/**
	 * @brief Get the default Font.
	 * 
	 * @return The default font. 
	 */
	inline static std::shared_ptr<Font> GetFont() {
		return Renderer::CurrentFont;
	}

	/**
	 * @brief Submit a line of text for rendering.
	 * 
	 * @param text The text to be rendered.
	 * @param pos The position to render the text.
	 * @param color The color of the text when rendered.
	 * @param gravity The gravity applied to the text(changes position).
	 * @param scale The scale(multiplier of size) of the text.
	 * @param rotation The rotation about the z axis.
	 */
	inline static void SubmitText(const std::string& text, const glm::vec2& pos, const glm::vec3& color, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, glm::vec3{ pos, 0.0f }, glm::vec4{ color, 1.0f }, gravity, scale, rotation);
	}

	/**
	 * @brief Submit a line of text for rendering.
	 * 
	 * @param text The text to be rendered.
	 * @param pos The position to render the text.
	 * @param color The color of the text when rendered.
	 * @param gravity The gravity applied to the text(changes position).
	 * @param scale The scale(multiplier of size) of the text.
	 * @param rotation The rotation about the z axis.
	 */
	inline static void SubmitText(const std::string& text, const glm::vec2& pos, const glm::vec4& color, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, glm::vec3{ pos, 0.0f }, color, gravity, scale, rotation);
	}
	
	/**
	 * @brief Submit a line of text for rendering.
	 * 
	 * @param text The text to be rendered.
	 * @param pos The position to render the text.
	 * @param color The color of the text when rendered.
	 * @param gravity The gravity applied to the text(changes position).
	 * @param scale The scale(multiplier of size) of the text.
	 * @param rotation The rotation about the z axis.
	 */
	inline static void SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec3& color, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, pos, glm::vec4{ color, 1.0f }, gravity, scale, rotation);
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
	inline static void SubmitText(const std::string& text, const glm::vec2& pos, const glm::vec3& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, glm::vec3{ pos, 0.0f }, glm::vec4{ color, 1.0f }, font, gravity, scale, rotation);
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
	inline static void SubmitText(const std::string& text, const glm::vec2& pos, const glm::vec4& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, glm::vec3{ pos, 0.0f }, color, font, gravity, scale, rotation);
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
	inline static void SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec3& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, pos, glm::vec4{ color, 1.0f }, font, gravity, scale, rotation);
	}

	/**
	 * @brief Submit a line of text for rendering.
	 * 
	 * @param text The text to be rendered.
	 * @param pos The position to render the text.
	 * @param color The color of the text when rendered.
	 * @param gravity The gravity applied to the text(changes position).
	 * @param scale The scale(multiplier of size) of the text.
	 * @param rotation The rotation about the z axis.
	 */
	static void SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec4& color, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f);
	
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
	static void SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec4& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f);

	/**
	 * @brief Ends this 2D scene and flush the 2D renderer.
	 */
	static void End2DScene();

	/**
	 * @brief Destroy all of the rendering context.
	 */
	static void Shutdown();
};

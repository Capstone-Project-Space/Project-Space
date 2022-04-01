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
	static void Begin3DScene(const Camera& camera);

	static void SubmitLightSource(const LightSource& light);
	static void SubmitModel(const std::shared_ptr<Model> model, const glm::mat4& transform);

	static void End3DScene();

	static void Begin2DScene(const Camera& camera, const std::shared_ptr<Font> font = nullptr);

	static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation = 0.0f);
	static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation = 0.0f);

	inline static void ChangeFont(const std::shared_ptr<Font> font) {
		assert(Renderer::mode == Mode::BATCH && "Renderer did not begin 2D scene.");
		Renderer::CurrentFont = font;
	}

	inline static void SubmitText(const std::string& text, const glm::vec2& pos, const glm::vec3& color, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, glm::vec3{ pos, 0.0f }, glm::vec4{ color, 1.0f }, gravity, scale, rotation);
	}

	inline static void SubmitText(const std::string& text, const glm::vec2& pos, const glm::vec4& color, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, glm::vec3{ pos, 0.0f }, color, gravity, scale, rotation);
	}
	
	inline static void SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec3& color, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, pos, glm::vec4{ color, 1.0f }, gravity, scale, rotation);
	}


	inline static void SubmitText(const std::string& text, const glm::vec2& pos, const glm::vec3& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, glm::vec3{ pos, 0.0f }, glm::vec4{ color, 1.0f }, font, gravity, scale, rotation);
	}

	inline static void SubmitText(const std::string& text, const glm::vec2& pos, const glm::vec4& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, glm::vec3{ pos, 0.0f }, color, font, gravity, scale, rotation);
	}

	inline static void SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec3& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, pos, glm::vec4{ color, 1.0f }, font, gravity, scale, rotation);
	}

	static void SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec4& color, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f);
	static void SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec4& color, const std::shared_ptr<Font> font, Gravity gravity = Gravity::LEFT, float scale = 1.0f, float rotation = 0.0f);

	static void End2DScene();

	static void Shutdown();
};

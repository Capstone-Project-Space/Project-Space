#pragma once

#include <src/engine/graphics/batch_renderer.h>
#include <src/engine/graphics/model_renderer.h>
#include <src/engine/graphics/text_renderer.h>

struct Renderer {
private:
	static bool ModelRendererInitialized;
	static ModelRenderer ModelRenderer;
	
	static bool BatchRendererInitialized;
	static BatchRenderer BatchRenderer;
	static TextRenderer TextRenderer;

	enum class Mode { NONE, MODEL, BATCH };
	static Mode mode;

public:
	static void Begin3DScene(const Camera& camera);

	static void SubmitLightSource(const LightSource& light);
	static void SubmitModel(const std::shared_ptr<Model> model, const glm::mat4& transform);

	static void End3DScene();

	static void Begin2DScene(const Camera& camera);

	static void SubmitQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation = 0.0f);
	static void SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation = 0.0f);

	inline static void SubmitText(const std::string& text, const glm::vec2& pos, const glm::vec3& color, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, glm::vec3{ pos, 0.0f }, glm::vec4{ color, 1.0f }, scale, rotation);
	}

	inline static void SubmitText(const std::string& text, const glm::vec2& pos, const glm::vec4& color, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, glm::vec3{ pos, 0.0f }, color, scale, rotation);
	}
	
	inline static void SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec3& color, float scale = 1.0f, float rotation = 0.0f) {
		SubmitText(text, pos, glm::vec4{ color, 1.0f }, scale, rotation);
	}

	static void SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec4& color, float scale = 1.0f, float rotation = 0.0f);

	static void End2DScene();

	static void Shutdown();
};

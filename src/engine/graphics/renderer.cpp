#include "renderer.h"

#include <glad/glad.h>

bool Renderer::ModelRendererInitialized = false;
ModelRenderer Renderer::ModelRenderer;

bool Renderer::BatchRendererInitialized = false;
BatchRenderer Renderer::BatchRenderer;
TextRenderer Renderer::TextRenderer{1280.0f, 720.0f};

Renderer::Mode Renderer::mode;

void Renderer::Begin3DScene(const Camera& camera) {
	assert(Renderer::mode == Mode::NONE && "Renderer is already in a scene; missing end scene.");
	if (!Renderer::ModelRendererInitialized) {
		Renderer::ModelRenderer.init();
		Renderer::ModelRendererInitialized = true;
	}
	Renderer::mode = Mode::MODEL;
	Renderer::ModelRenderer.setCamera(camera);
}

void Renderer::SubmitLightSource(const LightSource& light) {
	assert(Renderer::mode == Mode::MODEL && "Renderer did not begin a 3D scene.");
	Renderer::ModelRenderer.submitLight(light);
}

void Renderer::SubmitModel(const std::shared_ptr<Model> model, const glm::mat4& transform) {
	assert(Renderer::mode == Mode::MODEL && "Renderer did not begin a 3D scene.");
	Renderer::ModelRenderer.submitModel(model, transform);
}

void Renderer::End3DScene() {
	assert(Renderer::mode == Mode::MODEL && "Renderer did not begin a 3D scene.");
	Renderer::ModelRenderer.draw();
	Renderer::mode = Mode::NONE;
}


void Renderer::Begin2DScene(const Camera& camera) {
	assert(Renderer::mode == Mode::NONE && "Renderer is already in a scene; missing end scene.");
	if (!Renderer::BatchRendererInitialized) {
		Renderer::BatchRenderer.init();
		Renderer::TextRenderer.init();
		Renderer::BatchRendererInitialized = true;
	}
	Renderer::mode = Mode::BATCH;
	Renderer::BatchRenderer.setCamera(camera);
	glDisable(GL_DEPTH_TEST);
}

void Renderer::SubmitQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation) {
	assert(Renderer::mode == Mode::BATCH && "Renderer did not begin 2D scene.");
	Renderer::BatchRenderer.submitQuad(pos, size, texture, rotation);
}

void Renderer::SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation) {
	assert(Renderer::mode == Mode::BATCH && "Renderer did not begin 2D scene.");
	Renderer::BatchRenderer.submitQuad(pos, size, texture, rotation);
}

void Renderer::SubmitText(const std::string& text, const glm::vec3& pos, const glm::vec4& color, float scale, float rotation) {
	assert(Renderer::mode == Mode::BATCH && "Renderer did not begin 2D scene.");
	Renderer::TextRenderer.submitText(text, pos, color, scale, rotation);
}

void Renderer::End2DScene() {
	assert(Renderer::mode == Mode::BATCH && "Renderer did not begin 2D scene.");
	Renderer::BatchRenderer.draw();
	Renderer::TextRenderer.draw();
	glEnable(GL_DEPTH_TEST);
	Renderer::mode = Mode::NONE;
}

void Renderer::Shutdown() {
	Renderer::ModelRendererInitialized = false;
	Renderer::ModelRenderer.destroy();
	Renderer::BatchRendererInitialized = false;
	Renderer::BatchRenderer.destroy();
	Renderer::TextRenderer.destroy();
}

#include "renderer.h"

#include <glad/glad.h>

bool Renderer::ModelRendererInitialized = false;
ModelRenderer Renderer::ModelRenderer;

bool Renderer::BatchRendererInitialized = false;
BatchRenderer Renderer::BatchRenderer;

Renderer::Mode Renderer::mode;

 void Renderer::Begin3DScene(const Camera& camera) {
	assert(Renderer::mode == NONE && "Renderer is already in a scene; missing end scene.");
	if (!Renderer::ModelRendererInitialized) {
		Renderer::ModelRenderer.init();
		Renderer::ModelRendererInitialized = true;
	}
	Renderer::mode = MODEL;
	Renderer::ModelRenderer.setCamera(camera);
}

 void Renderer::SubmitLightSource(const LightSource& light) {
	 assert(Renderer::mode == MODEL && "Renderer did not begin a 3D scene.");
	 Renderer::ModelRenderer.submitLight(light);
}

 void Renderer::SubmitModel(const std::shared_ptr<Model> model, const glm::mat4& transform) {
	 assert(Renderer::mode == MODEL && "Renderer did not begin a 3D scene.");
	 Renderer::ModelRenderer.submitModel(model, transform);
}

 void Renderer::End3DScene() {
	 assert(Renderer::mode == MODEL && "Renderer did not begin a 3D scene.");
	 Renderer::ModelRenderer.draw();
	 Renderer::mode = NONE;
}


 void Renderer::Begin2DScene(const Camera& camera) {
	 assert(Renderer::mode == NONE && "Renderer is already in a scene; missing end scene.");
	 if (!Renderer::BatchRendererInitialized) {
		 Renderer::BatchRenderer.init();
		 Renderer::BatchRendererInitialized = true;
	 }
	 Renderer::mode = BATCH;
	 Renderer::BatchRenderer.setCamera(camera);
	 glDisable(GL_DEPTH_TEST);
}

 void Renderer::SubmitQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation) {
	 assert(Renderer::mode == BATCH && "Renderer did not begin 2D scene.");
	 Renderer::BatchRenderer.submitQuad(pos, size, texture, rotation);
}

 void Renderer::SubmitQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture> texture, float rotation) {
	 assert(Renderer::mode == BATCH && "Renderer did not begin 2D scene.");
	 Renderer::BatchRenderer.submitQuad(pos, size, texture, rotation);
}

 void Renderer::End2DScene() {
	 assert(Renderer::mode == BATCH && "Renderer did not begin 2D scene.");
	 Renderer::BatchRenderer.draw();
	 glEnable(GL_DEPTH_TEST);
	 Renderer::mode = NONE;
}

 void Renderer::Shutdown() {
	 Renderer::ModelRendererInitialized = false;
	 Renderer::ModelRenderer.destroy();
	 Renderer::BatchRendererInitialized = false;
	 Renderer::BatchRenderer.destroy();
 }

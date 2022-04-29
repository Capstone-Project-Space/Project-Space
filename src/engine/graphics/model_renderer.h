#pragma once

#include <array>
#include <memory>

#include <src/engine/graphics/model.h>
#include <src/engine/graphics/buffers.h>
#include <src/engine/graphics/vertex_array.h>
#include <src/engine/graphics/shader.h>
#include <src/engine/graphics/camera.h>
#include <src/engine/graphics/light.h>

#include <glm/glm.hpp>

constexpr uint64_t MAX_LIGHTS = 64;
constexpr uint64_t MAX_INSTANCES = 512;

class ModelRenderer {
public:
	ModelRenderer();

	void init();
	void destroy();

	void submitLight(const LightSource& lightData);
	inline void submitModel(std::shared_ptr<Model> model, const glm::mat4& modelTransform, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }) {
		submitModel(model, modelTransform, this->program, color);
	}
	void submitModel(std::shared_ptr<Model> model, const glm::mat4& modelTransform, std::shared_ptr<ShaderProgram> shader, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	void draw();

	inline void setCamera(const Camera& camera) { this->camera = &camera; }

private:
	const Camera* camera;

	struct UniformData {
		glm::mat4 transform = glm::mat4{0.0f};
		glm::vec4 color = glm::vec4{0.0f};
	};
	std::map<std::shared_ptr<ShaderProgram>, std::map<std::shared_ptr<Model>, std::vector<UniformData>>> modelsToRender;
	
	struct UniformMatrixBuffer {
		glm::mat4 projection = glm::mat4{0.0f}, view = glm::mat4{ 0.0f };
		UniformData modelData[MAX_INSTANCES];
	} matricesBufferData;
	struct {
		LightSource lightsData[MAX_LIGHTS] = {{}};
		int32_t count = 0;
	} lightBufferData;
	std::shared_ptr<UniformBuffer> lightsBuffer = nullptr;
	std::shared_ptr<UniformBuffer> matricesBuffer = nullptr;
	std::shared_ptr<VertexArray> vArray = nullptr;
	std::shared_ptr<ShaderProgram> program = nullptr;

};

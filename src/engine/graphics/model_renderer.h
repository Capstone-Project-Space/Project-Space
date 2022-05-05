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

	/**
	 * @brief Initialize all of the graphics objects.
	 */
	void init();

	/**
	 * @brief Destroy all of the graphics objects.
	 */
	void destroy();

	/**
	 * @brief Submit a light to be rendered in this render pass.
	 * 
	 * @param lightData The data for this Light.s
	 */
	void submitLight(const LightSource& lightData);

	/**
	 * @brief Submit a Model for rendering.
	 * 
	 * @param model The Model to be rendered.
	 * @param modelTransform The transformation matrix to render the model with.
	 * @param color A tint color which will be multiplied by the models texture color.
	 */
	inline void submitModel(std::shared_ptr<Model> model, const glm::mat4& modelTransform, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }) {
		submitModel(model, modelTransform, this->program, color);
	}

	/**
	 * @brief Submit a Model for rendering.
	 * 
	 * @param model The model to be rendered.
	 * @param modelTransform The transformation matrix to render the model with.
	 * @param shader The ShaderProgram this model will use to render onto the screen.
	 * @param color A tint color which will be multipled by the models texture color.
	 */
	void submitModel(std::shared_ptr<Model> model, const glm::mat4& modelTransform, std::shared_ptr<ShaderProgram> shader, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	/**
	 * @brief Perform instance rendering for each Model with each ShaderProgram.
	 */
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

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
	ModelRenderer(const Camera& camera);

	void init();
	void destroy();

	void submitLight(const LightData& lightData);
	void submitModel(std::shared_ptr<Model> model, const glm::mat4& modelTransform);

	void draw();

private:
	const Camera* camera;

	std::map<std::shared_ptr<Model>, std::vector<glm::mat4>> modelsToRender;
	
	glm::mat4 matricesBufferData[MAX_INSTANCES + 2];
	struct {
		LightData lightsData[MAX_LIGHTS];
		int32_t count = 0;
	} lightBufferData;
	std::shared_ptr<UniformBuffer> lightsBuffer = nullptr;
	std::shared_ptr<UniformBuffer> matricesBuffer = nullptr;
	std::shared_ptr<VertexArray> vArray = nullptr;
	std::shared_ptr<ShaderProgram> program = nullptr;

};

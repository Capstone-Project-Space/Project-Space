#include "model_renderer.h"

#define LOG_GL_ERROR for (int glErrorGL = glGetError(); glErrorGL != 0;) { fprintf(stderr, "GLError: %d\n", glErrorGL); assert(false);}

const VertexLayout layout = {
	{"a_Vertex", ShaderDataType::Type::FLOAT3},
	{"a_UV", ShaderDataType::Type::FLOAT2},
	{"a_Normal", ShaderDataType::Type::FLOAT3},
	{"a_Texture", ShaderDataType::Type::INT},
	{"a_Ambient", ShaderDataType::Type::FLOAT3},
	{"a_Diffuse", ShaderDataType::Type::FLOAT3},
	{"a_Specular", ShaderDataType::Type::FLOAT3},
	{"a_SpecExponent", ShaderDataType::Type::FLOAT}
};

ModelRenderer::ModelRenderer() : camera(NULL) {}

void ModelRenderer::init() {
	this->vArray = VertexArray::CreateVertexArray(layout, nullptr);
	LOG_GL_ERROR;

	this->lightsBuffer = UniformBuffer::CreateUniformBuffer(1, sizeof(uint32_t) + (MAX_LIGHTS * sizeof(LightSource)));
	LOG_GL_ERROR;

	this->matricesBuffer = UniformBuffer::CreateUniformBuffer(MAX_INSTANCES * 2, sizeof(glm::mat4));
	LOG_GL_ERROR;

	this->program = ShaderProgram::Create("./resources/shaders/default_model.vert", "./resources/shaders/default_model.frag");
	LOG_GL_ERROR;

	this->program->bind();
	for (uint32_t i = 0; i < 32; i++) {
		program->uploadInt("u_Texture" + std::to_string(i), i);
	}
	LOG_GL_ERROR;
}

void ModelRenderer::destroy() {
	this->vArray = nullptr;
	this->lightsBuffer = nullptr;
	this->matricesBuffer = nullptr;
	this->program = nullptr;
	this->modelsToRender.clear();
}

void ModelRenderer::submitLight(const LightSource& lightData) {
	if (this->lightBufferData.count >= MAX_LIGHTS) {
		fprintf(stderr, "Cannot add more than %llu lights...Skipping.\n", MAX_LIGHTS);
	} else {
		lightBufferData.lightsData[lightBufferData.count++] = lightData;
	}
}

void ModelRenderer::submitModel(std::shared_ptr<Model> model, const glm::mat4& modelTransform, std::shared_ptr<ShaderProgram> shader, const glm::vec4& color) {
	auto& shaderSubmissions = modelsToRender[shader];
	auto& submissions = shaderSubmissions[model];
	submissions.push_back({ modelTransform, color });
}


void ModelRenderer::draw() {
	matricesBufferData.projection = this->camera->getProjection();
	matricesBufferData.view = this->camera->getView();
	for (auto&[program, modelSubmissions] : this->modelsToRender) {
		program->bind();
		program->bindUniformBuffer("Matrices", matricesBuffer);
		lightsBuffer->updateBuffer(&this->lightBufferData);
		program->bindUniformBuffer("Lights", lightsBuffer);
		for (auto& [model, submissions] : modelSubmissions) {
			uint64_t globalIndex = 0;
			uint64_t count = submissions.size();
			vArray->addVertexBuffer(layout, model->getVertices());
			model->bind();
			while (count > MAX_INSTANCES) {
				for (uint64_t i = 0; i < MAX_INSTANCES; i++) {
					matricesBufferData.modelData[i] = submissions[i + globalIndex];
				}
				matricesBuffer->updateBuffer(&matricesBufferData);
				glDrawElementsInstanced(GL_TRIANGLES, model->getIndices()->getCount(), GL_UNSIGNED_INT, NULL, MAX_INSTANCES);
				LOG_GL_ERROR;
				count -= MAX_INSTANCES;
				globalIndex += MAX_INSTANCES;
			}
			if (count > 0) {
				for (uint64_t i = 0; i < count; i++) {
					matricesBufferData.modelData[i] = submissions[i + globalIndex];
				}
				matricesBuffer->updateBuffer(&matricesBufferData);
				glDrawElementsInstanced(GL_TRIANGLES, model->getIndices()->getCount(), GL_UNSIGNED_INT, NULL, count);
				LOG_GL_ERROR;
			}
			submissions.clear();
		}
	}
	lightBufferData.count = 0;
}

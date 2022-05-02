#include "batch_renderer.h"

#include <glm/gtc/matrix_transform.hpp>

BatchRenderer::BatchRenderer() : camera(NULL), count(0), textureCount(0) {}

void BatchRenderer::init() {
	this->quads = new QuadData[MAX_QUADS];
	this->indexData = new uint32_t[MAX_INDICES];

	for (uint32_t i = 0; i < MAX_QUADS; i++) {
		for (uint32_t j = 0; j < 3; j++) {
			this->indexData[i * 6 + j] = j + (i * 4);
			this->indexData[i * 6 + j + 3] = (2 + j + (i * 4));
		}
		this->indexData[i * 6 + 5] = i * 4;
	}

	this->indices = IndexBuffer::CreateIndexBuffer(MAX_INDICES * sizeof(uint32_t), this->indexData);
	this->vertices = VertexBuffer::CreateVertexBuffer(MAX_QUADS * sizeof(QuadData));

	this->vArray = VertexArray::CreateVertexArray(
		{
			{ "a_Position",		ShaderDataType::Type::FLOAT2 },
			{ "a_UV",			ShaderDataType::Type::FLOAT2 },
			{ "a_Color",		ShaderDataType::Type::FLOAT4 },
			{ "a_Texture",		ShaderDataType::Type::INT },
			{ "a_Transform",	ShaderDataType::Type::MAT4 }
		}, this->vertices
	);

	this->program = ShaderProgram::Create("./resources/shaders/default_batch.vert", "./resources/shaders/default_batch.frag");
	this->program->bind();
	for (uint32_t i = 0; i < 32; i++) {
		program->uploadInt("u_Texture" + std::to_string(i), i);
	}
	delete[] this->indexData;
}

void BatchRenderer::destroy() {
	delete[] this->quads;
	this->indices = nullptr;
	this->vertices = nullptr;
	this->vArray = nullptr;
	this->program = nullptr;
	this->textures.fill(nullptr);
}

void BatchRenderer::submitQuad(QuadData data, const std::shared_ptr<Texture> texture) {
	// Don't overflow the vertex buffer.
	if (this->count >= MAX_QUADS) {
		this->draw();
	}

	// Don't overflow the texture buffer.
	auto found = std::find(textures.begin(), textures.end(), texture);
	auto idx = found - textures.begin();
	if (idx == 31) {
		if (this->textureCount >= 31)
			this->draw();
		idx = textureCount;
		this->textures[idx] = texture;
		textureCount++;
	}
	for (auto& vertex : data.vertices) {
		vertex.texture = idx + 1;
	}

	this->quads[this->count++] = data;
}

void BatchRenderer::submitQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& stpq, const std::shared_ptr<Texture> texture, const glm::vec4& tintColor, float rotation) {
	const glm::mat4 transform = glm::rotate(glm::scale(glm::translate(glm::identity<glm::mat4>(), pos), { size, 0.0f }), glm::radians(rotation), {0.0f, 0.0f, 1.0f});
	submitQuad(QuadData
		{
			QuadVertex{ { -.5f, -.5f },	{ stpq.s, stpq.t }, tintColor, 0, transform },
			QuadVertex{ { 0.5f, -.5f },	{ stpq.p, stpq.t }, tintColor, 0, transform },
			QuadVertex{ { 0.5f, 0.5f },	{ stpq.p, stpq.q }, tintColor, 0, transform },
			QuadVertex{ { -.5f, 0.5f },	{ stpq.s, stpq.q }, tintColor, 0, transform }
		}, texture
	);
}

void BatchRenderer::draw() {
	if (this->count == 0) return;
	this->vertices->updateBuffer(sizeof(QuadData) * this->count, this->quads, 0);
	this->vArray->bind();
	this->indices->bind();
	for (uint32_t i = 0; i < this->textureCount; i++) {
		this->textures[i]->bind(i + 1);
	}
	this->program->bind();

	this->program->uploadMat4("u_Projection", this->camera->getProjection());
	this->program->uploadMat4("u_View", this->camera->getView());

	glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_INT, NULL);
	this->count = 0;
	this->textureCount = 0;
	this->textures.fill(nullptr);
}

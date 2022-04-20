#include "text_renderer.h"

#include <src/engine/io/event.h>

#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

TextRenderer::TextRenderer() : camera(NULL), count(0) {}

void TextRenderer::init() {
	this->quads = new TextQuad[TEXT_MAX_QUADS];
	this->indexData = new uint32_t[TEXT_MAX_INDICES];

	for (uint32_t i = 0; i < TEXT_MAX_QUADS; i++) {
		for (uint32_t j = 0; j < 3; j++) {
			this->indexData[i * 6 + j] = j + (i * 4);
			this->indexData[i * 6 + j + 3] = (2 + j + (i * 4));
		}
		this->indexData[i * 6 + 5] = i * 4;
	}

	this->indices = IndexBuffer::CreateIndexBuffer(TEXT_MAX_INDICES * sizeof(uint32_t), this->indexData);
	this->vertices = VertexBuffer::CreateVertexBuffer(TEXT_MAX_QUADS * sizeof(TextQuad));

	this->vArray = VertexArray::CreateVertexArray(
		{
			{ "a_Position",		ShaderDataType::Type::FLOAT3 },
			{ "a_UV",			ShaderDataType::Type::FLOAT2 },
			{ "a_Font",			ShaderDataType::Type::INT },
			{ "a_Color",		ShaderDataType::Type::FLOAT4 },
			{ "a_Transform",	ShaderDataType::Type::MAT4 }
		}, this->vertices
	);

	this->program = ShaderProgram::Create("./resources/shaders/default_text.vert", "./resources/shaders/default_text.frag");
	this->program->bind();
	for (uint32_t i = 0; i < 32; i++) {
		this->program->uploadInt("u_Texture" + std::to_string(i), i);
	}

	delete[] this->indexData;
}

void TextRenderer::destroy() {
	delete[] this->quads;
	this->vertices = nullptr;
	this->indices = nullptr;
	this->vArray = nullptr;
	this->program = nullptr;
	this->fonts.fill(nullptr);
}

void TextRenderer::submitText(const std::string& text, const glm::vec3& pos, const glm::vec4& color, const std::shared_ptr<Font> font, Gravity gravity, float scale, float rotation) {
	float height = font->getTextHeight(text) * scale;
	float x = pos.x, y = pos.y - height;

	if ((gravity & Gravity::CENTER_HORIZONTAL) == Gravity::CENTER_HORIZONTAL) {
		x -= (font->getTextWidth(text) * scale) / 2.0f;
	} else if (gravity & Gravity::RIGHT) {
		x -= (font->getTextWidth(text) * scale);
	}
	if ((gravity & Gravity::CENTER_VERTICAL) == Gravity::CENTER_VERTICAL) {
		y += (height) / 2.0f;
	} else if (gravity & Gravity::TOP) {
		y -= (height);
	}

	// Don't overflow the texture buffer.
	auto found = std::find(fonts.begin(), fonts.end(), font);
	auto idx = found - fonts.begin();
	if (idx == 32) {
		if (this->fontCount >= 32)
			this->draw();
		idx = fontCount;
		this->fonts[idx] = font;
		fontCount++;
	}

	const float cosine = cos(glm::radians(rotation));
	const float sine = sin(glm::radians(rotation));
	const glm::mat4 transform = /*glm::identity<glm::mat4>();*/glm::translate(
		glm::mat4{ {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {x, y, 0.0f, 1.0f} } *
		glm::mat4{ {cosine, sine, 0.0f, 0.0f}, {-sine, cosine, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} },
		{-x, -y, 0.0f}
	);
	
	for (const auto& c : text) {
		auto& ch = font->getCharacterData(c);

		// Don't overflow the quad buffer.
		if (this->count >= TEXT_MAX_QUADS) {
			this->draw();
		}

		// Put the character quad into the quads buffer.
		float xpos = x + ch.offset.x * scale;
		float ypos = y - (ch.size.y - ch.offset.y) * scale;

		float wpos = xpos + ch.size.x * scale;
		float hpos = ypos - ch.size.y * scale;

		this->quads[count++] = TextQuad{
			TextVertex{ { xpos, ypos, pos.z }, { ch.stpq.s, ch.stpq.t }, (uint32_t) idx, color, transform },
			TextVertex{ { xpos, hpos, pos.z }, { ch.stpq.s, ch.stpq.q }, (uint32_t) idx, color, transform },
			TextVertex{ { wpos, hpos, pos.z }, { ch.stpq.p, ch.stpq.q }, (uint32_t) idx, color, transform },
			TextVertex{ { wpos, ypos, pos.z }, { ch.stpq.p, ch.stpq.t }, (uint32_t) idx, color, transform }
		};
		x += ch.advance * scale;
	}
}

void TextRenderer::draw() {
	if (count == 0) return;
	this->vertices->updateBuffer(sizeof(TextQuad) * this->count, this->quads, 0);
	this->vArray->bind();
	this->indices->bind();
	for (uint32_t i = 0; i < fontCount; i++) this->fonts[i]->bind(i);
	this->program->bind();

	this->program->uploadMat4("u_Projection", this->camera->getProjection());
	this->program->uploadMat4("u_View", this->camera->getView());

	glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_INT, NULL);
	this->count = 0;
	this->fontCount = 0;
	this->fonts.fill(nullptr);
}


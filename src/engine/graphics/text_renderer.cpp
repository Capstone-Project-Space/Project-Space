#include "text_renderer.h"

#include <numeric>

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

void TextRenderer::submitText(const std::string& rawtext, const glm::vec3& pos, const glm::vec4& color, const std::shared_ptr<Font> font, Gravity gravity, const glm::vec2& scale, float rotation) {
	float startX = pos.x, startY = pos.y;
	
	size_t lineCount = std::accumulate(rawtext.begin(), rawtext.end(), 1, [](size_t count, char c) { return count + (c == '\n'); });
	float height = font->getNewLineHeight() * scale.y * lineCount;
	
	if ((gravity & Gravity::CENTER_VERTICAL) == Gravity::CENTER_VERTICAL) {
		startY += (height) / 2.0f - height / (float) lineCount;
	} else if (gravity & Gravity::TOP) {
		startY -= (height / (float) lineCount);
	}

	height /= (float) lineCount;
	std::size_t offset = 0;
	while (offset < rawtext.size()) {
		std::size_t size = rawtext.find_first_of('\n', offset);
		std::string text = rawtext.substr(offset, size - offset);
		offset += text.size()+1;

		float x = startX, y = startY;

		if ((gravity & Gravity::CENTER_HORIZONTAL) == Gravity::CENTER_HORIZONTAL) {
			x -= (font->getTextWidth(text) * scale.x) / 2.0f;
		}
		else if (gravity & Gravity::RIGHT) {
			x -= (font->getTextWidth(text) * scale.x);
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
		const glm::mat4 transform = glm::translate(
			glm::mat4{ {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {x, y, 0.0f, 1.0f} } *
			glm::mat4{ {cosine, sine, 0.0f, 0.0f}, {-sine, cosine, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} },
			{ -x, -y, 0.0f }
		);

		for (const auto& c : text) {
			auto& ch = font->getCharacterData(c);

			// Don't overflow the quad buffer.
			if (this->count >= TEXT_MAX_QUADS) {
				this->draw();
			}
			// Put the character quad into the quads buffer.
			float xpos = x + ch.offset.x * scale.x;
			float ypos = y - (ch.size.y - ch.offset.y) * scale.y;

			float wpos = xpos + ch.size.x * scale.x;
			float hpos = ypos + ch.size.y * scale.y;

			this->quads[count++] = TextQuad{
				TextVertex{ { xpos, hpos, pos.z }, { ch.stpq.s, ch.stpq.t }, (uint32_t)idx, color, transform },
				TextVertex{ { xpos, ypos, pos.z }, { ch.stpq.s, ch.stpq.q }, (uint32_t)idx, color, transform },
				TextVertex{ { wpos, ypos, pos.z }, { ch.stpq.p, ch.stpq.q }, (uint32_t)idx, color, transform },
				TextVertex{ { wpos, hpos, pos.z }, { ch.stpq.p, ch.stpq.t }, (uint32_t)idx, color, transform }
			};
			x += ch.advance * scale.x;
		}
		startY -= height;
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


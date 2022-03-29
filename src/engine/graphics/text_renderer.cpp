#include "text_renderer.h"

#include <src/engine/io/event.h>

#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(float width, float height) : camera(Camera(0, width, 0, height)), count(0), textureCount(0)  {}

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
			{ "a_Color",		ShaderDataType::Type::FLOAT4 },
			{ "a_Texture",		ShaderDataType::Type::INT },
			{ "a_Transform",	ShaderDataType::Type::MAT4 }
		}, this->vertices
	);

	this->program = ShaderProgram::Create("./resources/shaders/default_text.vert", "./resources/shaders/default_text.frag");
	this->program->bind();
	for (uint32_t i = 0; i < 32; i++) {
		program->uploadInt("u_Texture" + std::to_string(i), i);
	}


	// TODO: Consider moving this to a Font class.
	FT_Library ft;
	FT_Face face;
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "ERROR::FREETYPE: Could not init FreeType Library\n");
		exit(1);
	}

	if (FT_New_Face(ft, "./resources/fonts/Movement.ttf", 0, &face)) {
		fprintf(stderr, "Error::FREETYPE: Failed to load font.\n");
		exit(1);
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int c = 0; c < 256; c++) {
		if (FT_Load_Char(face, (unsigned char) c, FT_LOAD_RENDER)) {
			continue;
		}
		uint32_t texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
			face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		this->fontCharacters[c] = {
			texture, 
			{face->glyph->bitmap.width, face->glyph->bitmap.rows},
			{face->glyph->bitmap_left, face->glyph->bitmap_top},
			(uint32_t) face->glyph->advance.x
		};
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	Events::RegisterReceiver(this);
	delete[] this->indexData;
}

void TextRenderer::destroy() {
	delete[] this->quads;
	this->vertices = nullptr;
	this->indices = nullptr;
	this->vArray = nullptr;
	this->program = nullptr;
}

void TextRenderer::submitText(const std::string& text, const glm::vec3& pos, const glm::vec4& color, float scale, float rotation) {
	float x = pos.x, y = pos.y;
	const glm::mat4 transform = glm::rotate(glm::identity<glm::mat4>(), glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
	for (const auto& c : text) {
		auto ch = fontCharacters[c];

		// Don't overflow the quad buffer.
		if (this->count >= TEXT_MAX_QUADS) {
			this->draw();
		}

		// Find the texture and make sure we don't overflow the texture buffer.
		auto found = std::find(textures.begin(), textures.end(), ch.textureID);
		uint32_t idx = found - textures.begin();
		if (ch.textureID != 0) {
			if (idx == 31) {
				if (this->textureCount >= 31)
					this->draw();
				idx = textureCount;
				this->textures[idx] = ch.textureID;
				textureCount++;
			}
		} else idx = -1;

		// Put the character quad into the quads buffer.
		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		this->quads[count++] = TextQuad{
			TextVertex{ { xpos, ypos + h, pos.z},		{ 0.0f, 0.0f }, color, idx + 1, transform },
			TextVertex{ { xpos, ypos, pos.z },			{ 0.0f, 1.0f }, color, idx + 1, transform },
			TextVertex{ { xpos + w, ypos, pos.z },		{ 1.0f, 1.0f }, color, idx + 1, transform },
			TextVertex{ { xpos + w, ypos + h, pos.z},	{ 1.0f, 0.0f }, color, idx + 1, transform }
		};
		x += (ch.advance >> 6) * scale;
	}
}

void TextRenderer::draw() {
	if (count == 0) return;
	this->vertices->updateBuffer(sizeof(TextQuad) * this->count, this->quads, 0);
	this->vArray->bind();
	this->indices->bind();
	for (uint32_t i = 0; i < this->textureCount; i++) {
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
	}
	this->program->bind();

	this->program->uploadMat4("u_Projection", this->camera.getProjection());
	this->program->uploadMat4("u_View", this->camera.getView());

	glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_INT, NULL);
	this->count = 0;
	this->textureCount = 0;
	this->textures.fill(0);
}

void TextRenderer::onWindowResize(const float oldWidth, const float oldHeight, const float newWidth, const float newHeight) {
	this->camera = Camera(0.0f, newWidth, 0.0f, newHeight);
}

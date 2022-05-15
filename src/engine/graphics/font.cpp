#include "font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

std::shared_ptr<Font> Font::CreateFont(const std::string_view& filepath, unsigned char startChar, uint32_t length) {
	assert(255 - startChar <= length);
	std::shared_ptr<Font> font = std::shared_ptr<Font>(new Font(startChar, length));
	
	FT_Library ft;
	FT_Face face;
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "ERROR::FREETYPE: Could not init FreeType Library\n");
		exit(1);
	}

	if (FT_New_Face(ft, filepath.data(), 0, &face)) {
		fprintf(stderr, "Error::FREETYPE: Failed to load font.\n");
		exit(1);
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	FT_Pos maxSingleDim = std::max(face->size->metrics.height, face->size->metrics.max_advance);
	int max_dim = (1 + (maxSingleDim >> 6)) * 8;
	int tex_width = 1;
	while (tex_width < max_dim) tex_width <<= 1;
	int tex_height = tex_width;

	unsigned char* fontAtlas = (unsigned char*)calloc(tex_height * tex_width, sizeof(unsigned char));
	assert(fontAtlas);

	int x = 0, y = 0;
	uint32_t endChar = startChar + length;
	for (uint32_t c = startChar; c < endChar; c++) {
		if (FT_Load_Char(face, (unsigned char)c, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT)) {
			fprintf(stderr, "Couldn't load character with ascii '%hhu'.\n", c);
			continue;
		}
		FT_Bitmap* bmp = &face->glyph->bitmap;

		if (x + bmp->width >= tex_width) {
			x = 0;
			y += (face->size->metrics.height >> 6) + 1;
		}

		for (int row = 0; row < bmp->rows; row++) {
			for (int col = 0; col < bmp->width; col++) {
				uint32_t i = (y + row) * tex_width + (x + col);
				fontAtlas[i] = bmp->buffer[row * bmp->pitch + col];
			}
		}

		font->characters[c] = {
			{
				(float)x / (float)tex_width, (float)y / (float)tex_height,
				(float)(x + bmp->width) / (float)tex_width, (float)(y + bmp->rows) / (float)tex_height
			},
			{ bmp->width, bmp->rows },
			{face->glyph->bitmap_left, face->glyph->bitmap_top},
			(uint32_t)face->glyph->advance.x >> 6
		};
		x += bmp->width + 1;
	}

	glGenTextures(1, &font->id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->id);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RED, tex_width, tex_height,
		0, GL_RED, GL_UNSIGNED_BYTE, fontAtlas
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	free(fontAtlas);
	return font;
}

Font::Font(unsigned char startChar, uint32_t length)
	: id(0), startChar(startChar), endChar(startChar + length) {
	this->characters = new Character[length];
}

Font::~Font() {
	delete[] characters;
}

uint32_t Font::getTextWidth(const std::string& text) {
	uint32_t width = 0;
	for (const auto& c : text) {
		width += (this->getCharacterData(c).advance);
	}
	return width;
}

int32_t Font::getTextHeight(const std::string& text) {
	size_t lineCount = 1;
	int32_t height = 0;
	for (const auto& c : text) {
		lineCount += (c == '\n');
		height = std::max(this->getCharacterData(c).size.y, height);
	}
	return height * lineCount;
}

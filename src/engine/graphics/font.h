#pragma once

#include <stdint.h>
#include <string>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Character {
	glm::vec4 stpq{ 0.0f };
	glm::ivec2 size{ 0.0f };
	glm::ivec2 offset{ 0.0f };
	uint32_t advance = 0;
};

class Font {
public:
	static std::shared_ptr<Font> CreateFont(const std::string& filepath, unsigned char startChar, uint32_t length);

	~Font();

	inline void bind(uint32_t textureSlot) {
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	inline const Character& getCharacterData(unsigned char c) {
		assert(c >= startChar && c < endChar);
		return characters[c];
	}

	uint32_t getTextWidth(const std::string& text);
	int32_t getTextHeight(const std::string& text);

private:
	uint32_t id;
	uint32_t startChar, endChar;
	Character* characters;
	Font(unsigned char startChar, uint32_t length);
};

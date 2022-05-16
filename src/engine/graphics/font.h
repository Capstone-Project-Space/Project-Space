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
	/**
	 * @brief Create a Font.
	 * 
	 * @param filepath The file path to the font file.
	 * @param startChar The starting character to generate the font at.
	 * @param length The number of characters to generate the font at.
	 * @return A std::shared_ptr to a Font.
	 */
	static std::shared_ptr<Font> CreateFont(const std::string_view& filepath, unsigned char startChar, uint32_t length);

	~Font();

	/**
	 * @brief Bind the font texture to a texture slot.
	 * 
	 * @param textureSlot The texture slot to bind to.
	 */
	inline void bind(uint32_t textureSlot) const {
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	/**
	 * @brief Get the Character Data for a single character.
	 * 
	 * @param c The character we want the Character data for.
	 * @return The Character data for c.
	 */
	inline const Character& getCharacterData(unsigned char c) {
		assert(c >= startChar && c < endChar);
		return characters[c];
	}

	uint32_t getTextWidth(const std::string& text);
	int32_t getTextHeight(const std::string& text);

	uint32_t getFontWidth();
	int32_t getFontHeight();

	inline uint32_t getNewLineHeight() const { return newlineHeight; }

private:
	uint32_t id;
	uint32_t startChar, endChar;
	Character* characters;
	uint32_t newlineHeight;
	Font(unsigned char startChar, uint32_t length);
};

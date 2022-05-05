#pragma once

#include <map>
#include <memory>
#include <string>

#include <glm/glm.hpp>

class Texture {
public:
	/**
	 * @brief Create a Texture.
	 * 
	 * @param filepath The file path to the texture/image file.
	 * @return A std::shared_ptr to a Texture.
	 */
	static std::shared_ptr<Texture> CreateTexture(const std::string& filepath);

	/**
	 * @brief Create a Texture of a solid color.
	 * 
	 * @param color The color of the texture.
	 * @return A std::shared_ptr to a Texture.
	 */
	static std::shared_ptr<Texture> CreateColoredTexture(const glm::vec4& color);


	~Texture();

	/**
	 * @brief Bind the Texture to a texture slot.
	 * 
	 * @param slot The slot to bind the texture to.
	 */
	void bind(uint32_t slot) const;

private:
	uint32_t id;
	uint64_t width, height;
	uint32_t internalFormat, dataFormat;
	Texture(const glm::vec4& color);
	Texture(const std::string& filepath);
};

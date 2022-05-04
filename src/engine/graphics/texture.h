#pragma once

#include <map>
#include <memory>
#include <string>

#include <glm/glm.hpp>

class Texture {
public:
	static std::shared_ptr<Texture> CreateTexture(const std::string& filepath);
	static std::shared_ptr<Texture> CreateColoredTexture(const glm::vec4& color);


	~Texture();

	void bind(uint32_t id);

private:
	uint32_t id;
	uint64_t width, height;
	uint32_t internalFormat, dataFormat;
	Texture(const glm::vec4& color);
	Texture(const std::string& filepath);
};

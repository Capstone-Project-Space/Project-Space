#pragma once

#include <memory>
#include <string>

class Texture {
public:
	std::shared_ptr<Texture> CreateTexture(const std::string& filepath);

	~Texture();

private:
	uint32_t id;
	uint64_t width, height;
	uint32_t internalFormat, dataFormat;
	Texture(const std::string& filepath);
};

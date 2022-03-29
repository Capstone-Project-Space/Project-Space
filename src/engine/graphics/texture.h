#pragma once

#include <map>
#include <memory>
#include <string>

class Texture {
public:
	static std::shared_ptr<Texture> CreateTexture(const std::string& filepath);

	~Texture();

	void bind(uint32_t id);

private:
	uint32_t id;
	uint64_t width, height;
	uint32_t internalFormat, dataFormat;
	Texture(const std::string& filepath);
};

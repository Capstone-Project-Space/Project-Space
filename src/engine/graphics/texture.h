#pragma once

#include <map>
#include <memory>
#include <string>

class Texture {
public:
	static std::shared_ptr<Texture> CreateTexture(const std::string& filepath);
	static void Clear();

	~Texture();

	void bind(uint32_t id);

private:
	static std::map<std::string, std::shared_ptr<Texture>> mapping;
	uint32_t id;
	uint64_t width, height;
	uint32_t internalFormat, dataFormat;
	Texture(const std::string& filepath);
};

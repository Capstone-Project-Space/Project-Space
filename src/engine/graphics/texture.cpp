#include "texture.h"

#include <assert.h>

#include <glad/glad.h>
#include <stb_image.h>

std::map<std::string, std::shared_ptr<Texture>> Texture::mapping;

#define LOG_GL_ERROR for (int glErrorGL = glGetError(); glErrorGL != 0;) { fprintf(stderr, "GLError: %d\n", glErrorGL); assert(false);}

std::shared_ptr<Texture> Texture::CreateTexture(const std::string& filepath) {
	if (Texture::mapping.find(filepath) == Texture::mapping.end()) {
		std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(new Texture(filepath));
		mapping.insert({ filepath, texture });
		return texture;
	}
	return Texture::mapping[filepath];
}

void Texture::Clear() {
	Texture::mapping.clear();
}

Texture::~Texture() {
	glDeleteTextures(1, &this->id);
	LOG_GL_ERROR;
}

void Texture::bind(uint32_t id) {
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, id);
	LOG_GL_ERROR;
}

Texture::Texture(const std::string& filepath) {
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* data = nullptr;
	data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
	assert(data);
	this->width = width;
	this->height = height;

	internalFormat = 0;
	dataFormat = 0;
	if (channels == 4) {
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3) {
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	assert(internalFormat & dataFormat);

	glGenTextures(1, &this->id);
	LOG_GL_ERROR;
	glBindTexture(GL_TEXTURE_2D, this->id);
	LOG_GL_ERROR;
	glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
	LOG_GL_ERROR;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	LOG_GL_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	LOG_GL_ERROR;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	LOG_GL_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	LOG_GL_ERROR;
	stbi_image_free(data);
}

#include "texture.h"

#include <assert.h>

#include <glad/glad.h>
#include <stb_image.h>

std::shared_ptr<Texture> Texture::CreateTexture(const std::string& filepath) {
	std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(new Texture(filepath));
	return texture;
}

Texture::~Texture() {
	glDeleteTextures(1, &this->id);
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
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(this->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(this->id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	stbi_image_free(data);
}

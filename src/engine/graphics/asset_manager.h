#pragma once

#include <src/engine/graphics/texture.h>
#include <src/engine/graphics/model.h>
#include <src/engine/graphics/shader.h>

#include <unordered_map>
#include <filesystem>

struct AssetManager {
	
	static std::unordered_map<std::string, std::shared_ptr<Model>> models;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

	static void Clear();

	template<typename T>
	static std::shared_ptr<T> GetOrCreate(const std::string& filepath);

	template<>
	static std::shared_ptr<Model> GetOrCreate(const std::string& filepath) {
		const std::string key = std::filesystem::absolute(filepath).generic_string();
		if (models.find(key) != models.end())
			return models.at(key);
		return (models[key] = Model::CreateModel(filepath));
	}

	template<>
	static std::shared_ptr<Texture> GetOrCreate(const std::string& filepath) {
		const std::string key = std::filesystem::absolute(filepath).generic_string();
		if (textures.find(key) != textures.end())
			return textures.at(key);
		return (textures[key] = Texture::CreateTexture(filepath));
	}

};

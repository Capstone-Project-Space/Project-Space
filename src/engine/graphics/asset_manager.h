#pragma once

#include <src/engine/graphics/texture.h>
#include <src/engine/graphics/model.h>
#include <src/engine/graphics/shader.h>
#include <src/engine/graphics/font.h>

#include <unordered_map>
#include <filesystem>

namespace std {

	template<>
	struct hash<std::pair<std::string, std::string>> {
		size_t operator()(const std::pair<std::string, std::string>& p) const {
			return hash<string>()(p.first) ^ hash<string>()(p.second);
		}
	};

};

struct AssetManager {
	
	static std::unordered_map<std::string, std::shared_ptr<Font>> Fonts;
	static std::unordered_map<std::string, std::shared_ptr<Model>> Models;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> Textures;

	static std::unordered_map<std::pair<std::string, std::string>, std::shared_ptr<ShaderProgram>> ShaderPrograms;

	static void Clear();

	template<typename T>
	static std::shared_ptr<T> GetOrCreate(const std::string& filepath);
	template<typename T>
	static std::shared_ptr<T> GetOrCreate(const std::string& vertFilePath, const std::string& fragFilePath);
	template<typename T>
	static std::shared_ptr<T> GetOrCreate(const std::string& filepath, std::shared_ptr<Texture> texture);

	template<typename T>
	static void Unload(const std::string& filepath);
	template<typename T>
	static void Unload(const std::string& vertFilePath, const std::string& fragFilePath);

	template<>
	static std::shared_ptr<Model> GetOrCreate(const std::string& filepath) {
		const std::string key = std::filesystem::absolute(filepath).generic_string();
		if (Models.find(key) != Models.end())
			return Models.at(key);
		return (Models[key] = Model::CreateModel(filepath));
	}

	template<>
	static std::shared_ptr<Model> GetOrCreate(const std::string& filepath, std::shared_ptr<Texture> texture) {
		const std::string key = std::filesystem::absolute(filepath).generic_string();
		if (Models.find(key) != Models.end())
			return Models.at(key);
		return (Models[key] = Model::CreateModel(filepath, texture));
	}

	template<>
	static std::shared_ptr<Texture> GetOrCreate(const std::string& filepath) {
		const std::string key = std::filesystem::absolute(filepath).generic_string();
		if (Textures.find(key) != Textures.end())
			return Textures.at(key);
		return (Textures[key] = Texture::CreateTexture(filepath));
	}

	template<>
	static std::shared_ptr<Font> GetOrCreate(const std::string& filepath) {
		const std::string key = std::filesystem::absolute(filepath).generic_string();
		if (Fonts.find(key) != Fonts.end())
			return Fonts.at(key);
		return (Fonts[key] = Font::CreateFont(filepath, 0, 256));
	}

	template<>
	static std::shared_ptr<ShaderProgram> GetOrCreate(const std::string& vertFilePath, const std::string& fragFilePath) {
		const auto names = std::pair{ std::filesystem::absolute(vertFilePath).generic_string(), std::filesystem::absolute(fragFilePath).generic_string() };
		if (ShaderPrograms.find(names) != ShaderPrograms.end())
			return ShaderPrograms.at(names);
		return (ShaderPrograms[names] = ShaderProgram::Create(vertFilePath, fragFilePath));
	}

	template<>
	static void Unload<Model>(const std::string& filepath) {
		const std::string key = std::filesystem::absolute(filepath).generic_string();
		Models.erase(key);
	}

	template<>
	static void Unload<Texture>(const std::string& filepath) {
		const std::string key = std::filesystem::absolute(filepath).generic_string();
		Textures.erase(key);
	}

	template<>
	static void Unload<Font>(const std::string& filepath) {
		const std::string key = std::filesystem::absolute(filepath).generic_string();
		Fonts.erase(key);
	}

	template<>
	static void Unload<ShaderProgram>(const std::string& vertFilePath, const std::string& fragFilePath) {
		const auto names = std::pair{ std::filesystem::absolute(vertFilePath).generic_string(), std::filesystem::absolute(vertFilePath).generic_string() };
		ShaderPrograms.erase(names);
	}

};

#pragma once

#include <unordered_map>
#include <filesystem>

#include <src/engine/graphics/texture.h>
#include <src/engine/graphics/model.h>
#include <src/engine/graphics/shader.h>
#include <src/engine/graphics/font.h>

#include <glm/glm.hpp>

namespace std {

	template<>
	struct hash<std::pair<std::string, std::string>> {
		size_t operator()(const std::pair<std::string, std::string>& p) const {
			return hash<string>()(p.first) ^ hash<string>()(p.second);
		}
	};

	template<>
	struct hash<std::pair<std::string, std::shared_ptr<Texture>>> {
		size_t operator()(const std::pair<std::string, std::shared_ptr<Texture>>& p) const {
			return hash<string>()(p.first) ^ hash<shared_ptr<Texture>>()(p.second);
		}
	};

	template<>
	struct hash<glm::vec4> {
		size_t operator()(const glm::vec4& p) const {
			return hash<float>()(p.x) ^ hash<float>()(p.y) ^ hash<float>()(p.z) ^ hash<float>()(p.w);
		}
	};

};

struct AssetManager {
	
	static std::unordered_map<std::string, std::shared_ptr<Font>> Fonts;
	static std::unordered_map<std::pair<std::string, std::shared_ptr<Texture>>, std::shared_ptr<Model>> Models;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> Textures;
	static std::unordered_map<glm::vec4, std::shared_ptr<Texture>> ColoredTextures;

	static std::unordered_map<std::pair<std::string, std::string>, std::shared_ptr<ShaderProgram>> ShaderPrograms;

	/**
	 * @brief Wipe all of the cached assets. This needs to be done before the rendering context has been destroyed.
	 */
	static void Clear();

	/**
	 * @brief Get Or Create an asset from a file.
	 * 
	 * @tparam T The type of asset to retrieve from cache or load.
	 * @param filepath The file path.
	 * @return A std::shared_ptr to the object loaded from file.
	 */
	template<typename T>
	static std::shared_ptr<T> GetOrCreate(const std::string& filepath);

	/**
	 * @brief Get Or Create an asset from two files.
	 * 
	 * @tparam T The type of asset to retrieve from cache or load.
	 * @param vertFilePath The first filepath.
	 * @param fragFilePath The second filepath.
	 * @return A std::shared_ptr to the object loaded from the two files.
	 */
	template<typename T>
	static std::shared_ptr<T> GetOrCreate(const std::string& vertFilePath, const std::string& fragFilePath);

	/**
	 * @brief Get Or Create an asset from a file and a Texture asset.
	 * 
	 * @tparam T The type of asset to retrieve from cache or load.
	 * @param filepath The filepath.
	 * @param texture The Texture.
	 * @return A std::shared_ptr to the object loaded from the file and Texture.
	 */
	template<typename T>
	static std::shared_ptr<T> GetOrCreate(const std::string& filepath, std::shared_ptr<Texture> texture);

	/**
	 * @brief Get Or Create an asset from a 4 component vector.
	 * 
	 * @tparam T The type of asset to retrieve from cache or load.
	 * @param color The 4 component vector.
	 * @return A std::shared_ptr to the object created from the 4 component vector.
	 */
	template<typename T>
	static std::shared_ptr<T> GetOrCreate(const glm::vec4& color);

	/**
	 * @brief Unload a specific asset by the single file used to create it.
	 * 
	 * @tparam T The type of asset to unload from the cache.
	 * @param filepath The file path used to initialize the object.
	 */
	template<typename T>
	static void Unload(const std::string& filepath);

	/**
	 * @brief Unload a specific asset by the two files used to create it.
	 * 
	 * @tparam T The type of asset to unload from the cache.
	 * @param vertFilePath The first file path used to initialize the object.
	 * @param fragFilePath The second file path used to initialize the object.
	 */
	template<typename T>
	static void Unload(const std::string& vertFilePath, const std::string& fragFilePath);

	template<>
	static std::shared_ptr<Model> GetOrCreate(const std::string& filepath) {
		const auto key = std::pair{ std::filesystem::absolute(filepath).generic_string(), nullptr };
		if (Models.find(key) != Models.end())
			return Models.at(key);
		return (Models[key] = Model::CreateModel(filepath));
	}

	template<>
	static std::shared_ptr<Model> GetOrCreate(const std::string& filepath, std::shared_ptr<Texture> texture) {
		const auto key = std::pair{ std::filesystem::absolute(filepath).generic_string(), texture };
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
	static std::shared_ptr<Texture> GetOrCreate(const glm::vec4& color) {
		if (ColoredTextures.find(color) != ColoredTextures.end())
			return ColoredTextures.at(color);
		return (ColoredTextures[color] = Texture::CreateColoredTexture(color));
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
		const auto key = std::pair{ std::filesystem::absolute(filepath).generic_string(), nullptr };
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

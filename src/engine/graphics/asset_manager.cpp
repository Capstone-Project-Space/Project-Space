#include "asset_manager.h"

std::unordered_map<std::string, std::shared_ptr<Font>> AssetManager::Fonts;
std::unordered_map<std::pair<std::string, std::optional<Material>>, std::shared_ptr<Model>> AssetManager::Models;
std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::Textures;
std::unordered_map<glm::vec4, std::shared_ptr<Texture>> AssetManager::ColoredTextures;
std::unordered_map<std::pair<std::string, std::string>, std::shared_ptr<ShaderProgram>> AssetManager::ShaderPrograms;

void AssetManager::Clear() {
	Fonts.clear();
	Models.clear();
	Textures.clear();
	ColoredTextures.clear();
	ShaderPrograms.clear();
}

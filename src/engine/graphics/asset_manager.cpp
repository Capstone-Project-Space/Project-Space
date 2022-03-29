#include "asset_manager.h"

std::unordered_map<std::string, std::shared_ptr<Model>> AssetManager::models;
std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::textures;

void AssetManager::Clear() {
	models.clear();
	textures.clear();
}

#include "model.h"

#include <algorithm>
#include <fstream>
#include <map>
#include <optional>
#include <unordered_map>
#include <set>
#include <sstream>
#include <tuple>
#include <vector>
#include <variant>

#include "src/engine/graphics/asset_manager.h"

#include <glad/glad.h>

ModelVertex::ModelVertex()
	: vertex({0.0f}), normal({0.0f}), uv({0.0f}), texture(0), ambientColor({0.0f}),
	  diffuseColor({0.0f}), specularColor({0.0f}), specularExponent(0.0f)
{}

ModelVertex::ModelVertex(
	glm::vec3 vertex, glm::vec2 uv, glm::vec3 normal, int texture,
	glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float specExponent
)
	: vertex(vertex), uv(uv), normal(normal), texture(texture), 
	  ambientColor(ambient), diffuseColor(diffuse), specularColor(specular), specularExponent(specExponent)
{}


const std::map<std::string, Material> Model::CreateMaterial(const std::string& filepath) {
	auto map = std::map<std::string, Material>();
	
	std::string line;
	std::ifstream mtl{ filepath};
	Material* mat = NULL;
	while (!mtl.eof()) {
		std::getline(mtl, line);
		if (line.rfind("newmtl ") == 0) {
			map.insert({ line.substr(7), Material() });
			mat = &map[line.substr(7)];
		} else if (line.rfind("Ka ") == 0) {
			assert(mat);
			int count = sscanf(
				line.c_str(), "Ka %f %f %f\n", 
				&mat->ambient.r, &mat->ambient.g, &mat->ambient.b
			);
			if (count != 3) throw "parsing ambient in mtl file: " + filepath;
		} else if (line.rfind("Kd ") == 0) {
			assert(mat);
			int count = sscanf(
				line.c_str(), "Kd %f %f %f\n",
				&mat->diffuse.r, &mat->diffuse.g, &mat->diffuse.b
			);
			if (count != 3) throw "parsing diffuse in mtl file: " + filepath;
		} else if (line.rfind("Ks ") == 0) {
			assert(mat);
			int count = sscanf(
				line.c_str(), "Ks %f %f %f\n",
				&mat->specular.r, &mat->specular.g, &mat->specular.b
			);
			if (count != 3) throw "parsing specular in mtl file: " + filepath;
		} else if (line.rfind("Ns ") == 0) {
			assert(mat);
			int count = sscanf(line.c_str(), "Ns %f\n", &mat->specExponent);
			if (count != 1) throw "parsing specular exponent in mtl file: " + filepath;
		} else if (line.rfind("map_Kd ") == 0) {
			assert(mat);
			mat->texture = Texture::CreateTexture("./resources/textures/" + line.substr(7));
		}
		if (mat && !mat->texture) mat->texture = AssetManager::GetOrCreate<Texture>(glm::vec4{1.f});
	}
	return map;
}

std::shared_ptr<Model> Model::CreateModel(const std::string_view& filepath, std::optional<Material> forcedMaterial) {
	std::shared_ptr<Model> model = std::shared_ptr<Model>(new Model());

	std::vector<uint32_t> indices;
	std::vector<ModelVertex> vertices;

	uint64_t uniqueVertices = 0;
	std::map<std::tuple<uint64_t, uint64_t, uint64_t>, uint64_t> vertexMap;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	Material* currentMaterial = NULL;
	auto mtls = std::map<std::string, Material>();
	if (forcedMaterial) {
		// TODO: Maybe allow for full Material control instead of just Texture.
		mtls["ForcedTexture"] = *forcedMaterial;
		model->textures[0] = (*forcedMaterial).texture;
	}

	std::string line;
	std::ifstream file{ filepath.data()};
	assert(file, "File does not exist.");
	std::stringstream obj;
	obj << file.rdbuf();

	float f1, f2, f3;
	while (!obj.eof()) {
		std::getline(obj, line);
		if (line.rfind("mtllib ") == 0) {
			mtls = Model::CreateMaterial(std::string(filepath.substr(0, filepath.find_last_of('/') + 1)) + line.substr(7));
			uint32_t count = forcedMaterial ? 1 : 0;
			for (auto&[_, material] : mtls) {
				auto found = std::find(model->textures.begin(), model->textures.end(), material.texture);
				if (found == model->textures.end()) model->textures[count++] = material.texture;
			}
			if (forcedMaterial) {
				// TODO: Maybe allow for full Material control instead of just Texture.
				mtls["ForcedTexture"] = *forcedMaterial;
			}
		} else if (line.rfind("usemtl ") == 0) {
			currentMaterial = &mtls[line.substr(7)];
		} else if (line.rfind("v ") == 0) {
			int count = sscanf(line.c_str(), "v %f %f %f\n", &f1, &f2, &f3);
			if (count != 3) throw "parsing vertex in wavefront obj model: " + std::string{ filepath };
			points.emplace_back(f1, f2, f3);
		} else if (line.rfind("vn ") == 0) {
			int count = sscanf(line.c_str(), "vn %f %f %f\n", &f1, &f2, &f3);
			if (count != 3) throw "parsing normal in wavefront obj model: " + std::string{ filepath };
			normals.emplace_back(f1, f2, f3);
		} else if (line.rfind("vt ") == 0) {
			int count = sscanf(line.c_str(), "vt %f %f\n", &f1, &f2);
			if (count != 2) throw "parsing uv in wavefront obj model: " + std::string{ filepath };
			uvs.emplace_back(f1, f2);
		} else if (line.rfind("f ") == 0) {
			uint64_t fpoints[3], fuvs[3], fnormals[3];
			int count = sscanf(
				line.c_str(), "f %llu/%llu/%llu %llu/%llu/%llu %llu/%llu/%llu\n",
				fpoints, fuvs, fnormals, &fpoints[1], &fuvs[1], &fnormals[1],
				&fpoints[2], &fuvs[2], &fnormals[2]
			);
			if (count != 9) throw "parsing face in wavefront obj model: " + std::string{ filepath };
			if (!currentMaterial) throw "currentMaterial is NULL when parsing face in wavefront obj model: " + std::string{ filepath };
			for (uint64_t i = 0; i < 3; i++) {
				std::tuple vertex{ fpoints[i], fuvs[i], fnormals[i] };
				if (vertexMap.find(vertex) == vertexMap.end()) {
					vertexMap.insert({ vertex, uniqueVertices });
					int textureSlot = (std::find(model->textures.begin(), model->textures.end(), currentMaterial->texture) - model->textures.begin());
					vertices.emplace_back(
						points[fpoints[i] - 1], uvs[fuvs[i] - 1], normals[fnormals[i] - 1],
						textureSlot,
						currentMaterial->ambient, currentMaterial->diffuse, currentMaterial->specular, currentMaterial->specExponent
					);
					indices.push_back(uniqueVertices++);
				} else {
					indices.push_back(vertexMap[vertex]);
				}
			}
		}
	}
	
	model->indexBuffer = IndexBuffer::CreateIndexBuffer(indices.size() * sizeof(uint32_t), indices.data());
	model->vertexBuffer = VertexBuffer::CreateVertexBuffer(vertices.size() * sizeof(ModelVertex), vertices.data());
	return model;
}

void Model::bind() const {
	this->indexBuffer->bind();
	this->vertexBuffer->bind();
	for (uint32_t i = 0; i < 32; i++) {
		if (this->textures[i] != nullptr) {
			this->textures[i]->bind(i);
		}
	}
}

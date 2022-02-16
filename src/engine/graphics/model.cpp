#include "model.h"

#include <fstream>
#include <map>
#include <sstream>
#include <tuple>
#include <vector>

ModelVertex::ModelVertex()
	: vertex(glm::vec3(0.0f)), normal(glm::vec3(0.0f)), uv(glm::vec2(0.0f)) {}

ModelVertex::ModelVertex(const glm::vec3& vertex, const glm::vec3 normal, const glm::vec2& uv)
	: vertex(vertex), normal(normal), uv(uv) {}

ModelVertex::ModelVertex(float vx, float vy, float vz, float nx, float ny, float nz, float s, float t)
	: vertex({vx, vy, vz}), normal({nx, ny, nz}), uv(s, t) {}


// TODO: Make this actually good code please.
// TODO: Add support for mtl files for selecting textures.
std::shared_ptr<Model> Model::CreateModel(const std::string& filepath) {
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	uint32_t vertexCount = 0;
	std::map<std::tuple<uint32_t, uint32_t, uint32_t>, uint32_t> vertexMap;

	std::string line;
	uint64_t indexCount = 0;
	std::ifstream modelFile = std::ifstream(filepath, std::ios::in | std::ios::binary);
	while (!modelFile.eof()) {
		std::getline(modelFile, line);
		if (line.rfind("v ", 0) == 0) {
			glm::vec3 vec;
			int count = sscanf(line.c_str() + 2, "%f %f %f\n", &vec[0], &vec[1], &vec[2]);
			if (count != 3) throw "parsing vertex in wavefront obj model: " + filepath;
			vertices.push_back(vec);
		} else if (line.rfind("vt ", 0) == 0) {
			glm::vec2 vec;
			int count = sscanf(line.c_str() + 3, "%f %f\n", &vec[0], &vec[1]);
			if (count != 2) throw "parsing uv in wavefront obj model: " + filepath;
			texCoords.push_back(vec);
		} else if (line.rfind("vn ", 0) == 0) {
			glm::vec3 vec;
			int count = sscanf(line.c_str() + 3, "%f %f %f\n", &vec[0], &vec[1], &vec[2]);
			if (count != 3) throw "parsing normal in wavefront obj model: " + filepath;
			normals.push_back(vec);
		} else if (line.rfind("f ", 0) == 0) {
			uint32_t vert, norm, uv;
			size_t offset = line.find_first_of("1234567890.", 0);
			for (uint32_t i = 0; i < 3; i++) {
				int count = sscanf(line.c_str() + offset, "%ud", &vert);
				offset = line.find_first_of('/', offset) + 1;
				count += sscanf(line.c_str() + offset, "%ud", &uv);
				offset = line.find_first_of('/', offset) + 1;
				count += sscanf(line.c_str() + offset, "%ud", &norm);
				if (count != 3) throw "parsing face in wavefront obj model: filepath";
				std::tuple<uint32_t, uint32_t, uint32_t> vertex{vert, norm, uv};
				if (vertexMap.find(vertex) == vertexMap.end()) {
					vertexMap.insert(std::pair{ vertex, vertexCount++ });
				}
				offset = line.find_first_of("1234567890.", offset);
			}
			indexCount += 3;
		}
	}
	
	uint32_t vertIndex = 0;
	std::shared_ptr<Model> model = std::shared_ptr<Model>(new Model(vertexCount, indexCount));
	for (const auto [key, _] : vertexMap) {
		model->vertices[vertIndex] = ModelVertex(
			vertices[std::get<0>(key)-1],
			normals[std::get<2>(key)-1],
			texCoords[std::get<1>(key)-1]
		);
	}

	modelFile.clear();
	modelFile.seekg(0, std::ios::beg);
	uint32_t indiceIndex = 0;
	while (!modelFile.eof()) {
		std::getline(modelFile, line);
		if (line.rfind("f ", 0) == 0) {
			uint32_t vert, norm, uv;
			size_t offset = line.find_first_of("1234567890.", 0);
			for (uint32_t i = 0; i < 3; i++) {
				int count = sscanf(line.c_str() + offset, "%ud", &vert);
				offset = line.find_first_of('/', offset) + 1;
				count += sscanf(line.c_str() + offset, "%ud", &uv);
				offset = line.find_first_of('/', offset) + 1;
				count += sscanf(line.c_str() + offset, "%ud", &norm);
				if (count != 3) throw "parsing face in wavefront obj model: filepath";
				std::tuple<uint32_t, uint32_t, uint32_t> vertex{ vert, norm, uv };
				model->indices[indiceIndex] = vertexMap[vertex];
				offset = line.find_first_of("1234567890.", offset);
			}
		}
	}

	return model;
}

Model::~Model() {
	delete[] vertices;
}

Model::Model(uint64_t vertCount, uint64_t indexCount) : vertCount(vertCount), indCount(indexCount) {
	this->vertices = new ModelVertex[vertCount];
	assert(this->vertices);
	this->indices = new uint32_t[indCount];
	assert(this->indices);
}
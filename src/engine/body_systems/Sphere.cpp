#include "Sphere.h"

SphereVertex::SphereVertex()
	: vertex({ 0.0f }), normal({ 0.0f }), uv({ 0.0f }), texture(0), ambientColor({ 0.0f }),
	diffuseColor({ 0.0f }), specularColor({ 0.0f }), specularExponent(0.0f)
{}

SphereVertex::SphereVertex(
	glm::vec3 vertex, glm::vec2 uv, glm::vec3 normal, int texture,
	glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float specExponent
)
	: vertex(vertex), uv(uv), normal(normal), texture(texture),
	ambientColor(ambient), diffuseColor(diffuse), specularColor(specular), specularExponent(specExponent)
{}

std::shared_ptr<Sphere> Sphere::CreateSphere(unsigned int resolution) {
     std::shared_ptr<Sphere> sphere = std::shared_ptr<Sphere>(new Sphere());
     std::vector<uint32_t> indices;
     std::vector<SphereVertex> vertices;

     float const R = 1. / (float)(resolution - 1);
     float const S = 1. / (float)(resolution - 1);
     int r, s;

     for (r = 0; r < resolution; r++) for (s = 0; s < resolution; s++) {
          float const y = sin(-M_PI_2 + M_PI * r * R);
          float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
          float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

          vertices.emplace_back(
               glm::vec3(x, y, z), glm::vec2(s * S, r * R), glm::vec3(x, y, z),
               0, glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f);

          indices.push_back(r * resolution + s);
          indices.push_back(r * resolution + (s + 1));
          indices.push_back((r + 1) * resolution + (s + 1));
          indices.push_back((r + 1) * resolution + s);
     }

     sphere->indexBuffer = IndexBuffer::CreateIndexBuffer(indices.size() * sizeof(uint32_t), indices.data());
     sphere->vertexBuffer = VertexBuffer::CreateVertexBuffer(vertices.size() * sizeof(SphereVertex), vertices.data());
     return sphere;
}

void Sphere::bind() const {
     this->indexBuffer->bind();
     this->vertexBuffer->bind();
}
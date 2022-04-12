#define _USE_MATH_DEFINES
#include <memory>
#include <vector>
#include <cmath>

#include <src/engine/graphics/buffers.h>
#include <src/engine/graphics/texture.h>

#include <glm/glm.hpp>

struct SphereVertex {
     glm::vec3 vertex;
     glm::vec2 uv;
     glm::vec3 normal;
     int texture;
     glm::vec3 ambientColor;
     glm::vec3 diffuseColor;
     glm::vec3 specularColor;
     float specularExponent;
     SphereVertex();
     SphereVertex(
          glm::vec3 vertex, glm::vec2 uv, glm::vec3 normal, int texture,
          glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float specExponent
     );
};

class Sphere {
public:
     static std::shared_ptr<Sphere> CreateSphere(unsigned int resolution);

     ~Sphere() = default;

     const inline std::shared_ptr<IndexBuffer> getIndices() const { return indexBuffer; }
     const inline std::shared_ptr<VertexBuffer> getVertices() const { return vertexBuffer;  }

     void bind() const;

private:
     std::shared_ptr<IndexBuffer> indexBuffer;
     std::shared_ptr<VertexBuffer> vertexBuffer;
     Sphere() = default;
};
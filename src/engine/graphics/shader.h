#pragma once

#include <stdint.h>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class ShaderProgram {
public:
	std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertSrc, const std::string& fragSrc);
	std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertPath, const std::string& fragPath);

	~ShaderProgram();

	void bind();

	void uploadFloat(const std::string& name, float f);
	void uploadFloat2(const std::string& name, glm::vec2 vec);
	void uploadFloat3(const std::string& name, glm::vec3 vec);
	void uploadFloat4(const std::string& name, glm::vec4 vec);

	void uploadInt(const std::string& name, int i);
	void uploadInt2(const std::string& name, glm::vec<2, int> vec);
	void uploadInt3(const std::string& name, glm::vec<3, int> vec);
	void uploadInt4(const std::string& name, glm::vec<4, int> vec);

	void uploadBool(const std::string& name, bool b);
	void uploadBool2(const std::string& name, glm::vec<2, bool> vec);
	void uploadBool3(const std::string& name, glm::vec<3, bool> vec);
	void uploadBool4(const std::string& name, glm::vec<4, bool> vec);

	void uploadMat3(const std::string& name, glm::mat3 matrix);
	void uploadMat4(const std::string& name, glm::mat4 matrix);

private:
	uint32_t id;
	ShaderProgram(const std::string& vertSrc, const std::string& fragSrc);

	static std::string readFile(const std::string& filepath);
	void compileShader(uint32_t id, const char* const src, int length);

};

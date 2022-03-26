#pragma once

#include <stdint.h>
#include <memory>
#include <string>

#include <src/engine/graphics/buffers.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

class ShaderProgram {
public:
	static std::shared_ptr<ShaderProgram> Create(const std::string& vertPath, const std::string& fragPath);
	static std::shared_ptr<ShaderProgram> CreateFromSource(const std::string& vertSrc, const std::string& fragSrc);

	~ShaderProgram();

	void bind();

	void bindUniformBuffer(const std::string& name, const std::shared_ptr<UniformBuffer> ub);

	void uploadFloat(const std::string& name, const float f) const;
	void uploadFloat2(const std::string& name, const glm::vec2& vec) const;
	void uploadFloat3(const std::string& name, const glm::vec3& vec) const;
	void uploadFloat4(const std::string& name, const glm::vec4& vec) const;

	void uploadInt(const std::string& name, const int i) const;
	void uploadInt2(const std::string& name, const glm::vec<2, int>& vec) const;
	void uploadInt3(const std::string& name, const glm::vec<3, int>& vec) const;
	void uploadInt4(const std::string& name, const glm::vec<4, int>& vec) const;

	void uploadBool(const std::string& name, const bool b) const;
	void uploadBool2(const std::string& name, const glm::vec<2, bool>& vec) const;
	void uploadBool3(const std::string& name, const glm::vec<3, bool>& vec) const;
	void uploadBool4(const std::string& name, const glm::vec<4, bool>& vec) const;

	void uploadMat3(const std::string& name, const glm::mat3& matrix) const;
	void uploadMat4(const std::string& name, const glm::mat4& matrix) const;

private:
	uint32_t id;
	ShaderProgram(const std::string& vertSrc, const std::string& fragSrc);

	static std::string readFile(const std::string& filepath);
	void compileShader(uint32_t id, const char* const src, int length);

};

#pragma once

#include <stdint.h>
#include <memory>
#include <string>

#include <src/engine/graphics/buffers.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

class ShaderProgram {
public:
	/**
	 * @brief Create a ShaderProgram from files.
	 * 
	 * @param vertPath The path to the vertex shader source.
	 * @param fragPath The path to the fragment shader source.
	 * @return A std::shared_ptr to a ShaderProgram.
	 */
	static std::shared_ptr<ShaderProgram> Create(const std::string_view& vertPath, const std::string_view& fragPath);
	
	/**
	 * @brief Create a ShaderProgram from source.
	 * 
	 * @param vertSrc The vertex shader source.
	 * @param fragSrc The fragment shader source.
	 * @return A std::shared_ptr to a ShaderProgram.
	 */
	static std::shared_ptr<ShaderProgram> CreateFromSource(const std::string_view& vertSrc, const std::string_view& fragSrc);

	~ShaderProgram();

	/**
	 * @brief Bind this ShaderProgram for use in rendering.
	 */
	void bind() const;

	/**
	 * @brief Bind a named UniformBuffer to the ShaderProgram.
	 * 
	 * @param name The name of the UniformBuffer.
	 * @param ub The UniformBuffer to bind.
	 */
	void bindUniformBuffer(const std::string& name, const std::shared_ptr<UniformBuffer> ub);

	/**
	 * @brief Upload a float as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The float to upload.
	 */
	void uploadFloat(const std::string& name, const float f) const;

	/**
	 * @brief Upload a vec2 as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The vec2 to upload.
	 */
	void uploadFloat2(const std::string& name, const glm::vec2& vec) const;

	/**
	 * @brief Upload a vec3 as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The vec3 to upload.
	 */
	void uploadFloat3(const std::string& name, const glm::vec3& vec) const;

	/**
	 * @brief Upload a vec4 as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The vec4 to upload.
	 */
	void uploadFloat4(const std::string& name, const glm::vec4& vec) const;

	/**
	 * @brief Upload an int as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The int to upload.
	 */
	void uploadInt(const std::string& name, const int i) const;

	/**
	 * @brief Upload a int vec2 as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The int vec2 to upload.
	 */
	void uploadInt2(const std::string& name, const glm::ivec2& vec) const;

	/**
	 * @brief Upload a int vec3 as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The int vec3 to upload.
	 */
	void uploadInt3(const std::string& name, const glm::ivec3& vec) const;

	/**
	 * @brief Upload a int vec4 as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The int vec4 to upload.
	 */
	void uploadInt4(const std::string& name, const glm::ivec4& vec) const;

	/**
	 * @brief Upload a bool as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The bool to upload.
	 */
	void uploadBool(const std::string& name, const bool b) const;

	/**
	 * @brief Upload a bool vec2 as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The bool vec2 to upload.
	 */
	void uploadBool2(const std::string& name, const glm::vec<2, bool>& vec) const;

	/**
	 * @brief Upload a bool vec3 as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The bool vec3 to upload.
	 */
	void uploadBool3(const std::string& name, const glm::vec<3, bool>& vec) const;

	/**
	 * @brief Upload a bool vec4 as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param f The bool vec4 to upload.
	 */
	void uploadBool4(const std::string& name, const glm::vec<4, bool>& vec) const;

	/**
	 * @brief Upload a 3x3 matrix as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param matrix The 3x3 matrix to upload.
	 */
	void uploadMat3(const std::string& name, const glm::mat3& matrix) const;

	/**
	 * @brief Upload a 4x4 matrix as a uniform variable.
	 * 
	 * @param name The name of the uniform variable.
	 * @param matrix The 4x4 matrix to upload.
	 */
	void uploadMat4(const std::string& name, const glm::mat4& matrix) const;

private:
	uint32_t id;
	ShaderProgram(const std::string_view& vertSrc, const std::string_view& fragSrc);

	static std::string readFile(const std::string_view& filepath);
	void compileShader(uint32_t id, const char* const src, int length);

};

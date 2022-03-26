#include "shader.h"

#include <fstream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#define LOG_GL_ERROR for (int glErrorGL = glGetError(); glErrorGL != 0;) { fprintf(stderr, "GLError: %d\n", glErrorGL); assert(false);}

std::shared_ptr<ShaderProgram> ShaderProgram::CreateFromSource(const std::string& vertSrc, const std::string& fragSrc) {
	std::shared_ptr<ShaderProgram> program = std::shared_ptr<ShaderProgram>(new ShaderProgram(vertSrc, fragSrc));
	return program;
}

std::shared_ptr<ShaderProgram> ShaderProgram::Create(const std::string& vertPath, const std::string& fragPath) {
	std::string vertSrc = readFile(vertPath);
	std::string fragSrc = readFile(fragPath);
	std::shared_ptr<ShaderProgram> program = std::shared_ptr<ShaderProgram>(new ShaderProgram(vertSrc, fragSrc));
	return program;
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(this->id);
	LOG_GL_ERROR;
}

void ShaderProgram::bind() { glUseProgram(this->id); }

void ShaderProgram::bindUniformBuffer(const std::string& name, const std::shared_ptr<UniformBuffer> ub) {
	uint32_t id = glGetUniformBlockIndex(this->id, name.c_str());
	glUniformBlockBinding(this->id, id, ub->getBlockBinding());
}

void ShaderProgram::uploadFloat(const std::string& name, const float f) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform1f(location, f);
}

void ShaderProgram::uploadFloat2(const std::string& name, const glm::vec2& vec) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform2f(location, vec[0], vec[1]);
}

void ShaderProgram::uploadFloat3(const std::string& name, const glm::vec3& vec) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform3f(location, vec[0], vec[1], vec[2]);
}

void ShaderProgram::uploadFloat4(const std::string& name, const glm::vec4& vec) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform4f(location, vec[0], vec[1], vec[2], vec[3]);
}


void ShaderProgram::uploadInt(const std::string& name, const int i) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform1i(location, i);
}

void ShaderProgram::uploadInt2(const std::string& name, const glm::vec<2, int>& vec) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform2i(location, vec[0], vec[1]);
}

void ShaderProgram::uploadInt3(const std::string& name, const glm::vec<3, int>& vec) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform3i(location, vec[0], vec[1], vec[2]);
}

void ShaderProgram::uploadInt4(const std::string& name, const glm::vec<4, int>& vec) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform4i(location, vec[0], vec[1], vec[2], vec[3]);
}


void ShaderProgram::uploadBool(const std::string& name, const bool b) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform1i(location, b);
}

void ShaderProgram::uploadBool2(const std::string& name, const glm::vec<2, bool>& vec) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform2i(location, vec[0], vec[1]);
}

void ShaderProgram::uploadBool3(const std::string& name, const glm::vec<3, bool>& vec) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform3i(location, vec[0], vec[1], vec[2]);
}

void ShaderProgram::uploadBool4(const std::string& name, const glm::vec<4, bool>& vec) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform4i(location, vec[0], vec[1], vec[2], vec[3]);
}


void ShaderProgram::uploadMat3(const std::string& name, const glm::mat3& matrix) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::uploadMat4(const std::string& name, const glm::mat4& matrix) const {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


ShaderProgram::ShaderProgram(const std::string& vertSrc, const std::string& fragSrc) {
	this->id = glCreateProgram();
	LOG_GL_ERROR;
	uint32_t vertId = glCreateShader(GL_VERTEX_SHADER);
	LOG_GL_ERROR;
	compileShader(vertId, vertSrc.c_str(), vertSrc.size());
	glAttachShader(this->id, vertId);
	LOG_GL_ERROR;
	uint32_t fragId = glCreateShader(GL_FRAGMENT_SHADER);
	LOG_GL_ERROR;
	compileShader(fragId, fragSrc.c_str(), fragSrc.size());
	glAttachShader(this->id, fragId);
	LOG_GL_ERROR;

	glLinkProgram(this->id);
	LOG_GL_ERROR;

	int isLinked = 0;
	glGetProgramiv(this->id, GL_LINK_STATUS, &isLinked);
	LOG_GL_ERROR;
	if (!isLinked) {
		int maxLength = 0;
		glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &maxLength);
		LOG_GL_ERROR;
		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(this->id, maxLength, &maxLength, &infoLog[0]);
		LOG_GL_ERROR;
		glDeleteProgram(this->id);
		LOG_GL_ERROR;
		glDeleteShader(vertId);
		LOG_GL_ERROR;
		glDeleteShader(fragId);
		LOG_GL_ERROR;
		throw std::string(infoLog.data());
	}
	glDeleteShader(vertId);
	LOG_GL_ERROR;
	glDeleteShader(fragId);
	LOG_GL_ERROR;
}

void ShaderProgram::compileShader(uint32_t id, const char* const src, int length) {
	glShaderSource(id, 1, &src, 0);
	LOG_GL_ERROR;
	glCompileShader(id);
	LOG_GL_ERROR;

	int status = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	LOG_GL_ERROR;

	if (!status) {
		int maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		LOG_GL_ERROR;
		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);
		LOG_GL_ERROR;
		glDeleteShader(id);
		LOG_GL_ERROR;
		fprintf(stderr, "%s\n", infoLog.data());
		throw std::string(infoLog.data());
	}
}

std::string ShaderProgram::readFile(const std::string& filepath) {
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in) {
		in.seekg(0, std::ios::end);
		result.resize((uint64_t) in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	} else {
		throw "Could not open file" + filepath;
	}
	return result;
}

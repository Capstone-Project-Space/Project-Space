#include "shader.h"

#include <fstream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

std::shared_ptr<ShaderProgram> ShaderProgram::CreateShaderProgram(const std::string& vertSrc, const std::string& fragSrc) {
	std::shared_ptr<ShaderProgram> program = std::shared_ptr<ShaderProgram>(new ShaderProgram(vertSrc, fragSrc));
	return program;
}

std::shared_ptr<ShaderProgram> ShaderProgram::CreateShaderProgram(const std::string& vertPath, const std::string& fragPath) {
	std::string vertSrc = readFile(vertPath);
	std::string fragSrc = readFile(fragPath);
	std::shared_ptr<ShaderProgram> program = std::shared_ptr<ShaderProgram>(new ShaderProgram(vertSrc, fragSrc));
	return program;
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(this->id);
}

void ShaderProgram::bind() { glUseProgram(this->id); }

void ShaderProgram::uploadFloat(const std::string& name, float f) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform1f(location, f);
}

void ShaderProgram::uploadFloat2(const std::string& name, glm::vec2 vec) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform2f(location, vec[0], vec[1]);
}

void ShaderProgram::uploadFloat3(const std::string& name, glm::vec3 vec) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform3f(location, vec[0], vec[1], vec[2]);
}

void ShaderProgram::uploadFloat4(const std::string& name, glm::vec4 vec) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform4f(location, vec[0], vec[1], vec[2], vec[3]);
}


void ShaderProgram::uploadInt(const std::string& name, int i) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform1i(location, i);
}

void ShaderProgram::uploadInt2(const std::string& name, glm::vec<2, int> vec) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform2i(location, vec[0], vec[1]);
}

void ShaderProgram::uploadInt3(const std::string& name, glm::vec<3, int> vec) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform3i(location, vec[0], vec[1], vec[2]);
}

void ShaderProgram::uploadInt4(const std::string& name, glm::vec<4, int> vec) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform4i(location, vec[0], vec[1], vec[2], vec[3]);
}


void ShaderProgram::uploadBool(const std::string& name, bool b) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform1i(location, b);
}

void ShaderProgram::uploadBool2(const std::string& name, glm::vec<2, bool> vec) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform2i(location, vec[0], vec[1]);
}

void ShaderProgram::uploadBool3(const std::string& name, glm::vec<3, bool> vec) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform3i(location, vec[0], vec[1], vec[2]);
}

void ShaderProgram::uploadBool4(const std::string& name, glm::vec<4, bool> vec) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniform4i(location, vec[0], vec[1], vec[2], vec[3]);
}


void ShaderProgram::uploadMat3(const std::string& name, glm::mat3 matrix) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::uploadMat4(const std::string& name, glm::mat4 matrix) {
	int location = glGetUniformLocation(this->id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


ShaderProgram::ShaderProgram(const std::string& vertSrc, const std::string& fragSrc) {
	this->id = glCreateProgram();
	uint32_t vertId = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertId, vertSrc.c_str(), vertSrc.size());
	glAttachShader(this->id, vertId);
	uint32_t fragId = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragId, fragSrc.c_str(), fragSrc.size());
	glAttachShader(this->id, fragId);

	glLinkProgram(this->id);

	int isLinked = 0;
	glGetProgramiv(this->id, GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
		int maxLength = 0;
		glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(this->id, maxLength, &maxLength, &infoLog[0]);
		glDeleteProgram(this->id);
		glDeleteShader(vertId);
		glDeleteShader(fragId);
		throw std::string(infoLog.data());
	}
	glDeleteShader(vertId);
	glDeleteShader(fragId);
}

void ShaderProgram::compileShader(uint32_t id, const char* const src, int length) {
	glShaderSource(id, 1, &src, &length);
	glCompileShader(id);

	int status = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	if (!status) {
		int maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);
		glDeleteShader(id);
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

#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <string>
#include <vector>

class Shader
{
public:
	Shader(const std::string &vertexPath, const std::string &fragmentPath);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void SetInt(const std::string &variableName, int val) const;
	void SetIntArray(const std::string &variableName,
		const std::vector<int> &array) const;
	void SetFloat(const std::string &variableName, float val) const;
	void SetVec4(const std::string &variableName, const glm::vec4 &vec) const;
	void SetVec4(const std::string &variableName, float x, float y, float z,
		float w) const;
	void SetMat4(const std::string &variableName, const glm::mat4 &mat) const;

private:
	std::string ReadFile(const std::string &filePath) const;
	GLuint CompileShader(GLenum type, const std::string &file) const;
	void CheckForCompileError(GLuint proccessId, GLenum type) const;

	GLuint id;
};

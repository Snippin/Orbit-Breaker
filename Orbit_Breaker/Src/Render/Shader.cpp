#include "Shader.hpp"

#include "../Utility/Echo.hpp"

#include <format>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
	std::string vertexSrc = ReadFile(vertexPath);
	std::string fragmentSrc = ReadFile(fragmentPath);

	const char *vShaderSrc = vertexSrc.c_str();
	const char *fShaderSrc = fragmentSrc.c_str();

	// Load and compile shaders
	GLuint vertex = CompileShader(GL_VERTEX_SHADER, vShaderSrc);
	GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fShaderSrc);

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	CheckForCompileError(id, GL_PROGRAM);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::Bind() const
{
	glUseProgram(id);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetInt(const std::string &variableName, int val) const
{
	glUniform1i(glGetUniformLocation(id, variableName.c_str()), val);
}

void Shader::SetIntArray(const std::string &variableName,
	const std::vector<int> &array) const
{
	glUniform1iv(glGetUniformLocation(id, variableName.c_str()),
		static_cast<GLsizei>(array.size()), array.data());
}

void Shader::SetFloat(const std::string &variableName, float val) const
{
	glUniform1f(glGetUniformLocation(id, variableName.c_str()), val);
}

void Shader::SetVec4(const std::string &variableName, const glm::vec4 &vec) const
{
	glUniform4fv(glGetUniformLocation(id, variableName.c_str()), 1, &vec[0]);
}

void Shader::SetVec4(const std::string &variableName, float x, float y, float z,
	float w) const
{
	glUniform4f(glGetUniformLocation(id, variableName.c_str()), x, y, z, w);
}

void Shader::SetMat4(const std::string &variableName,
	const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, variableName.c_str()), 1,
		GL_FALSE, &mat[0][0]);
}

std::string Shader::ReadFile(const std::string &filePath) const
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file)
	{
		Echo::Error("Unable to open file: {}", filePath);
		throw std::runtime_error("ERROR: Unable to open file: " + filePath);
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();

	return buffer.str();
}

GLuint Shader::CompileShader(GLenum type, const std::string &file) const
{
	GLuint shader = glCreateShader(type);
	const char *source = file.c_str();
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	CheckForCompileError(shader, type);
	return shader;
}

void Shader::CheckForCompileError(GLuint proccessId, GLenum type) const
{
	GLint success;
	const int logSize = 1024;
	std::string infoLog(logSize, '\0');
	std::string shaderType = "";

	switch (type)
	{
	case GL_PROGRAM:
		shaderType = "Program";
		break;
	case GL_VERTEX_SHADER:
		shaderType = "Vertex";
		break;
	case GL_FRAGMENT_SHADER:
		shaderType = "Fragment";
		break;

	default:
		Echo::Warn("Not any of compile processes were selected");
		return;
	}

	if (type != GL_PROGRAM)
	{
		glGetShaderiv(proccessId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(proccessId, logSize, nullptr, infoLog.data());
			Echo::Error("SHADER_COMPILATION_ERROR of type: {}\n{}",
				shaderType, infoLog);
		}
	}
	else
	{
		glGetProgramiv(proccessId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(proccessId, logSize, nullptr, infoLog.data());
			Echo::Error("PROGRAM_LINKING_ERROR of type: {}\n{}",
				shaderType, infoLog);
		}
	}
}

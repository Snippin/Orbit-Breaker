#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
public:
	Texture() = default;
	Texture(int width, int height);
	~Texture();

	void Init(const std::string &file);
	void Bind() const;
	void Unbind() const;

	int GetWidth() const;
	int GetHeight() const;
	int GetId() const;

	bool operator==(const Texture &other) const;
private:
	GLuint id;
	int width;
	int height;
};

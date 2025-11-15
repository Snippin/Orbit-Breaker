#include "Texture.hpp"

#include "../Utility/Echo.hpp"

#include <soil2/stb_image.h>
#include <string>

Texture::Texture(int width, int height) :
	width{width}, height{height}
{
	// Generate texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, nullptr);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::Init(const std::string &file)
{
	// Generate texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Set texture parameters
	// Repeat image in both directions
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// When stretching or shrinking respectively, pixelate
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load image
	int channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image = stbi_load(file.c_str(), &width, &height, &channels,
		0);

	if (image)
	{
		GLenum format = 0;

		if (channels == 3)
		{
			format = GL_RGB;
		}
		else if (channels == 4)
		{
			format = GL_RGBA;
		}

		if (format != 0)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
				GL_UNSIGNED_BYTE, image);
		}
		else
		{
			Echo::Error("(Texture) Unknown number of channels: {}", channels);
		}

		stbi_image_free(image);
	}
	else
	{
		Echo::Error("(Texture) Could not load image \'{}\'", file);
	}
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::GetWidth() const
{
	return width;
}

int Texture::GetHeight() const
{
	return height;
}

int Texture::GetId() const
{
	return id;
}

bool Texture::operator==(const Texture &other) const
{
	return width == other.GetWidth() &&
		height == other.GetHeight() &&
		id == other.GetId();
}

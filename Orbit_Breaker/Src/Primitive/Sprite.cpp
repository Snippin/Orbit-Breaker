#include "Sprite.hpp"

#include "../Render/Texture.hpp"

#include <array>
#include <glm/glm.hpp>
#include <memory>

Sprite::Sprite()
{
	Init(nullptr,
		{
			glm::vec2{1.f, 1.f},
			glm::vec2{1.f, 0.f},
			glm::vec2{0.f, 0.f},
			glm::vec2{0.f, 1.f}
		}
	);
}

Sprite::Sprite(std::shared_ptr<Texture> texture)
{
	Init(texture,
		{
			glm::vec2{1.f, 1.f},
			glm::vec2{1.f, 0.f},
			glm::vec2{0.f, 0.f},
			glm::vec2{0.f, 1.f}
		}
	);
}

Sprite::Sprite(std::shared_ptr<Texture> texture,
	const std::array<glm::vec2, 4> &tex_coords)
{
	Init(texture, tex_coords);
}

std::shared_ptr<Texture> Sprite::GetTexture() const
{
	return texture;
}

std::array<glm::vec2, 4> Sprite::GetTexCoords() const
{
	return tex_coords;
}

int Sprite::GetWidth() const
{
	return width;
}

int Sprite::GetHeight() const
{
	return height;
}

int Sprite::GetTexId() const
{
	return texture ? texture->GetId() : -1;
}

Sprite &Sprite::SetTexture(std::shared_ptr<Texture> new_texture)
{
	texture = new_texture;
	return *this;
}

Sprite &Sprite::SetTexCoords(const std::array<glm::vec2, 4> &new_tex_coords)
{
	tex_coords = new_tex_coords;
	return *this;
}

Sprite &Sprite::SetWidth(int new_width)
{
	width = new_width;
	return *this;
}

Sprite &Sprite::SetHeight(int new_height)
{
	height = new_height;
	return *this;
}

void Sprite::Init(std::shared_ptr<Texture> init_texture,
	const std::array<glm::vec2, 4> &init_tex_coords)
{
	texture = init_texture;
	tex_coords = init_tex_coords;
	width = 0;
	height = 0;
}

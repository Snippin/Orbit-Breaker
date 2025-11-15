#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>

class Texture;

class Sprite
{
public:
	Sprite();
	Sprite(std::shared_ptr<Texture> texture);
	Sprite(std::shared_ptr<Texture> texture,
		const std::array<glm::vec2, 4> &tex_coords);
	~Sprite() = default;

	std::shared_ptr<Texture> GetTexture() const;
	std::array<glm::vec2, 4> GetTexCoords() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetTexId() const;

	Sprite &SetTexture(std::shared_ptr<Texture> new_texture);
	Sprite &SetTexCoords(const std::array<glm::vec2, 4> &new_tex_coords);
	Sprite &SetWidth(int new_width);
	Sprite &SetHeight(int new_height);

private:
	void Init(std::shared_ptr<Texture> init_texture,
		const std::array<glm::vec2, 4> &init_tex_coords);

	std::shared_ptr<Texture> texture;
	std::array<glm::vec2, 4> tex_coords;
	int width;
	int height;
};

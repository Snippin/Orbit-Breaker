#include "SpriteRenderer.hpp"

#include "../../Primitive/GameObject.hpp"
#include "../../Primitive/Sprite.hpp"
#include "../../Render/Texture.hpp"

#include <glm/glm.hpp>
#include <memory>

SpriteRenderer::SpriteRenderer()
{
	Init(glm::vec4(1.f), nullptr);
}

SpriteRenderer::SpriteRenderer(const glm::vec4 &color)
{
	Init(color, nullptr);
}

SpriteRenderer::SpriteRenderer(std::shared_ptr<Texture> texture)
{
	Init(glm::vec4(1.f), std::make_shared<Sprite>(texture));
}

SpriteRenderer::SpriteRenderer(std::shared_ptr<Sprite> sprite)
{
	Init(glm::vec4(1.f), sprite);
}

void SpriteRenderer::Start()
{
	last_transform = gameObject->transform->Copy();
}

void SpriteRenderer::Update(float dt)
{
	if (last_transform != *gameObject->transform)
	{
		gameObject->transform->CopyTo(last_transform);
		SetDirty();
	}
}

glm::vec4 SpriteRenderer::GetColor() const
{
	return color;
}

std::shared_ptr<Texture> SpriteRenderer::GetTexture() const
{
	return sprite ? sprite->GetTexture() : nullptr;
}

std::array<glm::vec2, 4> SpriteRenderer::GetTexCoords() const
{
	return sprite ? sprite->GetTexCoords() :
		std::array<glm::vec2, 4>{
			glm::vec2(1.f, 1.f),
			glm::vec2(1.f, 0.f),
			glm::vec2(0.f, 0.f),
			glm::vec2(0.f, 1.f),
	};
}

bool SpriteRenderer::IsDirty() const
{
	return is_dirty;
}

void SpriteRenderer::SetColor(const glm::vec4 &new_color)
{
	if (color != new_color)
	{
		color = new_color;
		SetDirty();
	}
}

void SpriteRenderer::SetSprite(const std::shared_ptr<Sprite> &new_sprite)
{
	sprite = new_sprite;
	SetDirty();
}

void SpriteRenderer::SetClean()
{
	is_dirty = false;
}

void SpriteRenderer::SetDirty()
{
	is_dirty = true;
}

void SpriteRenderer::Init(const glm::vec4 &init_color,
	std::shared_ptr<Sprite> init_sprite)
{
	color = init_color;
	sprite = init_sprite;
	SetDirty();
}

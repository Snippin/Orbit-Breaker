#pragma once

#include "../../Primitive/Component.hpp"
#include "Transform.hpp"

#include <glm/glm.hpp>
#include <memory>

class Texture;
class Sprite;

class SpriteRenderer : public Component
{
public:
	SpriteRenderer();
	SpriteRenderer(const glm::vec4 &color);
	SpriteRenderer(std::shared_ptr<Texture> texture);
	SpriteRenderer(std::shared_ptr<Sprite> sprite);

	void Start() override;
	void Update(float dt) override;

	glm::vec4 GetColor() const;
	std::shared_ptr<Texture> GetTexture() const;
	std::array<glm::vec2, 4> GetTexCoords() const;
	bool IsDirty() const;

	void SetColor(const glm::vec4 &new_color);
	void SetSprite(const std::shared_ptr<Sprite> &new_sprite);
	void SetClean();
	void SetDirty();

private:
	void Init(const glm::vec4 &init_color,
		std::shared_ptr<Sprite> init_sprite);

	glm::vec4 color;
	std::shared_ptr<Sprite> sprite;
	Transform last_transform;
	bool is_dirty;
};

#pragma once

#include "../../Primitive/Component.hpp"

#include <glm/glm.hpp>

class Transform : public Component
{
public:
	Transform(const glm::vec2 &position = glm::vec2(0.f),
		const glm::vec2 &scale = glm::vec2(1.f), float rotation = 0.f,
		int layer = 0);

	Transform Copy() const;
	void CopyTo(Transform &to) const;
	bool operator==(const Transform &other) const;

	glm::vec2 position;
	glm::vec2 scale;
	float rotation;
	int layer;
};

#include "Transform.hpp"

Transform::Transform(const glm::vec2 &position, const glm::vec2 &scale,
	float rotation, int layer) :
	position{position}, scale{scale}, rotation{rotation}, layer{layer}
{
}

Transform Transform::Copy() const
{
	return Transform(position, scale, rotation, layer);
}

void Transform::CopyTo(Transform &to) const
{
	to.position = position;
	to.scale = scale;
	to.rotation = rotation;
}

bool Transform::operator==(const Transform &other) const
{
	return position == other.position &&
		scale == other.scale &&
		rotation == other.rotation &&
		layer == other.layer;
}

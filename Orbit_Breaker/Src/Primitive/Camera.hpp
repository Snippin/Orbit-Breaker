#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(glm::vec2 position = glm::vec2(0.f));
	~Camera() = default;

	const glm::mat4 &GetViewMatrix();
	const glm::mat4 &GetInverseViewMatrix() const;

	const glm::mat4 &GetProjectionMatrix() const;
	const glm::mat4 &GetInverseProjectionMatrix() const;

	const glm::vec2 &GetProjectionSize() const;

	float GetZoom() const;
	void SetZoom(float value);
	void AddZoom(float value);

	glm::vec2 position;

private:
	void AdjustProjection();

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::mat4 inverse_view_matrix;
	glm::mat4 inverse_projection_matrix;
	glm::vec2 projection_size;
	float zoom;
};

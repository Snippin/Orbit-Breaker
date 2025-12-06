#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec2 position)
{
	this->position = position;
	projection_matrix = glm::mat4(1.f);
	view_matrix = glm::mat4(1.f);
	projection_size = {32.f, 18.f};
	zoom = 1.f;

	AdjustProjection();
}

const glm::mat4 &Camera::GetViewMatrix()
{
	glm::vec3 front(0.f, 0.f, -1.f);
	glm::vec3 up(0.f, 1.f, 0.f);
	view_matrix = glm::mat4(1.f);

	view_matrix = glm::lookAt(glm::vec3(position, 20.f),
		glm::vec3(position.x, position.y, 0.f) + front, up);

	inverse_view_matrix = glm::inverse(view_matrix);

	return view_matrix;
}

const glm::mat4 &Camera::GetInverseViewMatrix() const
{
	return inverse_view_matrix;
}

const glm::mat4 &Camera::GetProjectionMatrix() const
{
	return projection_matrix;
}

const glm::mat4 &Camera::GetInverseProjectionMatrix() const
{
	return inverse_projection_matrix;
}

const glm::vec2 &Camera::GetProjectionSize() const
{
	return projection_size;
}

float Camera::GetZoom() const
{
	return zoom;
}

void Camera::SetZoom(float value)
{
	zoom = value;
	AdjustProjection();
}

void Camera::AddZoom(float value)
{
	zoom += value;
	AdjustProjection();
}

void Camera::AdjustProjection()
{
	projection_matrix = glm::mat4(1.f);
	projection_matrix = glm::ortho(0.f, projection_size.x * zoom, 0.f,
		projection_size.y * zoom, 0.f, 100.f);
	inverse_projection_matrix = glm::inverse(projection_matrix);
}

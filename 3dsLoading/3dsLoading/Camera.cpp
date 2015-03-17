#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float FoV, float aspectRatio, float near, float far)
{
	this->m_viewMatrix = glm::lookAt(position, target, up);
	this->m_projectionMatrix = glm::perspective(FoV, aspectRatio, near, far);

	this->m_position = position;
	this->m_targetPostiion = target;
	this->m_up = up;
	this->m_direction = this->m_targetPostiion - this->m_position;
	this->m_direction = glm::normalize(this->m_direction);
	this->m_speed = 3.0f;
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix()
{
	return this->m_viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return this->m_projectionMatrix;
}

void Camera::Frame(Input* input, float deltaTime)
{
	if (input->IsKeyDown(KeyCodes::LeftShift))
	{
		this->m_speed = 50.0f;
	}
	else
	{
		this->m_speed = 5.0f;
	}

	if (input->IsKeyDown(KeyCodes::W))
	{
		this->m_position += this->m_direction * deltaTime * this->m_speed;
	}

	if (input->IsKeyDown(KeyCodes::S))
	{
		this->m_position -= this->m_direction * deltaTime * this->m_speed;
	}

	if (input->IsKeyDown(KeyCodes::A))
	{
		this->m_position -= glm::cross(this->m_direction, this->m_up) * deltaTime * this->m_speed;
	}

	if (input->IsKeyDown(KeyCodes::D))
	{
		this->m_position += glm::cross(this->m_direction, this->m_up) * deltaTime * this->m_speed;
	}

	float dx = 0.0f;
	float dy = 0.0f;

	if (input->IsKeyDown(KeyCodes::Q))
	{
		this->m_direction = glm::vec3(glm::vec4(this->m_direction, 0.0f) * glm::rotate(-this->m_speed * deltaTime, this->m_up));
	}

	if (input->IsKeyDown(KeyCodes::E))
	{
		this->m_direction = glm::vec3(glm::vec4(this->m_direction, 0.0f) * glm::rotate(this->m_speed * deltaTime, this->m_up));
	}

	if (input->IsKeyDown(KeyCodes::Z))
	{
		this->m_direction = glm::vec3(glm::vec4(this->m_direction, 0.0f) * glm::rotate(this->m_speed * deltaTime, glm::cross(this->m_direction, this->m_up)));
	}

	if (input->IsKeyDown(KeyCodes::X))
	{
		this->m_direction = glm::vec3(glm::vec4(this->m_direction, 0.0f) * glm::rotate(-this->m_speed * deltaTime, glm::cross(this->m_direction, this->m_up)));
	}

	glm::vec4 direction = glm::vec4(this->m_direction, 0.0f) * glm::rotate(dx * deltaTime, this->m_up);
	this->m_direction = glm::vec3(direction.x, direction.y, direction.z);

	RecalculateViewMatrix();
}

void Camera::RecalculateViewMatrix()
{
	this->m_viewMatrix = glm::lookAt(this->m_position, this->m_position + this->m_direction, this->m_up);
}
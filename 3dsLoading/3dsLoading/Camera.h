#pragma once

#include <OpenGL/GLM/glm.hpp>
#include <OpenGL/GLM/gtx/transform.hpp>
#include "Input.h"

class Camera
{
private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	glm::vec3 m_position;
	glm::vec3 m_targetPostiion;
	glm::vec3 m_up;
	glm::vec3 m_right;

	glm::vec3 m_direction;
	float m_speed;

	float m_horizontalAngle, m_verticalAngle;
	float m_verticalActual, m_horizontalActual;

public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float FoV, float aspectRatio, float near, float far);
	~Camera();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void Frame(Input* input, float deltaTime);

private:
	void RecalculateViewMatrix();
};


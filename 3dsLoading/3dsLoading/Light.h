#pragma once

#include <OpenGL\GLM\glm.hpp>

class Light
{
private:
	glm::vec3 m_direction;
	glm::vec4 m_color;

public:
	Light();
	~Light();

	bool Initialize(glm::vec3 direction, glm::vec3 color);
	void Shutdown();

	glm::vec3 GetDirection();
	glm::vec4 GetColor();
};


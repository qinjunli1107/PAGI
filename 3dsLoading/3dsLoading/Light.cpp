#include "Light.h"


Light::Light()
{
}


Light::~Light()
{
}

bool Light::Initialize(glm::vec3 direction, glm::vec3 color)
{
	this->m_direction = direction;
	this->m_color = glm::vec4(color, 1.0f);

	return true;
}

void Light::Shutdown()
{

}

glm::vec3 Light::GetDirection()
{
	return glm::normalize(this->m_direction);
}

glm::vec4 Light::GetColor()
{
	return this->m_color;
}
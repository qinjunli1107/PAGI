#include "Input.h"

Input::Input()
{
	this->m_mouseX = 0;
	this->m_mouseY = 0;
	this->m_window = 0;
}

Input::~Input()
{

}

bool Input::Initialize(GLFWwindow* window)
{
	this->m_window = window;
	glfwSetInputMode(this->m_window, GLFW_STICKY_KEYS, GL_TRUE);
	//glfwSetInputMode(this->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(this->m_window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

	return true;
}

void Input::Shutdown()
{

}

glm::vec2 Input::GetMousePosition()
{
	glfwGetCursorPos(this->m_window, &(double)this->m_mouseX, &(double)this->m_mouseY);
	return glm::vec2(this->m_mouseX, this->m_mouseY);
}

bool Input::IsKeyDown(KeyCodes keyCode)
{
	if (glfwGetKey(this->m_window, keyCode) == GLFW_PRESS)
	{
		return true;
	}

	return false;
}

bool Input::IsKeyUp(KeyCodes keyCode)
{
	if (glfwGetKey(this->m_window, keyCode) == GLFW_RELEASE)
	{
		return true;
	}

	return false;
}

bool Input::IsMouseButtonDown(MouseButtons button)
{
	if (glfwGetMouseButton(this->m_window, button) == GLFW_PRESS)
	{
		return true;
	}

	return false;
}

bool Input::IsMouseButtonUp(MouseButtons button)
{
	if (glfwGetMouseButton(this->m_window, button) == GLFW_RELEASE)
	{
		return true;
	}

	return false;
}
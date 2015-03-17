#include "System.h"


System::System(string appName)
{
	this->m_appName = appName;
	this->m_Graphics = 0;
	this->currentTime = glfwGetTime();
	this->lastTime = this->currentTime;
}


System::~System()
{
}

bool System::Initialize()
{
	this->m_Graphics = new Graphics();
	if (!this->m_Graphics->Initialize(this->m_appName))
	{
		fprintf(stderr, "Failed to initialize graphics\n");
		return false;
	}

	return true;
}

void System::Run()
{
	GLFWwindow* window = this->m_Graphics->GetWindow();
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);	//Make sure we can capture ESC key

	Input* input = m_Graphics->GetInput();

	do
	{
		this->currentTime = glfwGetTime();
		m_Graphics->Frame(this->currentTime - this->lastTime);
		this->lastTime = this->currentTime;

	} while (input->IsKeyUp(KeyCodes::Escape) && glfwWindowShouldClose(window) == 0);
}

void System::Shutdown()
{
	if (this->m_Graphics != nullptr)
	{
		this->m_Graphics->Shutdown();
		delete this->m_Graphics;
		this->m_Graphics = 0;
	}
}
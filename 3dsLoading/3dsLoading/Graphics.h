#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>

#include <OpenGL\glew.h>
#include <OpenGL\glfw3.h>
#include <OpenGL\GLM\glm.hpp>

#include "Model.h"
#include "Camera.h"
#include "Input.h"

using namespace glm;
using namespace std;

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

class Graphics
{
private:
	GLFWwindow* m_window;
	std::vector<Model*> m_models;

	Camera* m_camera;
	Input* m_input;
	Light* m_light;

	GLuint m_vertexArrayID;

	int m_currentSelected;
	bool m_selectionKeyPressed;
	bool m_mousePressed;
	bool m_selectedMode;
	float timer;

public:
	Graphics();
	~Graphics();

	bool Initialize(string appName);
	void Frame(float deltaTime);
	void Shutdown();

	GLFWwindow* GetWindow();
	Input* GetInput();

private:
	bool InitializeWindow();
	GLuint InitializeShaders(const char const* vsPath, const char const* fsPath);
	string LoadShaderCode(const char const* shaderPath);
	void CompileShader(string  shaderCode, GLuint shaderCodeID);
	void Render();
	void SelectPixel();
};


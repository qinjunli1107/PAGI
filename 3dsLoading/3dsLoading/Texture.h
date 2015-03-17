#pragma once

#include <string>
#include <OpenGL\glew.h>
#include <OpenGL\glfw3.h>
#include <OpenGL\GLM\glm.hpp>
#include <OpenGL\GLM\gtx\transform.hpp>
#include <SOIL2\SOIL2.h>

class Texture
{
private:
	GLuint m_textureID;
	GLsizei m_textureWidth;
	GLsizei m_textureHeight;

public:
	Texture();
	~Texture();

	bool Initialize(const std::string const* filePath);
	void Shutdown();
	GLuint GetTextureID();

private:
	void LoadFromFile(const std::string const* filePath);
	
};


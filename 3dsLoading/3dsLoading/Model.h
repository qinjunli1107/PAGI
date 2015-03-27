#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define _MAIN_CHUNK 0x4D4D
#define _3D_EDITOR_CHUNK 0x3D3D
#define _OBJECT_BLOCK 0x4000
#define _TRIANGULAR_MESH 0x4100
#define _VERTICES_LIST 0x4110
#define _FACES_DESCRIPTION 0x4120
#define _MAPPING_COORDINATES_LIST 0x4140

#include <cstdint>
#include <OpenGL\glew.h>
#include <OpenGL\GLM\glm.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>

#include "Camera.h"
#include "Texture.h"
#include "Light.h"

class Model
{
private:
	//Selection
	glm::vec3 m_selectionColor;
	GLuint m_selectionColorID;

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
	std::vector<unsigned short> m_indices;

	int m_vertsNum;
	int m_polygonsNum;

	GLuint m_vertexBufferID;
	GLuint m_uvsBufferID;
	GLuint m_normalBufferID;
	GLuint m_indexBufferID;
	GLuint m_shaderID;

	GLuint m_lightDirectionID;
	GLuint m_lightColorID;
	GLuint m_matrixID;
	GLuint m_pickColorID;
	GLuint m_pickingID;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_translationMatrix;
	glm::mat4 m_rotationMatrix;
	glm::vec3 m_position;

	Texture* m_texture;

	std::vector<Model*> m_children;
	Model* m_parent;

	char m_name[20];

	GLchar pickColor[4];

public:
	Model();
	~Model();

	bool Initialize(GLuint shaderID, Camera* camera, int i);
	void Frame(Camera* camera);
	void Render(Camera* camera, Light* light);
	void RenderSelectionMode(Camera* camera);
	void Shutdown();

	void Rotate(float angle, glm::vec3 axis);
	void Translate(glm::vec3 direction);

	void AddChild(Model* child);
	void SetParent(Model* parent);

	void Select();
	void Unselect();

	Model* GetParent();

	GLchar* GetPickColor();

private:
	void RecalculateModelMatrix();
	bool LoadOBJ(const char const* filePath, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<unsigned short>& indices);
	bool Load3DS(const char const* filePath, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<unsigned short>& indices);
};


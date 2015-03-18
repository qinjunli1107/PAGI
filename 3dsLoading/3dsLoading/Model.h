#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <cstdint>
#include <OpenGL\glew.h>
#include <OpenGL\GLM\glm.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

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

	GLuint m_vertexBufferID;
	GLuint m_uvsBufferID;
	GLuint m_normalBufferID;
	GLuint m_indexBufferID;
	GLuint m_shaderID;

	GLuint m_lightDirectionID;
	GLuint m_lightColorID;
	GLuint m_matrixID;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_translationMatrix;
	glm::mat4 m_rotationMatrix;
	glm::vec3 m_position;

	Texture* m_texture;

	std::vector<Model*> m_children;
	Model* m_parent;

public:
	Model();
	~Model();

	bool Initialize(GLuint shaderID, Camera* camera, std::string texturePath, Model* parent, std::vector<Model*> children);
	void Frame(Camera* camera);
	void Render(Camera* camera, Light* light);
	void Shutdown();

	void Rotate(float angle, glm::vec3 axis);
	void Translate(glm::vec3 direction);

	void AddChild(Model* child);
	void SetParent(Model* parent);

	void Select();
	void Unselect();

	Model* GetParent();

private:
	void RecalculateModelMatrix();
	bool LoadOBJ(const char const* filePath, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals);
	void CalculateIndexVBO(std::vector<glm::vec3> inVerts, std::vector<glm::vec2> inUVs, std::vector<glm::vec3> inNormals, std::vector<unsigned short>& outIndices, std::vector<glm::vec3>& outVerts, std::vector<glm::vec2>& outUVs, std::vector<glm::vec3>& outNormals);
	bool FindSimilarVertex(glm::vec3& inVertex, glm::vec2& inUV, glm::vec3& inNormal, std::vector<glm::vec3>& outVerts, std::vector<glm::vec2>& outUVs, std::vector<glm::vec3>& outNormals, unsigned short& result);
	bool IsNear(float x, float y);
};


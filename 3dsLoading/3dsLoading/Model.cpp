#include "Model.h"

Model::Model()
{
	this->m_vertexBufferID = 0;
	this->m_modelMatrix = glm::mat4(1.0f);
	this->m_shaderID = 0;
	this->m_vertsNum = 0;
	this->m_texture = 0;
	this->m_uvsBufferID = 0;
	this->m_indexBufferID = 0;
}


Model::~Model()
{
}

bool Model::Initialize(GLuint shaderID, Camera* camera, std::string texturePath, Model* parent, std::vector<Model*> children)
{
	this->m_position = glm::vec3(0.f);
	this->m_translationMatrix = glm::translate(this->m_position);
	this->m_rotationMatrix = glm::mat4(1.0f);
	this->m_texture = new Texture();
	if (!this->m_texture->Initialize(&texturePath))
	{
		return false;
	}

	this->m_children = children;
	this->m_parent = parent;

	if (!this->LoadOBJ("../Models/Sphere.obj", this->m_vertices, this->m_uvs, this->m_normals, this->m_indices))
	{
		return false;
	}

	this->m_selectionColor = glm::vec3(0.0f, 0.0f, 0.0f);

	glGenBuffers(1, &this->m_vertexBufferID);	//generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertexBufferID);	//bind vertex buffer
	glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(glm::vec3), &this->m_vertices[0], GL_STATIC_DRAW); //give vertices to OpenGL

	glGenBuffers(1, &this->m_uvsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_uvsBufferID);
	glBufferData(GL_ARRAY_BUFFER, this->m_uvs.size() * sizeof(glm::vec2), &this->m_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->m_normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, this->m_normals.size() * sizeof(glm::vec3), &this->m_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->m_indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size() * sizeof(unsigned short), &(this->m_indices[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertexBufferID);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_uvsBufferID);
	glVertexAttribPointer
		(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_normalBufferID);
	glVertexAttribPointer
		(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glBindTexture(GL_TEXTURE_2D, this->m_texture->GetTextureID());

	this->m_shaderID = shaderID;
	glUseProgram(this->m_shaderID);

	this->m_modelMatrix = this->m_translationMatrix * this->m_rotationMatrix;

	glm::mat4 modelViewProjection = camera->GetProjectionMatrix() * camera->GetViewMatrix() * this->m_modelMatrix;
	glUniformMatrix4fv(this->m_matrixID, 1, GL_FALSE, &modelViewProjection[0][0]);
	this->m_lightDirectionID = glGetUniformLocation(this->m_shaderID, "lightDirection");
	this->m_lightColorID = glGetUniformLocation(this->m_shaderID, "lightColor");
	this->m_selectionColorID = glGetUniformLocation(this->m_shaderID, "selectionColor");

	return true;
}

void Model::Shutdown()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glDeleteBuffers(1, &this->m_vertexBufferID);
	glDeleteBuffers(1, &this->m_uvsBufferID);
	glDeleteBuffers(1, &this->m_normalBufferID);
	glDeleteProgram(this->m_shaderID);
}

void Model::Frame(Camera* camera)
{
	this->RecalculateModelMatrix();

	for each (Model* m in this->m_children)
	{
		m->Frame(camera);
	}
}

void Model::Render(Camera* camera, Light* light)
{
	glm::mat4 modelViewProjection = camera->GetProjectionMatrix() * camera->GetViewMatrix() * this->m_modelMatrix;
	glUniformMatrix4fv(this->m_matrixID, 1, GL_FALSE, &modelViewProjection[0][0]);
	glm::vec4 lightDir = glm::vec4(light->GetDirection(), 0.0f) * this->m_modelMatrix;
	glUniform4f(this->m_lightDirectionID, lightDir.x, lightDir.y, lightDir.z, lightDir.w);
	glm::vec4 lightColor = light->GetColor();
	glUniform4f(this->m_lightColorID, lightColor.r, lightColor.g, lightColor.b, lightColor.w);
	glUniform3f(this->m_selectionColorID, this->m_selectionColor.r, this->m_selectionColor.g, this->m_selectionColor.b);

	glBindTexture(GL_TEXTURE_2D, this->m_texture->GetTextureID());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_indexBufferID);
	glDrawElements(GL_TRIANGLES, this->m_indices.size(), GL_UNSIGNED_SHORT, (void*)0);

	for (std::vector<Model*>::iterator it = this->m_children.begin(); it != this->m_children.end(); ++it)
	{
		(*it)->Render(camera, light);
	}
}

void Model::AddChild(Model* child)
{
	this->m_children.push_back(child);
	child->SetParent(this);
}

void Model::SetParent(Model* parent)
{
	this->m_parent = parent;
	if (this->m_parent != nullptr)
	{
		for (std::vector<Model*>::iterator it = this->m_parent->m_children.begin(); it != this->m_parent->m_children.end(); ++it)
		{
			if ((*it) == this) return;
		}

		this->m_parent->m_children.push_back(this);
		this->RecalculateModelMatrix();
	}
}

void Model::Rotate(float angle, glm::vec3 axis)
{
	this->m_rotationMatrix *= glm::rotate(angle, axis);
}

void Model::Translate(glm::vec3 direction)
{
	this->m_position += direction;
	this->m_translationMatrix = glm::translate(this->m_position);
}

void Model::RecalculateModelMatrix()
{
	glm::mat4 parentMatrix = glm::mat4(1.0f);
	if (this->m_parent != nullptr)
	{
		parentMatrix = this->m_parent->m_modelMatrix;
	}
	this->m_modelMatrix = parentMatrix * this->m_translationMatrix * this->m_rotationMatrix;
}

bool Model::LoadOBJ(const char const* filePath, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<unsigned short>& indices)
{
	std::ifstream modelFile;
	modelFile.open(filePath, std::ios::in);

	std::vector<glm::vec3> verticesPos;
	std::vector<glm::vec2> verticesTex;
	std::vector<glm::vec3> verticesNormal;
	std::vector<unsigned long> indicesPos;
	std::vector<unsigned long> indicesTex;
	std::vector<unsigned long> indicesNormal;

	std::string materialName = "";
	std::string line = "";

	if (!modelFile.is_open())
	{
		fprintf(stderr, "Couldn't find any file with this path: %s\n", filePath);
	}
	else
	{
		char checkChar = ' ';
		while (checkChar != EOF)
		{
			checkChar = modelFile.get();
			float x, y, z;

			int i = 0;
			switch (checkChar)
			{
			case 'v':
				//Vertices info
				checkChar = modelFile.get();
				switch (checkChar)
				{
				case ' ':
					//Vertex position;
					modelFile >> x >> y >> z;
					verticesPos.push_back(glm::vec3(x, y, z));
					break;
				case 't':
					//Vertex tex coord
					modelFile >> x >> y;
					verticesTex.push_back(glm::vec2(x, y));
					break;
				case 'n':
					//Vertex normal
					modelFile >> x >> y >> z;
					verticesNormal.push_back(glm::vec3(x, y, z));
					break;
				}
				break;
			case 'f':
				//Faces info (indices info also)
				//vpos/vnormal/vtex

				checkChar = modelFile.get();
				if (checkChar != ' ')
				{
					continue;
				}

				line = "";
				while (checkChar != '\n')
				{
					if (checkChar == EOF)
					{
						break;
					}
					checkChar = modelFile.get();
					line += checkChar;
				}

				i = 0;

				//FIRST VERTEX
				while (line[i] != '/')
				{
					++i;
				}

				indicesPos.push_back(atoi(line.substr(0, i).c_str()));
				line = line.substr(i + 1);

				i = 0;
				while (line[i] != '/')
				{
					++i;
				}
				indicesTex.push_back(atoi(line.substr(0, i).c_str()));
				line = line.substr(i + 1);
				i = 0;

				while (line[i] != ' ')
				{
					++i;
				}
				indicesNormal.push_back(atoi(line.substr(0, i).c_str()));
				line = line.substr(i + 1);
				i = 0;

				//SECOND VERTEX
				while (line[i] != '/')
				{
					++i;
				}

				indicesPos.push_back(atoi(line.substr(0, i).c_str()));
				line = line.substr(i + 1);

				i = 0;
				while (line[i] != '/')
				{
					++i;
				}
				indicesTex.push_back(atoi(line.substr(0, i).c_str()));
				line = line.substr(i + 1);
				i = 0;

				while (line[i] != ' ')
				{
					++i;
				}
				indicesNormal.push_back(atoi(line.substr(0, i).c_str()));
				line = line.substr(i + 1);
				i = 0;

				//THIRD VERTEX
				while (line[i] != '/')
				{
					++i;
				}

				indicesPos.push_back(atoi(line.substr(0, i).c_str()));
				line = line.substr(i + 1);

				i = 0;
				while (line[i] != '/')
				{
					++i;
				}
				indicesTex.push_back(atoi(line.substr(0, i).c_str()));
				line = line.substr(i + 1);
				i = 0;

				indicesNormal.push_back(atoi(line.c_str()));

				break;
			case 'm':
				//Material info
				//Later I'll do this
				break;
			}
		}
	}

	this->m_vertsNum = indicesPos.size();

	std::string myTextureExtenstion = "";
	std::string myTextureName = this->m_texture->GetName();
	int i = 0;
	i = myTextureName.find_last_of('.') + 1;
	while (myTextureName[i] != '\0')
	{
		myTextureExtenstion += myTextureName[i];
		++i;
	}
	bool isDDS = myTextureExtenstion == "dds" || myTextureExtenstion == "DDS";

	for (int i = 0; i < this->m_vertsNum; ++i)
	{
		vertices.push_back(verticesPos[indicesPos[i] - 1]);
		normals.push_back(verticesNormal[indicesNormal[i] - 1]);
		if (isDDS)
		{
			glm::vec2 uv = glm::vec2(1.0f - verticesTex[indicesTex[i] - 1].x, verticesTex[indicesTex[i] - 1].y);
			uvs.push_back(uv);
		}
		else
		{
			uvs.push_back(verticesTex[indicesTex[i] - 1]);
		}

		indices.push_back(i);
	}

	return true;
}

void Model::Select()
{
	this->m_selectionColor.r = 1.0f;
}

void Model::Unselect()
{
	this->m_selectionColor.r -= 1.0f;
}

Model* Model::GetParent()
{
	return this->m_parent;
}
#include "Model.h"

Model::Model()
{
	this->m_vertexBufferID = 0;
	this->m_modelMatrix = glm::mat4(1.0f);
	this->m_shaderID = 0;
	this->m_vertsNum = 0;
	this->m_texture = 0;
	this->m_uvsBufferID = 0;
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

	if (!this->LoadOBJ("../Models/Cube.obj", this->m_vertices, this->m_uvs, this->m_normals))
	{
		return false;
	}

	this->m_vertsNum = this->m_vertices.size();

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

	glDrawArrays(GL_TRIANGLES, 0, this->m_vertsNum);

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

bool Model::LoadOBJ(const char const* filePath, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals)
{
	printf("Loading OBJ file %s...\n", filePath);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(filePath, "r");
	if (file == NULL){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while (1){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		vertices.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);

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
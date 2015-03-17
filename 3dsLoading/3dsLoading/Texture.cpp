#include "Texture.h"

Texture::Texture()
{
	this->m_textureHeight = 0;
	this->m_textureWidth = 0;
	this->m_textureID = 0;
}

Texture::~Texture()
{
	
}

bool Texture::Initialize(const std::string const* filePath)
{
	LoadFromFile(filePath);

	if (this->m_textureID == 0)
	{
		return false;
	}

	return true;
}

void Texture::Shutdown()
{

}

GLuint Texture::GetTextureID()
{
	return this->m_textureID;
}

void Texture::LoadFromFile(const std::string const* filePath)
{
	this->m_textureID = SOIL_load_OGL_texture((*filePath).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}
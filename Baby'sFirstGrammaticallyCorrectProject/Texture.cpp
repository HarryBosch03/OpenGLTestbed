#include "Texture.h"

#include "ShaderProgram.h"
#include "Logger.h"

#include "stb_image.h"

#include <map>

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
}

std::map<std::string, Texture*> boundTextures;

void Texture::PassDataToGL(void* data, GLenum type, GLint internalFormat, const std::string& fileLoc)
{
	if (!data)
	{
		LOG_ERROR("Error Loading \"" << fileLoc << "\"");
		return;
	}
	if (channels != 3 && channels != 4)
	{
		LOG_ERROR("Texture \"" << FNAME(fileLoc) << "\" has an invalid number of channels (" << channels << ")");
	}

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, type, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	initalized = true;
}

Texture& Texture::LoadFromFile(const std::string& fileLoc)
{
	stbi_set_flip_vertically_on_load(true);
	bool hdr = stbi_is_hdr(fileLoc.c_str());

	if (hdr)
	{
		float* data = stbi_loadf(fileLoc.c_str(), &width, &height, &channels, 4);
		PassDataToGL(data, GL_FLOAT, GL_RGB16F, fileLoc);
	}
	else
	{
		unsigned char* data = stbi_load(fileLoc.c_str(), &width, &height, &channels, 4);
		PassDataToGL(data, GL_UNSIGNED_BYTE, GL_RGBA, fileLoc);
	}
	return *this;
}

void Texture::Bind(const std::string& ref)
{
	boundTextures[ref] = this;
	this->ref = ref;
}

void Texture::Unbind()
{
	boundTextures.erase(ref);
}

void Texture::BindAll()
{
	for (std::pair<const std::string, Texture*>& pair : boundTextures)
	{
		if (pair.second->bound) continue;

		int index = glGetUniformLocation(ShaderProgram::Current->programHandle, pair.first.c_str());
		if (index == -1) continue;

		pair.second->bound = true;
		pair.second->boundIndex = index;

		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, pair.second->handle);

		glUniform1i(glGetUniformLocation(ShaderProgram::Current->programHandle, pair.first.c_str()), index);
	}
}

void Texture::UnbindAll()
{
	for (std::pair<const std::string, Texture*>& pair : boundTextures)
	{
		if (!pair.second->bound) continue;

		glActiveTexture(GL_TEXTURE0 + pair.second->boundIndex);
		glBindTexture(GL_TEXTURE_2D, 0);

		glUniform1i(glGetUniformLocation(ShaderProgram::Current->programHandle, pair.first.c_str()), 0);

		pair.second->bound = false;
		pair.second->boundIndex = 0;
	}
}
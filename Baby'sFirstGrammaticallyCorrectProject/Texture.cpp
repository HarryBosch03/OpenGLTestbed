#include "Texture.h"

#include "ShaderProgram.h"
#include "LogMaster.h"
#include "FileUtility.h"

#include "stb_image.h"

std::map<std::string, Texture*> boundTextures;

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
}

void Texture::PassDataToGL(void* data, GLenum type, GLint internalFormat, const std::string& fileLoc, const TextureImportSettings& settings)
{
	if (!data)
	{
		LogError("Error Loading \"" << fileLoc << "\"");
		return;
	}
	if (channels != 3 && channels != 4)
	{
		LogError("Texture \"" << Utility::Files::FileName(fileLoc) << "\" has an invalid number of channels (" << channels << ")");
	}

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)settings.filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, type, data);
	if (settings.useMipmaps) glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	initalized = true;
}

void Texture::Orphan(int width, int height, GLint internalFormat, GLint format, GLenum type)
{
	if (!handle)
	{
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, handle);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Asset& Texture::LoadFromFile(const std::string& fileLoc, void* args)
{
	Asset::LoadFromFile(fileLoc, args);

	settings = args ? *static_cast<TextureImportSettings*>(args) : TextureImportSettings();

	stbi_set_flip_vertically_on_load(true);
	bool hdr = stbi_is_hdr(fileLoc.c_str());

	if (hdr)
	{
		float* data = stbi_loadf(fileLoc.c_str(), &width, &height, &channels, 4);
		PassDataToGL(data, GL_FLOAT, GL_RGB16F, fileLoc, settings);
	}
	else
	{
		unsigned char* data = stbi_load(fileLoc.c_str(), &width, &height, &channels, 4);
		PassDataToGL(data, GL_UNSIGNED_BYTE, GL_RGBA, fileLoc, settings);
	}
	return *this;
}

Asset& Texture::Reload()
{
	glDeleteTextures(1, &handle);
	handle = 0;
	width = 0;
	height = 0;
	channels = 0;
	initalized = false;
	return Asset::Reload();
}

bool Texture::DoesFileMatch(const std::string& fileloc)
{
	std::string ext = Utility::Files::Ext(fileloc);
	if (ext == "png") return true;
	if (ext == "tga") return true;
	if (ext == "hdr") return true;
	return false;
}

void Texture::Bind(const std::string& ref)
{
	boundTextures[ref] = this;
	if (!this) return;
	this->ref = ref;
}

void Texture::Unbind()
{
	if (!this) return;
	boundTextures.erase(ref);
}

void Texture::BindAll()
{
	for (std::pair<const std::string, Texture*>& pair : boundTextures)
	{
		if (!pair.second) continue;

		int index = glGetUniformLocation(ShaderProgram::Current->programHandle, pair.first.c_str());
		if (index == -1) continue;

		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, pair.second->handle);

		glUniform1i(glGetUniformLocation(ShaderProgram::Current->programHandle, pair.first.c_str()), index);
	}
}

void Texture::UnbindAll()
{
	for (std::pair<const std::string, Texture*>& pair : boundTextures)
	{
		int index = glGetUniformLocation(ShaderProgram::Current->programHandle, pair.first.c_str());
		if (index == -1) continue;

		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, 0);

		glUniform1i(glGetUniformLocation(ShaderProgram::Current->programHandle, pair.first.c_str()), 0);
	}
}

const std::map<std::string, Texture*>& Texture::BoundTextures()
{
	return boundTextures;
}

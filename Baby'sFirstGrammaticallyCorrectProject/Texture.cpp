#include "Texture.h"

#include "stb_image.h"
#include "Logger.h"

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
}

Texture& Texture::LoadFromFile(const std::string& fileLoc)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileLoc.c_str(), &width, &height, &channels, 4);
	if (!data)
	{
		LOG_ERROR("File \"" << fileLoc << "\" was not Found.");
		return *this;
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	initalized = true;
	return *this;
}

void Texture::Bind(int index)
{
	if (bound) return;

	bound = true;
	boundIndex = index;

	glActiveTexture(GL_TEXTURE0 + boundIndex);
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::Unbind()
{
	if (!bound) return;
	
	glActiveTexture(GL_TEXTURE0 + boundIndex);
	glBindTexture(GL_TEXTURE_2D, 0);

	bound = false;
	boundIndex = -1;
}

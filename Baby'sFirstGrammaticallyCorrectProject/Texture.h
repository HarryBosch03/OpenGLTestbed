#pragma once

#include "Graphics.h"
#include <string>

class ShaderProgram;

class Texture
{
	GLuint handle = 0;

	int width = 0, height = 0, channels = 0;
	bool initalized = false;

	int boundIndex = -1;
	bool bound = false;
	std::string ref;

	void PassDataToGL(void* data, GLenum type, GLint internalFormat, const std::string& fileLoc);

public:
	Texture() = default;
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	~Texture();

	Texture& LoadFromFile(const std::string& fileLoc);
	void Bind(const std::string& ref);
	void Unbind();

	inline const int& Width() const { return width; }
	inline const int& Height() const { return height; }
	inline const int& Channels() const { return channels; }

	static void BindAll();
	static void UnbindAll();
};
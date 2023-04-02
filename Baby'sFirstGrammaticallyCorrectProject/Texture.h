#pragma once

#include "assimp/texture.h"
#include "Graphics.h"

class Texture
{
	GLuint handle = 0;

	int width = 0, height = 0, channels = 0;
	bool initalized = false;

	int boundIndex = -1;
	bool bound = false;

public:
	Texture() = default;
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	~Texture();

	Texture& LoadFromFile(const std::string& fileLoc);
	void Bind(int index);
	void Unbind();

	inline const int& Width() const { return width; }
	inline const int& Height() const { return height; }
	inline const int& Channels() const { return channels; }
};
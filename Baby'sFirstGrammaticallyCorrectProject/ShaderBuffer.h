#pragma once

#include "Graphics.h"
#include "maths.h"

#include <string>
#include <vector>

class ShaderBuffer
{
	GLuint handle;
	int bindingIndex;

public:
	byte* data;
	int dataSize;

	ShaderBuffer() = default;
	ShaderBuffer(int bindingIndex);
	ShaderBuffer(const ShaderBuffer& other) = delete;
	ShaderBuffer& operator=(const ShaderBuffer& other) = delete;
	~ShaderBuffer();

	void Initalize(int bindingIndex, byte* data, int dataSize);

	void Send();
};
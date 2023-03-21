#include "ShaderBuffer.h"

ShaderBuffer::ShaderBuffer(int bindingIndex)
{
	Initalize(bindingIndex, data, dataSize);
}

ShaderBuffer::~ShaderBuffer()
{
	glDeleteBuffers(1, &handle);
}

void ShaderBuffer::Initalize(int bindingIndex, byte* data, int dataSize)
{
	glGenBuffers(1, &handle);
	this->bindingIndex = bindingIndex;

	this->data = data;
	this->dataSize = dataSize;
}

void ShaderBuffer::Send()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, handle);
	glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, handle);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

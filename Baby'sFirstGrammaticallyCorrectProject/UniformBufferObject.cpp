#include "UniformBufferObject.h"

#include "ShaderProgram.h"

#include <map>

std::map<std::string, UniformBufferObject*> UniformBufferObject::map;

UniformBufferObject::UniformBufferObject()
{
	glGenBuffers(1, &handle);
}

UniformBufferObject::~UniformBufferObject()
{
	delete[] data;
}

UniformBufferObject& UniformBufferObject::Find(const std::string& ref)
{
	if (!map.count(ref)) map[ref] = new UniformBufferObject();
	return *map[ref];
}

void UniformBufferObject::SendToActiveShader(ShaderProgram* program)
{
	int binding = 0;
	for (const std::pair<const std::string, UniformBufferObject*>& buffer : map)
	{
		GLuint& programHandle = program->programHandle;
		int index = glGetUniformBlockIndex(programHandle, buffer.first.c_str());
		if (index == -1) continue;
		
		GLint debug;
		glGetActiveUniformBlockiv(programHandle, index, GL_UNIFORM_BLOCK_DATA_SIZE, &debug);

		glUniformBlockBinding(programHandle, index, binding++);

		glBindBuffer(GL_UNIFORM_BUFFER, buffer.second->handle);
		glBufferData(GL_UNIFORM_BUFFER, buffer.second->sizeBytes, buffer.second->data, GL_STATIC_DRAW);

		glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer.second->handle);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer.second->sizeBytes, buffer.second->data);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

byte* UniformBufferObject::Get(const std::string& ref, int sizeBytes, int offsetBytes, const byte* fallback)
{
	UniformBufferObject& buffer = Find(ref);
	if (!buffer.data || buffer.sizeBytes != sizeBytes)
	{
		delete[] buffer.data;
		buffer.data = new byte[sizeBytes];
		buffer.sizeBytes = sizeBytes;
		if (fallback) std::memcpy(&buffer.data[offsetBytes], &fallback[offsetBytes], sizeBytes);
	}
	return &((byte*)buffer.data)[offsetBytes];
}
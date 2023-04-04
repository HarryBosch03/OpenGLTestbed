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
	for (const std::pair<const std::string, UniformBufferObject*>& buffer : map)
	{
		GLuint& programHandle = program->programHandle;
		int index = glGetUniformBlockIndex(programHandle, buffer.first.c_str());
		if (index == -1) continue;
		GLint binding = 0;
		glGetActiveUniformBlockiv(programHandle, index, GL_UNIFORM_BLOCK_BINDING, &binding);
		glUniformBlockBinding(programHandle, index, binding);

		glBindBuffer(GL_UNIFORM_BUFFER, buffer.second->handle);
		glBufferData(GL_UNIFORM_BUFFER, buffer.second->sizeBytes, buffer.second->data, GL_STATIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, buffer.second->handle, 0, buffer.second->sizeBytes);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
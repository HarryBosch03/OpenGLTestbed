#include "UniformBufferObject.h"

#include <map>

std::map<std::string, UniformBufferObject> map;

void UniformBufferObject::Set(const std::string& ref, void* data, int sizeBytes)
{
	UniformBufferObject& buffer = map[ref];

	if (!buffer.handle) glGenBuffers(1, &buffer.handle);

	glBindBuffer(GL_UNIFORM_BUFFER, buffer.handle);
	glBufferData(GL_UNIFORM_BUFFER, sizeBytes, data, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::SendToActiveShader()
{
	for (const std::pair<const std::string, UniformBufferObject>& buffer : map)
	{
		// TODO
	}
}
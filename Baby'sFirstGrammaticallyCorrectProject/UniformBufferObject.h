#pragma once

#include "Graphics.h"

#include <string>

class UniformBufferObject
{
	GLuint handle = 0;

	UniformBufferObject() = default;
	UniformBufferObject(const UniformBufferObject& other) = delete;
	UniformBufferObject& operator=(const UniformBufferObject& other) = delete;
	~UniformBufferObject() = default;

public:

	static void Set(const std::string& ref, void* data, int sizeBytes);
	static void SendToActiveShader();
};
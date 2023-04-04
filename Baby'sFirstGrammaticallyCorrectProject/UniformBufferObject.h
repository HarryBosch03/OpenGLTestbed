#pragma once

#include "Graphics.h"

#include <string>
#include <map>

class ShaderProgram;

class UniformBufferObject
{
	GLuint handle = 0;
	void* data = nullptr;
	int sizeBytes = 0;

	static std::map<std::string, UniformBufferObject*> map;
	
	UniformBufferObject();
	UniformBufferObject(const UniformBufferObject& other) = delete;
	UniformBufferObject& operator=(const UniformBufferObject& other) = delete;
	~UniformBufferObject();

	static UniformBufferObject& Find(const std::string& ref);

public:
	static void SendToActiveShader(ShaderProgram* program);

	template<typename T, int size>
	static T& Lookup(const std::string& ref)
	{
		UniformBufferObject& buffer = Find(ref);
		void* entry = buffer.data;
		if (!buffer.data)
		{
			buffer.data = new T[size];
			buffer.sizeBytes = sizeof(T) * size;
		}
		return *(T*)buffer.data;
	}

	template<typename T>
	static T& Lookup(const std::string& ref)
	{
		return Lookup<T, 1>(ref);
	}
};
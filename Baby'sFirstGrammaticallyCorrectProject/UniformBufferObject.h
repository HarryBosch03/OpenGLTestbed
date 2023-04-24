#pragma once

#include "Graphics.h"
#include "Maths.h"

#include <string>
#include <map>

class ShaderProgram;

class UniformBufferObject
{
	GLuint handle = 0;
	byte* data = nullptr;
	int sizeBytes = 0;

	static std::map<std::string, UniformBufferObject*> map;
	
	UniformBufferObject();
	UniformBufferObject(const UniformBufferObject& other) = delete;
	UniformBufferObject& operator=(const UniformBufferObject& other) = delete;
	~UniformBufferObject();

	static UniformBufferObject& Find(const std::string& ref);

public:
	static void SendToActiveShader(ShaderProgram* program);

	static byte* Get(const std::string& ref, int sizeBytes, int offsetBytes = 0, const byte* fallback = nullptr);

	template<typename T, int size>
	static T& Lookup(const std::string& ref, const T& fallback = {})
	{
		return *(T*)Get(ref, sizeof(T) * size, 0, (const byte*)(&fallback));
	}

	template<typename T>
	static T& Lookup(const std::string& ref, const T& fallback = {})
	{
		return Lookup<T, 1>(ref, fallback);
	}
};
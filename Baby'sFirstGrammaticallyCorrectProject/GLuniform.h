#pragma once

#include "Graphics.h"
#include "Maths.h"

#include <string>
#include <vector>

class ShaderProgram;

class IGLuniform
{
protected:
	std::string name;

	GLint GetHandle(ShaderProgram* shaderProgram);
	virtual void Send(ShaderProgram* shaderProgram) = 0;

public:
	IGLuniform(const std::string& name);
	~IGLuniform();

	static void SendAll(ShaderProgram* shaderProgram);
};

template<typename T>
class GLuniform : public IGLuniform
{
public:
	T value;
	
	GLuniform(const std::string& name) : IGLuniform(name) { }
	GLuniform(const std::string& name, const T& value) : IGLuniform(name)
	{
		this->value = value;
	}

	T& operator =(const T& other)
	{
		return value = other;
	}

	void Set(ShaderProgram* shaderProgram);

	void Send(ShaderProgram* shaderProgram) override
	{
		Set(shaderProgram);
	}
};

template<typename T>
class GLuniformList : public GLuniform<std::vector<T>>
{
	void Send(ShaderProgram* shaderProgram) override
	{
		GLuniform<std::vector<T>>::Set(shaderProgram);
	}

public:
	GLuniformList(const std::string& name, int buffer = 0) : GLuniform<std::vector<T>>(name) 
	{
		Buffer(buffer);
	}

	GLuniformList& Buffer(int count)
	{
		for (int i = 0; i < count; i++)
		{
			GLuniform<std::vector<T>>::value.push_back({});
		}
		return *this;
	}

	T& operator[](int i)
	{
		return GLuniform<std::vector<T>>::value[i];
	}

	T& operator[](const GLuniform<int>& i)
	{
		return (*this)[i.value];
	}
};
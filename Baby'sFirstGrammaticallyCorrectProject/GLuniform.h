#pragma once

#include "Graphics.h"
#include "Maths.h"
#include "GLuniform.h"

#include <string>
#include <vector>
#include <map>

class ShaderProgram;

class IGLuniform;

template<typename T>
class GLuniform;

template<typename T>
class GLuniformList;

namespace Uniform
{
	extern std::map<std::string, IGLuniform*> uniforms;

	template<typename T>
	T& GetUniform(const std::string& ref)
	{
		if (!uniforms.count(ref)) uniforms[ref] = new T(ref);
		return *static_cast<T*>(uniforms[ref]);
	}

	template<typename T>
	void Set(const std::string& ref, const T& val)
	{
		GetUniform<GLuniform<T>>(ref).value = val;
	}

	template<typename T>
	T& Get(const std::string& ref)
	{
		return GetUniform<GLuniform<T>>(ref).value;
	}

	template<typename T>
	void SetList(const std::string& ref, int index, const T& val)
	{
		GetUniform<GLuniformList<T>>(ref).value[index] = val;
	}

	template<typename T>
	T& GetList(const std::string& ref, int index)
	{
		return GetUniform<GLuniformList<T>>(ref).value[index];
	}

	template<typename T>
	void PushList(const std::string& ref, const T& val)
	{
		GetUniform<GLuniformList<T>>(ref).value.push_back(val);
	}
	
	template<typename T>
	void SetBuffer(const std::string& ref, int size, int index, const T& val)
	{
		GLuniformList<T>& uniform = GetUniform<GLuniformList<T>>(ref);
		for (size_t i = 0; i < size - uniform.value.size(); i++)
		{
			uniform.value.push_back({});
		}
		uniform.value[index] = val;
	}

	template<typename T>
	const void Allocate(const std::string ref, int count)
	{
		for (size_t i = 0; i < count; i++)
		{
			GetUniform<GLuniformList<T>>(ref).value.push_back({});
		}
	}
}

class IGLuniform
{
protected:
	std::string name;

	GLint GetHandle(ShaderProgram* shaderProgram);
	virtual void Send(ShaderProgram* shaderProgram) = 0;

public:
	IGLuniform(const std::string& name);

	static void SendAll(ShaderProgram* shaderProgram);
};

template<typename T>
class GLuniform : public IGLuniform
{
public:
	T value = {};

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
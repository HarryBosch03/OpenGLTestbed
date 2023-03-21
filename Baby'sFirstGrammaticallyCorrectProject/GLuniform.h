#pragma once

#include "Graphics.h"
#include "Maths.h"
#include <string>

class ShaderProgram;

class GLuniform
{
	std::string name;
	GLint handle;

public:
	GLuniform() = default;
	GLuniform(const GLuniform&) = delete;
	GLuniform& operator= (const GLuniform&) = delete;

	GLuniform& Initalize(std::string name);

	void SetVec3(const Vec3& data);
	void SetMat4(const Mat4& data);
	void SetFloat(float data);
	void SetInt(int data);

	void SetVec3Array(const Vec3* data, int count);

	static void BindInitalizationContext(const ShaderProgram& shader);
	static void UnbindInitalizationContext();
};
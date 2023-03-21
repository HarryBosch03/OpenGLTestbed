#include "GLuniform.h"

#include "ShaderProgram.h"

const ShaderProgram* currentShader = nullptr;

GLuniform& GLuniform::Initalize(std::string name)
{
	this->name = name;
	const char* nameC = name.c_str();
	handle = glGetUniformLocation(currentShader->shaderProgram, nameC);
	return *this;
}

void GLuniform::SetVec3(const Vec3& data)
{
	SetVec3Array(&data, 1);
}

void GLuniform::SetMat4(const Mat4& data)
{
	glUniformMatrix4fv(handle, 1, GL_FALSE, &data[0][0]);
}

void GLuniform::SetFloat(float data)
{
	glUniform1fv(handle, 1, &data);
}

void GLuniform::SetInt(int data)
{
	glUniform1iv(handle, 1, &data);
}

void GLuniform::SetVec3Array(const Vec3* data, int count)
{
	glUniform3fv(handle, count, &data->x);
}

void GLuniform::BindInitalizationContext(const ShaderProgram& shader)
{
	currentShader = &shader;
}

void GLuniform::UnbindInitalizationContext()
{
	currentShader = nullptr;
}
#include "GLuniform.h"

#include "ShaderProgram.h"

const ShaderProgram* currentShader = nullptr;

GLint GLuniform::GetHandle()
{
	const char* nameC = name.c_str();
	return glGetUniformLocation(ShaderProgram::Current->shaderProgram, nameC);
}

GLuniform::GLuniform(const std::string& name)
{
	this->name = name;
}
void GLuniform::Set(const Mat4& data)
{
	Set(&data, 1);
}

void GLuniform::Set(const Mat4* data, int count)
{
	glUniformMatrix4fv(GetHandle(), count, GL_FALSE, (float*)data);
}

void GLuniform::BindInitalizationContext(const ShaderProgram& shader)
{
	currentShader = &shader;
}

void GLuniform::UnbindInitalizationContext()
{
	currentShader = nullptr;
}
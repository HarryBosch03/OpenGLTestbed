#include "GLuniform.h"

#include "ShaderProgram.h"

const ShaderProgram* currentShader = nullptr;

std::vector<IGLuniform*> uniforms;

GLint IGLuniform::GetHandle(ShaderProgram* shaderProgram)
{
	const char* nameC = name.c_str();
	return glGetUniformLocation(shaderProgram->programHandle, nameC);
}

IGLuniform::IGLuniform(const std::string& name)
{
	this->name = name;
	uniforms.push_back(this);
}

IGLuniform::~IGLuniform()
{
	for (auto it = uniforms.begin(); it != uniforms.end(); ++it)
	{
		if (*it != this) continue;
		uniforms.erase(it);
		break;
	}
}

void IGLuniform::SendAll(ShaderProgram* shaderProgram)
{
	for (IGLuniform* uniform : uniforms)
	{
		uniform->Send(shaderProgram);
	}
}

#define SET_DEF(t, set, cast) \
void GLuniform<t>::Set(ShaderProgram* shaderProgram) \
{ \
	set(GetHandle(shaderProgram), 1, (cast*)&value); \
} \
void GLuniform<std::vector<t>>::Set(ShaderProgram* shaderProgram) \
{ \
	set(GetHandle(shaderProgram), value.size(), (cast*)value.data()); \
}

#define SET_DEF_F(t, set) SET_DEF(t, set, GLfloat)
#define SET_DEF_I(t, set) SET_DEF(t, set, GLint)

void UniformSetMat(GLint handle, int size, GLfloat* start)
{
	glUniformMatrix4fv(handle, size, false, start);
}

SET_DEF_F(float, glUniform1fv)
SET_DEF_I(int, glUniform1iv)

SET_DEF_F(Vec2, glUniform2fv)
SET_DEF_F(Vec3, glUniform3fv)
SET_DEF_F(Vec4, glUniform4fv)
SET_DEF_F(Mat4, UniformSetMat)
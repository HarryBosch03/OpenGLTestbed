#include "GLuniform.h"

#include "ShaderProgram.h"
#include <map>

const ShaderProgram* currentShader = nullptr;

GLint GLuniform::GetHandle(ShaderProgram* shaderProgram)
{
	const char* nameC = name.c_str();
	return glGetUniformLocation(shaderProgram->programHandle, nameC);
}

GLuniform::GLuniform(const std::string& name)
{
	this->name = name;
	uniforms[name] = this;
}

GLuniform::~GLuniform()
{
	for (auto it = uniforms.begin(); it != uniforms.end(); ++it)
	{
		if ((*it).second != this) continue;
		uniforms.erase(it);
		break;
	}
}

void GLuniform::SendAll(ShaderProgram* shaderProgram)
{
	for (std::pair<const std::string, GLuniform*>& uniform : uniforms)
	{
		uniform.second->Send(shaderProgram);
	}
}

#define SET_DEF(t, set, cast) \
void GLuniformData<t>::Set(ShaderProgram* shaderProgram) \
{ \
	set(GetHandle(shaderProgram), 1, (cast*)&value); \
} \
void GLuniformData<std::vector<t>>::Set(ShaderProgram* shaderProgram) \
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
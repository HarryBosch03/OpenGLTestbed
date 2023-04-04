#include "GLuniform.h"

#include "ShaderProgram.h"
#include <map>

const ShaderProgram* currentShader = nullptr;

std::map<std::string, IGLuniform*> Uniform::uniforms;

GLint IGLuniform::GetHandle(ShaderProgram* shaderProgram)
{
	const char* nameC = name.c_str();
	return glGetUniformLocation(shaderProgram->programHandle, nameC);
}

IGLuniform::IGLuniform(const std::string& name)
{
	this->name = name;
}

void IGLuniform::SendAll(ShaderProgram* shaderProgram)
{
	for (std::pair<const std::string, IGLuniform*>& uniform : Uniform::uniforms)
	{
		uniform.second->Send(shaderProgram);
	}
}

#define SET_DEF(t, set, cast) \
void GLuniform<t>::Set(ShaderProgram* shaderProgram) \
{ \
	GLint handle = GetHandle(shaderProgram); \
	if (handle == -1) return; \
	set(handle, 1, (cast*)&value); \
} \
void GLuniform<std::vector<t>>::Set(ShaderProgram* shaderProgram) \
{ \
	GLint handle = GetHandle(shaderProgram); \
	if (handle == -1) return; \
	set(handle, value.size(), (cast*)value.data()); \
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
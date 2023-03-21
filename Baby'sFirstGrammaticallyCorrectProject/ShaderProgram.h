#pragma once

#include "Graphics.h"
#include "GLuniform.h"

#include <string>

class ShaderProgram
{
	bool bad = true;

	void LoadShader(GLuint& shader, std::string fileName, GLenum shaderType, GLchar* errorLog, int logSize, GLint& success);
	void LogGLError(const std::string message, const GLuint& glObject, GLchar* errorLog, const int logSize);
	void Cleanup();

	GLuniform modelMat;
	GLuniform viewMat;
	GLuniform projMat;
	GLuniform viewProjMat;
	GLuniform modelViewProjMat;
	GLuniform time;

	void SetupUniforms();

public:
	GLuint vertShader;
	GLuint fragShader;
	GLuint shaderProgram;

	ShaderProgram() = default;
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;
	~ShaderProgram();

	void Initalize(std::string vertFileName, std::string fragFileName);

	void Bind();
	void Unbind();

	void SetModelMatrix(const Mat4& model);

	static ShaderProgram* Current;
};
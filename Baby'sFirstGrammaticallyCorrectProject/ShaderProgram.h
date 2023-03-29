#pragma once

#include "Graphics.h"
#include "GLuniform.h"

#include <string>

class ShaderProgram
{
	bool bad = true;

	void LoadShader(GLuint& shader, const std::string& fileName, GLenum shaderType, GLchar* errorLog, int logSize, GLint& success);
	void LogGLError(const std::string& message, const GLuint& glObject, GLchar* errorLog, const int logSize);
	void Cleanup();

	GLuniform 
		modelMat = GLuniform("_Model"),
		viewMat = GLuniform("_View"),
		projMat = GLuniform("_Projection"),
		viewProjMat = GLuniform("_VP"),
		modelViewProjMat = GLuniform("_MVP"),
		time = GLuniform("_Time"),
		camPos = GLuniform("_CamPos");

	void SetupUniforms();

public:
	GLuint 
		vertShader, 
		fragShader, 
		shaderProgram;

	std::string 
		vertFileName, 
		fragFileName;

	ShaderProgram() = default;
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;
	~ShaderProgram();

	void Initalize(const std::string& vertFileName, const std::string& fragFileName);

	void HotReload();

	void Bind();
	void Unbind();

	void SetModelMatrix(const Mat4& model);

	static ShaderProgram* Current;
};
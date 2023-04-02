#pragma once

#include "Graphics.h"
#include "GLuniform.h"

#include <string>

const std::string ShaderPath = "./Assets/Shaders/";

class ShaderProgram
{
	bool bad = true;

	void LoadShader(GLuint& handle, const std::string& shader, GLenum shaderType, GLchar* errorLog, int logSize, GLint& success);
	void LogGLError(const std::string& message, const GLuint& glObject, GLchar* errorLog, const int logSize);
	void Cleanup();

	void Load(const std::string& name);
	void Initalize(const std::string& vert, const std::string& frag);

	ShaderProgram() = default;
	~ShaderProgram();

	void HotReload();

public:
	GLuint 
		vertHandle, 
		fragHandle, 
		programHandle;

	std::string name = "Unnamed Shader Program";

	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;

	void Bind();
	void Unbind();

	void SetModelMatrix(const Mat4& model);

	static ShaderProgram* Current;

	static ShaderProgram* Find(const std::string& name);
	static ShaderProgram* Fallback();
	static void CleanupAll();
	static void HotReloadAll();
};
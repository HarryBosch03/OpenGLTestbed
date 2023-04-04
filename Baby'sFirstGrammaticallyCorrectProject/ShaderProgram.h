#pragma once

#include "Graphics.h"
#include "GLuniform.h"
#include "Asset.h"

#include <string>

const std::string ShaderPath = "Shaders/";

class ShaderProgram : public Asset
{
	bool bad = true;

	void LoadShader(GLuint& handle, const std::string& shader, GLenum shaderType, GLchar* errorLog, int logSize, GLint& success);
	void LogGLError(const std::string& message, const GLuint& glObject, GLchar* errorLog, const int logSize);
	void Cleanup();

	void Initalize(const std::string& vert, const std::string& frag);

public:

	GLuint 
		vertHandle, 
		fragHandle, 
		programHandle;

	std::string name = "Unnamed Shader Program";

	ShaderProgram() = default;
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;
	~ShaderProgram();

	const AssetType& GetType() override { return AssetType::Shader; }
	Asset& LoadFromFile(const std::string& fileLoc, void*) override;

	void Bind();
	void Unbind();
	Asset& Reload() override;

	void SetModelMatrix(const Mat4& model);

	static ShaderProgram* Current;

	static ShaderProgram* Fallback();
};
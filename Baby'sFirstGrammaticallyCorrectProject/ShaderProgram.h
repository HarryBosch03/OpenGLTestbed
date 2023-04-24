#pragma once

#include "Graphics.h"
#include "GLuniform.h"
#include "Asset.h"

#include <string>

const std::string ShaderPath = "Shaders/";

class ShaderProperty
{
	std::string name, ref, type, def;
	std::vector<std::string> args;

public:
	ShaderProperty(const std::string& name, const std::string& ref, const std::string& type, const std::string& def);

	inline const std::string& Name() const { return name; }
	inline const std::string& Ref() const { return ref; }
	inline const std::string& Typename() const { return type; }
	inline const std::vector<std::string>& Args() const { return args; }
	inline const std::string& Default() const { return def; }
};

class ShaderProgram : public Asset
{
	bool bad = true;
	std::vector<ShaderProperty> properties;

	void LoadShader(GLuint& handle, const std::string& shader, GLenum shaderType, GLchar* errorLog, int LoggerSize, GLint& success);
	void LogGLError(const std::string& message, const GLuint& glObject, GLchar* errorLog, const int LoggerSize);
	void Cleanup();

	void Initalize(const std::string& vert, const std::string& frag);

public:

	GLuint 
		vertHandle = {},
		fragHandle = {},
		programHandle = {};

	inline const std::vector<ShaderProperty>& Properties() const { return properties; }

	ShaderProgram() = default;
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;
	~ShaderProgram();

	AssetType GetType() const override { return AssetType::Shader; }
	Asset& LoadFromFile(const std::string& fileLoc, void*) override;
	static bool DoesFileMatch(const std::string& fileloc);

	void Bind();
	void Unbind();
	Asset& Reload() override;

	void SetModelMatrix(const Mat4& model);

	static ShaderProgram* Current;

	static ShaderProgram* Fallback();
};
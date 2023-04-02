#include "ShaderProgram.h"

#include "ShaderPreprocessor.h"
#include "Application.h"
#include "Logger.h"
#include "Uniforms.h"

#include <iostream>
#include <string>
#include <sstream>
#include <map>

ShaderProgram* ShaderProgram::Current = nullptr;
ShaderProgram* fallback;

std::map<std::string, ShaderProgram*> loadedShaders;

#define ERROR_DEF_START \
const int logSize = 512; \
GLchar errorLog[logSize] = {'\0'}; \
GLint success = 0;

#define ERROR_DEF_END \
if (success == GL_FALSE) \
{ \
	LogGLError(std::string() + "Error Linking Shader Program \"" + name + "\"\n", programHandle, errorLog, logSize); \
}

void ShaderProgram::LogGLError(const std::string& message, const GLuint& glObject, GLchar* errorLog, const int logSize)
{
	std::stringstream s;
	s << message << '\n';
	glGetShaderInfoLog(glObject, logSize, nullptr, errorLog);
	s << errorLog;
	s << '\n';
	LOG_ERROR(s.str());

	bad = true;
}

void ShaderProgram::Cleanup()
{
	glDeleteShader(vertHandle);
	glDeleteShader(fragHandle);
	glDeleteProgram(programHandle);

	vertHandle = 0;
	fragHandle = 0;
	programHandle = 0;

	bad = true;
}

void ShaderProgram::HotReloadAll()
{
	for (auto& shader : loadedShaders)
	{
		shader.second->HotReload();
	}
}

void ShaderProgram::LoadShader(GLuint& handle, const std::string& shader, GLenum shaderType, GLchar* errorLog, int logSize, GLint& success)
{
	handle = glCreateShader(shaderType);
	const char* shaderC = shader.c_str();
	glShaderSource(handle, 1, &shaderC, nullptr);
	glCompileShader(handle);

	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		LogGLError(std::string() + "Shader \"" + name + "\" Failed with error: ", handle, errorLog, logSize);
	}
	else
	{
		LOG("Shader \"" << name << "\" Compiled Successfully");
	}
}

std::string ReadShader(const std::string shader, const std::string program)
{
	std::stringstream out;

	int i = 0;
	while (i < shader.length())
	{
		if (shader.substr(i, 11 + program.size()) == "#program " + program + "\n") break;
		i++;
	}

	while (i < shader.length())
	{
		if (shader.substr(i, 10) == "#program ") break;
		out << shader[i++];
	}

	return out.str();
}

void ShaderProgram::Load(const std::string& name)
{
	this->name = name;

	std::string shaderPath = ShaderPath + name;

	bool success;
	std::string shaderRaw = BMUtil::LoadTextFromFile(shaderPath + ".shader", &success, false);

	std::string vertRaw;
	std::string fragRaw;

	if (success)
	{
		vertRaw = ReadShader(shaderRaw, "vert");
		fragRaw = ReadShader(shaderRaw, "frag");
	}
	else
	{
		vertRaw = BMUtil::LoadTextFromFile(shaderPath + ".vert", &success);
		fragRaw = BMUtil::LoadTextFromFile(shaderPath + ".frag", &success);
		if (!success)
		{
			LOG_ERROR("Failed to load shader file");
			Cleanup();
			return;
		}
	}

	std::string vert = ShaderPreprocessor::ParseIncludes(vertRaw, name + "[VERT]");
	std::string frag = ShaderPreprocessor::ParseIncludes(fragRaw, name + "[FRAG]");

	Initalize(vert, frag);
}

void ShaderProgram::Initalize(const std::string& vert, const std::string& frag)
{
	bad = false;

	vertHandle = glCreateShader(GL_VERTEX_SHADER);
	fragHandle = glCreateShader(GL_FRAGMENT_SHADER);

	programHandle = glCreateProgram();

	ERROR_DEF_START
	LoadShader(vertHandle, vert, GL_VERTEX_SHADER, errorLog, logSize, success);
	LoadShader(fragHandle, frag, GL_FRAGMENT_SHADER, errorLog, logSize, success);

	glAttachShader(programHandle, vertHandle);
	glAttachShader(programHandle, fragHandle);
	glLinkProgram(programHandle);
	glGetProgramiv(programHandle, GL_LINK_STATUS, &success);

	ERROR_DEF_END

	if (bad)
	{
		Cleanup();
	}
	else
	{
		LOG_SUCCESS("Shader Compiled and Linked successfully.") << '\n';
	}
}

ShaderProgram::~ShaderProgram()
{
	Cleanup();
}

void ShaderProgram::HotReload()
{
	glDeleteShader(vertHandle);
	glDeleteShader(fragHandle);
	glDeleteProgram(programHandle);

	vertHandle = 0;
	fragHandle = 0;
	programHandle = 0;
	bad = true;

	Load(name);
}

void ShaderProgram::Bind()
{
	if (!this)
	{
		Fallback()->Bind();
		return;
	}

	if (bad && this != fallback)
	{
		Fallback()->Bind();
		return;
	}

	Current = this;
	glUseProgram(programHandle);

	const Camera& camera = *Camera::Current;

	Uniforms::ViewMat.value = Camera::Current->view;
	Uniforms::ProjMat.value = Camera::Current->projection;

	Mat4 vp = camera.projection * camera.view;
	Uniforms::ViewProjMat.value = vp;

	Uniforms::Time.value = Application::Current->Time();
	Uniforms::CamPos.value = Camera::Current->position;

	IGLuniform::SendAll(this);
}

void ShaderProgram::Unbind()
{
	glUseProgram(0);
	Current = nullptr;
}

void ShaderProgram::SetModelMatrix(const Mat4& model)
{
	const Camera& camera = *Camera::Current;

	Uniforms::ModelMat.value = model;
	Mat4 mvp = Camera::Current->projection * Camera::Current->view * model;
	Uniforms::ModelViewProjMat.value = mvp;
}

ShaderProgram* ShaderProgram::Find(const std::string& name)
{
	if (loadedShaders.count(name) > 0) return loadedShaders.at(name);

	ShaderProgram* newShader = new ShaderProgram();
	newShader->Load(name);
	loadedShaders[name] = newShader;
	return newShader;
}

ShaderProgram* ShaderProgram::Fallback()
{
	if (!fallback)
	{
		fallback = new ShaderProgram();
	}
	if (fallback->bad)
	{
		fallback->Load("fallback");
	}
	return fallback;
}

void ShaderProgram::CleanupAll()
{
	for (auto& shader : loadedShaders)
	{
		delete shader.second;
	}
	loadedShaders.clear();
}
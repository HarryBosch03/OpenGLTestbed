#include "ShaderProgram.h"

#include "ShaderPreprocessor.h"
#include "Application.h"
#include "LogMaster.h"
#include "FileUtility.h"
#include "UniformBufferObject.h"
#include "AssetDatabase.h"

#include <iostream>
#include <string>
#include <sstream>
#include <map>

ShaderProgram* ShaderProgram::Current = nullptr;

#define ERROR_DEF_START \
const int LoggerSize = 512; \
GLchar errorLog[LoggerSize] = {' '}; \
GLint success = 0;

#define ERROR_DEF_END \
if (success == GL_FALSE) \
{ \
	LogGLError(std::string() + "Error Linking Shader Program \"" + name + "\"\n", programHandle, errorLog, LoggerSize); \
}

void ShaderProgram::LogGLError(const std::string& message, const GLuint& glObject, GLchar* errorLog, const int LoggerSize)
{
	std::stringstream s;
	s << message << '\n';
	glGetShaderInfoLog(glObject, LoggerSize, nullptr, errorLog);
	s << errorLog;
	s << '\n';
	LogError(s.str());

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

void ShaderProgram::LoadShader(GLuint& handle, const std::string& shader, GLenum shaderType, GLchar* errorLog, int LoggerSize, GLint& success)
{
	handle = glCreateShader(shaderType);
	const char* shaderC = shader.c_str();
	glShaderSource(handle, 1, &shaderC, nullptr);
	glCompileShader(handle);

	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		std::string type;
		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			type = "Vertex";
			break;
		case GL_FRAGMENT_SHADER:
			type = "Fragment";
			break;
		default:
			type = "Unknown";
			break;
		}
		LogGLError(std::string() + type + " Shader \"" + name + "\" Failed with error: ", handle, errorLog, LoggerSize);
	}
	else
	{
		LogMessage("Shader \"" << name << "\" Compiled Successfully");
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

Asset& ShaderProgram::LoadFromFile(const std::string& _fileLoc, void* args)
{
	Asset::LoadFromFile(Utility::Files::RemoveExtension(_fileLoc), args);
	this->name = Utility::Files::FileName(fileloc);

	bool success;
	std::string shaderRaw = Utility::Files::LoadTextFromFile(AssetDatabase::AssetLocation() + fileloc + ".shader", &success, false);

	std::string vertRaw;
	std::string fragRaw;

	if (success)
	{
		vertRaw = ReadShader(shaderRaw, "vert");
		fragRaw = ReadShader(shaderRaw, "frag");
	}
	else
	{
		vertRaw = Utility::Files::LoadTextFromFile(AssetDatabase::AssetLocation() + fileloc + ".vert", &success);
		fragRaw = Utility::Files::LoadTextFromFile(AssetDatabase::AssetLocation() + fileloc + ".frag", &success);
		if (!success)
		{
			LogError("Failed to load shader file \"" << name << "\" at \"" << _fileLoc << "\"");
			Cleanup();
			return *this;
		}
	}

	std::string vert = ShaderPreprocessor::ParseIncludes(vertRaw, name + "[VERT]");
	std::string frag = ShaderPreprocessor::ParseIncludes(fragRaw, name + "[FRAG]");

	Initalize(vert, frag);
	return *this;
}

bool ShaderProgram::DoesFileMatch(const std::string& fileloc)
{
	std::string ext = Utility::Files::Ext(fileloc);
	if (ext == "vert") return true;
	if (ext == "frag") return true;
	if (ext == "shader") return true;
	return false;
}

void ShaderProgram::Initalize(const std::string& vert, const std::string& frag)
{
	bad = false;

	vertHandle = glCreateShader(GL_VERTEX_SHADER);
	fragHandle = glCreateShader(GL_FRAGMENT_SHADER);

	programHandle = glCreateProgram();

	ERROR_DEF_START
		LoadShader(vertHandle, vert, GL_VERTEX_SHADER, errorLog, LoggerSize, success);
	LoadShader(fragHandle, frag, GL_FRAGMENT_SHADER, errorLog, LoggerSize, success);

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
			LogSuccess("Shader Compiled and Linked successfully.");
		}
}

ShaderProgram::~ShaderProgram()
{
	Cleanup();
}

Asset& ShaderProgram::Reload()
{
	glDeleteShader(vertHandle);
	glDeleteShader(fragHandle);
	glDeleteProgram(programHandle);

	vertHandle = 0;
	fragHandle = 0;
	programHandle = 0;
	bad = true;

	return Asset::Reload();
}

void ShaderProgram::Bind()
{
	if (!this)
	{
		Fallback()->Bind();
		return;
	}

	if (bad && this != Fallback())
	{
		Fallback()->Bind();
		return;
	}

	Current = this;
	glUseProgram(programHandle);

	if (Camera::Current())
	{
		const Camera& camera = *Camera::Current();
		Mat4 vp = camera.projection * camera.view;

		Uniform::Set<Mat4>("View", camera.view);
		Uniform::Set<Mat4>("Projection", camera.projection);
		Uniform::Set<Mat4>("VP", vp);

		Uniform::Set<Mat4>("V_I", glm::inverse(camera.view));
		Uniform::Set<Mat4>("P_I", glm::inverse(camera.projection));
		Uniform::Set<Mat4>("VP_I", glm::inverse(vp));

		Uniform::Set<Vec3>("CamPosition", camera.position);
	}

	Uniform::Set<float>("Time", Application::Current()->Time());

	IGLuniform::SendAll(this);
	UniformBufferObject::SendToActiveShader(this);
}

void ShaderProgram::Unbind()
{
	glUseProgram(0);
	Current = nullptr;
}

void ShaderProgram::SetModelMatrix(const Mat4& model)
{
	if (!Camera::Current()) return;
	const Camera& camera = *Camera::Current();

	Mat4 mvp = camera.projection * camera.view * model;
	Uniform::Set<Mat4>("Model", model);
	Uniform::Set<Mat4>("MVP", mvp);
}

ShaderProgram* ShaderProgram::Fallback()
{
	return AssetDatabase::Get<ShaderProgram>("Shaders/fallback");
}
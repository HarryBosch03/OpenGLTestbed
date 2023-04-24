#include "ShaderProgram.h"

#include "Application.h"
#include "LogMaster.h"
#include "FileUtility.h"
#include "StringUtility.h"
#include "UniformBufferObject.h"
#include "ShaderCompiler.h"
#include "AssetDatabase.h"

#include <iostream>
#include <string>
#include <sstream>
#include <map>

ShaderProgram* ShaderProgram::Current = nullptr;
ShaderCompiler compiler;

#define ERROR_DEF_START \
const int LoggerSize = 512; \
GLchar errorLog[LoggerSize] = {' '}; \
GLint success = 0;

#define ERROR_DEF_END \
if (success == GL_FALSE) \
{ \
	LogGLError(std::string() + "Error Linking Shader Program \"" + fileloc + "\"\n", programHandle, errorLog, LoggerSize); \
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
		LogGLError(std::string() + type + " Shader \"" + fileloc + "\" Failed with error: ", handle, errorLog, LoggerSize);
	}
	else
	{
		LogMessage("Shader \"" << fileloc << "\" Compiled Successfully");
	}
}

Asset& ShaderProgram::LoadFromFile(const std::string& fileloc, void* args)
{
	Asset::LoadFromFile(fileloc, args);

	std::string vert, frag;

	compiler.Compile(fileloc, vert, frag, properties);

	Initalize(vert, frag);
	return *this;
}

bool ShaderProgram::DoesFileMatch(const std::string& fileloc)
{
	std::string ext = Utility::Files::Ext(fileloc);
	return ext == "shad";
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
			LogSuccess("Shader \"" << fileloc << "\" Compiled and Linked successfully.");
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
	return GetAsset<ShaderProgram>("Shaders/fallback");
}

ShaderProperty::ShaderProperty(const std::string& name, const std::string& ref, const std::string& type, const std::string& def) : name(name), ref(ref), def(def)
{
	int i = type.find('(');
	this->type = type.substr(0, i);
	args = Utility::String::Split(type.substr(i + 1), ',');
}

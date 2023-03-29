#include "ShaderProgram.h"

#include "ShaderPreprocessor.h"
#include "Application.h"
#include "Logger.h"

#include <iostream>
#include <string>
#include <sstream>

ShaderProgram* ShaderProgram::Current = nullptr;
ShaderProgram fallback;

#define ERROR_DEF_START \
const int logSize = 512; \
GLchar errorLog[logSize]; \
GLint success = 0;

#define ERROR_DEF_END \
if (success == GL_FALSE) \
{ \
	LogGLError(std::string() + "Error Linking Shaders \"" + vertFileName + "\" and \"" + fragFileName + "\"\n", shaderProgram, errorLog, logSize); \
}

void ShaderProgram::LogGLError(const std::string& message, const GLuint& glObject, GLchar* errorLog, const int logSize)
{
	std::stringstream s;
	s << message << '\n';
	glGetShaderInfoLog(glObject, logSize, nullptr, errorLog);
	s << errorLog;
	s << '\n';
	Logger::LogError(s.str());

	bad = true;
}

void ShaderProgram::Cleanup()
{
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteProgram(shaderProgram);
}

void ShaderProgram::SetupUniforms()
{
	GLuniform::BindInitalizationContext(*this);

	GLuniform::UnbindInitalizationContext();
}

void ShaderProgram::LoadShader(GLuint& shader, const std::string& fileName, GLenum shaderType, GLchar* errorLog, int logSize, GLint& success)
{
	shader = glCreateShader(shaderType);
	std::string file = ShaderPreprocessor::GenerateShaderFromPath(fileName);
	const char* fileC = file.c_str();
	glShaderSource(shader, 1, &fileC, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		LogGLError(std::string() + "Shader \"" + fileName + "\" Failed with error: ", shader, errorLog, logSize);
	}
	else
	{
		Logger::Log("Shader \"" + fileName + "\" Compiled Successfully\n", FOREGROUND_GREEN);
	}
}

void ShaderProgram::Initalize(const std::string& vertFileName, const std::string& fragFileName)
{
	if (fallback.bad && this != &fallback)
	{
		fallback.Initalize("./Assets/Shaders/fallback.vert", "./Assets/Shaders/fallback.frag");
	}

	bad = false;

	this->vertFileName = vertFileName;
	this->fragFileName = fragFileName;

	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();

	ERROR_DEF_START
	LoadShader(vertShader, vertFileName, GL_VERTEX_SHADER, errorLog, logSize, success);
	LoadShader(fragShader, fragFileName, GL_FRAGMENT_SHADER, errorLog, logSize, success);

	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	ERROR_DEF_END

	if (bad)
	{
		Cleanup();
	}
	else
	{
		Logger::Log("Shader Compiled and Linked successfully.\n", FOREGROUND_GREEN);
	}

	SetupUniforms();
}

void ShaderProgram::HotReload()
{
	Logger::Log("Attempting to perform Hot Reload of Shader Program...\n", FOREGROUND_RED | FOREGROUND_GREEN);

	Cleanup();
	Initalize(vertFileName, fragFileName);

	Logger::Log("Successfully Reloaded Shader Program\n\n", FOREGROUND_GREEN);
}

ShaderProgram::~ShaderProgram()
{
	Cleanup();
}

void ShaderProgram::Bind()
{
	if (bad && this != &fallback)
	{
		fallback.Bind();
		return;
	}

	Current = this;
	glUseProgram(shaderProgram);

	const Camera& camera = *Camera::Current;

	viewMat.Set(Camera::Current->view);
	projMat.Set(Camera::Current->projection);

	Mat4 vp = camera.projection * camera.view;
	viewProjMat.Set(vp);

	time.Set(Application::Current->Time());
	camPos.Set(Camera::Current->position);
}

void ShaderProgram::Unbind()
{
	glUseProgram(0);

	Current = nullptr;
}

void ShaderProgram::SetModelMatrix(const Mat4& model)
{
	const Camera& camera = *Camera::Current;

	modelMat.Set(model);
	Mat4 mvp = Camera::Current->projection * Camera::Current->view * model;
	modelViewProjMat.Set(mvp);
}
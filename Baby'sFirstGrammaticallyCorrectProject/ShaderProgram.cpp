#include "ShaderProgram.h"

#include "BMUtil.h"
#include "Application.h"
#include <iostream>

ShaderProgram* ShaderProgram::Current = nullptr;

void ShaderProgram::LogGLError(const std::string message, const GLuint& glObject, GLchar* errorLog, const int logSize)
{
	std::cout << message << std::endl;
	glGetShaderInfoLog(glObject, logSize, nullptr, errorLog);
	std::cout << errorLog << std::endl;
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

	modelMat.Initalize("_Model");
	viewMat.Initalize("_View");
	projMat.Initalize("_Projection");
	viewProjMat.Initalize("_VP");
	modelViewProjMat.Initalize("_MVP");
	time.Initalize("_Time");

	GLuniform::UnbindInitalizationContext();
}

void ShaderProgram::LoadShader(GLuint& shader, std::string fileName, GLenum shaderType, GLchar* errorLog, int logSize, GLint& success)
{
	shader = glCreateShader(shaderType);
	std::string file = BMUtil::LoadTextFromFile(fileName);
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
		std::cout << "Shader \"" << fileName << "\" Compiled Successfully" << std::endl;
	}
}

void ShaderProgram::Initalize(std::string vertFileName, std::string fragFileName)
{
	bad = false;

	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();

	const int logSize = 512;
	GLchar errorLog[logSize];
	GLint success = 0;
	LoadShader(vertShader, vertFileName, GL_VERTEX_SHADER, errorLog, logSize, success);
	LoadShader(fragShader, fragFileName, GL_FRAGMENT_SHADER, errorLog, logSize, success);

	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		LogGLError(std::string() + "Error Linking Shaders \"" + vertFileName + "\" and \"" + fragFileName + "\"\n", shaderProgram, errorLog, logSize);
	}

	if (bad)
	{
		Cleanup();
	}
	else
	{
		std::cout << "Shader Compiled and Linked successfully.\n";
	}

	SetupUniforms();
}

ShaderProgram::~ShaderProgram()
{
	Cleanup();
}

void ShaderProgram::Bind()
{
	glUseProgram(shaderProgram);

	const Camera& camera = *Camera::Current;

	viewMat.SetMat4(Camera::Current->view);
	projMat.SetMat4(Camera::Current->projection);

	Mat4 vp = camera.projection * camera.view;
	viewProjMat.SetMat4(vp);

	time.SetFloat(Application::Current->Time());

	Current = this;
}

void ShaderProgram::Unbind()
{
	glUseProgram(0);

	Current = nullptr;
}

void ShaderProgram::SetModelMatrix(const Mat4& model)
{
	const Camera& camera = *Camera::Current;

	modelMat.SetMat4(model);
	Mat4 mvp = Camera::Current->projection * Camera::Current->view * model;
	modelViewProjMat.SetMat4(mvp);
}
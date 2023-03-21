#include "GLBabysitter.h"

#include <string>
#include <sstream>
#include <iostream>

void APIENTRY GLCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, const void*)
{
	std::stringstream output;
	output << "GL Log:\nSource: ";

	switch (source)
	{
	default:
		output << "GL_DEBUG_SOURCE_OTHER";
		break;
	case GL_DEBUG_SOURCE_API:
		output << "GL_DEBUG_SOURCE_API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		output << "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		output << "GL_DEBUG_SOURCE_SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		output << "GL_DEBUG_SOURCE_THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		output << "GL_DEBUG_SOURCE_APPLICATION";
		break;
	}

	output << "\nType: ";

	auto oStream = &std::cout;

	switch (type)
	{
	default:
		output << "GL_DEBUG_TYPE_OTHER";
		break;
	case GL_DEBUG_TYPE_ERROR:
		output << "GL_DEBUG_TYPE_ERROR";
		oStream = &std::cerr;
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		output << "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
		oStream = &std::cerr;
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		output << "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
		oStream = &std::cerr;
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		output << "GL_DEBUG_TYPE_PORTABILITY";
		oStream = &std::cerr;
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		output << "GL_DEBUG_TYPE_PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_MARKER:
		output << "GL_DEBUG_TYPE_MARKER";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		output << "GL_DEBUG_TYPE_PUSH_GROUP";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		output << "GL_DEBUG_TYPE_POP_GROUP";
		break;
	}

	output << "\nID: " << std::to_string(id) << "\nSeverity: ";

	switch (severity)
	{
	default:
		output << "GL_DEBUG_SEVERITY_NOTIFICATION";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		output << "GL_DEBUG_SEVERITY_HIGH";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		output << "GL_DEBUG_SEVERITY_MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		output << "GL_DEBUG_SEVERITY_LOW";
		break;
	}

	output << "\nMessage:\n" << message;
	(*oStream) << output.str() << "\n\n";
}

void GLBabysitter::Initalize()
{
#ifndef _DEBUG
	return;
#endif

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	glEnable(GL_DEBUG_OUTPUT);

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageCallback(GLCallback, nullptr);

	const char* testMessage = "GLBabysitter Initalized Correctly!";
	glDebugMessageInsert(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, strlen(testMessage), testMessage);
}
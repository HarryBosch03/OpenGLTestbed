#pragma once

#include "Graphics.h"

void APIENTRY GLCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message);

class GLBabysitter
{
public:
	void Initalize();
};
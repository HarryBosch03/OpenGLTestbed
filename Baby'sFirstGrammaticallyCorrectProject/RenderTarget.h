#pragma once

#include "Graphics.h"

class RenderTarget
{
	GLint internalFormat = GL_RGBA16F;
	GLint format = GL_RGBA;
	GLint type = GL_FLOAT;

	GLuint bufferHandle;
	GLuint colTexHandle;
	GLuint depthTexHandle;

public:
	RenderTarget() = default;
	RenderTarget(const RenderTarget& other) = delete;
	RenderTarget& operator=(const RenderTarget& other) = delete;
	~RenderTarget();

	void Initalize();

	void Bind(GLsizei width, GLsizei height);
	void Unbind();
};
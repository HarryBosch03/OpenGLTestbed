#include "RenderTarget.h"

RenderTarget::~RenderTarget()
{
//	glDeleteFramebuffers(1, &bufferHandle);
//	glDeleteTextures(1, &texHandle);
}

void RenderTarget::Initalize()
{
//	glGenFramebuffers(1, &bufferHandle);
//	glGenTextures(1, &texHandle);
}

void RenderTarget::Bind(GLsizei width, GLsizei height)
{
//	glBindFramebuffer(GL_FRAMEBUFFER, bufferHandle);
//	glBindTexture(GL_TEXTURE_2D, texHandle);
//	
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texHandle, 0);
//
//	glTexImage2D(
//		GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
//		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
//	);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texHandle, 0);
//	
//	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderTarget::Unbind()
{
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
#include "Framebuffer.h"

#include "Application.h"
#include "LogMaster.h"
#include "Texture.h"

Framebuffer* bound = nullptr;

void Framebuffer::Init()
{
	glGenFramebuffers(1, &fbHandle);
	glGenRenderbuffers(1, &rbHandle);

	glBindFramebuffer(GL_FRAMEBUFFER, fbHandle);
	glBindRenderbuffer(GL_RENDERBUFFER, rbHandle);

	texture = new Texture();

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbHandle);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Cleanup()
{
	glDeleteFramebuffers(1, &fbHandle);
}

void Framebuffer::UpdateTexture()
{
	int width, height;
	glfwGetWindowSize(Application::Current()->Window(), &width, &height);

	if (width == texWidth && height == texHeight) return;

	glBindTexture(GL_TEXTURE_2D, texture->Handle());
	glBindRenderbuffer(GL_RENDERBUFFER, rbHandle);

	// Update Render Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	texWidth = width;
	texHeight = height;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Update Render Buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Framebuffer::~Framebuffer()
{
	Cleanup();
}

void Framebuffer::Bind()
{
	if (this == nullptr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		bound = nullptr;
		return;
	}

	if (!fbHandle) Init();

	glBindFramebuffer(GL_FRAMEBUFFER, fbHandle);
	UpdateTexture();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LogError("Cannot bind incomplete Framebuffer");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}

	bound = this;
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	bound = nullptr;
}

Framebuffer* Framebuffer::Bound()
{
	return bound;
}

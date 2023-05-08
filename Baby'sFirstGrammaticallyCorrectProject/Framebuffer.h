#pragma once

#include "Graphics.h"

class Texture;

class Framebuffer
{
	GLuint fbHandle = 0;
	GLuint rbHandle = 0;
	Texture* texture;

	int texWidth = 0, texHeight = 0;

	void Init();
	void Cleanup();

	void UpdateTexture();

public:

	inline const GLuint FramebufferHandle() const { return fbHandle; }
	inline const GLuint RenderbufferHandle() const { return rbHandle; }
	inline Texture& RenderTexture() const { return *texture; }

	Framebuffer() = default;
	Framebuffer(const Framebuffer& other) = delete;
	Framebuffer& operator=(const Framebuffer& other) = delete;
	~Framebuffer();

	void Bind();
	void Unbind();

	static Framebuffer* Bound();
};
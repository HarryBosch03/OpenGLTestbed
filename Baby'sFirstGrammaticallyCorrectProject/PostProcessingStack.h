#pragma once

#include <vector>
#include "Framebuffer.h"
#include "Material.h"

class PostProcessingStack
{
	Framebuffer bufferA, bufferB;
	bool swapBuffers;

public:
	std::vector<Material*> stack;

	void Init();

	void Bind();
	void Draw();
	void Unbind();

	inline Framebuffer& Source() { return swapBuffers ? bufferA : bufferB; }
	inline Framebuffer& Dest() { return swapBuffers ? bufferB : bufferA; }
	void SwapBuffers();

	static void Blit(Framebuffer* src, Framebuffer* dst, Material* material = nullptr);
};
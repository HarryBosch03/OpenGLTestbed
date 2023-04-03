#pragma once

#include "Material.h"
#include "Graphics.h"

class Skybox
{
	GLuint handle = 0;

	const GLuint& Handle();

public:
	Skybox() = default;
	Skybox(const Skybox& other) = delete;
	Skybox& operator=(const Skybox& other) = delete;
	~Skybox();

	Material material;

	void Bind();
	void Draw();
	void Unbind();
};
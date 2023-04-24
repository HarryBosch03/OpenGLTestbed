#pragma once

#include "Material.h"
#include "Graphics.h"
#include "Texture.h"

const TextureImportSettings SkyboxImportSettings
{
	TextureFiltering::Linear,
	false,
};

class Skybox
{
	GLuint handle = 0;
	Material material;

	const GLuint& Handle();

public:
	Skybox() = default;
	Skybox(const Skybox& other) = delete;
	Skybox& operator=(const Skybox& other) = delete;
	~Skybox();

	void Setup(const std::string& shaderName = "Shaders/sky.yaml");

	void Draw();
};
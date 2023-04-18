#pragma once

#include <map>
#include <string>

#include "Maths.h"

class ShaderProgram;
class Texture;

struct MaterialProperties
{
	Vec4
		color = One;
	Vec2
		metalness = { 0.0f, 1.0f},
		roughness = { 0.0f, 1.0f};
	float
		normal = 1.0f,
		height = 1.0f,
		ao = 0.0f;
};

class Material
{
	ShaderProgram* program = nullptr;

public:
	std::map<std::string, Texture*> textures;
	MaterialProperties properties;

	Material() = default;
	Material(const std::string& shader);
	Material(ShaderProgram* program);

	Material& SetShader(const std::string& shader);
	Material& SetShader(ShaderProgram* program);
	Material& SetTexture(const std::string& ref, Texture* tex);

	void Bind() const;
	void Unbind() const;

	inline ShaderProgram* Shader() { return program; }
};
#pragma once

#include <map>
#include <string>

#include "Maths.h"

class ShaderProgram;
class Texture;

class Material
{
	ShaderProgram* program = nullptr;
	std::map<std::string, Texture*> textures;

public:
	Material() = default;
	Material(const std::string& shader);
	Material(ShaderProgram* program);

	Material& SetShader(const std::string& shader);
	Material& SetShader(ShaderProgram* program);
	Material& SetTexture(const std::string& ref, Texture* tex);
	Texture*& Textures(const std::string& ref);
	const Texture* Textures(const std::string& ref) const;

	void Bind() const;
	void Unbind() const;

	inline const ShaderProgram* Shader() const { return program; }
	inline ShaderProgram* Shader() { return program; }
};
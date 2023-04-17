#pragma once

#include <map>
#include <string>

class ShaderProgram;
class Texture;

class Material
{
	ShaderProgram* program = nullptr;

public:
	std::map<std::string, Texture*> textures;

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
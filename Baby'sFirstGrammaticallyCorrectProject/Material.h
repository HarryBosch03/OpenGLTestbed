#pragma once

#include <map>
#include <string>

class ShaderProgram;
class Texture;

class Material
{
	ShaderProgram* program = nullptr;
	std::map<std::string, Texture*> textures;
	int textureIndex;

public:
	Material() = default;
	Material(const std::string& shader);
	Material(ShaderProgram* program);

	Material& SetShader(const std::string& shader);
	Material& SetShader(ShaderProgram* program);
	Material& SetTexture(const std::string& ref, Texture* tex);

	void Bind();
	void Unbind();

	inline ShaderProgram* Shader() { return program; }
};
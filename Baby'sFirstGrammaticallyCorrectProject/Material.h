#pragma once

#include <map>
#include <string>

class ShaderProgram;
class Texture;

class Material
{
	ShaderProgram* program = nullptr;
	std::map<int, Texture*> textures;

public:
	Material() = default;
	Material(const std::string& shader);
	Material(ShaderProgram* program);

	Material& SetShader(const std::string& shader);
	Material& SetShader(ShaderProgram* program);
	Material& SetTexture(int i, Texture* tex);
	inline Material& SetMainTexture(Texture* tex) { return SetTexture(0, tex); }

	void Bind();
	void Unbind();

	inline ShaderProgram* Shader() { return program; }
};
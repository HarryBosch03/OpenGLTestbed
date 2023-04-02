#include "Material.h"

#include "ShaderProgram.h"
#include "Texture.h"

Material& Material::SetShader(ShaderProgram* program)
{
	this->program = program;
	return *this;
}

Material::Material(const std::string& shader)
{
	SetShader(shader);
}

Material::Material(ShaderProgram* program)
{
	SetShader(program);
}

Material& Material::SetShader(const std::string& shader)
{
	this->program = ShaderProgram::Find(shader);
	return *this;
}

Material& Material::SetTexture(int i, Texture* tex)
{
	textures[i] = tex;
	return *this;
}

void Material::Bind()
{
	program->Bind();

	for (auto& pair : textures)
	{
		pair.second->Bind(pair.first);
	}
}

void Material::Unbind()
{
	for (auto& pair : textures)
	{
		pair.second->Unbind();
	}

	program->Unbind();
}

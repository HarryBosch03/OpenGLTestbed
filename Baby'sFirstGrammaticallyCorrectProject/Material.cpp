#include "Material.h"

#include "ShaderProgram.h"
#include "AssetDatabase.h"
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
	this->program = AssetDatabase::LoadAsset<ShaderProgram>(ShaderPath + shader);
	return *this;
}

Material& Material::SetTexture(const std::string& ref, Texture* tex)
{
	textures[ref] = tex;
	return *this;
}

void Material::Bind() const
{
	if (this == nullptr)
	{
		ShaderProgram::Fallback()->Bind();
		return;
	}

	program->Bind();

	for (auto& pair : textures)
	{
		pair.second->Bind(pair.first);
	}

	Texture::BindAll();
}

void Material::Unbind() const
{
	if (this == nullptr)
	{
		ShaderProgram::Fallback()->Unbind();
		return;
	}

	Texture::UnbindAll();

	for (auto& pair : textures)
	{
		pair.second->Unbind();
	}

	program->Unbind();
}
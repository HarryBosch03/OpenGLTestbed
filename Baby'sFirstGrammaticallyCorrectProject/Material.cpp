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

Material& Material::SetTexture(const std::string& ref, Texture* tex)
{
	textures[ref] = tex;
	return *this;
}

void Material::Bind()
{
	program->Bind();

	for (auto& pair : textures)
	{
		glUniform1i(glGetUniformLocation(program->programHandle, pair.first.c_str()), textureIndex);
		pair.second->Bind(textureIndex);
		textureIndex++;
	}
}

void Material::Unbind()
{
	for (auto& pair : textures)
	{
		pair.second->Unbind();
	}

	program->Unbind();
	textureIndex = 0;
}

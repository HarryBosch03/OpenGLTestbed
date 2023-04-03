#include "Skybox.h"

#include "Maths.h"
#include "Texture.h"

const Vec3 A = {-1.0f,  1.0f, 0.9999f };
const Vec3 B = { 1.0f,  1.0f, 0.9999f };
const Vec3 C = { 1.0f, -1.0f, 0.9999f };
const Vec3 D = {-1.0f, -1.0f, 0.9999f };

const Vec3 verticies[] = { A, B, C, C, D, A };

Skybox::~Skybox()
{
	glDeleteBuffers(1, &Handle());
}

const GLuint& Skybox::Handle()
{
	if (!handle) glGenBuffers(1, &handle);
	return handle;
}

void Skybox::Bind()
{
	for (std::pair<const std::string, Texture*>& texture : material.textures)
	{
		texture.second->Bind(texture.first);
	}
}

void Skybox::Draw()
{
	// Bind Local
	material.Bind();

	glBindBuffer(GL_ARRAY_BUFFER, Handle());
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vec3), verticies, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), nullptr);
	glEnableVertexAttribArray(0);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Unbind Local
	material.Unbind();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Skybox::Unbind()
{
	for (std::pair<const std::string, Texture*>& texture : material.textures)
	{
		texture.second->Unbind();
	}
}
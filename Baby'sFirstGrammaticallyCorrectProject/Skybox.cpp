#include "Skybox.h"

#include "Maths.h"
#include "LightingEnviroment.h"
#include "AssetDatabase.h"

const Vec3 A = {-1.0f,  1.0f, 0.999999f };
const Vec3 B = { 1.0f,  1.0f, 0.999999f };
const Vec3 C = { 1.0f, -1.0f, 0.999999f };
const Vec3 D = {-1.0f, -1.0f, 0.999999f };

const Vec3 vertices[] = { C, B, A, A, D, C };

Skybox::~Skybox()
{
	glDeleteBuffers(1, &Handle());
}

void Skybox::Setup(const std::string& shaderName)
{
	material.SetShader(shaderName);
}

const GLuint& Skybox::Handle()
{
	if (!handle) glGenBuffers(1, &handle);
	return handle;
}

void Skybox::Draw()
{
	// Bind Local
	material.Bind();
	glBindBuffer(GL_ARRAY_BUFFER, Handle());
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vec3), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), nullptr);
	glEnableVertexAttribArray(0);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Unbind Local
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	material.Unbind();
}
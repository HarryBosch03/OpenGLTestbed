#include "MeshRenderData.h"

#include "AssetDatabase.h"
#include "ShaderProgram.h"

#include <stdexcept>

MeshRenderData::MeshRenderData(const MeshData& meshData)
{
	Load(meshData);
}

void SetVertexAttribute(std::string ref, int size, size_t start, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE)
{
	GLint handle = glGetAttribLocation(ShaderProgram::Current->programHandle, ref.c_str());
	if (handle == -1) return;
	glEnableVertexAttribArray(handle);
	glVertexAttribPointer(handle, size, type, normalized, sizeof(VertexData), (void*)start);
}

MeshRenderData& MeshRenderData::Load(const MeshData& meshData)
{
	this->data = meshData;

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	return *this;
}

MeshRenderData& MeshRenderData::Load(MeshData* meshData)
{
	Load(*meshData);
	return* this;
}

void MeshRenderData::Bind()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(VertexData), data.vertices.data(), GL_STATIC_DRAW);

	SetVertexAttribute("position", 4, offsetof(VertexData, position));
	SetVertexAttribute("normal", 4, offsetof(VertexData, normal));
	SetVertexAttribute("texcoord0", 2, offsetof(VertexData, textureCoordinates));
	SetVertexAttribute("tangent", 4, offsetof(VertexData, tangent));
}

void MeshRenderData::Draw() const
{
	glDrawElements(GL_TRIANGLES, (GLsizei)data.indices.size(), data.indexType, data.indices.data());
}

void MeshRenderData::Unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

MeshRenderData::~MeshRenderData()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}
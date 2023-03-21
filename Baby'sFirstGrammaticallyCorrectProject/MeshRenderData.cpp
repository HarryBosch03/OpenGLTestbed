#include "MeshRenderData.h"

#include <stdexcept>

MeshRenderData::MeshRenderData(const MeshData& meshData)
{
	Load(meshData);
}

MeshRenderData::MeshRenderData(std::string path) 
{
	Load(path);
}

void MeshRenderData::Load(const MeshData& meshData)
{
	this->data = meshData;

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(VertexData), data.vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(offsetof(VertexData, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(offsetof(VertexData, normal)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshRenderData::Load(std::string path)
{
	Load(MeshData::LoadMeshFromFile(path, 0));
}

void MeshRenderData::Bind()
{
	glBindVertexArray(vao);
}

void MeshRenderData::Draw() const
{
	glDrawElements(GL_TRIANGLES, (GLsizei)data.indices.size(), data.indexType, data.indices.data());
}

void MeshRenderData::Unbind()
{
	glBindVertexArray(0);
}

MeshRenderData::~MeshRenderData()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}
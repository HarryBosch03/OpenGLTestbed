#include "Submesh.h"

#include "ShaderProgram.h"

void Submesh::UploadMeshData() const
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Submesh::Submesh(int index)
{
	this->index = index;

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
}

Submesh::Submesh(int index, const std::vector<VertexData>& vertices, const std::vector<MeshIndex>& indices) : Submesh(index)
{
	Vertices() = vertices;
	Indices() = indices;
}

Submesh::~Submesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void SetVertexAttribute(std::string ref, int size, size_t start, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE)
{
	GLint handle = glGetAttribLocation(ShaderProgram::Current->programHandle, ref.c_str());
	if (handle == -1) return;
	glEnableVertexAttribArray(handle);
	glVertexAttribPointer(handle, size, type, normalized, sizeof(VertexData), (void*)start);
}

void Submesh::Draw()
{
	if (dirty)
	{
		UploadMeshData();
		dirty = false;
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	SetVertexAttribute("position", 4, offsetof(VertexData, position));
	SetVertexAttribute("normal", 4, offsetof(VertexData, normal));
	SetVertexAttribute("texcoord0", 2, offsetof(VertexData, textureCoordinates));
	SetVertexAttribute("tangent", 4, offsetof(VertexData, tangent));

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), IndexType, indices.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

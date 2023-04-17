#pragma once

#include "VertexData.h"

#include <vector>

class Submesh
{
	int index;
	std::vector<VertexData> vertices;
	std::vector<MeshIndex> indices;

	GLuint vao = 0, vbo = 0;
	bool dirty = false;

	void UploadMeshData() const;

public:
	const inline std::vector<VertexData>& Vertices() const { return vertices; };
	inline std::vector<VertexData>& Vertices() { dirty = true; return vertices; };

	const inline std::vector<MeshIndex>& Indices() const { return indices; };
	inline std::vector<MeshIndex>& Indices() { dirty = true; return indices; };

	Submesh(int index);
	Submesh(int index, const std::vector<VertexData>& vertices, const std::vector<MeshIndex>& indices);
	Submesh(const Submesh& other) = delete;
	Submesh& operator=(const Submesh& other) = delete;
	~Submesh();

	void Draw();
};
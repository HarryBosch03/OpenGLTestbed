#pragma once

#include "Graphics.h"

#include "Maths.h"
#include "MeshData.h"

#include <vector>

class MeshRenderData
{
	MeshData data;
	GLuint vao, vbo, ibo;

public:
	MeshRenderData() = default;
	MeshRenderData(const MeshData& meshData);
	MeshRenderData(std::string path);
	MeshRenderData(const MeshRenderData& other) = delete;
	MeshRenderData& operator=(const MeshRenderData& other) = delete;
	~MeshRenderData();

	void Load(const MeshData& meshData);
	void Load(std::string path);

	void Bind();
	void Draw() const;
	void Unbind();
};
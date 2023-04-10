#pragma once

#include "Graphics.h"

#include "Maths.h"
#include "MeshData.h"
#include "Material.h"

#include <vector>

class MeshRenderData
{
	MeshData data;
	GLuint vao = {}, vbo = {}, ibo = {};

public:
	MeshRenderData() = default;
	MeshRenderData(const MeshData& meshData);
	MeshRenderData(const MeshRenderData& other) = delete;
	MeshRenderData& operator=(const MeshRenderData& other) = delete;
	~MeshRenderData();

	MeshRenderData& Load(const MeshData& meshData);
	MeshRenderData& Load(MeshData* meshData);

	void Bind();
	void Draw() const;
	void Unbind();
};
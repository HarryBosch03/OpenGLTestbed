#pragma once

#include "Graphics.h"
#include "Maths.h"
#include "Asset.h"

#include <vector>
#include <string>

typedef unsigned int MeshIndex;

struct VertexData;

typedef std::vector<VertexData> VertexList;
typedef std::vector<MeshIndex> IndexList;

typedef aiScene MeshScene;

struct VertexData
{
	Vec4 position;
	Vec4 normal;
	Vec2 textureCoordinates;
	Vec4 tangent;
};

class MeshData: public Asset
{
	std::string fileLoc;
	int subMeshIndex = 0;

public:
	VertexList vertices;
	IndexList indices;
	GLenum indexType = GL_UNSIGNED_INT;

	MeshData& MergeByDistance(float threshold = 0.01f);
	MeshData& SplitNormals();
	MeshData& Subdivide();
	MeshData& Subdivide(int itterations);
	MeshData& CalculateNormals(bool flip = false);

	inline AssetType GetType() const override { return AssetType::Texture; }

	Asset& LoadFromFile(const std::string& path, void* args) override;
	Asset& Reload() override;
};
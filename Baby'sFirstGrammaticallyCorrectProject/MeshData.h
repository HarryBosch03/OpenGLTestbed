#pragma once

#include "Graphics.h"
#include "Maths.h"
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

	VertexData(Vec4 position = {0, 0, 0, 1}, Vec4 normal = Zero, Vec2 textureCoordiates = Zero);

	VertexData& SetPosition(Vec3 position);
	VertexData& SetPosition(aiVector3D position);
	VertexData& SetPosition(Vec4 position);

	VertexData& SetNormal(Vec3 normal);
	VertexData& SetNormal(aiVector3D normal);
	VertexData& SetNormal(Vec4 normal);
	
	VertexData& SetTextureCoordinates(Vec2 textureCoordinates);
	VertexData& SetTextureCoordinates(aiVector3D textureCoordinates);
};

struct MeshData
{
	VertexList vertices;
	IndexList indices;
	GLenum indexType = GL_UNSIGNED_INT;

	MeshData& MergeByDistance(float threshold = 0.01f);
	MeshData& SplitNormals();
	MeshData& Subdivide();
	MeshData& Subdivide(int itterations);
	MeshData& RecalculateNormals(bool flip = false);

	MeshData& LoadFromFile(const std::string& path, int subMeshIndex);
};
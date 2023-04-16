#pragma once

#include "Graphics.h"
#include "Math.h"

#include "assimp/mesh.h"

#include <vector>

typedef unsigned short MeshIndex;

struct VertexData
{
	Vec4 vertex, normal, tangent, texcoords[AI_MAX_NUMBER_OF_TEXTURECOORDS];
};

struct Submesh
{
	GLuint vao = 0, vbo = 0, vio = 0;

	std::vector<MeshIndex> indicies;
	std::vector<VertexData> verticies;

	void Upload();
};
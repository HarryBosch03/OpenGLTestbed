#pragma once
#include "Maths.h"
#include "Graphics.h"

typedef unsigned int MeshIndex;
const GLenum IndexType = GL_UNSIGNED_INT;

struct VertexData
{
	Vec4 position;
	Vec4 normal;
	Vec2 textureCoordinates;
	Vec4 tangent;
};
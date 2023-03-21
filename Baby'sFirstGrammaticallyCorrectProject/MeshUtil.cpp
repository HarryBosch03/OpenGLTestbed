#include "MeshUtil.h"

#include "graphics.h"
#include <iostream>

using glm::cos;
using glm::sin;

Mat4 MeshUtil::TRMat(const Vec3& offset)
{
	return TRMat(offset, 0.0f, Up);
}

Mat4 MeshUtil::TRMat(const float& angleDeg, const Vec3& axis)
{
	return TRMat(Zero, angleDeg, axis);
}

Mat4 MeshUtil::TRMat(const Vec3& offset, const float& angleDeg, const Vec3& axis)
{
	Mat4 mat = MatIdentity;
	mat = glm::translate(mat, offset);
	mat = glm::rotate(mat, glm::radians(angleDeg), axis);
	return mat;
}

MeshData MeshUtil::Quad(const Vec2& size, const Mat4& mat)
{
	MeshData verts;
	Quad(verts, size, mat);
	return verts;

}

void MeshUtil::Quad(MeshData& meshData, const Vec2& size, const Mat4& mat)
{
	Vec3 xSize = Right * size.x * 0.5f;
	Vec3 ySize = Up * size.y * 0.5f;

	Vec3 a = mat * Point(xSize + ySize);
	Vec3 b = mat * Point(-xSize + ySize);
	Vec3 c = mat * Point(-xSize - ySize);
	Vec3 d = mat * Point(xSize - ySize);

	int i = meshData.vertices.size();
	meshData.vertices.push_back(Point(a));
	meshData.vertices.push_back(Point(b));
	meshData.vertices.push_back(Point(c));
	meshData.vertices.push_back(Point(d));

	meshData.indices.push_back(i);
	meshData.indices.push_back(i + 1);
	meshData.indices.push_back(i + 2);

	meshData.indices.push_back(i + 2);
	meshData.indices.push_back(i + 3);
	meshData.indices.push_back(i);
}

MeshData MeshUtil::Box(const Vec3& size, const Mat4& mat)
{
	MeshData verts;
	Box(verts, size, mat);
	return verts;
}

void MeshUtil::Box(MeshData& meshData, const Vec3& size, const Mat4& mat)
{
	Vec3 xSize = Right * size.x * 0.5f;
	Vec3 ySize = Up * size.y * 0.5f;
	Vec3 zSize = Forward * size.z * 0.5f;

	Quad(meshData, { size.z, size.y }, mat * TRMat(xSize, 90.0f, Up));
	Quad(meshData, { size.z, size.y }, mat * TRMat(-xSize, -90.0f, Up));

	Quad(meshData, { size.x, size.z }, mat * TRMat(ySize, -90.0f, Right));
	Quad(meshData, { size.x, size.z }, mat * TRMat(-ySize, 90.0f, Right));

	Quad(meshData, { size.x, size.y }, mat * TRMat(zSize));
	Quad(meshData, { size.x, size.y }, mat * TRMat(-zSize, 180.0f, Up));
}

MeshData MeshUtil::Plane(const Vec2& size, int subdivisions, const Mat4& mat)
{
	MeshData verts;
	Plane(verts, size, subdivisions, mat);
	return verts;
}

void MeshUtil::Plane(MeshData& meshData, const Vec2& size, int subdivisions, const Mat4& mat)
{
	for (int x = 0; x < subdivisions; x++)
	{
		for (int y = 0; y < subdivisions; y++)
		{
			float px = (x - (subdivisions / 2.0f) + 0.5f) / subdivisions;
			float py = (y - (subdivisions / 2.0f) + 0.5f) / subdivisions;

			float sx = size.x / subdivisions;
			float sy = size.y / subdivisions;

			Quad(meshData, { sx, sy }, mat * glm::translate(MatIdentity, Vec3(px * size.x, py * size.y, 0.0f)));
		}
	}
}


MeshData MeshUtil::RoundCube(const float radius, int subdivisions, const Mat4& mat)
{
	MeshData verts;
	RoundCube(verts, radius, subdivisions, mat);
	return verts;
}

void MeshUtil::RoundCube(MeshData& meshData, const float radius, int subdivisions, const Mat4& mat)
{
	Plane(meshData, One * 2.0f, subdivisions, mat * TRMat(Right, 90.0f, Up));
	Plane(meshData, One * 2.0f, subdivisions, mat * TRMat(-Right, -90.0f, Up));

	Plane(meshData, One * 2.0f, subdivisions, mat * TRMat(Up, -90.0f, Right));
	Plane(meshData, One * 2.0f, subdivisions, mat * TRMat(-Up, 90.0f, Right));

	Plane(meshData, One * 2.0f, subdivisions, mat * TRMat(Forward));
	Plane(meshData, One * 2.0f, subdivisions, mat * TRMat(-Forward, 180.0f, Up));

	for (VertexData& vert : meshData.vertices)
	{
		vert.position = Point(glm::normalize((Vec3)vert.position) * radius * 0.5f);
	}

	meshData.MergeByDistance();
}

MeshData MeshUtil::Sphere(const float radius, int rows, int columns, const Mat4& mat)
{
	MeshData verts;
	Sphere(verts, radius, rows, columns, mat);
	return verts;
}

void MeshUtil::Sphere(MeshData& meshData, const float radius, int rows, int columns, const Mat4& mat)
{
	meshData.vertices.push_back(Up * radius * 0.5f);

	for (int i = 1; i < rows - 1; i++)
	{
		float a1 = i / (float)rows * pi;

		for (int j = 0; j < columns; j++)
		{
			float a2 = j / (float)columns * tau;
			Vec3 p = {cos(a2) * sin(a1), cos(a1), sin(a2) * sin(a1)};
			meshData.vertices.push_back(Point(p * radius * 0.5f));
		}
	}

	meshData.vertices.push_back(Up * -radius * 0.5f);

	for (int i = 0; i < columns; i++)
	{
		meshData.indices.push_back((i + 1) % columns + 1);
		meshData.indices.push_back(i % columns + 1);
		meshData.indices.push_back(0);
	}
	
	for (int i = 0; i < rows - 3; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			int o = i * columns + 1;

			int a = o + j;
			int b = o + (j + 1) % columns;
			int c = b + columns;
			int d = a + columns;

			meshData.indices.push_back(a);
			meshData.indices.push_back(b);
			meshData.indices.push_back(c);

			meshData.indices.push_back(c);
			meshData.indices.push_back(d);
			meshData.indices.push_back(a);
		}
	}

	int end = meshData.vertices.size() - 1;
	for (int i = 0; i < columns; i++)
	{
		int i1 = 0;
		int i2 = i % columns + 1;
		int i3 = (i + 1) % columns + 1;

		meshData.indices.push_back(end - i3);
		meshData.indices.push_back(end - i2);
		meshData.indices.push_back(end - i1);
	}
}

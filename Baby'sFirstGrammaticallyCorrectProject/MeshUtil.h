#pragma once

#include "Maths.h"
#include "MeshData.h"

namespace MeshUtil
{
	Mat4 TRMat(const Vec3& offset);
	Mat4 TRMat(const float& angleDeg, const Vec3& axis);
	Mat4 TRMat(const Vec3& offset, const float& angleDeg, const Vec3& axis);

	MeshData Quad(const Vec2& size, const Mat4& mat = MatIdentity);
	void Quad(MeshData& data, const Vec2& size, const Mat4& mat = MatIdentity);

	MeshData Box(const Vec3& size, const Mat4& mat = MatIdentity);
	void Box(MeshData& data, const Vec3& size, const Mat4& mat = MatIdentity);

	MeshData Plane(const Vec2& size, int subdivisions, const Mat4& mat = MatIdentity);
	void Plane(MeshData& data, const Vec2& size, int subdivisions, const Mat4& mat = MatIdentity);

	MeshData RoundCube(const float radius, int subdivisions, const Mat4& mat = MatIdentity);
	void RoundCube(MeshData& data, const float radius, int subdivisions, const Mat4& mat = MatIdentity);

	MeshData Sphere(const float radius, int rows, int columns, const Mat4& mat = MatIdentity);
	void Sphere(MeshData& data, const float radius, int rows, int columns, const Mat4& mat = MatIdentity);
}
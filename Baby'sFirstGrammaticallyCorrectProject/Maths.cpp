#include "Maths.h"

#include "Graphics.h"

Mat4 TRS(Vec3 position, Vec4 angleAxisRotation, float scale)
{
	return TRS(position, angleAxisRotation, {scale, scale, scale});
}
Mat4 TRS(Vec3 position, Vec4 angleAxisRotation, Vec3 scale)
{
	Mat4 mat = MatIdentity;
	mat = glm::translate(mat, position);
	mat = glm::scale(mat, scale);
	mat = glm::rotate(mat, glm::radians(angleAxisRotation.w), Vec3(angleAxisRotation.x, angleAxisRotation.y, angleAxisRotation.z));
	return mat;
}
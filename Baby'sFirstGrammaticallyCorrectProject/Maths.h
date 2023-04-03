#pragma once

#define GLM_FORCE_ONLY_XYZW

#include "math.h"
#include "glm.hpp"
#include "assimp/scene.h"

#include "gtx/quaternion.hpp"
#include "gtc/quaternion.hpp"

const float pi = 3.141592653589783238f;
const float tau = 2.0f * pi;
const float rad2Deg = 180.0f / pi;
const float deg2Rad = pi / 180.0f;

typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;
typedef glm::mat4 Mat4;
typedef glm::quat Quat;

typedef unsigned char byte;

inline Vec4 Point(Vec3 vec) { return { vec.x, vec.y, vec.z, 1.0f }; };
inline Vec4 Vector(Vec3 vec) { return { vec.x, vec.y, vec.z, 0.0f }; };
inline Vec4 Normal(Vec3 vec) { return Vector(glm::normalize(vec)); };

inline Vec4 Point(aiVector3D vec) { return { vec.x, vec.y, vec.z, 1.0f }; };
inline Vec4 Vector(aiVector3D vec) { return { vec.x, vec.y, vec.z, 0.0f }; };
inline Vec4 Normal(aiVector3D vec) { return Vector(vec.Normalize()); };

inline Vec2 TexCoord(aiVector3D vec) { return { vec.x, vec.y }; }

const Vec4 Zero = { 0.0f, 0.0f, 0.0f, 0.0f };
const Vec4 One = { 1.0f, 1.0f, 1.0f, 1.0f };

const Vec4 Right = { 1.0f, 0.0f, 0.0f, 0.0f };
const Vec4 Up = { 0.0f, 1.0f, 0.0f, 0.0f };
const Vec4 Forward = { 0.0f, 0.0f, 1.0f, 0.0f };

const Mat4 MatIdentity = Mat4(1);
const Quat QIdentity = { 1.0f, 0.0f, 0.0f, 0.0f };

inline float SqrLength(Vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

Mat4 TRS(Vec3 position, Vec4 angleAxisRotation, float scale);
Mat4 TRS(Vec3 position, Vec4 angleAxisRotation, Vec3 scale);
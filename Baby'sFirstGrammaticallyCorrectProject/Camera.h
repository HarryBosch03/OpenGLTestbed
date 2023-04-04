#pragma once

#include "Maths.h"
#include "Graphics.h"
#include "GLuniform.h"

class Application;

class Camera
{
public:
	Vec3 position = { 0.0f, 0.0f, -5.0f };
	Quat rotation = QIdentity;
	float fov = 60.0f;
	float orthoSize = 5.0f;
	bool ortho = false;

	Mat4 view;
	Mat4 projection;

	void Bind();
	void Unbind();

	static Camera* Current;
};
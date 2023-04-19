#pragma once

#include "Maths.h"
#include "DrawGUIListener.h"

class Camera;

class CameraController : DrawGUIListener
{
	void ProcessInput();
	void SetPosition(Camera& camera);

public:
	float moveSpeed = 10.0f;
	float accelerationTime = 0.1f;
	float sensitivity = 0.3f;

	Vec3 moveInput = {};

	Vec2 rotation = {};
	Vec3 velocity = {};
	Vec3 acceleration = {};

	Vec2 lastCursorPosition = {};

	void Control(Camera& camera);
	void DrawGUI() override;
};
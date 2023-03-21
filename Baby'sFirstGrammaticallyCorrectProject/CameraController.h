#pragma once

#include "Maths.h"

class Camera;

class CameraController
{
	void ProcessInput();
	void SetPosition(Camera& camera);

public:
	float panSpeed = 0.3f;
	float distance = 5.0f;
	float drag = 15.0f;
	
	Vec2 position = Zero;
	Vec2 lastPosition = Zero;

	Vec2 lastCursorPos = Zero;

	void Control(Camera& camera);
};
#include "CameraController.h"

#include "Graphics.h"
#include "Application.h"

GLFWwindow* window = nullptr;
Vec2 cursorPosition;

using glm::sin;
using glm::cos;
using glm::radians;
using glm::clamp;

void CameraController::ProcessInput()
{
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) return;

	Vec2 cursorDelta = cursorPosition - lastCursorPos;
	position += cursorDelta * panSpeed;
}

void CameraController::SetPosition(Camera& camera)
{
	position.y = clamp(position.y, -89.9f, 89.9f);
	float a1 = radians(position.x), a2 = radians(position.y);

	Vec2 tempPos = position;
	float dt = Application::Current->FrameTime();
	Vec2 velocity = (position - lastPosition) / dt;
	Vec2 force = -velocity * drag;

	position += velocity * dt + force * dt * dt;
	lastPosition = tempPos;

	camera.position = Vec3(cos(a1) * cos(a2), sin(a2), sin(a1) * cos(a2)) * distance;
}

void CameraController::Control(Camera& camera)
{
	window = Application::Current->Window();

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	cursorPosition = { (float)xPos, (float)yPos };

	ProcessInput();
	SetPosition(camera);

	lastCursorPos = cursorPosition;
}

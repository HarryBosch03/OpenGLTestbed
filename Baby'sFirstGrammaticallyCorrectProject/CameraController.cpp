#include "CameraController.h"

#include "Graphics.h"
#include "Application.h"
#include "imgui.h"
#include "input.h"

GLFWwindow* window = nullptr;
Vec2 cursorPosition;

using glm::sin;
using glm::cos;
using glm::radians;
using glm::clamp;

void CameraController::ProcessInput()
{
	moveInput =
	{
		Input::GetKeyDown(GLFW_KEY_A) - Input::GetKeyDown(GLFW_KEY_D),
		Input::GetKeyDown(GLFW_KEY_LEFT_SHIFT) - Input::GetKeyDown(GLFW_KEY_SPACE),
		Input::GetKeyDown(GLFW_KEY_W) - Input::GetKeyDown(GLFW_KEY_S),
	};

	Vec2 delta = cursorPosition - lastCursorPosition;
	rotation += delta * sensitivity;
	rotation.y = glm::clamp(rotation.y, -90.0f, 90.0f);
}

void CameraController::SetPosition(Camera& camera)
{
	Vec3 targetVelocity = (camera.rotation * moveInput) * moveSpeed;
	acceleration += (targetVelocity - velocity) * moveAcceleration;

	camera.position += velocity * Application::FrameTime();
	velocity += acceleration * Application::FrameTime();
	acceleration = Zero;

	Vec2 rotRad = glm::radians(rotation);
	camera.rotation = Quat(Vec3(0.0f, rotRad.x, rotRad.y));
}

void CameraController::Control(Camera& camera)
{
	window = Application::Current->Window();

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	cursorPosition = { (float)xPos, (float)yPos };

	ProcessInput();
	SetPosition(camera);

	lastCursorPosition = cursorPosition;
}

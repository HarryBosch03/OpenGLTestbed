#include "CameraController.h"

#include "Graphics.h"
#include "Application.h"
#include "imgui.h"
#include "input.h"

Vec2 cursorPosition;

using glm::sin;
using glm::cos;
using glm::radians;
using glm::clamp;

void CameraController::ProcessInput()
{
	moveInput =
	{
		Input::GetKeyDown(GLFW_KEY_D) - Input::GetKeyDown(GLFW_KEY_A),
		Input::GetKeyDown(GLFW_KEY_E) - Input::GetKeyDown(GLFW_KEY_Q),
		Input::GetKeyDown(GLFW_KEY_S) - Input::GetKeyDown(GLFW_KEY_W),
	};

	if (Input::GetMouseDown(GLFW_MOUSE_BUTTON_2))
	{
		Vec2 delta = cursorPosition - lastCursorPosition;
		rotation += delta * sensitivity;
		rotation.y = glm::clamp(rotation.y, -90.0f, 90.0f);
		glfwSetInputMode(Application::Current()->Window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(Application::Current()->Window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void CameraController::SetPosition(Camera& camera)
{
	Vec3 targetVelocity = (glm::inverse(camera.rotation) * moveInput) * moveSpeed;
	acceleration += (targetVelocity - velocity) * moveAcceleration;

	camera.position += velocity * Application::FrameTime();
	velocity += acceleration * Application::FrameTime();
	acceleration = Zero;

	Vec2 rotRad = glm::radians(rotation);
	camera.rotation = Quat(Vec3(rotRad.y, 0.0f, 0.0f)) * Quat(Vec3(0.0f, rotRad.x, 0.0f));
}

void CameraController::Control(Camera& camera)
{
	GLFWwindow* window = Application::Current()->Window();

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	cursorPosition = { (float)xPos, (float)yPos };

	ProcessInput();
	SetPosition(camera);

	lastCursorPosition = cursorPosition;
}

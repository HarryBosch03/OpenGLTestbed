#include "CameraController.h"

#include "Graphics.h"
#include "Application.h"
#include "imgui.h"
#include "input.h"

#include "imgui.h"

Vec2 cursorPosition;

using glm::sin;
using glm::cos;
using glm::radians;
using glm::clamp;

void CameraController::ProcessInput()
{
	moveInput =
	{
		Input::Keyboard().Down(GLFW_KEY_D) - Input::Keyboard().Down(GLFW_KEY_A),
		Input::Keyboard().Down(GLFW_KEY_E) - Input::Keyboard().Down(GLFW_KEY_Q),
		Input::Keyboard().Down(GLFW_KEY_S) - Input::Keyboard().Down(GLFW_KEY_W),
	};

	if (Input::Mouse().Down(GLFW_MOUSE_BUTTON_2))
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

	float moveSpeedExp = log2(max(moveSpeed, FLT_MIN));
	moveSpeedExp += Input::Mouse().ScrollDelta().y * 0.1f;
	moveSpeed = pow(2.0f, moveSpeedExp);
}

void CameraController::SetPosition(Camera& camera)
{
	float accelerationMagnitude = 1.0f / accelerationTime;
	Vec3 targetVelocity = (glm::inverse(camera.rotation) * moveInput) * moveSpeed;
	acceleration += (targetVelocity - velocity) * accelerationMagnitude;

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

void CameraController::DrawGUI()
{
	if (!ImGui::CollapsingHeader("Camera Controller")) return;

	ImGui::Indent();
	ImGui::DragFloat("Move Speed##CameraController", &moveSpeed, 0.1f, 0.0f, FLT_MAX);
	ImGui::Unindent();
}
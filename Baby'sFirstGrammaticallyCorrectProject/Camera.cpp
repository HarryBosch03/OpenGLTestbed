#include "Camera.h"

#include "Application.h"
#include <iostream>

Camera* Camera::Current = nullptr;

void Camera::Bind()
{
	Current = this;

	view = glm::lookAt(position, (Vec3)Zero, { 0.0f, 1.0f, 0.0f });

	int width, height;
	glfwGetWindowSize(Application::Current->Window(), &width, &height);
	float aspect = (float)width / height;

	float fovRad = fov * deg2Rad;
	projection = glm::perspective(fovRad, aspect, 1.0f, 100.0f);
}

void Camera::Unbind()
{
	Current = nullptr;
}
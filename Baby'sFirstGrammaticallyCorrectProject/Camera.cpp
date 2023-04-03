#include "Camera.h"

#include "Application.h"
#include <iostream>

Camera* Camera::Current = nullptr;

void Camera::Bind()
{
	Current = this;

	view = glm::translate(glm::toMat4(rotation), position);

	int width, height;
	glfwGetWindowSize(Application::Current->Window(), &width, &height);
	if (width == 0 || height == 0) return;

	float aspect = (float)width / height;

	float fovRad = fov * deg2Rad;
	projection = glm::perspective(fovRad, aspect, 1.0f, 100.0f);
}

void Camera::Unbind()
{
	Current = nullptr;
}
#include "Camera.h"

#include "Application.h"
#include <iostream>
#include <vector>

Camera* current = nullptr;

std::vector<Camera*> cameras;

Camera::Camera()
{
	for (const Camera* other : cameras)
	{
		if (other == this) return;
	}
	cameras.push_back(this);
}

Camera::~Camera()
{
	for (int i = 0; i < cameras.size(); i++)
	{
		const Camera* other = cameras[i];
		if (other != this) continue;
		cameras.erase(cameras.begin() + i);
		break;
	}
}

void Camera::Bind()
{
	current = this;

	view = glm::translate(glm::toMat4(rotation), -position);

	int width, height;
	glfwGetWindowSize(Application::Current()->Window(), &width, &height);
	if (width == 0 || height == 0) return;

	float aspect = (float)width / height;

	float fovRad = fov * deg2Rad;
	projection = glm::perspective(fovRad, aspect, 0.01f, 100.0f);
}

void Camera::Unbind()
{
	current = nullptr;
}

const std::vector<Camera*>& Camera::Itterator()
{
	return cameras;
}

const Camera* Camera::Current()
{
	return current;
}

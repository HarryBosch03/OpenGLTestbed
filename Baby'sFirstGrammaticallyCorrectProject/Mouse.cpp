#include "Mouse.h"

#include "imgui/imgui.h"

void MouseDevice::Update()
{
	InputDevice::Update();

	lastPosition = position;
	lastScroll = scroll;

	glm::vec<2, double, glm::defaultp> dpos;
	glfwGetCursorPos(Window(), &dpos.x, &dpos.y);
	position = dpos;
}

void MouseDevice::ScrollCallback(float x, float y)
{
	scroll += Vec2{x, y};
}
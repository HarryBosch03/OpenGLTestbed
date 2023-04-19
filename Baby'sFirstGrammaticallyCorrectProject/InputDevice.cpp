#include "InputDevice.h"

#include "Application.h"

GLFWwindow* InputDevice::Window()
{
	return Application::Current()->Window();
}

bool InputDevice::Down(int key) const
{
	if (!cMap.count(key)) return false;
	return cMap.at(key);
}

bool InputDevice::Pressed(int key) const
{
	if (!cMap.count(key) || !lMap.count(key)) return false;
	return cMap.at(key) && !lMap.at(key);
}

bool InputDevice::Released(int key) const
{
	if (!cMap.count(key) || !lMap.count(key)) return false;
	return !cMap.at(key) && lMap.at(key);
}

void InputDevice::Update()
{
	lMap = cMap;
}

void InputDevice::ButtonCallback(int key, bool state)
{
	cMap[key] = state;
}

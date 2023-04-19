#pragma once

#include <map>

class GLFWwindow;

class InputDevice
{
	std::map<int, bool> cMap;
	std::map<int, bool> lMap;

protected:
	GLFWwindow* Window();

public:
	bool Down(int key) const;
	bool Pressed(int key) const;
	bool Released(int key) const;
	virtual void Update();

	virtual void ButtonCallback(int key, bool state);
};
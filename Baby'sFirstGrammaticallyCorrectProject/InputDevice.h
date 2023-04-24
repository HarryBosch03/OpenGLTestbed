#pragma once

#include <map>

class GLFWwindow;

class InputDevice
{
	std::map<int, bool> cMap; // current map
	std::map<int, bool> lMap; // last map

protected:
	GLFWwindow* Window();

public:
	bool Down(int key) const;
	bool Pressed(int key) const;
	bool Released(int key) const;
	virtual void Update();

	virtual void ButtonCallback(int key, bool state);
};
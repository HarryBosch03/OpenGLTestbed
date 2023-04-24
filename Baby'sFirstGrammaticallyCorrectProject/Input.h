#pragma once

#include "imgui/imgui.h"
#include "Maths.h"
#include "Mouse.h"

#include <map>

namespace Input
{
	void Init();
	void Update();
	void DrawGUI();

	const MouseDevice& Mouse();
	const InputDevice& Keyboard();
}
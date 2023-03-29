#pragma once

#include "imgui.h"

#define MAP_FUNCTIONS_DEF_H(name) \
bool Get ## name ## Down(int key); \
bool Get ## name ## Pressed(int key); \
bool Get ## name ## Released(int key); 

namespace Input
{
	void Init();
	void Update();

	MAP_FUNCTIONS_DEF_H(Key)
	MAP_FUNCTIONS_DEF_H(Mouse)
}
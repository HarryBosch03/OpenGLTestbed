#pragma once

#include "InputDevice.h"

#include "Maths.h"
#include "Graphics.h"

class MouseDevice : public InputDevice
{
	Vec2 position = {};
	Vec2 lastPosition = {};

	Vec2 scroll = {};
	Vec2 lastScroll = {};
public:

	inline const Vec2& Position() const { return position; }
	inline const Vec2& LastPosition() const { return lastPosition; }
	inline const Vec2 Delta() const { return position - lastPosition; }

	inline const Vec2& Scroll() const { return scroll; }
	inline const Vec2& LastScroll() const { return lastScroll; }
	inline const Vec2 ScrollDelta() const { return scroll - lastScroll; }

	void Update() override;
	
	void ScrollCallback(float x, float y);
};
#pragma once

#include "Input.h"
#include "Maths.h"
#include "Graphics.h"
#include "Application.h"

namespace Input
{
	class InputDevice
	{
		std::map<int, bool> cMap;
		std::map<int, bool> lMap;

	protected:
		inline GLFWwindow* Window() { return Application::Current()->Window(); }

	public:
		bool GetDown(int key);
		bool GetPressed(int key);
		bool GetReleased(int key);
		virtual void Update();

		virtual bool PollKey(int key) = 0;
	};

	struct Mouse : public InputDevice
	{
		Vec2
			position,
			lastPosition,
			delta;
		float
			scroll,
			lastScroll,
			scrollDelta;

		void Initalize();
		void Update() override;
		inline bool PollKey(int key) override { return glfwGetMouseButton(Window(), key); }
	};
}
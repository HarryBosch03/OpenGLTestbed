#pragma once

#include <string>
#include <chrono>

class RenderProfillingContext
{
	std::string name;
	float durationMillisecconds;

	std::chrono::steady_clock::time_point start;
	float renderTime;

	void Stop();
	static void StopLast();

public:

	RenderProfillingContext(const std::string& name);

	static void Start(const std::string& name);
	static void Finalize();
	static void RenderGUI();
};


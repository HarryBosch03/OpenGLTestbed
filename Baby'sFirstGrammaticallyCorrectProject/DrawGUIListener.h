#pragma once

#include <vector>

class DrawGUIListener
{
	void Register();
	void Deregister();
	bool registered = false;

public:
	static void DrawAllListeners();

	DrawGUIListener();
	DrawGUIListener(const DrawGUIListener& other);
	DrawGUIListener& operator=(const DrawGUIListener& other);
	~DrawGUIListener();

	virtual void DrawGUI() {};
};
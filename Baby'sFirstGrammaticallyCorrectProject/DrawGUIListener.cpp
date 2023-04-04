#include "DrawGUIListener.h"

std::vector<DrawGUIListener*> listeners;

void DrawGUIListener::Register()
{
	if (registered) return;

	listeners.push_back(this);
	registered = true;
}

void DrawGUIListener::Deregister()
{
	if (!registered) return;

	for (auto it = listeners.begin(); it != listeners.end(); ++it)
	{
		if (*it != this) continue;
		listeners.erase(it);
		break;
	}
	registered = false;
}

void DrawGUIListener::DrawAllListeners()
{
	for (DrawGUIListener* listener : listeners)
	{
		listener->DrawGUI();
	}
}

DrawGUIListener::DrawGUIListener()
{
	Register();
}

DrawGUIListener::DrawGUIListener(const DrawGUIListener& other)
{
	Register();
}

DrawGUIListener& DrawGUIListener::operator=(const DrawGUIListener& other)
{
	if (this == &other) return *this;
	Deregister();
}

DrawGUIListener::~DrawGUIListener()
{
	Deregister();
}
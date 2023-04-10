#pragma once

#include "LightingEnviroment.h"
#include "Skybox.h"

#include <vector>

class SceneObject;

class Scene
{
	std::vector<SceneObject*> objects;
	LightingEnviroment lightingEnviroment;
	Skybox skybox;

	void Register(SceneObject* object);
	void Deregister(SceneObject* object);

public:
	bool doomed = false;

	void LoadFromFile(const std::string& fileLoc);

	void Update();
	void FixedUpdate();

	void Render();

	void Clear();

	friend SceneObject;
};
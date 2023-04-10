#pragma once

#include <string>

class Scene;

namespace SceneManager
{
	void Update();
	void FixedUpdate();

	void LoadFromFile(const std::string& fileLoc, bool additive = false);

	void Render();

	void Finalize();

	const Scene* ActiveScene();
}
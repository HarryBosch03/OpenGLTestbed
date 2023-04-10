#include "SceneManager.h"

#include "Scene.h"

#include <vector>

std::vector<Scene*> scenes;
std::vector<Scene*> newScenes;

void SceneManager::LoadFromFile(const std::string& fileLoc, bool additive)
{
	if (!additive)
	{
		for (Scene* scene : scenes) scene->doomed = true;
		for (Scene* scene : newScenes) scene->doomed = true;
	}

	Scene& scene = *new Scene();
	newScenes.push_back(&scene);
	scene.LoadFromFile(fileLoc);
}

void SceneManager::Render()
{
	for (Scene* scene : scenes)
	{
		scene->Render();
	}
}

void SceneManager::Finalize()
{
	for (Scene* scene : newScenes)
	{
		scenes.push_back(scene);
	}
	newScenes.clear();

	for (int i = 0; i < scenes.size(); i++)
	{
		if (!scenes[i]->doomed) continue;

		delete scenes[i];
		scenes.erase(scenes.begin() + i);
		i--;
	}
}

const Scene* SceneManager::ActiveScene()
{
	return scenes.size() > 0 ? scenes[0] : nullptr;
}

void SceneManager::Update()
{

}

void SceneManager::FixedUpdate()
{

}
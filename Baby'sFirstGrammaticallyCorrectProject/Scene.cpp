#include "Scene.h"

#include "SceneObject.h"

std::vector<Scene*> scenes;

Scene::Scene(const std::string& name) : name(name)
{
	scenes.push_back(this);
}

Scene::~Scene()
{
	for (int i = 0; i < scenes.size(); i++)
	{
		if (scenes[i] != this) continue;
		scenes.erase(scenes.begin() + i);
		break;
	}
}

void Scene::LoadFromFile(const std::string& fileLoc)
{

}

void Scene::Update()
{
	for (const SceneObject* old : oldObjects)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i] != old) continue;

			objects.erase(objects.begin() + i);
			i--;
		}
		delete old;
	}
	oldObjects.clear();

	for (SceneObject* n : newObjects)
	{
		objects.push_back(n);
	}
	newObjects.clear();

	for (SceneObject*& mesh : objects)
	{
		mesh->Update();
	}
}

void Scene::FixedUpdate()
{

}

void Scene::Render()
{
	for (SceneObject*& mesh : objects)
	{
		mesh->Draw();
	}
}

void Scene::Add(SceneObject* sceneObject)
{
	newObjects.push_back(sceneObject);
}

void Scene::Delete(SceneObject* sceneObject)
{
	newObjects.push_back(sceneObject);
}

void Scene::Clear()
{
	for (SceneObject*& object : objects)
	{
		delete object;
	}
	objects.clear();
}

const std::vector<Scene*>& Scene::Scenes()
{
	return scenes;
}

const Scene& Scene::Main()
{
	return *scenes[0];
}

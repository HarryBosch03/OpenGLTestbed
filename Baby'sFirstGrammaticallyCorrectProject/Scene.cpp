#include "Scene.h"

#include "SceneObject.h"

void Scene::Register(SceneObject* object)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (object == objects[i]) return;
	}
	objects.push_back(object);
}

void Scene::Deregister(SceneObject* object)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (object != objects[i]) continue;
		delete object;
		objects.erase(objects.begin() + i);
		return;
	}
}

void Scene::LoadFromFile(const std::string& fileLoc)
{

}

void Scene::Update()
{
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
	lightingEnviroment.Bind();
	skybox.Bind();

	for (SceneObject*& mesh : objects)
	{
		mesh->Draw();
	}
	skybox.Draw();

	skybox.Unbind();
	lightingEnviroment.Unbind();
}

void Scene::Clear()
{
	for (SceneObject*& object : objects)
	{
		delete object;
	}
	objects.clear();
}

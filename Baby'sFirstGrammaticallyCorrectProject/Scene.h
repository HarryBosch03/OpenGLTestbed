#pragma once

#include <string>
#include <vector>

class SceneObject;

class Scene
{
	std::vector<SceneObject*> objects;

	std::vector<SceneObject*> newObjects;
	std::vector<SceneObject*> oldObjects;

public:
	std::string name;	
	Scene(const std::string& name);
	Scene(const Scene& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	~Scene();

	void LoadFromFile(const std::string& fileLoc);

	void Update();
	void FixedUpdate();

	void Render();

	void Add(SceneObject* sceneObject);
	void Delete(SceneObject* sceneObject);
	void Clear();
	
	static const std::vector<Scene*>& Scenes();
	static const Scene& Main();

	friend SceneObject;
};
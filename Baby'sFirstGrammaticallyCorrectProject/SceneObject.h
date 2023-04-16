#pragma once

class Scene;

class SceneObject
{
	Scene* scene;

public:
	virtual void Update() {};
	virtual void Draw() {};
	inline const Scene& ParentScene() const { return *scene; }

	friend Scene;
};
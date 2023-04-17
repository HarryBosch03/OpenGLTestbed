#pragma once

#include <vector>

class Scene;

class SceneObject
{
	Scene* scene = nullptr;

public:
	SceneObject();
	SceneObject(const SceneObject& other) = delete;
	SceneObject& operator=(const SceneObject& other) = delete;
	~SceneObject();

	virtual void Update() {};
	virtual void Draw() {};
	inline const Scene& ParentScene() const { return *scene; }

	friend Scene;

	static const std::vector<SceneObject*>& All();

	template<typename T>
	static const std::vector<T*> All()
	{
		std::vector<T*> res;
		for (SceneObject* object : All())
		{
			T* cast = dynamic_cast<T*>(object);
			if (!cast) continue;
			res.push_back(cast);
		}
		return res;
	}
};
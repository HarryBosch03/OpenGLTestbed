#pragma once

#include <vector>
#include <string>

class Scene;

class SceneObject
{
	Scene* scene = nullptr;

protected:
	void SetTypename(const std::string& _typename);

public:
	const int id = -1;
	bool enabled = true;
	std::string name;

	SceneObject();
	SceneObject(const SceneObject& other) = delete;
	SceneObject& operator=(const SceneObject& other) = delete;
	~SceneObject();

	virtual void Update() {};
	void DrawGUI();
	virtual void DrawGUIContent();
	virtual void Draw() {};
	inline const Scene& ParentScene() const { return *scene; }

	friend Scene;

	static const std::vector<SceneObject*>& All(bool includeDisabled = false);

	template<typename T>
	static const std::vector<T*> All(bool includeDisabled = false)
	{
		std::vector<T*> res;
		for (SceneObject* object : All())
		{
			if (!object->enabled) continue;

			T* cast = dynamic_cast<T*>(object);
			if (!cast) continue;
			res.push_back(cast);
		}
		return res;
	}
};
#include "SceneObject.h"

#include <iostream>

std::vector<SceneObject*> instances;

SceneObject::SceneObject()
{
	scene = nullptr;
	instances.push_back(this);	
}

SceneObject::~SceneObject()
{
	for (int i = 0; i < instances.size(); i++)
	{
		const SceneObject* other = instances[i];
		if (other != this) continue;
		instances.erase(instances.begin() + i);
		break;
	}
}

const std::vector<SceneObject*>& SceneObject::All()
{
	return instances;
}

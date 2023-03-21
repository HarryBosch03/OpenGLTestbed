#pragma once

struct MeshData;

#include <string>

namespace AssetDatabase
{
	void LoadAllAssets();
	MeshData* GetMesh(std::string name);
};
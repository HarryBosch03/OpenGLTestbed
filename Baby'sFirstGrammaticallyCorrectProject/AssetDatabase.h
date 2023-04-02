#pragma once

#include "Logger.h"

#include <string>
#include <map>

class Texture;
class MeshData;

namespace AssetDatabase
{
	void Use();
	void Cleanup();

	Texture* GetTexture(const std::string& fileLoc);
	MeshData* GetMesh(const std::string& fileLoc, int subMeshIndex);
};

#define ASSET(type, fileLoc) AssetDatabase::Get##type(fileLoc)
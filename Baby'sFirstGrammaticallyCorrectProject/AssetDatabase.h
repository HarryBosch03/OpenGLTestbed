#pragma once

#include "LogMaster.h"
#include "Texture.h"
#include "Mesh.h"

#include <string>
#include <map>

typedef std::pair<const std::string, Asset*> AssetEntry;

extern std::map<std::string, Asset*> assets;

namespace AssetDatabase
{
	void Cleanup();
	void HotReload();
	void HotReload(bool(*predicate)(const AssetEntry& entry));

	template<typename T>
	T* Get(const std::string& fileLoc, void* args = nullptr)
	{
		T* asset = assets.count(fileLoc) > 0 ? (T*)assets.at(fileLoc) : nullptr;
		if (asset) return asset;
		asset = new T();
		asset->LoadFromFile("../Assets/" + fileLoc, args);
		assets[fileLoc] = (Asset*)asset;
		return (T*)asset;
	}
};
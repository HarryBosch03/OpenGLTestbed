#pragma once

#include "LogMaster.h"
#include "Texture.h"
#include "Mesh.h"

#include <string>
#include <map>
#include <vector>

typedef std::pair<const std::string, Asset*> AssetEntry;

extern std::map<std::string, Asset*> assets;

class AssetDatabase
{
	std::map<std::string, Asset*> assets;

public:
	void Cleanup();
	void HotReload();
	void HotReload(bool(*predicate)(const AssetEntry& entry));

	static const std::string AssetLocation();
	static const std::string AssetLocation(const std::string& filename);
	static const AssetDatabase& Main();

	template<typename T>
	T* Get(const std::string& fileloc, void* args = nullptr)
	{
		T* asset = assets.count(fileloc) > 0 ? (T*)assets.at(fileloc) : nullptr;
		if (asset) return asset;
		asset = new T();
		asset->LoadFromFile(AssetLocation() + fileloc, args);
		assets[fileloc] = (Asset*)asset;
		return (T*)asset;
	}

	template<typename T>
	std::vector<std::string> GetType(const std::string& subdir = "")
	{
		std::vector<std::string> res;

		for (auto dir : std::filesystem::recursive_directory_iterator(AssetLocation() + subdir))
		{
			if (dir.is_directory()) continue;

			const std::string& path = dir.path().string();
			if (!T::DoesFileMatch(path)) continue;

			bool dirty = false;
			for (const std::string& other : res)
			{
				if (path != other) continue;
				dirty = true;
				break;
			}
			if (dirty) continue;

			res.push_back(path);
		}

		return res;
	}
};

AssetDatabase& Assets();

template<typename T>
T* GetAsset(const std::string& fileloc, void* args = nullptr)
{
	return Assets().Get<T>(fileloc, args);
}
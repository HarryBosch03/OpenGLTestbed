#include "AssetDatabase.h"

#include "Texture.h"
#include "MeshData.h"

#include <filesystem>

std::map<std::string, void*> assets;

template<typename T>
T* TryLoadExistingAsset(const std::string& fileLoc)
{
	return assets.count(fileLoc) > 0 ? (T*)assets.at(fileLoc) : nullptr;
}

void AssetDatabase::Cleanup()
{
	for (auto& entry : assets)
	{
		delete entry.second;
	}
	assets.clear();
}

#define LOAD_TYPE_DEF(T, ...) \
{ \
	T* tex; \
	if (tex = TryLoadExistingAsset<T>(fileLoc)) return tex; \
	tex = &(new T())->LoadFromFile(fileLoc, __VA_ARGS__); \
	return tex; \
}

MeshData* AssetDatabase::GetMesh(const std::string& fileLoc, int subMeshIndex) LOAD_TYPE_DEF(MeshData, 0)

Texture* AssetDatabase::GetTexture(const std::string& fileLoc) LOAD_TYPE_DEF(Texture)
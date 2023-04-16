#include "AssetDatabase.h"

#include "Asset.h"
#include "LogMaster.h"
#include "Mesh.h"

#include <filesystem>

AssetDatabase* current;

Asset* CreateAssetType(const std::string& ext)
{
	if (ext == "fbx") return new Mesh();
	return nullptr;
}

void AssetDatabase::LoadAsset(const std::string& assetPath)
{
	if (assetMap.count(assetPath)) return;

	std::string ext = assetPath.substr(assetPath.rfind('.') + 1);
	Asset* asset = CreateAssetType(ext);
	if (!asset)
	{
		LogWarning("\"" << assetPath << "\" with Extension \"" << ext << "\" was not Recognised as a Valid Asset Type.");
		return;
	}

	if (!asset->LoadFromFile(assetPath))
	{
		LogError("Failed to Load Asset \"" << assetPath << "\"");
		delete asset;
		asset = nullptr;
		return;
	}

	assetMap[assetPath] = asset;
}

AssetDatabase::AssetDatabase(const std::string& assetPath) : assetPath(assetPath) { }

AssetDatabase::~AssetDatabase()
{
	for (const auto& pair : assetMap)
	{
		delete pair.second;
	}
	assetMap.clear();
}

void AssetDatabase::PreloadAll(const std::string& subpath)
{
	for (const auto& dir : std::filesystem::recursive_directory_iterator(assetPath + subpath))
	{
		if (dir.is_directory()) continue;
		LoadAsset(dir.path().string());
	}
}

const Asset* AssetDatabase::Get(const std::string& assetPath)
{
	if (!assetMap.count(assetPath)) LoadAsset(assetPath);
	return assetMap.at(assetPath);
}

const AssetDatabase& AssetDatabase::Current()
{
	return *current;
}

#pragma once

#include <string>
#include <map>

class Asset;

class AssetDatabase
{
	std::map<std::string, Asset*> assetMap;

	void LoadAsset(const std::string& assetPath);

public:
	std::string assetPath;

	AssetDatabase(const std::string& assetPath = "../Assets/");
	AssetDatabase(const AssetDatabase& other) = delete;
	AssetDatabase& operator=(const AssetDatabase& other) = delete;
	~AssetDatabase();

	void PreloadAll(const std::string& subpath = "");
	const Asset* Get(const std::string& assetPath);

	static const AssetDatabase& Current();
};
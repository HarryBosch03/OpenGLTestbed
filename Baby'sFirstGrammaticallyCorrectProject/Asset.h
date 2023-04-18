#pragma once

#include <string>
#include <filesystem>

enum class AssetType;

class Asset
{
protected:
	std::string fileloc;
	void* args = nullptr;
		
public:
	inline const std::string& FileName() { return fileloc; }
	virtual AssetType GetType() const = 0;

	virtual Asset& LoadFromFile(const std::string& fileloc, void* args);
	virtual Asset& Reload() = 0;
	Asset& ActiveHotReload();
};

enum class AssetType
{
	Invalid,
	Texture,
	Mesh,
	Shader,

	Count
};

const std::string AssetTypenames[]
{
	"Invalid",

	"Texture",
	"Mesh",
	"Shader",
};

const std::string AssetTypenamePlurals[]
{
	"Invalid",

	"Textures",
	"Meshes",
	"Shaders",
};
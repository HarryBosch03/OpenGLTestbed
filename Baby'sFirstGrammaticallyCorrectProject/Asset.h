#pragma once

#include <string>

enum class AssetType;

class Asset
{
protected:
	std::string fileLoc;
	void* args = nullptr;
		
public:
	inline const std::string& FileName() { return fileLoc; }
	virtual AssetType GetType() const = 0;

	virtual Asset& LoadFromFile(const std::string& fileLoc, void* args);
	virtual Asset& Reload() = 0;
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
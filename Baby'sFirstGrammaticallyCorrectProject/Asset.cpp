#include "Asset.h"

#include "AssetDatabase.h"

Asset& Asset::LoadFromFile(const std::string& fileloc, void* args)
{
	int offset = AssetDatabase::AssetLocation().size();
	this->fileloc = fileloc.substr(offset);
	this->args = args;
	return *this;
}

Asset& Asset::Reload()
{
	return LoadFromFile(AssetDatabase::AssetLocation() + fileloc, args);
}
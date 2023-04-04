#include "Asset.h"

Asset& Asset::LoadFromFile(const std::string& fileLoc, void* args)
{
	this->fileLoc = fileLoc;
	return *this;
}
#include "Asset.h"

Asset& Asset::LoadFromFile(const std::string& fileloc, void* args)
{
	this->fileloc = fileloc;
	this->args = args;
	return *this;
}
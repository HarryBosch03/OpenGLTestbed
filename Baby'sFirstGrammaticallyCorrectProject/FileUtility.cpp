#include "FileUtility.h"

std::string Utility::Files::FileName(const std::string& filePath)
{
	return filePath.substr(filePath.find_last_of("/\\") + 1);
}

std::string Utility::Files::Directory(const std::string& filePath)
{
	return filePath.substr(0, filePath.find_last_of("/\\") + 1);
}

std::string Utility::Files::RemoveExtension(const std::string& filePath)
{
	std::string name = FileName(filePath);
	return Directory(filePath) + name.substr(0, name.find_last_of('.') - 1);
}
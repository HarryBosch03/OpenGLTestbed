#include "FileIO.h"

#include "LogMaster.h"

#include <fstream>
#include <iostream>
#include <filesystem>

bool DoesFileExist(std::string path)
{
	bool exists = std::ifstream(path).is_open();
	if (!exists)
	{
		std::string fullpath = std::filesystem::absolute(path).string();
		LogError("Path: \"" << fullpath << "\" does not exist!");
	}
	return exists;
}

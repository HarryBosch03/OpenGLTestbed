#include "FileIO.h"

#include "Logger.h"

#include <fstream>
#include <iostream>
#include <filesystem>

bool DoesFileExist(std::string path)
{
	bool exists = std::ifstream(path).is_open();
	if (!exists)
	{
		std::string fullpath = std::filesystem::absolute(path).string();
		LOG_ERROR("Path: \"" << fullpath << "\" does not exist!");
	}
	return exists;
}

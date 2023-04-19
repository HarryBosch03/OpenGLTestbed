#include "FileUtility.h"

#include "LogMaster.h"

#include <sstream>
#include <fstream>
#include <filesystem>

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

std::string Utility::Files::Ext(const std::string& filepath)
{
	return filepath.substr(filepath.rfind('.') + 1);
}

bool Utility::Files::Exists(const std::string& filepath)
{
	return std::filesystem::exists(filepath);
}

std::string Utility::Files::LoadTextFromFile(const std::string& fileName, bool* success, bool log)
{
	std::stringstream res;
	std::fstream file(fileName.c_str(), std::ios::in);
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::string thisLine;
			std::getline(file, thisLine);
			res << thisLine << std::endl;
		}
	}
	else
	{
		if (log) LogError("Could not find file \"" << fileName << "\"");
		if (success) *success = false;
		return "";
	}
	file.close();
	if (success) *success = true;
	return res.str();
}
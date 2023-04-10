#include "BMUtil.h"

#include "LogMaster.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string BMUtil::LoadTextFromFile(const std::string& fileName, bool* success, bool log)
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
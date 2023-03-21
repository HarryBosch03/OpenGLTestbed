#include "BMUtil.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string BMUtil::LoadTextFromFile(std::string fileName)
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
		std::cout << std::string() << "Could not find file \"" + fileName + "\"\n";
	}
	file.close();
	return res.str();
}
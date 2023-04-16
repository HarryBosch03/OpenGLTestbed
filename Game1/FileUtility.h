#pragma once

#include <string>

namespace Utility
{
	namespace Files
	{
		std::string FileName(const std::string& filePath);
		std::string Directory(const std::string& filePath);
		
		std::string RemoveExtension(const std::string& filePath);
	}
}
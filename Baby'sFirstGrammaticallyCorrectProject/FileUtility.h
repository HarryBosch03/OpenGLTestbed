#pragma once

#include <string>

namespace Utility
{
	namespace Files
	{
		std::string FileName(const std::string& filePath);
		std::string Directory(const std::string& filePath);
		
		std::string RemoveExtension(const std::string& filePath);
		std::string Ext(const std::string& filepath);

		bool Exists(const std::string& filepath);

		std::string LoadTextFromFile(const std::string& fileName, bool* success = nullptr, bool log = true);
	}
}
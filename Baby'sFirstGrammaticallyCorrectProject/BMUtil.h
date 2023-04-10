#pragma once

#include <string>

namespace BMUtil
{
	std::string LoadTextFromFile(const std::string& fileName, bool* success, bool log = true);
}
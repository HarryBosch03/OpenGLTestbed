#pragma once

#include <string>

namespace Utility
{
	namespace String
	{
		std::string PadStart(const std::string& str, int tLength, char pad = ' ');
		std::string PadEnd(const std::string& str, int tLength, char pad = ' ');
		std::string PadBoth(const std::string& str, int tLength, char pad = ' ');
	}
}
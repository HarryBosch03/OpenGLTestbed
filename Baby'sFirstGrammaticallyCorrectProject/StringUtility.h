#pragma once

#include "Maths.h"

#include <string>
#include <vector>

namespace Utility
{
	namespace String
	{
		std::string PadStart(const std::string& str, int tLength, char pad = ' ');
		std::string PadEnd(const std::string& str, int tLength, char pad = ' ');
		std::string PadBoth(const std::string& str, int tLength, char pad = ' ');

		std::vector<std::string> Split(const std::string in, const char split);

		Vec4 FromVec(const std::string& str);
		void FromVec(const std::string& str, std::vector<float>& out);
	}
}
#include "StringUtility.h"

std::string Utility::String::PadStart(const std::string& str, int tLength, char pad)
{
	return std::string(pad, tLength) + str;
}

std::string Utility::String::PadEnd(const std::string& str, int tLength, char pad)
{
	return str + std::string(pad, tLength);
}

std::string Utility::String::PadBoth(const std::string& str, int tLength, char pad)
{
	return std::string(pad, tLength / 2) + str + std::string(pad, tLength - tLength / 2);
}
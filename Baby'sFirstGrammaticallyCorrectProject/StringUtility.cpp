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

std::vector<std::string> Utility::String::Split(const std::string in, const char split)
{
	std::vector<std::string> out;
	int h = 0;
	while (h < in.length())
	{
		int n = in.find(split, h);
		if (n == -1) n = in.size() - 1;
		out.push_back(in.substr(h, n - h));
		h = n + 1;
		if (in[h] == ' ') h++;
	}
	return out;
}

Vec4 Utility::String::FromVec(const std::string& str)
{
	Vec4 out = {};
	auto splits = Split(str, ',');
	int i = 0;
	for (const auto& split : splits)
	{
		out[i++] = std::stof(split);
	}
	return out;
}

void Utility::String::FromVec(const std::string& str, std::vector<float>& out)
{
	auto splits = Split(str, ',');
	for (const auto& split : splits)
	{
		out.push_back(std::stof(split));
	}
}

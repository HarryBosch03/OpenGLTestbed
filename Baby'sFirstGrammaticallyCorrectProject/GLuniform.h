#pragma once

#include <map>
#include <vector>

namespace GLuniform
{
	std::map<std::string, std::vector<unsigned char>> map;

	void Set(const std::string& ref, unsigned char* data, int length);
	void Get(const std::string& ref, unsigned char** data, int* length);


	template<typename T>
	void Set(const std::string& ref, const T& data)
	{
		unsigned char* raw[sizeof(T)];
		for (int i = 0; i < sizeof(T); i++)
		{
			raw[i] = ((unsigned char*)&data)[i];
		}
		Set(ref, raw, sizeof(T));
	}
}
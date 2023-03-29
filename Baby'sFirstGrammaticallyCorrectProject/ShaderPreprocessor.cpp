#include "ShaderPreprocessor.h"

#include "stb_include.h"
#include "Logger.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>

typedef std::stringstream sstream;
typedef std::string string;
typedef std::fstream fstream;

std::vector<string> includes;

void WriteOutputToFile(const string& data, string filePath)
{
	int period = filePath.find_last_of('.');

	filePath += "[COMPILED].glsl";

	fstream fs;

	fs.open(filePath, std::ios::out);
	if (fs.is_open())
	{
		fs.write(data.c_str(), data.size());
	}
	else
	{
		std::cout << "Failed to write to " << filePath << "\n\n";
	}
	fs.close();
}

#define C_STRING_COPY(str) \
char* str ## C = new char[str.size() + 1]; \
strcpy(str ## C, str.data()); \
str ## C[str.size()] = '\0';

string ShaderPreprocessor::GenerateShaderFromPath(const string& filePath)
{
	string shader = BMUtil::LoadTextFromFile(filePath);
	char error[256];

	std::string inject = "";
	std::string include = "./Assets/Shaders";

	C_STRING_COPY(filePath)
	C_STRING_COPY(inject)
	C_STRING_COPY(include)

	char* data = stb_include_file(filePathC, injectC, includeC, error);
	if (!data)
	{
		LOG_ERROR("Failed to load scene file at \"filePath\" does not exist!");
		LOG_ERROR("STB ERROR DUMP: " << error);
		return {};
	}

	string output = string() + data;
	free(data);

	delete[] filePathC;
	delete[] injectC;
	delete[] includeC;

	WriteOutputToFile(output, filePath);
	return output;
}

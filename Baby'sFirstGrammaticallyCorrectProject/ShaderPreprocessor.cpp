#include "ShaderPreprocessor.h"

#include "stb_include.h"
#include "LogMaster.h"
#include "ShaderProgram.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>

std::vector<std::string> includes;
const std::string CompiledLocation = "../Assets/" + ShaderPath + "Compiled/";

void ShaderPreprocessor::Initalize()
{
	std::filesystem::remove_all(CompiledLocation);
	std::filesystem::create_directory(CompiledLocation);
}

void WriteOutputToFile(const std::string& data, const std::string& filePath, int attempt = 0)
{
	std::string path = filePath;

	if (attempt > 0) path += "(" + std::to_string(attempt) + ")";
	path += "[COMPILED].glsl";

	std::fstream fs(path, std::ios::out);
	if (fs.is_open())
	{
		fs.write(data.c_str(), data.size());
	}
	else
	{
		std::cout << "Failed to write to \"" << path << "\"\n\n";
	}
	fs.close();
}

std::string ShaderPreprocessor::ParseIncludes(const std::string& shader, const std::string& shaderName)
{
	char error[256] = {'\0'};

	std::string shaderPath = ("../Assets/" + ShaderPath).c_str();

	char* shaderC = new char[shader.size() + 1];
	char* includeC = new char[shaderPath.size() + 1];

	std::strcpy(shaderC, shader.c_str());
	std::strcpy(includeC, shaderPath.c_str());

	char* data = stb_include_string(shaderC, nullptr, includeC, nullptr, error);
	if (!data)
	{
		LogError("Failed Shader Preprocess for \"" << shaderName << "\"");
		LogError("STB ERROR DUMP: " << error);
		return {};
	}

	delete[] shaderC;
	delete[] includeC;

	std::string output = std::string() + data;
	free(data);

	WriteOutputToFile(output, CompiledLocation + shaderName);

	return output;
}

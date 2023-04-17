#pragma once

#include <string>
#include <sstream>

namespace ShaderPreprocessor
{
	void Initalize();
	std::string ParseIncludes(const std::string& shader, const std::string& shaderName);
};
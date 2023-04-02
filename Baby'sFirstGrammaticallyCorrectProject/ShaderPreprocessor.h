#pragma once

#include <string>
#include <sstream>
#include "BMUtil.h"

namespace ShaderPreprocessor
{
	void Initalize();
	std::string ParseIncludes(const std::string& shader, const std::string& shaderName);
};
#pragma once

#include <string>
#include <vector>

class ShaderProperty;

class ShaderCompiler
{
public:
	void Compile(const std::string& fileloc, std::string& vert, std::string& frag, std::vector<ShaderProperty>& properties);
};
#pragma once

#include <string>

class Asset
{
	std::string filepath;

public:

	inline const std::string& Filepath() const { return filepath; }

	virtual bool LoadFromFile(const std::string& filepath);
};
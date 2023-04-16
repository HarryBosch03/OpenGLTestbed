#pragma once

#include "Asset.h"
#include "Submesh.h"

#include <vector>

class aiScene;

class Mesh : public Asset
{
	std::vector<Submesh> submeshes;

public:
	bool LoadFromFile(const std::string& filepath) override;
};
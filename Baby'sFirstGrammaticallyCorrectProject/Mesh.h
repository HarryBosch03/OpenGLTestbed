#pragma once

#include "Asset.h"
#include "VertexData.h"

#include <vector>

class Material;
class Submesh;

class Mesh : public Asset
{
public:
	std::vector<Submesh*> submeshes;
	
	inline AssetType GetType() const override { return AssetType::Mesh; }

	Asset& LoadFromFile(const std::string& path, void* args) override;
	static bool DoesFileMatch(const std::string& fileloc);

	void Draw(const std::vector<Material>& materials) const;
};
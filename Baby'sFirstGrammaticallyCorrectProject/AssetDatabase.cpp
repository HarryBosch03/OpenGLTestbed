#include "AssetDatabase.h"

#include "Texture.h"
#include "MeshData.h"
#include "Asset.h"
#include "DrawGUIListener.h"
#include "FileUtility.h"
#include "Tree.h"

#include "imgui.h"
#include "AssetDatabasePredicate.h"

#include <chrono>

std::map<std::string, Asset*> assets;

void AssetDatabase::Cleanup()
{
	for (auto& entry : assets)
	{
		delete entry.second;
	}
	assets.clear();
}

void AssetDatabase::HotReload()
{
	HotReload(AssetDatabase::Predicates::All);
}

void AssetDatabase::HotReload(bool(*predicate)(const AssetEntry& entry))
{
	LOG_WARNING("Reloading All Assets...");

	auto start = std::chrono::high_resolution_clock::now();
	for (const AssetEntry& asset : assets)
	{
		if (!predicate(asset)) continue;
		asset.second->Reload();
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	LOG_SUCCESS("Finished Reloading " << assets.size() << " Assets Successfully in " << duration.count() << "ms");
}

class AssetDatabaseDrawer : public DrawGUIListener
{
public:
	void DrawGUI() override
	{
		std::map<std::string, std::vector<std::string>> pathMap;
		for (const AssetEntry& asset : assets)
		{
			int i = asset.first.substr(0, 2) == "./" ? 2 : 0;
			std::stringstream working;
			while (i < asset.first.size())
			{
				char c = asset.first[i++];
				if (c == '/' || c == '\\')
				{
					pathMap[asset.first].push_back(working.str());
					working.clear();
					continue;
				}
				working << c;
			}
		}

		if (ImGui::CollapsingHeader("Asset Database"))
		{
			int depth;
			for (const AssetEntry& asset : assets)
			{
				
			}
		}
	}
};

AssetDatabaseDrawer drawer;
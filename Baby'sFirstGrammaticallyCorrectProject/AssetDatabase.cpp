#include "AssetDatabase.h"

#include "Texture.h"
#include "Mesh.h"
#include "Asset.h"
#include "DrawGUIListener.h"
#include "FileUtility.h"
#include "Application.h"

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
	LogWarning("Reloading All Assets...");

	auto start = std::chrono::high_resolution_clock::now();
	for (const AssetEntry& asset : assets)
	{
		if (!predicate(asset)) continue;
		asset.second->Reload();
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	LogSuccess("Finished Reloading " << assets.size() << " Assets Successfully in " << duration.count() << "ms");
}
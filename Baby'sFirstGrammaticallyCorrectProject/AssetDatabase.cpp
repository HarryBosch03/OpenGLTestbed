#include "AssetDatabase.h"

#include "Texture.h"
#include "Mesh.h"
#include "Asset.h"
#include "FileUtility.h"
#include "Application.h"

#include "imgui/imgui.h"
#include "AssetDatabasePredicate.h"

#include <chrono>

AssetDatabase _main;

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
	HotReload(AssetDatabasePredicates::All);
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

const std::string AssetDatabase::AssetLocation()
{
#if _DEBUG
	return "../Assets/";
#else
	return "./Assets/";
#endif
}

const std::string AssetDatabase::AssetLocation(const std::string& filename)
{
	return AssetLocation() + filename;
}

const AssetDatabase& AssetDatabase::Main()
{
	return _main;
}

AssetDatabase& Assets()
{
	return _main;
}
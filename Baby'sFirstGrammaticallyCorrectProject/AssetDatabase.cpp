#include "AssetDatabase.h"

#include <filesystem>

namespace fs = std::filesystem;

void LoadAllMeshes()
{
    fs::current_path("./Assets");
    for (auto const& entry : fs::recursive_directory_iterator(".obj"))
    {

    }
}

void AssetDatabase::LoadAllAssets()
{

}

MeshData* AssetDatabase::GetMesh(std::string name)
{
    return nullptr;
}

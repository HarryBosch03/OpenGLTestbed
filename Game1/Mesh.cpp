#include "Mesh.h"

#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"

Vec4 Vec(const aiVector3D& vec, float w)
{
    return {vec.x, vec.y, vec.z, w};
}

const aiScene* LoadMeshSceneFromFile(const std::string& filepath)
{
    const char* pathC = filepath.c_str();
    return aiImportFile(pathC, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_GenNormals);
}

void LoadNextSubmesh(int submesh, Submesh& data, const aiScene& scene)
{
    const aiMesh& mesh = *scene.mMeshes[submesh];
    
    for (int i = 0; i < mesh.mNumFaces; i++)
    {
        for (int j = 0; j < mesh.mFaces[i].mNumIndices; j++)
        {
            data.indicies.push_back(mesh.mFaces[i].mIndices[j]);
        }
    }

    for (int i = 0; i < mesh.mNumVertices; i++)
    {
        VertexData& vertex = data.verticies.emplace_back();

        vertex.vertex = Vec(mesh.mVertices[i], 1.0);
        vertex.normal = Vec(mesh.mNormals[i], 0.0);
        vertex.tangent = Vec(mesh.mTangents[i], 1.0);
        for (int j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; j++)
        {
            if (!mesh.mTextureCoords[j]) break;
            vertex.texcoords[j] = Vec(mesh.mTextureCoords[j][i], 0.0);
        }
    }
}

bool Mesh::LoadFromFile(const std::string& filepath)
{
    Asset::LoadFromFile(filepath);

    const aiScene* scene = LoadMeshSceneFromFile(filepath);
    if (!scene) return false;

    for (int i = 0; i < scene->mNumMeshes; i++)
    {
        LoadNextSubmesh(i, submeshes.emplace_back(), *scene);
    }
    
    aiReleaseImport(scene);
    return true;
}
#include "Mesh.h"

#include "LogMaster.h"
#include "Material.h"
#include "Submesh.h"

#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"

const aiScene* LoadMeshSceneFromFile(const std::string& fileLoc)
{
	const char* pathC = fileLoc.c_str();
	const aiScene* mesh = aiImportFile(pathC, aiPostProcessSteps::aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_GenNormals);
	if (!mesh)
	{
		const char* error = aiGetErrorString();
		LogError("Error importing file \"" << fileLoc << "\"\nDump: \n" << error << "\nAt Line ");
	}
	return mesh;
}

Vec4 Vec(const aiVector3D& v, float w)
{
	return Vec4(v.x, v.y, v.z, w);
}

Vec3 Vec(const aiVector3D& v)
{
	return Vec3(v.x, v.y, v.z);
}

void LoadSubmesh(Submesh& data, const aiMesh& mesh)
{
	std::vector<VertexData>& vertices = data.Vertices();
	std::vector<MeshIndex>& indices = data.Indices();

	vertices.clear();
	indices.clear();

	for (int i = 0; i < mesh.mNumFaces; i++)
	{
		for (int j = 0; j < mesh.mFaces[i].mNumIndices; j++)
		{
			indices.push_back(mesh.mFaces[i].mIndices[j]);
		}
	}

	for (int i = 0; i < mesh.mNumVertices; i++)
	{
		Vec4 vertex, normal, tangent;
		Vec2 texCoord;

		vertex = Vec(mesh.mVertices[i], 1.0f);
		normal = Vec(mesh.mNormals[i], 0.0f);
		tangent = Vec(mesh.mTangents[i], 1.0);

		texCoord = TexCoord(mesh.mTextureCoords[0][i]);

		vertices.push_back(
			{
				vertex,
				normal,
				texCoord,
				tangent,
			});
	}
}

Asset& Mesh::LoadFromFile(const std::string& fileLoc, void* args)
{
	Asset::LoadFromFile(fileLoc, args);

	submeshes.clear();
	const aiScene* scene = LoadMeshSceneFromFile(fileLoc);
	if (!scene) return *this;

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		Submesh* submesh = new Submesh(i);
		submeshes.push_back(submesh);
		LoadSubmesh(*submesh, *scene->mMeshes[i]);
	}

	aiReleaseImport(scene);
	return *this;
}

Asset& Mesh::Reload()
{
	LoadFromFile(fileloc, args);
	return *this;
}

void Mesh::Draw(const std::vector<Material>& materials) const
{
	for (int i = 0; i < submeshes.size(); i++)
	{
		Submesh& submesh = *submeshes[i];
		const Material& material = i < materials.size() ? materials[i] : nullptr;

		material.Bind();
		submesh.Draw();
		material.Unbind();
	}
}
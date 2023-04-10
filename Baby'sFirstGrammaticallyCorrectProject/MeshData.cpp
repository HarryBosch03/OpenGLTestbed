#include "MeshData.h"

#include "Maths.h"
#include "FileIO.h"
#include "LogMaster.h"
#include "FileUtility.h"

#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"

#include <fstream>

MeshData& MeshData::MergeByDistance(float threshold)
{
	MeshData old = *this;

	for (int i = 0; i < vertices.size(); i++)
	{
		Vec3 a = vertices[i].position;
		for (int j = i + 1; j < vertices.size(); j++)
		{
			Vec3 b = vertices[j].position;
			if (SqrLength(a - b) > threshold) continue;

			for (int k = 0; k < indices.size(); k++)
			{
				if (indices[k] == j) indices[k] = i;
				if (indices[k] > j) indices[k]--;
			}

			vertices.erase(vertices.begin() + j);
			j--;
		}
	}

	return *this;
}

MeshData& MeshData::SplitNormals()
{
	MeshData old = *this;

	vertices.clear();
	indices.clear();

	for (int i = 0; i < old.indices.size(); i++)
	{
		vertices.push_back(old.vertices[old.indices[i]]);
		indices.push_back(i);
	}
	return *this;
}

MeshData& MeshData::Subdivide()
{
	MeshData old = *this;

	vertices.clear();
	indices.clear();

	size_t i;
	for (i = 0; i < old.indices.size(); i += 3)
	{
		int i0 = old.indices[i];
		int i1 = old.indices[i + 1];
		int i2 = old.indices[i + 2];

		Vec3 a = old.vertices[i0].position;
		Vec3 b = old.vertices[i2].position;
		Vec3 c = old.vertices[i2].position;

		Vec3 ab = (a + b) / 2.0f;
		Vec3 bc = (b + c) / 2.0f;
		Vec3 ca = (c + a) / 2.0f;

		int s = vertices.size();

		vertices.push_back({ Point(a) });
		vertices.push_back({ Point(ab) });
		vertices.push_back({ Point(b) });
		vertices.push_back({ Point(bc) });
		vertices.push_back({ Point(c) });
		vertices.push_back({ Point(ca) });

		indices.push_back(s);
		indices.push_back(s + 1);
		indices.push_back(s + 5);

		indices.push_back(s + 1);
		indices.push_back(s + 2);
		indices.push_back(s + 3);

		indices.push_back(s + 3);
		indices.push_back(s + 4);
		indices.push_back(s + 5);

		indices.push_back(s + 5);
		indices.push_back(s + 1);
		indices.push_back(s + 3);
	}

	return *this;
}

MeshData& MeshData::Subdivide(int itterations)
{
	for (int i = 0; i < itterations; i++)
	{
		Subdivide();
	}
	return *this;
}

MeshData& MeshData::CalculateNormals(bool flip)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i].normal = Zero;
	}

	for (int i = 0; i < indices.size(); i += 3)
	{
		unsigned int i1 = indices[i];
		unsigned int i2 = indices[i + 1];
		unsigned int i3 = indices[i + 2];

		Vec3 a = vertices[i1].position, b = vertices[i2].position, c = vertices[i3].position;

		Vec3 ba = b - a;
		Vec3 ca = c - a;
		Vec3 normal = glm::normalize(glm::cross(ca, ba));

		vertices[i1].normal += Normal(normal);
		vertices[i2].normal += Normal(normal);
		vertices[i3].normal += Normal(normal);
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i].normal = Normal(vertices[i].normal * (flip ? -1.0f : 1.0f));
	}

	return *this;
}

void AddTri(aiMesh* mesh, IndexList& indices, int face, int a, int b, int c)
{
	indices.push_back(mesh->mFaces[face].mIndices[a]);
	indices.push_back(mesh->mFaces[face].mIndices[b]);
	indices.push_back(mesh->mFaces[face].mIndices[c]);
}

const MeshScene* LoadMeshSceneFromFile(const std::string& fileLoc)
{
	const char* pathC = fileLoc.c_str();
	const MeshScene* mesh = aiImportFile(pathC, aiPostProcessSteps::aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType);
	if (!mesh)
	{
		const char* error = aiGetErrorString();
		LogError("Error importing file \"" << fileLoc << "\"\nDump: \n" << error << "\nAt Line ");
	}
	return mesh;
}

void ReleaseMeshData(const MeshScene* scene)
{
	aiReleaseImport(scene);
}

Vec4 Vec(const aiVector3D& v, float w)
{
	return Vec4(v.x, v.y, v.z, w);
}

Vec3 Vec(const aiVector3D& v)
{
	return Vec3(v.x, v.y, v.z);
}

Asset& MeshData::LoadFromFile(const std::string& fileLoc, void* args)
{
	Asset::LoadFromFile(fileLoc, args);

	int subMeshIndex = args ? *(int*)args : 0;
	vertices.clear();
	indices.clear();

	if (!DoesFileExist(fileLoc)) return *this;

	const MeshScene* scene = LoadMeshSceneFromFile(fileLoc);
	aiMesh* mesh = scene->mMeshes[subMeshIndex];
	int faceCount = mesh->mNumFaces;
	for (int i = 0; i < faceCount; i++)
	{
		for (int j = 0; j < mesh->mFaces[i].mNumIndices - 2; j++)
		{
			AddTri(mesh, indices, i, 0, j + 2, j + 1);
		}
	}

	int vertexCount = mesh->mNumVertices;
	for (int i = 0; i < vertexCount; i++)
	{
		Vec4 vertex, normal, tangent;
		Vec2 texCoord;
		bool hasTangents = mesh->HasTangentsAndBitangents();

		vertex = Vec(mesh->mVertices[i], 1.0f);
		normal = Vec(mesh->mNormals[i], 0.0f);
		tangent = hasTangents ? Vec(mesh->mTangents[i], 1.0) : Zero;

		texCoord = TexCoord(mesh->mTextureCoords[0][i]);

		vertices.push_back(
			{
				vertex,
				normal,
				texCoord,
				tangent,
			});

		if (!hasTangents)
		{
			LogWarning("Mesh \"" << Utility::Files::FileName(fileLoc) << "\" Does not have Tangents");
		}
	}

	ReleaseMeshData(scene);
	return *this;
}

Asset& MeshData::Reload()
{
	return *this;
}
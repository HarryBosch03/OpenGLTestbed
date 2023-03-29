#include "MeshData.h"

#include "Maths.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "FileIO.h"

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

		vertices.push_back(Point(a));
		vertices.push_back(Point(ab));
		vertices.push_back(Point(b));
		vertices.push_back(Point(bc));
		vertices.push_back(Point(c));
		vertices.push_back(Point(ca));
	   
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

MeshData& MeshData::RecalculateNormals(bool flip)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i].normal = Zero;
	}

	for (int i = 0; i < indices.size(); i += 3)
	{
		int i1 = indices[i];
		int i2 = indices[i + 1];
		int i3 = indices[i + 2];

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

const MeshScene* LoadMeshSceneFromFile(std::string path)
{
	const char* pathC = path.c_str();
	return aiImportFile(pathC, 0);
}

MeshData MeshData::LoadMeshFromFile(std::string path, int subMeshIndex)
{
	if (!DoesFileExist(path)) return {};

	const MeshScene* scene = LoadMeshSceneFromFile(path);

	MeshData data;
	aiMesh* mesh = scene->mMeshes[subMeshIndex];

	int faceCount = mesh->mNumFaces;
	for (int i = 0; i < faceCount; i++)
	{
		for (int j = 0; j < mesh->mFaces[i].mNumIndices - 2; j++)
		{
			AddTri(mesh, data.indices, i, 0, j + 2, j + 1);
		}
	}

	int vertexCount = mesh->mNumVertices;
	for (int i = 0; i < vertexCount; i++)
	{
		data.vertices.push_back(VertexData()
		.SetPosition(mesh->mVertices[i])
		.SetNormal(mesh->mNormals[i])
		.SetTextureCoordinates(mesh->mTextureCoords[0][i]));
	}

	return data;
}

VertexData::VertexData(Vec4 position, Vec4 normal, Vec2 textureCoordiates)
{
	this->position = position;
	this->normal = normal;
	this->textureCoordinates = textureCoordinates;
}

VertexData& VertexData::SetPosition(Vec3 position)
{
	return SetPosition(Point(position));
}

VertexData& VertexData::SetPosition(aiVector3D position)
{
	return SetPosition({ position.x, position.y, position.z, 1.0f });
}

VertexData& VertexData::SetPosition(Vec4 position)
{
	this->position = position;
	return *this;
}

VertexData& VertexData::SetNormal(Vec3 normal)
{
	return SetNormal(Normal(normal));
}

VertexData& VertexData::SetNormal(aiVector3D normal)
{
	return SetNormal({normal.x, normal.y, normal.z, 0.0f});
}

VertexData& VertexData::SetNormal(Vec4 normal)
{
	this->normal = normal;
	return *this;
}

VertexData& VertexData::SetTextureCoordinates(aiVector3D textureCoordinates)
{
	return SetTextureCoordinates(Vec2(textureCoordinates.x, textureCoordinates.y));
}

VertexData& VertexData::SetTextureCoordinates(Vec2 textureCoordinates)
{
	this->textureCoordinates = textureCoordinates;
	return *this;
}
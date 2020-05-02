#include "ObjectLoader.h"
#include "Core/Log.h"
#include "Core/Core.h"

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace OpenGLRendering {

	Mesh::Mesh() { }

	Mesh::~Mesh() { }

	bool Mesh::LoadMesh(const std::string& filePath)
	{
		Clear();

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

		if (scene)
		{
			return InitFromScene(scene, filePath);
		}
		else
		{
			OGL_WARN("Couldn't read scene from file {0}", filePath);
		}

		return false;
	}

	bool Mesh::InitFromScene(const aiScene* scene, const std::string& filePath)
	{
		m_Entries.resize(scene->mNumMeshes);
		m_Textures.resize(scene->mNumMaterials);

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			const aiMesh* mesh = scene->mMeshes[i];
			InitMesh(i, mesh);
		}

		return InitMaterials(scene, filePath);
	}

	void Mesh::InitMesh(uint32_t index, const aiMesh* mesh)
	{
		m_Entries[index].MaterialIndex = mesh->mMaterialIndex;

		std::vector<Assimp::Vertex> vertices;
		std::vector<uint32_t> indices;

		vertices.reserve(mesh->mNumVertices);
		indices.reserve((long long)mesh->mNumFaces * 3);

		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			vertices.push_back(Assimp::Vertex(mesh, i));
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];
			OGL_ASSERT(face.mNumIndices == 3, "Importer tried to parse a non triangular face");

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		m_Entries[index].Init(vertices, indices);
	}

	bool Mesh::InitMaterials(const aiScene* scene, const std::string& filePath)
	{
		return true;
	}

	void Mesh::Clear()
	{

	}

	Mesh::MeshEntry::MeshEntry()
	{

	}

	Mesh::MeshEntry::~MeshEntry()
	{

	}

	bool Mesh::MeshEntry::Init(const std::vector<Assimp::Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		return true;
	}

}
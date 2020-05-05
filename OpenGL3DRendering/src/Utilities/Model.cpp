#include "oglpch.h"

#include "Model.h"

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>

namespace OpenGLRendering {

	TextureType GetTypeFromAIType(aiTextureType type)
	{
		switch (type)
		{
		case aiTextureType_NONE:		break;
		case aiTextureType_DIFFUSE:		return TextureType::DIFFUSE;
		case aiTextureType_SPECULAR:
			break;
		case aiTextureType_AMBIENT:
			break;
		case aiTextureType_EMISSIVE:
			break;
		case aiTextureType_HEIGHT:		break;
		case aiTextureType_NORMALS:		return TextureType::NORMAL;
		case aiTextureType_SHININESS:
			break;
		case aiTextureType_OPACITY:
			break;
		case aiTextureType_DISPLACEMENT:
			break;
		case aiTextureType_LIGHTMAP:
			break;
		case aiTextureType_REFLECTION:
			break;
		case aiTextureType_BASE_COLOR:
			break;
		case aiTextureType_NORMAL_CAMERA:
			break;
		case aiTextureType_EMISSION_COLOR:
			break;
		case aiTextureType_METALNESS:
			break;
		case aiTextureType_DIFFUSE_ROUGHNESS:
			break;
		case aiTextureType_AMBIENT_OCCLUSION:
			break;
		case aiTextureType_UNKNOWN:
			break;
		case _aiTextureType_Force32Bit:
			break;
		}

		OGL_ASSERT(false, "Unknown type");

		return TextureType::DIFFUSE;
	}


	Model::Model(const std::string& filePath) 
	{
		LoadModel(filePath);
	}

	Model::~Model() { }

	void Model::LoadModel(const std::string& filePath)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			OGL_WARN("Couldn't read scene from file {0}", filePath);
			return;
		}

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Ref<Texture2D>> textures;

		vertices.reserve(mesh->mNumVertices);
		indices.reserve((long long)mesh->mNumFaces * 3);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
				
			if (mesh->mTextureCoords[0])
			{
				vertex.TextureCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}
			else
			{
				vertex.TextureCoords = { 0.0f, 0.0f };
			}

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}
			else
			{
				vertex.Tangent = { 0.0f, 0.0f, 0.0f };
				vertex.Bitangent = { 0.0f, 0.0f, 0.0f };
			}

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];
			OGL_ASSERT(face.mNumIndices == 3, "Importer tried to parse a non triangular face");

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Ref<Texture2D>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Ref<Texture2D>> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, scene);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		Ref<Material> mat = CreateRef<Material>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), textures);

		return { vertices, indices, mat };
	}

	std::vector<Ref<Texture2D>> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene)
	{
		std::vector<Ref<Texture2D>> textures;

		for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString str;
			material->GetTexture(type, i, &str);

			const aiTexture* texture = scene->GetEmbeddedTexture(str.C_Str());
			std::string path = std::string(str.C_Str());
			if (texture)
			{
				if (texture->mHeight == 0)
				{
					textures.push_back(CreateRef<Texture2D>(texture->mWidth, (unsigned char*)texture->pcData, path, GetTypeFromAIType(type)));
				}
				else
				{
					//textures.push_back(std::make_shared<Texture2D>(texture->mWidth, texture->mHeight, DataFormat::ARGB, texture->pcData, path, TextureType::DIFFUSE));
				}
			}
		}

		return textures;
	}

	void Model::Render(Shader& shader) const
	{
		for (const Mesh& mesh : m_Meshes)
		{
			mesh.Render(shader);
		}
	}

}
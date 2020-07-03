#include "oglpch.h"

#include "Model.h"

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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


	Model::Model(const std::string& filePath, bool flipUVs)
		: m_Orientation(0.0f, 0.0f, 0.0f, 1.0f)
	{
		LoadModel(filePath, flipUVs);
	}

	Model::~Model() { }

	void Model::LoadModel(const std::string& filePath, bool flipUVs)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filePath.c_str(), flipUVs ? (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_FlipUVs) : (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace));

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

		float xMin = 0.0f, xMax = 0.0f, yMin = 0.0f, yMax = 0.0f, zMin = 0.0f, zMax = 0.0f;

		std::string meshName(mesh->mName.C_Str());

		vertices.reserve(mesh->mNumVertices);
		indices.reserve((long long)mesh->mNumFaces * 3);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			if (mesh->mVertices[i].x > xMax)
			{
				xMax = mesh->mVertices[i].x;
			}
			if (mesh->mVertices[i].x < xMin)
			{
				xMin = mesh->mVertices[i].x;
			}
			if (mesh->mVertices[i].y > yMax)
			{
				yMax = mesh->mVertices[i].y;
			}
			if (mesh->mVertices[i].y < yMin)
			{
				yMin = mesh->mVertices[i].y;
			}
			if (mesh->mVertices[i].z > zMax)
			{
				zMax = mesh->mVertices[i].z;
			}
			if (mesh->mVertices[i].z < zMin)
			{
				zMin = mesh->mVertices[i].z;
			}


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

		aiColor3D color (0.0f, 0.0f, 0.0f);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		glm::vec4 baseColor(color.r, color.g, color.b, 1.0f);

		std::vector<Ref<Texture2D>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Ref<Texture2D>> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, scene);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		Ref<Material> mat = CreateRef<Material>(textures);
		mat->SetAlbedo(baseColor);

		glm::vec3 boundingBoxCenter = { (xMin + xMax) / 2.0f, (yMin + yMax) / 2.0f, (zMin + zMax) / 2.0f };

		return { meshName, vertices, indices, mat, boundingBoxCenter, (uint32_t)vertices.size(), (uint32_t)(indices.size() / 3) };
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
				if (texture->mHeight == 0) // Texture is compressed
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

	void Model::SetTranslation(const glm::vec3& translation)
	{
		m_Translation = translation;
	}


	// Doesn't really set the rotation. This function increments the rotation by the specified amount in euler angles.
	// This helps prevent singularity (gimbal lock), because the orientation of the object is stored as a quaternion that is updated with the new rotation.
	// By this approach the orientation is only updated in small steps, which results in a valid quaternion without singularity.
	void Model::SetRotation(const glm::vec3& rotation)
	{
		m_Orientation = glm::quat(rotation) * m_Orientation;
		glm::normalize(m_Orientation); // Normalize quaternion to ensure that it's properly formatted. Errors can occur by rounding floating point numbers.
	}

	void Model::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	}

	void Model::CalculateModelMatrix()
	{
		glm::mat4 m_TranslationMatrix = glm::translate(glm::mat4(1.0f), m_Translation);
		glm::mat4 m_RotationMatrix = glm::toMat4(m_Orientation); // Parse quaternion to 4x4 matrix for building the model matrix.
		glm::mat4 m_ScaleMatrix = glm::scale(glm::mat4(1.0f), m_Scale);

		m_ModelMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
	}

}
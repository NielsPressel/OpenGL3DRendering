#pragma once

#include <string>
#include <vector>

#include <assimp/scene.h>

#include "Core/Core.h"
#include "Mesh.h"
#include "Renderer/Shader.h"

namespace OpenGLRendering {

	class Model
	{
	public:
		Model(const std::string& filePath, bool flipUVs);
		~Model();
		
		void Render(Shader& shader) const;
		const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }

	private:
		void LoadModel(const std::string& filePath, bool flipUVs);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene);


	private:
		std::vector<Mesh> m_Meshes;
	};

}
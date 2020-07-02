#pragma once

#include <string>
#include <vector>

#include <assimp/scene.h>

#include "Core/Core.h"
#include "Mesh.h"
#include "Renderer/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace OpenGLRendering {

	class Model
	{
	public:
		Model(const std::string& filePath, bool flipUVs);
		~Model();
		
		void Render(Shader& shader) const;
		void RenderLoD(Shader& shader, uint32_t level, uint32_t meshesPerLoD) const;

		const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
		std::vector<Mesh>& GetMeshes() { return m_Meshes; }
		const glm::mat4& GetModelMatrix() const { return m_ModelMatrix; }

		void SetTranslation(const glm::vec3& translation);
		void SetRotation(const glm::vec3& rotation);
		void SetScale(const glm::vec3& scale);
		void CalculateModelMatrix();

	private:
		void LoadModel(const std::string& filePath, bool flipUVs);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene);



	private:
		std::vector<Mesh> m_Meshes;

		glm::mat4 m_ModelMatrix;

		glm::quat m_Orientation;
		glm::vec3 m_Scale;
		glm::vec3 m_Translation;
	};

}
#pragma once

#include <string>
#include <vector>

#include <assimp/scene.h>

#include "Core/Core.h"
#include "Mesh.h"
#include "Renderer/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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
		const glm::vec3& GetTranslation() const { return m_Translation; }
		const glm::vec3& GetRotation() const { return m_Rotation; }
		const glm::vec3& GetScale() const { return m_Scale; }
		const glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }

		glm::vec3& GetTranslation() { return m_Translation; }
		glm::vec3& GetRotation() { return m_Rotation; }
		glm::vec3& GetScale() { return m_Scale; }

		void SetTranslation(const glm::vec3& translation, bool recalculate = true) { m_Translation = translation; if (recalculate) RecalculateModelMatrix(); }
		void SetRotation(const glm::vec3& rotation, bool recalculate = true);
		void SetScale(const glm::vec3& scale, bool recalculate = true) { m_Scale = scale; if (recalculate) RecalculateModelMatrix(); }
		void RecalculateModelMatrix();

	private:
		void LoadModel(const std::string& filePath, bool flipUVs);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene);



	private:
		std::vector<Mesh> m_Meshes;
		glm::vec3 m_Translation;
		glm::vec3 m_Scale;
		glm::vec3 m_Rotation;

		glm::mat4 m_ModelMatrix;
	};

}
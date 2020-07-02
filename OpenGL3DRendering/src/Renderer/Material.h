#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Texture.h"

// Simple material class that stores a base class in case no textures are provided and several textures otherwise

namespace OpenGLRendering {

	class Material
	{
	public:
		Material();
		Material(const std::vector<Ref<Texture2D>>& textures);

		void AddTexture(const Ref<Texture2D>& texture) { m_Textures.push_back(texture); }
		void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
		void SetRoughness(float roughness) { m_Roughness = roughness; }
		void SetMetallic(float metallic) { m_Metallic = metallic; }
		void SetAmbientOcclusion(float ao) { m_AmbientOcclusion = ao; }
		
		const std::vector<Ref<Texture2D>>& GetTextures() const { return m_Textures; }
		const glm::vec3& GetAlbedo() const { return m_Albedo; }

		glm::vec3& GetBaseColor() { return m_Albedo; }
		float GetRoughness() { return m_Roughness; }
		float GetMetallic() { return m_Metallic; }
		float GetAmbientOcclusion() { return m_AmbientOcclusion; }

	private:
		std::vector<Ref<Texture2D>> m_Textures;
		
		glm::vec3 m_Albedo;
		float m_Roughness;
		float m_Metallic;
		float m_AmbientOcclusion;
	};

}
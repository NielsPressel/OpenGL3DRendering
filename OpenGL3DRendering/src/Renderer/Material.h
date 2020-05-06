#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Texture.h"

// Simple material class that stores a base class in case no textures are provided and several textures otherwise

namespace OpenGLRendering {

	class Material
	{
	public:
		Material(const glm::vec4& baseColor);
		Material(const glm::vec4& baseColor, const std::vector<Ref<Texture2D>>& textures);

		void AddTexture(const Ref<Texture2D>& texture) { m_Textures.push_back(texture); }
		void SetBaseColor(const glm::vec4& baseColor) { m_BaseColor = baseColor; }
		
		const std::vector<Ref<Texture2D>>& GetTextures() const { return m_Textures; }
		const glm::vec4& GetBaseColor() const { return m_BaseColor; }

		glm::vec4& GetBaseColor() { return m_BaseColor; }

	private:
		std::vector<Ref<Texture2D>> m_Textures;
		glm::vec4 m_BaseColor;
	};

}
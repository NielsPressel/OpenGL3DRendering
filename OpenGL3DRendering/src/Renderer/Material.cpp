#include "oglpch.h"

#include "Material.h"

namespace OpenGLRendering {

	Material::Material(const glm::vec4& baseColor)
		: m_BaseColor(baseColor) { }

	Material::Material(const glm::vec4& baseColor, const std::vector<Ref<Texture2D>>& textures)
		: m_BaseColor(baseColor), m_Textures(textures) { }


}
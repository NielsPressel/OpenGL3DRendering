#include "oglpch.h"

#include "Material.h"

namespace OpenGLRendering {

	Material::Material()
		: m_Metallic(0.0f), m_Roughness(0.0f), m_AmbientOcclusion(0.0f) { }

	Material::Material(const std::vector<Ref<Texture2D>>& textures)
		: m_Metallic(0.0f), m_Roughness(0.0f), m_AmbientOcclusion(0.0f), m_Textures(textures) { }


}
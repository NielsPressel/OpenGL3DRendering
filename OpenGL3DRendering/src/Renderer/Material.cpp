#include "oglpch.h"

#include "Material.h"

namespace OpenGLRendering {

	Material::Material()
		: m_Albedo({ 0.0f, 0.0f, 0.0f }), m_Metallic(0.0f), m_Roughness(0.0f), m_AmbientOcclusion(0.0f), m_UseTextures(false) { }

	Material::Material(const std::unordered_map<TextureType, Ref<Texture2D>>& textures)
		: m_Albedo({ 0.0f, 0.0f, 0.0f }), m_Metallic(0.0f), m_Roughness(0.0f), m_AmbientOcclusion(0.0f), m_Textures(textures), m_UseTextures(true) { }


}
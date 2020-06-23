#pragma once

#include <string>
#include <glm/glm.hpp>

namespace OpenGLRendering {

	enum TextureType : uint16_t
	{
		DIFFUSE = 0, ALBEDO, NORMAL, METALLIC, ROUGHNESS, AMBIENT_OCCLUSION, METALLIC_SMOOTHNESS
	};

	// Texture wrapper class (supports loading from file and memory)
	class Texture2D
	{
	public:
		Texture2D(const std::string& filePath, TextureType type);
		Texture2D(uint32_t size, unsigned char* data, const std::string& path, TextureType type);
		~Texture2D();

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetRendererID() const { return m_RendererID; }
		const std::string& GetPath() const { return m_Path; }
		TextureType GetType() const { return m_Type; }

		void SetData(void* data, uint32_t size);

		void Bind(uint32_t slot = 0) const;

		bool operator==(const Texture2D& other) const
		{
			return m_RendererID == other.m_RendererID;
		}
	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		std::string m_Path;
		uint32_t m_InternalFormat, m_DataFormat;
		TextureType m_Type;
	};

}
#include "oglpch.h"

#include "Cubemap.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace OpenGLRendering {

	Cubemap::Cubemap(const std::string& filepath)
	{
		Initialize(filepath);
	}

	Cubemap::~Cubemap()
	{

	}

	void Cubemap::Render() const
	{

	}

	void Cubemap::Initialize(const std::string& filepath)
	{
		int width, height, channels;
		float* data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_CubemapTextureId);
			glBindTexture(GL_TEXTURE_2D, m_CubemapTextureId);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			OGL_WARN("Couldn't load cubemap texture from {0}!", filepath);
		}


	}

}
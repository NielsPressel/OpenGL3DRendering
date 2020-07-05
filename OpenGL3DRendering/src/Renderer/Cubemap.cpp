#include "oglpch.h"

#include "Cubemap.h"
#include "Renderer/RendererAPI.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>


// Static cube geometry, matrices and settings
static const glm::mat4 s_Views[] =
{
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
};
static const glm::mat4 s_Projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

static const uint32_t s_FramebufferWidth = 512;
static const uint32_t s_FramebufferHeight = 512;

static float s_CubeVertexBuffer[]
{
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
};

static uint32_t s_CubeIndexBuffer[] =
{
	0, 1, 3, 
	3, 1, 2,
	1, 5, 2,
	2, 5, 6,
	5, 4, 6,
	6, 4, 7,
	4, 0, 7,
	7, 0, 3,
	3, 2, 7,
	7, 2, 6,
	4, 5, 0,
	0, 5, 1,
};

namespace OpenGLRendering {

	Cubemap::Cubemap(const std::string& filepath)
	{
		Initialize(filepath);
	}

	Cubemap::~Cubemap()
	{
		glDeleteTextures(1, &m_CubemapTextureId);
		glDeleteTextures(1, &m_EnvironmentMapId);
		glDeleteTextures(1, &m_IrradianceMapId);
		glDeleteTextures(1, &m_PrefilterMapId);
		glDeleteTextures(1, &m_BrdfLutTexture);

		glDeleteRenderbuffers(1, &m_RenderbufferAttachmentId);
		glDeleteFramebuffers(1, &m_FramebufferId);
	}

	void Cubemap::BindEnvironmentMap(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvironmentMapId);
	}

	void Cubemap::BindIrradianceMap(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMapId);
	}

	void Cubemap::BindPrefilterMap(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMapId);
	}

	void Cubemap::BindBrdfLutTexture(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_BrdfLutTexture);
	}

	void Cubemap::Initialize(const std::string& filepath)
	{
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glDepthFunc(GL_LEQUAL);

		// Load cube geometry
		Ref<VertexBuffer> vb = CreateRef<VertexBuffer>(s_CubeVertexBuffer, 4 * 8 * 3); // float is 4 bytes; cube consists of 8 vertices with 3 floats each
		vb->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
			});

		Ref<IndexBuffer> ib = CreateRef<IndexBuffer>(s_CubeIndexBuffer, 12 * 3); // 12 triangles with 3 vertices each

		m_VertexArray = CreateRef<VertexArray>();
		m_VertexArray->Bind();
		m_VertexArray->AddVertexBuffer(vb);
		m_VertexArray->SetIndexBuffer(ib);

		// Load image data
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		float* data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 0);

		OGL_ASSERT(data, "Couldn't load cubemap texture from {0}!", filepath);
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_CubemapTextureId);
		glBindTexture(GL_TEXTURE_2D, m_CubemapTextureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
			
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		// Environment map
		{
			// Compile and link conversion shader
			Shader conversionShader("src/Resources/ShaderSource/Conversion/conversion_vertex.glsl", "src/Resources/ShaderSource/Conversion/equirectengular_conversion_fragment.glsl");

			conversionShader.Bind();
			conversionShader.SetInt("u_EquirectengularMap", 0);
			conversionShader.SetMat4("u_Projection", s_Projection);

			// Setup framebuffer for capturing the cube map
			glGenFramebuffers(1, &m_FramebufferId);
			glGenRenderbuffers(1, &m_RenderbufferAttachmentId);

			glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferAttachmentId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, s_FramebufferWidth, s_FramebufferHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderbufferAttachmentId);

			glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_EnvironmentMapId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvironmentMapId);

			for (unsigned int i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, s_FramebufferWidth, s_FramebufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// Render cube map to framebuffer
			glViewport(0, 0, s_FramebufferWidth, s_FramebufferHeight);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_CubemapTextureId);

			for (unsigned int i = 0; i < 6; i++)
			{
				conversionShader.SetMat4("u_View", s_Views[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvironmentMapId, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				RendererAPI::DrawIndexed(m_VertexArray, 0);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// Irradiance map
		{		
			// Compile and link irradiance conversion shader
			Shader irradianceConversionShader("src/Resources/ShaderSource/Conversion/conversion_vertex.glsl", "src/Resources/ShaderSource/Conversion/irradiance_conversion_fragment.glsl");

			// Create irradiance texture
			glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_IrradianceMapId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMapId);

			for (unsigned int i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferAttachmentId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

			irradianceConversionShader.Bind();
			irradianceConversionShader.SetInt("u_EnvironmentMap", 0);
			irradianceConversionShader.SetMat4("u_Projection", s_Projection);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvironmentMapId);

			glViewport(0, 0, 32, 32);

			// Render irradiance map to framebuffer / texture
			for (unsigned int i = 0; i < 6; i++)
			{
				irradianceConversionShader.SetMat4("u_View", s_Views[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMapId, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				RendererAPI::DrawIndexed(m_VertexArray, 0);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// Prefilter map
		{
			// Compile and link prefilter shader
			Shader prefilterShader("src/Resources/ShaderSource/Conversion/cubemap_vertex.glsl", "src/Resources/ShaderSource/Conversion/prefilter_fragment.glsl");

			// Generate prefilter map
			glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_PrefilterMapId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMapId);

			for (unsigned int i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			prefilterShader.Bind();
			prefilterShader.SetInt("u_EnvironmentMap", 0);
			prefilterShader.SetMat4("u_Projection", s_Projection);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvironmentMapId);

			glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
			unsigned int maxMipLevels = 5;
			for (unsigned int mip = 0; mip < maxMipLevels; mip++)
			{
				unsigned int mipWidth = 128 * pow(0.5, mip);
				unsigned int mipHeight = 128 * pow(0.5, mip);

				glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferAttachmentId);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
				glViewport(0, 0, mipWidth, mipHeight);

				float roughness = (float)mip / float(maxMipLevels - 1);
				prefilterShader.SetFloat("u_Roughness", roughness);

				for (unsigned int i = 0; i < 6; i++)
				{
					prefilterShader.SetMat4("u_View", s_Views[i]);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMapId, mip);

					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					RendererAPI::DrawIndexed(m_VertexArray, 0);
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// BRDF LUT texture
		{
			Shader brdfShader("src/Resources/ShaderSource/Conversion/brdf_vertex.glsl", "src/Resources/ShaderSource/Conversion/brdf_fragment.glsl");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_BrdfLutTexture);
			glBindTexture(GL_TEXTURE_2D, m_BrdfLutTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferAttachmentId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BrdfLutTexture, 0);

			glViewport(0, 0, 512, 512);
			brdfShader.Bind();

			float quadVertices[] =
			{
				-1.0f, -1.0f, 0.0f, 0.0f,
				 1.0f, -1.0f, 1.0f, 0.0f,
				 1.0f,  1.0f, 1.0f, 1.0f,
				-1.0f,  1.0f, 0.0f, 1.0f,
			};

			uint32_t quadIndices[] =
			{
				0, 1, 2,
				0, 2, 3,
			};

			Ref<VertexBuffer> vb = CreateRef<VertexBuffer>(quadVertices, 4 * 4 * 4);
			vb->SetLayout(
			{
				{ ShaderDataType::Float2, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoords" },
			});

			Ref<IndexBuffer> ib = CreateRef<IndexBuffer>(quadIndices, 2 * 3);

			Ref<VertexArray> va = CreateRef<VertexArray>();
			va->AddVertexBuffer(vb);
			va->SetIndexBuffer(ib);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RendererAPI::DrawIndexed(va, 0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}
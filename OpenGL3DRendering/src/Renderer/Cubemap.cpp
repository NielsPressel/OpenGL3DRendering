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

		glDeleteRenderbuffers(1, &m_RenderbufferAttachmentId);
		glDeleteFramebuffers(1, &m_FramebufferId);
	}

	void Cubemap::Render(const glm::mat4& projection, const glm::mat4& view) const
	{
		m_CubemapShader->Bind();
		m_CubemapShader->SetMat4("u_Projection", projection);
		m_CubemapShader->SetMat4("u_View", view);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvironmentMapId);
		m_CubemapShader->SetInt("u_EnvironmentMap", 0);

		glDepthFunc(GL_LEQUAL);
		RendererAPI::DrawIndexed(m_VertexArray, 0);
		glDepthFunc(GL_LESS);
	}

	void Cubemap::BindIrradianceMap(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMapId);
	}

	void Cubemap::Initialize(const std::string& filepath)
	{
		// Load image data
		int width, height, channels;
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

		// Compile and link conversion shader
		Shader conversionShader("src/Resources/ShaderSource/conversion_vertex.glsl", "src/Resources/ShaderSource/equirectengular_conversion_fragment.glsl");

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


		// Compile and link irradiance conversion shader
		Shader irradianceConversionShader("src/Resources/ShaderSource/conversion_vertex.glsl", "src/Resources/ShaderSource/irradiance_conversion_fragment.glsl");

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

		// Compile and link cube map shader
		m_CubemapShader = CreateScope<Shader>("src/Resources/ShaderSource/cubemap_vertex.glsl", "src/Resources/ShaderSource/cubemap_fragment.glsl");
	}
}
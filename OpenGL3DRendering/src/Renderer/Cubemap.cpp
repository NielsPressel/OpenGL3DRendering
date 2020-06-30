#include "oglpch.h"

#include "Cubemap.h"
#include "Renderer/RendererAPI.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

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
	0, 1, 5,
	0, 5, 4,
	1, 2, 6,
	1, 6, 5,
	2, 3, 7,
	2, 7, 6,
	3, 0, 4,
	3, 4, 7,
	4, 5, 6,
	4, 6, 7,
	0, 1, 2,
	0, 2, 5,
};

namespace OpenGLRendering {

	Cubemap::Cubemap(const std::string& filepath)
	{
		Initialize(filepath);
	}

	Cubemap::~Cubemap()
	{
		glDeleteTextures(1, &m_CubemapTextureId);
		glDeleteTextures(1, &m_CubemapColorAttachmentId);

		glDeleteRenderbuffers(1, &m_RenderbufferAttachmentId);
		glDeleteFramebuffers(1, &m_FramebufferId);
	}

	void Cubemap::Render() const
	{

	}

	void Cubemap::Initialize(const std::string& filepath)
	{
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

		m_ConversionShader = CreateScope<Shader>("src/Resources/ShaderSource/equirectengular_conversion_vertex.glsl", "src/Resources/ShaderSource/equirectengular_conversion_fragment.glsl");

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_CubemapColorAttachmentId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapColorAttachmentId);

		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, s_FramebufferWidth, s_FramebufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glBindTextureUnit(0, m_CubemapTextureId);

		m_ConversionShader->Bind();
		m_ConversionShader->SetInt("u_EquirectengularMap", 0);
		m_ConversionShader->SetMat4("u_Projection", s_Projection);

		glGenFramebuffers(1, &m_FramebufferId);
		glGenRenderbuffers(1, &m_RenderbufferAttachmentId);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferAttachmentId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderbufferAttachmentId);

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

		glViewport(0, 0, s_FramebufferWidth, s_FramebufferHeight);

		for (unsigned int i = 0; i < 6; i++)
		{
			m_ConversionShader->SetMat4("u_View", s_Views[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_CubemapColorAttachmentId, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			RendererAPI::DrawIndexed(m_VertexArray, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
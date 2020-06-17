#include "oglpch.h"

#include "Framebuffer.h"
#include "RendererAPI.h"

#include <glad/glad.h>

namespace OpenGLRendering {

	Framebuffer::Framebuffer(const FramebufferSettings& settings)
		: m_Settings(settings), m_RendererId(0)
	{
		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteTextures(1, &m_ColorTextureId);
		glDeleteTextures(1, &m_DepthTextureId);
		glDeleteFramebuffers(1, &m_RendererId);
	}

	void Framebuffer::Invalidate()
	{
		if (m_RendererId != 0)
		{
			glDeleteTextures(1, &m_ColorTextureId);
			glDeleteTextures(1, &m_DepthTextureId);
			glDeleteFramebuffers(1, &m_RendererId);
		}

		glCreateFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorTextureId);
		glBindTexture(GL_TEXTURE_2D, m_ColorTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Settings.Width, m_Settings.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureId, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthTextureId);
		glBindTexture(GL_TEXTURE_2D, m_DepthTextureId);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Settings.Width, m_Settings.Height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureId, 0);

		OGL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Settings.Width = width;
		m_Settings.Height = height;

		Invalidate();
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		glViewport(0, 0, m_Settings.Width, m_Settings.Height);
	}

	void Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
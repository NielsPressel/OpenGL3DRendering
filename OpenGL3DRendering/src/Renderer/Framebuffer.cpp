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

		if (m_RenderbufferId)
			glDeleteRenderbuffers(1, &m_RenderbufferId);

		glDeleteFramebuffers(1, &m_RendererId);
	}

	void Framebuffer::Invalidate()
	{
		if (m_RendererId)
		{
			glDeleteTextures(1, &m_ColorTextureId);

			if (m_RenderbufferId)
				glDeleteRenderbuffers(1, &m_RenderbufferId);
			
			glDeleteFramebuffers(1, &m_RendererId);
		}

		glCreateFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		if (m_Settings.EnableMultisampling)
		{
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_ColorTextureId);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorTextureId);

			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Settings.NumberSamples, GL_RGBA8, m_Settings.Width, m_Settings.Height, GL_TRUE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorTextureId, 0);

			if (m_Settings.EnableDepthStencil)
			{
				glCreateRenderbuffers(1, &m_RenderbufferId);
				glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferId);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Settings.NumberSamples, GL_DEPTH24_STENCIL8, m_Settings.Width, m_Settings.Height);

				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderbufferId);
			}
		}
		else
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorTextureId);
			glBindTexture(GL_TEXTURE_2D, m_ColorTextureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Settings.Width, m_Settings.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureId, 0);

			if (m_Settings.EnableDepthStencil)
			{
				glCreateRenderbuffers(1, &m_RenderbufferId);
				glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferId);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Settings.Width, m_Settings.Height);

				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderbufferId);
			}
		}

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

	void Framebuffer::BindForRead() const
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererId);
	}

	void Framebuffer::BindForWrite() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererId);
	}
}
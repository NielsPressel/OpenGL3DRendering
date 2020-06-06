#include "oglpch.h"

#include "Framebuffer.h"

#include <glad/glad.h>

namespace OpenGLRendering {

	Framebuffer::Framebuffer(const glm::vec2& size)
		: m_Size(size)
	{
		glGenFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		m_RenderTexture = CreateRef<Texture2D>(size, TextureType::DIFFUSE);

		glGenRenderbuffers(1, &m_DepthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_RenderTexture->GetRendererID(), 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferId);

		GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, buffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			OGL_ASSERT(false, "Failed to create framebuffer");
		}
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteRenderbuffers(1, &m_DepthBufferId);
		glDeleteFramebuffers(1, &m_RendererId);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
	}

	void Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
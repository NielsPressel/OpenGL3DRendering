#pragma once

#include "Texture.h"
#include "glm/glm.hpp"

// Framebuffer wrapper class (OpenGL abstraction)

namespace OpenGLRendering {

	struct FramebufferSettings
	{
		uint32_t Width, Height;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSettings& settings);
		~Framebuffer();

		void Invalidate();
		void Resize(uint32_t width, uint32_t height);
		void Bind() const;
		void Unbind() const;

		void SetSettings(const FramebufferSettings& settings) { m_Settings = settings; }
		uint32_t GetColorTextureId() const { return m_ColorTextureId; }
		const FramebufferSettings& GetSettings() const { return m_Settings; }

	private:
		uint32_t m_RendererId;
		uint32_t m_ColorTextureId;
		uint32_t m_DepthTextureId;
		FramebufferSettings m_Settings;
	};

}
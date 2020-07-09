#pragma once

#include "Texture.h"
#include "glm/glm.hpp"

// Framebuffer wrapper class (OpenGL abstraction)

namespace OpenGLRendering {

	struct FramebufferSettings
	{
		uint32_t Width, Height;

		bool EnableDepthStencil = true;
		bool EnableMultisampling = false;
		uint8_t NumberSamples = 0;
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

		void BindForRead() const;
		void BindForWrite() const;
		void BindColorTexture(uint32_t slot) const;

		void SetSettings(const FramebufferSettings& settings) { m_Settings = settings; }
		uint32_t GetColorTextureId() const { return m_ColorTextureId; }
		const FramebufferSettings& GetSettings() const { return m_Settings; }

	private:
		uint32_t m_RendererId;
		uint32_t m_ColorTextureId;
		uint32_t m_RenderbufferId;

		FramebufferSettings m_Settings;
	};

}
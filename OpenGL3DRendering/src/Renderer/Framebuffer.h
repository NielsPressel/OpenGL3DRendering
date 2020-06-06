#pragma once

#include "Texture.h"
#include "glm/glm.hpp"

namespace OpenGLRendering {

	class Framebuffer
	{
	public:
		Framebuffer(const glm::vec2& size);
		~Framebuffer();

		void Bind() const;
		void Unbind() const;

		const Ref<Texture2D>& GetRenderTexture() const { return m_RenderTexture; }
		const glm::vec2& GetSize() const { return m_Size; }

	private:
		uint32_t m_RendererId;
		uint32_t m_DepthBufferId;
		glm::vec2 m_Size;
		Ref<Texture2D> m_RenderTexture;
	};

}
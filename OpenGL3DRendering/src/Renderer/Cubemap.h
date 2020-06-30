#pragma once

#include <string>
#include <memory>

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace OpenGLRendering {

	class Cubemap
	{
	public:
		Cubemap(const std::string& filepath);
		~Cubemap();

		void Render() const;

	private:
		void Initialize(const std::string& filepath);

	private:
		uint32_t m_CubemapTextureId;
		uint32_t m_CubemapColorAttachmentId;
		uint32_t m_RenderbufferAttachmentId;
		uint32_t m_FramebufferId;
		
		Scope<Shader> m_ConversionShader;
		Ref<VertexArray> m_VertexArray;
	};

}
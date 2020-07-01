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

		void Render(const glm::mat4& projection, const glm::mat4& view) const;

	private:
		void Initialize(const std::string& filepath);

	private:
		uint32_t m_CubemapTextureId;
		uint32_t m_EnvironmentMapId;
		uint32_t m_IrradianceMapId;
		uint32_t m_RenderbufferAttachmentId;
		uint32_t m_FramebufferId;

		Ref<VertexArray> m_VertexArray;
		Scope<Shader> m_CubemapShader;
	};

}
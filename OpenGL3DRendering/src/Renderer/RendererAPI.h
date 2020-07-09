#pragma once

#include "VertexArray.h"
#include "Framebuffer.h"

#include <memory>
#include <glm/glm.hpp>

namespace OpenGLRendering {

	// Abstraction for simple OpenGL calls and settings
	class RendererAPI
	{
	public:
		static void Init();
		static void SetDepthTesting(bool enable);
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount);
		static void BlitFramebuffer(const Ref<Framebuffer>& src, const Ref<Framebuffer>& dest);
	};

}
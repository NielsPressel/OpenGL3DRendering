#pragma once

#include "VertexArray.h"

#include <memory>
#include <glm/glm.hpp>

namespace OpenGLRendering {

	class RendererAPI
	{
	public:
		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount);
	};

}
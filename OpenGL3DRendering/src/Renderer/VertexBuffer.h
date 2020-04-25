#pragma once
#include <stdint.h>

namespace OpenGLRendering {

	class VertexBuffer
	{
	public:
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_RendererID;
	};

}

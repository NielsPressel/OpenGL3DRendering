#pragma once
#include <stdint.h>

// IndexBuffer wrapper class (OpenGL abstraction)

namespace OpenGLRendering {

	class IndexBuffer
	{
	public:
		IndexBuffer(const uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetIndexCount() const { return m_IndexCount; }

	private:
		uint32_t m_RendererID;
		uint32_t m_IndexCount;
	};

}

#pragma once
#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace OpenGLRendering {

	class Object
	{
	public:
		Object()
			: m_VertexBufferCount(0), m_IndexBufferCount(0) {}

		float* GetVertexBuffer(uint32_t* countRef) { *countRef = m_VertexBufferCount; return m_VertexBuffer.get(); }
		uint32_t* GetIndexBuffer(uint32_t* countRef) { *countRef = m_IndexBufferCount; return m_IndexBuffer.get(); }

		const float* GetVertexBuffer(uint32_t* countRef) const { *countRef = m_VertexBufferCount; return m_VertexBuffer.get(); }
		const uint32_t* GetIndexBuffer(uint32_t* countRef) const { *countRef = m_IndexBufferCount; return m_IndexBuffer.get(); }

		void SetVertexBuffer(float* vertexBuffer, uint32_t count) { m_VertexBuffer = std::unique_ptr<float>(vertexBuffer); m_VertexBufferCount = count; }
		void SetIndexBuffer(uint32_t* indexBuffer, uint32_t count) { m_IndexBuffer = std::unique_ptr<uint32_t>(indexBuffer); m_IndexBufferCount = count; }

	private:
		std::unique_ptr<float> m_VertexBuffer;
		uint32_t m_VertexBufferCount;

		std::unique_ptr<uint32_t> m_IndexBuffer;
		uint32_t m_IndexBufferCount;
	};

}
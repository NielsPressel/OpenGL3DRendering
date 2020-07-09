#include "oglpch.h"

#include "Renderer2D.h"
#include "RendererAPI.h"

#include "Shader.h"

namespace OpenGLRendering {

	struct QuadrantVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxVertices = MaxQuads * 4;

		QuadrantVertex* QuadVertexBufferStart = nullptr;
		QuadrantVertex* QuadVertexBufferPtr = nullptr;

		Ref<VertexArray> VertexArray;
	};

	static Renderer2DData s_RendererData;

	void Renderer2D::Init()
	{
		s_RendererData.VertexArray = CreateRef<VertexArray>();

		Ref<VertexBuffer> vb = CreateRef<VertexBuffer>(s_RendererData.MaxVertices * sizeof(QuadrantVertex));
		vb->SetLayout(
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoords" },
			{ ShaderDataType::Float, "a_TexIndex" },
		});

		uint32_t* indices = new uint32_t[s_RendererData.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_RendererData.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 0;
			indices[i + 4] = offset + 2;
			indices[i + 5] = offset + 3;
		}

		Ref<IndexBuffer> ib = CreateRef<IndexBuffer>(indices, s_RendererData.MaxIndices);

		s_RendererData.VertexArray->AddVertexBuffer(vb);
		s_RendererData.VertexArray->SetIndexBuffer(ib);

		delete[] indices;

		s_RendererData.QuadVertexBufferStart = new QuadrantVertex[s_RendererData.MaxVertices];
	}

}

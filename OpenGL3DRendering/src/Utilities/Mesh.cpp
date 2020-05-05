#include "oglpch.h"

#include "Mesh.h"
#include "Renderer/RendererAPI.h"

namespace OpenGLRendering {

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material)
	{
		Init(vertices, indices, material);
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material)
	{
		m_Material = material;

		m_VertexArray = CreateRef<VertexArray>();

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>((float*)&(vertices[0]), vertices.size() * sizeof(Vertex));
		vertexBuffer->SetLayout(
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal"},
			{ ShaderDataType::Float2, "a_TextureCoords"},
			{ ShaderDataType::Float3, "a_Tangent"},
			{ ShaderDataType::Float3, "a_Bitangent"}
		});

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(&indices[0], indices.size());

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void Mesh::Render(Shader& shader) const
	{
		shader.Bind();
		m_VertexArray->Bind();

		for (const Ref<Texture2D>& texture : m_Material->GetTextures())
		{
			if (texture->GetType() == TextureType::DIFFUSE)
			{
				texture->Bind(0);
				shader.SetInt("u_TextureDiffuse", 0);
				break;
			}
			else if (texture->GetType() == TextureType::NORMAL)
			{
				texture->Bind(1);
				shader.SetInt("u_TextureNormal", 1);
			}
		}

		RendererAPI::DrawIndexed(m_VertexArray, 0);
	}

}
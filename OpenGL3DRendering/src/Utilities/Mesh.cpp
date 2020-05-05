#include "Mesh.h"

#include "Renderer/RendererAPI.h"

namespace OpenGLRendering {

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<std::shared_ptr<Texture2D>>& textures)
	{
		Init(vertices, indices, textures);
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<std::shared_ptr<Texture2D>>& textures)
	{
		m_Textures = textures;

		m_VertexArray = std::make_shared<VertexArray>();

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

		for (const std::shared_ptr<Texture2D>& texture : m_Textures)
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
#include "oglpch.h"

#include "Mesh.h"
#include "Renderer/RendererAPI.h"

namespace OpenGLRendering {

	Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material, const glm::vec3& boundingBoxCenter)
		: m_Name(name), m_BoundingBoxCenter(boundingBoxCenter), m_Render(true)
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
		if (!m_Render)
		{
			return;
		}

		shader.Bind();
		m_VertexArray->Bind();

		shader.SetFloat4("u_Color", m_Material->GetBaseColor());

		for (const Ref<Texture2D>& texture : m_Material->GetTextures())
		{
			if (texture->GetType() == TextureType::DIFFUSE)
			{
				texture->Bind(0);
				shader.SetInt("u_TextureDiffuse", 0);
			}
			else if (texture->GetType() == TextureType::NORMAL)
			{
				texture->Bind(1);
				shader.SetInt("u_TextureNormal", 1);
			}
			else if (texture->GetType() == TextureType::ALBEDO)
			{
				texture->Bind(0);
				shader.SetInt("u_TextureAlbedo", 0);
			}
			else if (texture->GetType() == TextureType::METALLIC_SMOOTHNESS)
			{
				texture->Bind(2);
				shader.SetInt("u_TextureMetallicSmooth", 2);
			}
			else if (texture->GetType() == TextureType::AMBIENT_OCCLUSION)
			{
				texture->Bind(3);
				shader.SetInt("u_TextureAmbient", 3);
			}
		}

		RendererAPI::DrawIndexed(m_VertexArray, 0);
	}

}
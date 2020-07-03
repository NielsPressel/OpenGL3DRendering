#include "oglpch.h"

#include "Mesh.h"
#include "Renderer/RendererAPI.h"

namespace OpenGLRendering {

	Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material, const glm::vec3& boundingBoxCenter, uint32_t vertexCount, uint32_t faceCount)
		: m_Name(name), m_BoundingBoxCenter(boundingBoxCenter), m_Render(true), m_VertexCount(vertexCount), m_FaceCount(faceCount)
	{
		Init(vertices, indices, material);
	}

	Mesh::Mesh(const std::string& name, const std::vector<SimpleVertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material, uint32_t vertexCount, uint32_t faceCount)
		: m_Name(name), m_Render(true), m_VertexCount(vertexCount), m_FaceCount(faceCount), m_Material(material)
	{
		m_VertexArray = CreateRef<VertexArray>();
		Ref<VertexBuffer> vertexBuffer = CreateRef<VertexBuffer>((float*)&(vertices[0]), vertices.size() * sizeof(SimpleVertex));
		vertexBuffer->SetLayout(
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
		});

		Ref<IndexBuffer> indexBuffer = CreateRef<IndexBuffer>(&indices[0], indices.size());

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material)
	{
		m_Material = material;

		m_VertexArray = CreateRef<VertexArray>();

		Ref<VertexBuffer> vertexBuffer = CreateRef<VertexBuffer>((float*)&(vertices[0]), vertices.size() * sizeof(Vertex));
		vertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float2, "a_TextureCoords" },
				{ ShaderDataType::Float3, "a_Tangent" },
				{ ShaderDataType::Float3, "a_Bitangent" }
			});

		Ref<IndexBuffer> indexBuffer = CreateRef<IndexBuffer>(&indices[0], indices.size());

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

}
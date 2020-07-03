#pragma once

#include <glm/glm.hpp>

#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"

#include "Core/Core.h"

namespace OpenGLRendering {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct SimpleVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
	};

	class Mesh
	{
	public:
		Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material, const glm::vec3& boundingBoxCenter, uint32_t vertexCount = 0, uint32_t faceCount = 0);
		Mesh(const std::string& name, const std::vector<SimpleVertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material, uint32_t vertexCount = 0, uint32_t faceCoount = 0);
		~Mesh();

		const std::string& GetName() const { return m_Name; }
		const Ref<Material>& GetMaterial() const { return m_Material; }
		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const glm::vec3& GetBoundingBoxCenter() const { return m_BoundingBoxCenter; }

		bool& IsRendering() { return m_Render; }

		uint32_t GetVertexCount() const { return m_VertexCount; }
		uint32_t GetFaceCount() const { return m_FaceCount; }

		Ref<Material>& GetMaterial() { return m_Material; }

	private:
		void Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material);
	private:
		std::string m_Name;
		Ref<VertexArray> m_VertexArray;
		Ref<Material> m_Material;
		glm::vec3 m_BoundingBoxCenter;
		bool m_Render;

		uint32_t m_VertexCount;
		uint32_t m_FaceCount;
	};

}

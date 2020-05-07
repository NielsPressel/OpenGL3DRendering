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

	class Mesh
	{
	public:
		Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material, const glm::vec3& boundingBoxCenter);
		~Mesh();

		void Render(Shader& shader) const;
		const std::string& GetName() const { return m_Name; }
		const Ref<Material>& GetMaterial() const { return m_Material; }
		const glm::vec3& GetBoundingBoxCenter() const { return m_BoundingBoxCenter; }

		Ref<Material>& GetMaterial() { return m_Material; }

	private:
		void Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material);
	private:
		std::string m_Name;
		Ref<VertexArray> m_VertexArray;
		Ref<Material> m_Material;
		glm::vec3 m_BoundingBoxCenter;
	};

}

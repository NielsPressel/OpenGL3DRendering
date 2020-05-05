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
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material);
		~Mesh();

		void Render(Shader& shader) const;

	private:
		void Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Ref<Material>& material);
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<Material> m_Material;
	};

}

#pragma once

#include <glm/glm.hpp>

#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

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
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Ref<Texture2D>>& textures);
		~Mesh();

		void Render(Shader& shader) const;

	private:
		void Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Ref<Texture2D>>& textures);
	private:
		std::shared_ptr<VertexArray> m_VertexArray;
		std::vector<Ref<Texture2D>> m_Textures;
	};

}

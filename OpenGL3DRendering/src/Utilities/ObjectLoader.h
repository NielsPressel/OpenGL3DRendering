#pragma once

#include <string>
#include <vector>

#include <assimp/Vertex.h>
#include <assimp/scene.h>

namespace OpenGLRendering {

	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		bool LoadMesh(const std::string& filePath);

	private:
		bool InitFromScene(const aiScene* scene, const std::string& filePath);
		void InitMesh(uint32_t index, const aiMesh* mesh);
		bool InitMaterials(const aiScene* scene, const std::string& filePath);

		void Clear();

		struct MeshEntry
		{
			MeshEntry();
			~MeshEntry();

			bool Init(const std::vector<Assimp::Vertex>& vertices, const std::vector<uint32_t>& indices);

			uint32_t NumIndices;
			uint32_t MaterialIndex;
		};

	private:
		std::vector<MeshEntry> m_Entries;
		std::vector<uint32_t> m_Textures;
	};

}
#include "ObjectLoader.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace OpenGLRendering {

	typedef glm::vec<3, uint32_t, glm::defaultp> vec3int;

	class Chlit
	{
	public:
		Chlit(char c)
			: m_Char(c) { }

		char GetChar() const { return m_Char; }

	private:
		char m_Char;
	};

	class Face
	{
	public:
		Face() = default;

		Face(const vec3int& vertexIndices, const vec3int& textureIndices, uint32_t normalIndex)
			: m_VertexIndices(vertexIndices), m_TextureIndices(textureIndices), m_NormalIndex(normalIndex) { }

		bool IsVertexInFace(uint32_t vertexIndex) const
		{
			return vertexIndex == m_VertexIndices.x || vertexIndex == m_VertexIndices.y || vertexIndex == m_VertexIndices.z;
		}

		uint32_t GetNormalIndex() const { return m_NormalIndex; }
		const vec3int& GetIndices() const { return m_VertexIndices; }

	private:
		vec3int m_VertexIndices;
		vec3int m_TextureIndices;
		uint32_t m_NormalIndex;
	};

	std::istream& operator>>(std::istream& is, Chlit chlit)
	{
		char c;
		if (is >> c && c != chlit.GetChar())
		{
			is.setstate(std::iostream::failbit);
		}

		return is;
	}

	inline void ltrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
			}));
	}


	void ObjectLoader::LoadObject(const std::string & filePath, Object* obj)
	{
		std::ifstream input(filePath, std::ios::in | std::ios::binary);

		if (input.is_open())
		{
			std::vector<glm::vec3> positions;
			std::vector<Face> faces;
			std::vector<glm::vec3> normals;

			std::string line;

			while (std::getline(input, line))
			{
				ltrim(line);
				std::string firstWord = line.substr(0, line.find(" "));

				if (firstWord == "v")
				{
					glm::vec3 vertex;

					std::istringstream iss(line.substr(line.find(" "), line.length()));
					iss >> vertex.x >> vertex.y >> vertex.z;

					positions.push_back(vertex);
				}
				else if (firstWord == "vn")
				{
					glm::vec3 normal;

					std::istringstream iss(line.substr(line.find(" "), line.length()));

					iss >> normal.x >> normal.y >> normal.z;

					normals.push_back(normal);
				}
				else if (firstWord == "f")
				{
					vec3int position;
					vec3int texture;
					uint32_t normal;

					std::istringstream iss(line.substr(line.find(" "), line.length()));

					iss >> position.x >> Chlit('/') >> texture.x >> Chlit('/') >> normal;

					iss >> position.y >> Chlit('/') >> texture.y >> Chlit('/') >> normal;

					iss >> position.z >> Chlit('/') >> texture.z >> Chlit('/') >> normal;
					
					faces.push_back({ position, texture, normal });
				}
				
			}

			float* vertexBuffer = new float[positions.size() * 6];

			for (unsigned int i = 1; i < positions.size() + 1; i++)
			{
				glm::vec3 normalSum(0.0f, 0.0f, 0.0f);
				float count = 0.0f;

				for (const Face& face : faces)
				{
					if (face.IsVertexInFace(i))
					{
						normalSum += normals[face.GetNormalIndex() - 1];
						count++;
					}
				}

				glm::vec3 normalAvg = normalSum / count;

				vertexBuffer[(i - 1) * 6 + 0] = positions[i - 1].x;
				vertexBuffer[(i - 1) * 6 + 1] = positions[i - 1].y;
				vertexBuffer[(i - 1) * 6 + 2] = positions[i - 1].z;
				vertexBuffer[(i - 1) * 6 + 3] = normalAvg.x;
				vertexBuffer[(i - 1) * 6 + 4] = normalAvg.y;
				vertexBuffer[(i - 1) * 6 + 5] = normalAvg.z;
			}

			uint32_t* indexBuffer = new uint32_t[faces.size() * 3];

			for (unsigned int i = 0; i < faces.size(); i++)
			{
				const auto& indices = faces[i].GetIndices();

				indexBuffer[i * 3 + 0] = indices.x - 1;
				indexBuffer[i * 3 + 1] = indices.y - 1;
				indexBuffer[i * 3 + 2] = indices.z - 1;
			}

			obj->SetVertexBuffer(vertexBuffer, positions.size() * 6);
			obj->SetIndexBuffer(indexBuffer, faces.size() * 3);
		}
	}
}
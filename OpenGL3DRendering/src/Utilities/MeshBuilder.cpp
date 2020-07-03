#include "oglpch.h"

#include "MeshBuilder.h"

namespace OpenGLRendering {

	Ref<Mesh> MeshBuilder::CreateSphere()
	{
        std::vector<SimpleVertex> vertices;
		std::vector<uint32_t> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;

        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                vertices.push_back({ glm::vec3(xPos, yPos, zPos), glm::vec3(xPos, yPos, zPos) });
                
            }
        }

        // Calc The Index Positions
        for (int i = 0; i < Y_SEGMENTS * X_SEGMENTS + Y_SEGMENTS; ++i) {

            indices.push_back(i);
            indices.push_back(i + Y_SEGMENTS + 1);
            indices.push_back(i + Y_SEGMENTS);

            indices.push_back(i + Y_SEGMENTS + 1);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        Ref<Material> material = CreateRef<Material>();
        return CreateRef<Mesh>("Sphere", vertices, indices, material, vertices.size(), indices.size() / 3);
	}

}

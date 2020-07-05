#include "oglpch.h"

#include "MeshBuilder.h"

namespace OpenGLRendering {

    static float s_CubeVertexBuffer[]
    {
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, // left face
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
                                     
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, // front face
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                                     
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, // right face
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
                                            
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,

         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, // bottom face
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,

        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, // top face
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    };

    static uint32_t s_CubeIndexBuffer[] =
    {
        20, 21, 22,
        20, 23, 21,
         6,  4,  5,
         6,  7,  4,
         2,  0,  1,
         2,  3,  0,
        16, 18, 19,
        16, 17, 18,
        10,  8,  9,
        10, 11,  8,
        14, 12, 13,
        14, 15, 12,
    };

    static float s_PyramidVertexBuffer[] =
    {
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, // bottom face
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,

        -1.0f, -1.0f,  1.0f,  0.0f,  0.4472f,  0.8944f, // front face
         1.0f, -1.0f,  1.0f,  0.0f,  0.4472f,  0.8944f,
         0.0f,  1.0f,  0.0f,  0.0f,  0.4472f,  0.8944f,

         1.0f, -1.0f,  1.0f,  0.8944f,  0.4472f,  0.0f, // right face
         1.0f, -1.0f, -1.0f,  0.8944f,  0.4472f,  0.0f,
         0.0f,  1.0f,  0.0f,  0.8944f,  0.4472f,  0.0f,

         1.0f, -1.0f, -1.0f,  0.0f,  0.4472f, -0.8944f, // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.4472f, -0.8944f,
         0.0f,  1.0f,  0.0f,  0.0f,  0.4472f, -0.8944f,

        -1.0f, -1.0f, -1.0f, -0.8944f,  0.4472f,  0.0f, // left face
        -1.0f, -1.0f,  1.0f, -0.8944f,  0.4472f,  0.0f,
         0.0f,  1.0f,  0.0f, -0.8944f,  0.4472f,  0.0f,
    };

    static uint32_t s_PyramidIndexBuffer[] =
    {
         0,  1,  2,
         0,  2,  3,
         4,  5,  6,
         7,  8,  9,
        10, 11, 12,
        13, 14, 15,
    };

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

    Ref<Mesh> MeshBuilder::CreateCube()
    {
        Ref<Material> material = CreateRef<Material>();
        return CreateRef<Mesh>("Cube", (SimpleVertex*)s_CubeVertexBuffer, s_CubeIndexBuffer, material, 24, 12);
    }

    Ref<Mesh> MeshBuilder::CreatePyramid()
    {
        Ref<Material> material = CreateRef<Material>();
        return CreateRef<Mesh>("Pyramid", (SimpleVertex*)s_PyramidVertexBuffer, s_PyramidIndexBuffer, material, 16, 6);
    }

}

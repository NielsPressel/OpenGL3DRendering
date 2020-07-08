#pragma once

#include "Core/Camera.h"
#include "Renderer/Cubemap.h"

#include "Utilities/Mesh.h"
#include "Utilities/Model.h"

namespace OpenGLRendering {

	struct LightInfo
	{
		glm::vec3 LightPos;
		glm::vec3 LightColor;
	};

	struct RendererStats
	{
		uint32_t VertexCount;
		uint32_t FaceCount;
		uint32_t DrawCalls;
	};

	class Renderer {
	public:

		static void Init();

		static void BeginScene(Ref<Camera>& camera, Ref<Cubemap>& cubemap, const LightInfo& lightInfo);
		static void EndScene();
		
		static void Submit(Ref<Mesh>& mesh, const glm::mat4& modelMatrix = glm::identity<glm::mat4>());

		static void Submit(Ref<Model>& model);
		static void Submit(Ref<Model>& model, uint16_t lod, uint16_t meshesPerLod);

		static const RendererStats& GetStatistics();

	};

}
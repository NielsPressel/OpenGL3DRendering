#pragma once

#include "Core/Camera.h"
#include "Renderer/Cubemap.h"

#include "Utilities/Mesh.h"
#include "Utilities/Model.h"

namespace OpenGLRendering {

	class Renderer {

		static void Init();

		static void BeginScene(Ref<Camera> camera, Ref<Cubemap> cubemap);
		static void EndScene();
		
		static void Submit(Ref<Mesh> mesh, const glm::mat4& modelMatrix = glm::identity<glm::mat4>());
		static void Submit(Ref<Model> model, uint16_t lod = 0, uint16_t meshesPerLod = 0);

	};

}
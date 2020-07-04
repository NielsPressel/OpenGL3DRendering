#pragma once

#include "Mesh.h"

namespace OpenGLRendering {
	
	class MeshBuilder
	{
	public:
		static Ref<Mesh> CreateSphere();
		static Ref<Mesh> CreateCube();
		static Ref<Mesh> CreatePyramid();
	};

}
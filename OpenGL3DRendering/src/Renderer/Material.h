#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Texture.h"

namespace OpenGLRendering {

	class Material
	{
	public:
		Material(const glm::vec4& baseColor, const std::vector<Texture2D>& textures) { }
	};

}
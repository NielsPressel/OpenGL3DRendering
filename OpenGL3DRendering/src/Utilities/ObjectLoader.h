#pragma once

#include <string>
#include "Core/Object.h"

namespace OpenGLRendering {

	class ObjectLoader
	{
	public:
		static void LoadObject(const std::string& filePath, Object* obj);
	};

}
#pragma once

#include <memory>

#include "MouseCodes.h"
#include "KeyCodes.h"

namespace OpenGLRendering {

	class Input
	{
	public:
		Input() = delete;

		static bool IsKeyPressed(KeyCode keyCode);
		static bool IsMouseButtonPressed(MouseCode mouseCode);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
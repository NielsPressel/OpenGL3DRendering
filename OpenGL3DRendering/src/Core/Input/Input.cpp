#include "Input.h"

#include "Core/ApplicationHandler.h"

#include <GLFW/glfw3.h>

namespace OpenGLRendering {

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		auto window = ApplicationHandler::Get().GetWindow().GetNativeWindow();
		auto state = glfwGetKey(window, static_cast<int32_t>(keyCode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(MouseCode mouseCode)
	{
		auto window = ApplicationHandler::Get().GetWindow().GetNativeWindow();
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(mouseCode));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = ApplicationHandler::Get().GetWindow().GetNativeWindow();
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}

}
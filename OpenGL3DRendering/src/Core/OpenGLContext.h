#pragma once

#include <GLFW/glfw3.h>

namespace OpenGLRendering {

	class OpenGLContext
	{
	public:
		OpenGLContext(GLFWwindow* window);
		void Init();
		void SwapBuffers();

	private:
		GLFWwindow* m_Window;
	};

}

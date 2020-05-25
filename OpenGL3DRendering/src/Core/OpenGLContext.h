#pragma once

#include <GLFW/glfw3.h>

namespace OpenGLRendering {

	class OpenGLContext
	{
	public:
		OpenGLContext(GLFWwindow* window);
		void Init();
		void SwapBuffers();

		uint32_t GetMaxTextureSlots() { return m_MaxTextureSlots; }
		uint32_t GetMaxCombinedTextureSlots() { return m_MaxCombinedTextureSlots; }

	private:
		GLFWwindow* m_Window;

		int32_t m_MaxTextureSlots;
		int32_t m_MaxCombinedTextureSlots;
	};

}

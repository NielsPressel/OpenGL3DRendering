#include "oglpch.h"

#include "OpenGLContext.h"

#include <glad/glad.h>

namespace OpenGLRendering {

	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_Window(window)
	{

	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		OGL_ASSERT(status, "Failed to initialize glad!");

		OGL_INFO("OpenGL Info:");
		OGL_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		OGL_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		OGL_INFO("Version: {0}", glGetString(GL_VERSION));

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_MaxTextureSlots);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_MaxCombinedTextureSlots);
		
		OGL_INFO("Max Texture Slots: {0}", m_MaxTextureSlots);
		OGL_INFO("Max combined Texture Slots: {0}", m_MaxCombinedTextureSlots);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}
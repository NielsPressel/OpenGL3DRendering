#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"


#include "Core/Log.h"



int main()
{
	OpenGLRendering::Log::Init();

	int success = glfwInit();

	if (!success)
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(1000, 500, "OpenGL3DRendering", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	if (!status)
	{
		return -1;
	}

	OGL_INFO("OpenGL Info:");
	OGL_INFO("Vendor: {0}", glGetString(GL_VENDOR));
	OGL_INFO("Renderer: {0}", glGetString(GL_RENDERER));
	OGL_INFO("Version: {0}", glGetString(GL_VERSION));

	std::cin.get();
}
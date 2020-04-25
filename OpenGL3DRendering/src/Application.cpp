#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"


#include "Core/Log.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Shader.h"

void OpenGLMessageCallback(
	unsigned source,
	unsigned type,
	unsigned id,
	unsigned severity,
	int length,
	const char* message,
	const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         OGL_CRITICAL(message); return;
	case GL_DEBUG_SEVERITY_MEDIUM:       OGL_ERROR(message); return;
	case GL_DEBUG_SEVERITY_LOW:          OGL_WARN(message); return;
	case GL_DEBUG_SEVERITY_NOTIFICATION: OGL_TRACE(message); return;
	}
}


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

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
	 	 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,	0.5f, 0.0f
	};

	uint32_t indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

#ifdef OGL_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

	OpenGLRendering::Shader shader("src/ShaderSource/vertex.glsl", "src/ShaderSource/fragment.glsl");
	shader.Bind();

	uint32_t vertexArray;
	glCreateVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	uint32_t vertexBuffer;
	glCreateBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	uint32_t indexBuffer;
	glCreateBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	while (1)
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
	}

	std::cin.get();

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vertexArray);
}
#include "Window.h"

#include "Core/Core.h"

namespace OpenGLRendering {

	static uint8_t s_WindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		OGL_ERROR("GLFW error ({1}): {1}", error, description);
	}

	Window::Window(const WindowSettings& settings)
	{
		Init(settings);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(const WindowSettings& settings)
	{
		m_Data.Title = settings.Title;
		m_Data.Width = settings.Width;
		m_Data.Height = settings.Height;

		if (s_WindowCount == 0)
		{
			int success = glfwInit();
			OGL_ASSERT(success, "Couldn't initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_Handle = glfwCreateWindow((int)settings.Width, (int)settings.Height, settings.Title.c_str(), nullptr, nullptr);
		s_WindowCount++;

		m_Context = std::make_unique<OpenGLContext>(m_Handle);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Handle, &m_Data);

		glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetScrollCallback(m_Handle, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event(xOffset, yOffset);
			data.EventCallback(event);
		});
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Handle);
		s_WindowCount--;

		if (s_WindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void Window::SetVsync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

}
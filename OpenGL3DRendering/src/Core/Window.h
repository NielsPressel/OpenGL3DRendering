#pragma once

#include <string>
#include <memory>
#include <functional>

#include <GLFW/glfw3.h>

#include "Core/OpenGLContext.h"
#include "Core/Event.h"
#include "Core/Core.h"

namespace OpenGLRendering {

	struct WindowSettings
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowSettings(const std::string& title = "OpenGLRendering", unsigned int width = 1080, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{

		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowSettings& settings);
		~Window();

		void OnUpdate();
		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }
		bool IsVSync() const { return m_Data.VSync; }
		GLFWwindow* GetNativeWindow() const { return m_Handle; }

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVsync(bool enabled);

	private:
		void Init(const WindowSettings& settings);
		void Shutdown();

	private:
		GLFWwindow* m_Handle;
		Scope<OpenGLContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}

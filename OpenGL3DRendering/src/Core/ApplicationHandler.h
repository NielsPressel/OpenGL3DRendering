#pragma once

#include "Event.h"
#include "Timestep.h"
#include "Window.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include "ImGui/ImGuiLayer.h"

#include "Core/CameraController.h"
#include "Utilities/Model.h"
#include "Core/Core.h"

namespace OpenGLRendering {

	class ApplicationHandler
	{
	public:
		ApplicationHandler();
		~ApplicationHandler();

		void StartLoop();
		const Window& GetWindow() { return *m_Window; }
		
		static ApplicationHandler& Get() { return *s_Instance; }

	private:
		void OnStartup();
		void OnUpdate(Timestep t);
		void OnImGuiRender();
		void OnEvent(Event& event);

		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		bool m_Running;
		static ApplicationHandler* s_Instance;
		
		Window* m_Window;
		Scope<ImGuiLayer> m_ImGuiLayer;
		Scope<CameraController> m_CameraController;
		Scope<Shader> m_Shader;
		Scope<Model> m_Model;
	};

}
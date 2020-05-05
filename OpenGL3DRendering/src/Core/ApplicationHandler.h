#pragma once

#include "Event.h"
#include "Timestep.h"
#include "Window.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include "ImGui/ImGuiLayer.h"

#include "Core/CameraController.h"
#include "Utilities/Model.h"

#include <memory>

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
		std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
		std::unique_ptr<CameraController> m_CameraController;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Model> m_Model;
	};

}
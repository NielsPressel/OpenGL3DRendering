#pragma once

#include "Event.h"
#include "Timestep.h"
#include "Window.h"
#include "Renderer/Shader.h"
#include "Renderer/Framebuffer.h"

#include "ImGui/ImGuiLayer.h"

#include "Core/CameraController.h"
#include "Utilities/Model.h"
#include "Core/Core.h"


namespace OpenGLRendering {


	// Runtime handler of the application
	// Change the OnStartup(), OnUpdate(Timestep t) and OnImGuiRender(Timestep t) methods to change the contents of the application
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
		void OnImGuiRender(Timestep t);
		void OnEvent(Event& event);

		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		bool m_Running;
		bool m_Sleeping;
		static ApplicationHandler* s_Instance;
		
		Scope<Window> m_Window;
		Scope<ImGuiLayer> m_ImGuiLayer;
		Scope<CameraController> m_CameraController;
		Scope<Shader> m_Shader;
		Scope<Shader> m_PBRShader;
		Scope<Model> m_Model;
		Scope<Framebuffer> m_Framebuffer;
		glm::vec3 m_LightPos = { 0.0f, 0.0f, 4.0f };
		glm::vec3 m_LightColor = { 1.0f, 1.0f, 1.0f };
		glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	};

} // namespace OpenGLRendering
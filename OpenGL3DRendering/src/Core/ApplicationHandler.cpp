#include "oglpch.h"

#include "ApplicationHandler.h"

#include "Core/Input/Input.h"

#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

namespace OpenGLRendering {

	ApplicationHandler* ApplicationHandler::s_Instance = nullptr;

	ApplicationHandler::ApplicationHandler()
		: m_Running(false)
	{
		OGL_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = new Window({ "Niels Pressel" });
		m_Window->SetEventCallback(BIND_EVENT_FN(ApplicationHandler::OnEvent));
		m_Window->SetVsync(true);

		m_ImGuiLayer = CreateScope<ImGuiLayer>();

		RendererAPI::Init();
	}

	ApplicationHandler::~ApplicationHandler()
	{
		delete m_Window;
	}

	void ApplicationHandler::StartLoop()
	{
		if (!m_Running)
		{
			OnStartup();
			m_ImGuiLayer->OnStart();
			
			float time = glfwGetTime();
			float lastTime = time;

			m_Running = true;
			while (m_Running)
			{
				float time = glfwGetTime();
				Timestep step = time - lastTime;
				lastTime = time;

				OnUpdate(step);

				m_ImGuiLayer->Begin();
				OnImGuiRender();
				m_ImGuiLayer->End();

				m_Window->OnUpdate();
			}
		}
	}

	void ApplicationHandler::OnStartup()
	{
		float vertices[] =
		{
			-1.0, -1.0,  1.0,
			 1.0, -1.0,  1.0,
			 1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			-1.0, -1.0, -1.0,
			 1.0, -1.0, -1.0,
			 1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0
		};

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0,
			1, 5, 6,
			6, 2, 1,
			7, 6, 5,
			5, 4, 7,
			4, 0, 3,
			3, 7, 4,
			4, 5, 1,
			1, 0, 4,
			3, 2, 6,
			6, 7, 3
		};

		m_Shader = CreateScope<Shader>("src/Resources/ShaderSource/vertex.glsl", "src/Resources/ShaderSource/fragment.glsl");
		m_Shader->Bind();

		m_Model = CreateScope<Model>("src/Resources/Assets/Sniper_Rifle_Textured.fbx", true);

		/*
		Ref<Texture2D> diffuseTexture = CreateRef<Texture2D>("src/Resources/Assets/cottage_textures/cottage_diffuse.png", TextureType::DIFFUSE);
		Ref<Texture2D> normalTexture = CreateRef<Texture2D>("src/Resources/Assets/cottage_textures/cottage_normal.png", TextureType::NORMAL);
		m_Model->GetMeshes()[0].GetMaterial()->AddTexture(diffuseTexture);
		m_Model->GetMeshes()[0].GetMaterial()->AddTexture(normalTexture);
		*/

		m_CameraController = CreateScope<CameraController>(glm::vec3(0.0f, 0.0f, 2.0f));

		//glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		RendererAPI::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	}

	void ApplicationHandler::OnUpdate(Timestep t)
	{
		m_CameraController->OnUpdate(t);
		RendererAPI::Clear();

		float time = glfwGetTime();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
		model = glm::rotate(model, glm::radians(-90.0f), { 1.0f, 0.0f, 0.0f });

		glm::mat4 view = m_CameraController->GetCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(45.0f, 1.0f * m_Window->GetWidth() / m_Window->GetHeight(), 0.1f, 100.0f);

		m_Shader->SetMat4("u_Model", model);
		m_Shader->SetMat4("u_View", view);
		m_Shader->SetMat4("u_Projection", projection);
		m_Shader->SetFloat3("u_LightPos", { cos(glm::radians(time) * 10.0f) * 10.0f ,  0.0f, sin(glm::radians(time) * 10.0f) * 10.0f });
		m_Shader->SetFloat3("u_ViewPos", m_CameraController->GetCamera().GetPosition());

		m_Model->Render(*m_Shader.get());
	}

	void ApplicationHandler::OnImGuiRender()
	{
#ifdef OGL_DEBUG
		ImGui::Begin("Meshes");
		uint32_t i = 0;
		for (const Mesh& mesh : m_Model->GetMeshes())
		{
			if (ImGui::TreeNode(mesh.GetName().c_str()))
			{
				if (ImGui::TreeNode("Material"))
				{
					ImGui::ColorEdit4("Base Color", (float*)&mesh.GetMaterial()->GetBaseColor());

					for (const Ref<Texture2D>& texture : mesh.GetMaterial()->GetTextures())
					{
						ImGui::Text(texture->GetPath().c_str());
					}

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
		}
		ImGui::End();
#endif
	}

	void ApplicationHandler::OnEvent(Event& event)
	{
		m_CameraController->OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ApplicationHandler::OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(ApplicationHandler::OnWindowClose));
	}

	bool ApplicationHandler::OnWindowResize(WindowResizeEvent& event)
	{
		glViewport(0, 0, event.GetWidth(), event.GetHeight());

		return true;
	}

	bool ApplicationHandler::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;

		return true;
	}

}
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

#define DROPSHIP 1

namespace OpenGLRendering {

	ApplicationHandler* ApplicationHandler::s_Instance = nullptr;

	ApplicationHandler::ApplicationHandler()
		: m_Running(false)
	{
		OGL_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = new Window({ "OpenGL3DRendering" });
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
		m_Shader = CreateScope<Shader>("src/Resources/ShaderSource/vertex.glsl", "src/Resources/ShaderSource/fragment.glsl");
		m_PBRShader = CreateScope<Shader>("src/Resources/ShaderSource/vertex_pbr.glsl", "src/Resources/ShaderSource/fragment_pbr.glsl");
		m_PBRShader->Bind();

		m_Model = CreateScope<Model>("src/Resources/Assets/Dropship.fbx", false);

#if PISTOL
		Ref<Texture2D> diffuseTexture = CreateRef<Texture2D>("src/Resources/Assets/textures/Pistol_Albedo.png", TextureType::ALBEDO);
		Ref<Texture2D> normalTexture = CreateRef<Texture2D>("src/Resources/Assets/textures/Pistol_Normal.png", TextureType::NORMAL);
		Ref<Texture2D> metallicSmoothnessTexture = CreateRef<Texture2D>("src/Resources/Assets/textures/Pistol_MetallicSmooth.png", TextureType::METALLIC_SMOOTHNESS);
		Ref<Texture2D> ambientOcclusionTexture = CreateRef<Texture2D>("src/Resources/Assets/textures/Pistol_Occlusion.png", TextureType::AMBIENT_OCCLUSION);

		m_Model->GetMeshes()[0].GetMaterial()->AddTexture(diffuseTexture);
		m_Model->GetMeshes()[0].GetMaterial()->AddTexture(normalTexture);
		m_Model->GetMeshes()[0].GetMaterial()->AddTexture(metallicSmoothnessTexture);
		m_Model->GetMeshes()[0].GetMaterial()->AddTexture(ambientOcclusionTexture);
#endif

#if DROPSHIP
		Ref<Texture2D> albedoTexture01 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_01_Albedo.png", TextureType::ALBEDO);
		Ref<Texture2D> normalTexture01 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_01_Normal.png", TextureType::NORMAL);
		Ref<Texture2D> metallicSmoothTexture01 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_01_MetallicSmooth.png", TextureType::METALLIC_SMOOTHNESS);
		Ref<Texture2D> occlusionTexture01 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_01_Occlusion.png", TextureType::AMBIENT_OCCLUSION);

		Ref<Texture2D> albedoTexture02 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_02_Albedo.png", TextureType::ALBEDO);
		Ref<Texture2D> normalTexture02 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_02_Normal.png", TextureType::NORMAL);
		Ref<Texture2D> metallicSmoothTexture02 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_02_MetallicSmooth.png", TextureType::METALLIC_SMOOTHNESS);
		Ref<Texture2D> occlusionTexture02 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_02_Occlusion.png", TextureType::AMBIENT_OCCLUSION);

		Ref<Texture2D> albedoTexture03 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_03_Albedo.png", TextureType::ALBEDO);
		Ref<Texture2D> normalTexture03 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_03_Normal.png", TextureType::NORMAL);
		Ref<Texture2D> metallicSmoothTexture03 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_03_MetallicSmooth.png", TextureType::METALLIC_SMOOTHNESS);
		Ref<Texture2D> occlusionTexture03 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_03_Occlusion.png", TextureType::AMBIENT_OCCLUSION);

		Ref<Texture2D> albedoTexture04 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_04_Albedo.png", TextureType::ALBEDO);
		Ref<Texture2D> metallicSmoothTexture04 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_04_MetallicSmooth.png", TextureType::METALLIC_SMOOTHNESS);
		Ref<Texture2D> occlusionTexture04 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_04_Occlusion.png", TextureType::AMBIENT_OCCLUSION);

		Ref<Texture2D> albedoTexture05 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_05_Albedo.png", TextureType::ALBEDO);
		Ref<Texture2D> normalTexture05 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_05_Normal.png", TextureType::NORMAL);
		Ref<Texture2D> metallicSmoothTexture05 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_05_MetallicSmooth.png", TextureType::METALLIC_SMOOTHNESS);

		m_Model->GetMeshes()[0].GetMaterial()->AddTexture(albedoTexture01);
		m_Model->GetMeshes()[0].GetMaterial()->AddTexture(normalTexture01);
		m_Model->GetMeshes()[0].GetMaterial()->AddTexture(metallicSmoothTexture01);
		m_Model->GetMeshes()[0].GetMaterial()->AddTexture(occlusionTexture01);

		m_Model->GetMeshes()[1].GetMaterial()->AddTexture(albedoTexture02);
		m_Model->GetMeshes()[1].GetMaterial()->AddTexture(normalTexture02);
		m_Model->GetMeshes()[1].GetMaterial()->AddTexture(metallicSmoothTexture02);
		m_Model->GetMeshes()[1].GetMaterial()->AddTexture(occlusionTexture02);

		m_Model->GetMeshes()[2].GetMaterial()->AddTexture(albedoTexture03);
		m_Model->GetMeshes()[2].GetMaterial()->AddTexture(normalTexture03);
		m_Model->GetMeshes()[2].GetMaterial()->AddTexture(metallicSmoothTexture03);
		m_Model->GetMeshes()[2].GetMaterial()->AddTexture(occlusionTexture03);

		m_Model->GetMeshes()[3].GetMaterial()->AddTexture(albedoTexture04);
		m_Model->GetMeshes()[3].GetMaterial()->AddTexture(metallicSmoothTexture04);
		m_Model->GetMeshes()[3].GetMaterial()->AddTexture(occlusionTexture04);

		m_Model->GetMeshes()[4].GetMaterial()->AddTexture(albedoTexture05);
		m_Model->GetMeshes()[4].GetMaterial()->AddTexture(normalTexture05);
		m_Model->GetMeshes()[4].GetMaterial()->AddTexture(metallicSmoothTexture05);
#endif

		m_CameraController = CreateScope<CameraController>(glm::vec3(0.0f, 0.0f, 2.0f));
		m_CameraController->LookAtPoint(m_Model->GetMeshes()[0].GetBoundingBoxCenter());
	}

	void ApplicationHandler::OnUpdate(Timestep t)
	{
		m_CameraController->OnUpdate(t);
		RendererAPI::SetClearColor(m_ClearColor);
		RendererAPI::Clear();

		float time = glfwGetTime();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
		//model = glm::rotate(model, glm::radians(time * 10.0f), { 1.0f, 0.5f, 0.2f });

		glm::mat4 view = m_CameraController->GetCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(45.0f, 1.0f * m_Window->GetWidth() / m_Window->GetHeight(), 0.1f, 100.0f);

		m_PBRShader->SetMat4("u_Model", model);
		m_PBRShader->SetMat4("u_View", view);
		m_PBRShader->SetMat4("u_Projection", projection);
		m_PBRShader->SetFloat3("u_LightPos", m_LightPos);
		m_PBRShader->SetFloat3("u_CameraPos", m_CameraController->GetCamera().GetPosition());

		m_Model->RenderLoD(*m_PBRShader.get(), 0, 5);
	}

	void ApplicationHandler::OnImGuiRender()
	{
		ImGui::Begin("Meshes");
		uint32_t i = 0;
		for (Mesh& mesh : m_Model->GetMeshes())
		{
			std::stringstream ss;
			ss << mesh.GetName() << " " << i;

			if (ImGui::TreeNode(ss.str().c_str()))
			{
				ImGui::Checkbox("Render", &mesh.IsRendering());
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
			i++;
		}
		ImGui::DragFloat3("Camera Position", (float*)&m_CameraController->GetPosition());
		ImGui::DragFloat3("Light Position", (float*)&m_LightPos);
		ImGui::ColorEdit4("Clear Color", (float*)&m_ClearColor);
		ImGui::End();
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
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


// Defines for which model to render
#define PISTOL 1
#define DROPSHIP 0

namespace OpenGLRendering {

	ApplicationHandler* ApplicationHandler::s_Instance = nullptr;

	ApplicationHandler::ApplicationHandler()
		: m_Running(false), m_Sleeping(false)
	{
		OGL_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		WindowSettings settings("OpenGL3DRendering");
		m_Window = CreateScope<Window>(settings);
		m_Window->SetEventCallback(BIND_EVENT_FN(ApplicationHandler::OnEvent));
		m_Window->SetVsync(true);

		m_ImGuiLayer = CreateScope<ImGuiLayer>();

		RendererAPI::Init();
	}

	ApplicationHandler::~ApplicationHandler() { }

	void ApplicationHandler::StartLoop()
	{
		if (!m_Running)
		{
			OnStartup();
			m_ImGuiLayer->OnStart();
			
			float time = (float)glfwGetTime();
			float lastTime = time;

			m_Running = true;
			while (m_Running)
			{
				m_Window->OnUpdate();
				if (m_Sleeping)
					continue;

				float time = (float)glfwGetTime();
				Timestep step = time - lastTime;
				lastTime = time;

				OnUpdate(step);

				m_ImGuiLayer->Begin();
				OnImGuiRender(step);
				m_ImGuiLayer->End();
			}
		}
	}

	void ApplicationHandler::OnStartup()
	{
		m_Shader = CreateScope<Shader>("src/Resources/ShaderSource/vertex.glsl", "src/Resources/ShaderSource/fragment.glsl");
		m_PBRShader = CreateScope<Shader>("src/Resources/ShaderSource/vertex_pbr.glsl", "src/Resources/ShaderSource/fragment_pbr.glsl");
		m_PBRShader->Bind();

		FramebufferSettings settings = { 1920, 1080 };
		m_Framebuffer = CreateScope<Framebuffer>(settings);
		m_Framebuffer->Unbind();

#if PISTOL
		m_Model = CreateScope<Model>("src/Resources/Assets/Pistol.fbx", false);
		m_Model->SetTranslation({ 0.0f, 0.0f, 0.0f });
		m_Model->SetRotation({ 0.0f, 0.0f, 0.0f });
		m_Model->SetScale({ 0.1f, 0.1f, 0.1f });

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
		m_Model = CreateScope<Model>("src/Resources/Assets/Dropship.fbx", false);
		m_Model->SetTranslation({ 0.0f, 0.0f, 0.0f });
		m_Model->SetRotation({ 0.0f, 0.0f, 0.0f });
		m_Model->SetScale({ 0.01f, 0.01f, 0.01f });

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

		m_CameraController = CreateScope<CameraController>(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	void ApplicationHandler::OnUpdate(Timestep t)
	{
		m_CameraController->OnUpdate(t);
		float time = (float)glfwGetTime();

		RendererAPI::SetClearColor(m_ClearColor);
		RendererAPI::Clear();

		m_Framebuffer->Bind();
		glm::mat4 view = m_CameraController->GetCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(45.0f, 1.0f * m_Framebuffer->GetSettings().Width / m_Framebuffer->GetSettings().Height, 0.1f, 100.0f);

		m_PBRShader->SetMat4("u_View", view);
		m_PBRShader->SetMat4("u_Projection", projection);
		m_PBRShader->SetFloat3("u_LightPos", m_LightPos);
		m_PBRShader->SetFloat3("u_LightColor", m_LightColor);
		m_PBRShader->SetFloat3("u_CameraPos", m_CameraController->GetCamera().GetPosition());

		RendererAPI::SetClearColor(m_ClearColor);
		RendererAPI::Clear();

		m_Model->RenderLoD(*m_PBRShader.get(), 0, 5);

		m_Framebuffer->Unbind();
		RendererAPI::SetViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	}

	void ApplicationHandler::OnImGuiRender(Timestep t)
	{
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		static bool showDockspace = true;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Editor", &showDockspace, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("Editor Space");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close")) m_Running = false;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		// Mesh control
		ImGui::Begin("Meshes");
		ImGui::Text("Object");
		ImGui::Spacing();

		uint32_t i = 0;
		for (Mesh& mesh : m_Model->GetMeshes())
		{
			std::stringstream ss;
			ss << mesh.GetName() << " " << i;

			if (ImGui::TreeNode(ss.str().c_str()))
			{
				std::stringstream ssVertices;
				ssVertices << "Vertices: " << mesh.GetVertexCount();
				std::stringstream ssFaces;
				ssFaces << "Faces: " << mesh.GetFaceCount();

				ImGui::Checkbox("Render", &mesh.IsRendering());
				ImGui::Text(ssVertices.str().c_str());
				ImGui::Text(ssFaces.str().c_str());


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
		static glm::vec3 translation = { 1.0f, 1.0f, 1.0f };
		static glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		static glm::vec3 lastRotation = rotation;
		static glm::vec3 scale = { 1.0f, 1.0f, 1.0f };


		ImGui::Spacing();
		ImGui::DragFloat3("Translation", (float*)&translation);
		ImGui::SliderAngle("Rotation X", (float*)&rotation);
		ImGui::SliderAngle("Rotation Y", (float*)&rotation[1]);
		ImGui::SliderAngle("Rotation Z", (float*)&rotation[2]);
		ImGui::DragFloat3("Scale", (float*)&scale, 0.005f, 0.0f);

		ImGui::Dummy({ 1.0, 10.0 });
		ImGui::Text("Global Properties");
		ImGui::Spacing();
		ImGui::DragFloat3("Camera Position", (float*)&m_CameraController->GetPosition());
		ImGui::DragFloat3("Light Position", (float*)&m_LightPos);
		ImGui::ColorEdit3("Light Color", (float*)&m_LightColor);
		ImGui::ColorEdit4("Clear Color", (float*)&m_ClearColor);
		ImGui::End();

		m_Model->SetTranslation(translation);
		m_Model->SetRotation(rotation - lastRotation);
		m_Model->SetScale(scale);
		m_Model->CalculateModelMatrix();
		lastRotation = rotation;

		// Render Stats
		ImGui::Begin("Render Stats");

		std::stringstream ss;
		ss << "Frametime: " << t.GetMilliseconds() << ", FPS: " << 1000.0f / t.GetMilliseconds();
		ImGui::Text(ss.str().c_str());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("Viewport");
		
		ImVec2 size = ImGui::GetContentRegionAvail();
		const FramebufferSettings& framebufferSettings = m_Framebuffer->GetSettings();
		if (framebufferSettings.Width != size.x || framebufferSettings.Height != size.y)
		{
			FramebufferSettings settings = { size.x, size.y };
			m_Framebuffer->SetSettings(settings);
			m_Framebuffer->Invalidate();
		}
		ImGui::Image((void*)m_Framebuffer->GetColorTextureId(), size);
		
		ImGui::End();
		ImGui::PopStyleVar();

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
		if (event.GetHeight() == 0 || event.GetWidth() == 0)
		{
			m_Sleeping = true;
			return true;
		}

		m_Sleeping = false;
		RendererAPI::SetViewport(0, 0, event.GetWidth(), event.GetHeight());

		return true;
	}

	bool ApplicationHandler::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;

		return true;
	}

}
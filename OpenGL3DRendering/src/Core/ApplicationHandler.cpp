#include "oglpch.h"

#include "ApplicationHandler.h"

#include "Core/Input/Input.h"

#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Texture.h"
#include "Renderer/Cubemap.h"
#include "Renderer/Renderer.h"

#include "Utilities/MeshBuilder.h"

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
		m_Window->SetEventCallback(BIND_EVENT_FN(ApplicationHandler::OnEvent)); // All event functions get dispatched to ApplicationHandler::OnEvent
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


			// Runtime loop
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
		Renderer::Init();
		m_Cubemap = CreateRef<Cubemap>("src/Resources/Assets/textures/cubemap/newport_loft.hdr");
		
		m_Sphere = MeshBuilder::CreateSphere();
		m_Sphere->GetMaterial()->UseTextures(false);
		m_Sphere->GetMaterial()->SetAlbedo({ 1.0f, 0.0f, 0.0f });
		m_Sphere->GetMaterial()->SetRoughness(0.25f);
		m_Sphere->GetMaterial()->SetMetallic(1.0f);
		m_Sphere->GetMaterial()->SetAmbientOcclusion(1.0f);

		m_Cube = MeshBuilder::CreateCube();
		m_Cube->GetMaterial()->UseTextures(false);
		m_Cube->GetMaterial()->SetAlbedo({ 0.0f, 1.0f, 0.0f });
		m_Cube->GetMaterial()->SetRoughness(0.0f);
		m_Cube->GetMaterial()->SetMetallic(0.0f);
		m_Cube->GetMaterial()->SetAmbientOcclusion(1.0f);

		m_Pyramid = MeshBuilder::CreatePyramid();
		m_Pyramid->GetMaterial()->UseTextures(false);
		m_Pyramid->GetMaterial()->SetAlbedo({ 0.0f, 0.0f, 1.0f });
		m_Pyramid->GetMaterial()->SetRoughness(0.3f);
		m_Pyramid->GetMaterial()->SetMetallic(0.0f);
		m_Pyramid->GetMaterial()->SetAmbientOcclusion(1.0f);

		// Pistol setup
#if PISTOL
		m_Model = CreateRef<Model>("src/Resources/Assets/Pistol.fbx", false);
		m_Model->SetTranslation({ 0.0f, 0.0f, 0.0f });
		m_Model->SetRotation({ 0.0f, 0.0f, 0.0f });
		m_Model->SetScale({ 0.1f, 0.1f, 0.1f });

		Ref<Texture2D> diffuseTexture = CreateRef<Texture2D>("src/Resources/Assets/textures/Pistol_Albedo.png");
		Ref<Texture2D> normalTexture = CreateRef<Texture2D>("src/Resources/Assets/textures/Pistol_Normal.png");
		Ref<Texture2D> metallicSmoothnessTexture = CreateRef<Texture2D>("src/Resources/Assets/textures/Pistol_MetallicSmooth.png");
		Ref<Texture2D> ambientOcclusionTexture = CreateRef<Texture2D>("src/Resources/Assets/textures/Pistol_Occlusion.png");

		m_Model->GetMeshes()[0].GetMaterial()->SetTextureOfType(TextureType::ALBEDO, diffuseTexture);
		m_Model->GetMeshes()[0].GetMaterial()->SetTextureOfType(TextureType::NORMAL, normalTexture);
		m_Model->GetMeshes()[0].GetMaterial()->SetTextureOfType(TextureType::METALLIC_SMOOTHNESS, metallicSmoothnessTexture);
		m_Model->GetMeshes()[0].GetMaterial()->SetTextureOfType(TextureType::AMBIENT_OCCLUSION, ambientOcclusionTexture);
		m_Model->GetMeshes()[0].GetMaterial()->UseTextures(true);
#endif


		// Dropship setup
#if DROPSHIP
		m_Model = CreateRef<Model>("src/Resources/Assets/Dropship.fbx", false);
		m_Model->SetTranslation({ 0.0f, 0.0f, 0.0f });
		m_Model->SetRotation({ 0.0f, 0.0f, 0.0f });
		m_Model->SetScale({ 0.01f, 0.01f, 0.01f });

		Ref<Texture2D> albedoTexture01 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_01_Albedo.png");
		Ref<Texture2D> normalTexture01 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_01_Normal.png");
		Ref<Texture2D> metallicSmoothTexture01 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_01_MetallicSmooth.png");
		Ref<Texture2D> occlusionTexture01 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_01_Occlusion.png");

		Ref<Texture2D> albedoTexture02 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_02_Albedo.png");
		Ref<Texture2D> normalTexture02 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_02_Normal.png");
		Ref<Texture2D> metallicSmoothTexture02 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_02_MetallicSmooth.png");
		Ref<Texture2D> occlusionTexture02 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_02_Occlusion.png");

		Ref<Texture2D> albedoTexture03 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_03_Albedo.png");
		Ref<Texture2D> normalTexture03 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_03_Normal.png");
		Ref<Texture2D> metallicSmoothTexture03 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_03_MetallicSmooth.png");
		Ref<Texture2D> occlusionTexture03 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_03_Occlusion.png");

		Ref<Texture2D> albedoTexture04 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_04_Albedo.png");
		Ref<Texture2D> metallicSmoothTexture04 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_04_MetallicSmooth.png");
		Ref<Texture2D> occlusionTexture04 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_04_Occlusion.png");

		Ref<Texture2D> albedoTexture05 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_05_Albedo.png");
		Ref<Texture2D> normalTexture05 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_05_Normal.png");
		Ref<Texture2D> metallicSmoothTexture05 = CreateRef<Texture2D>("src/Resources/Assets/textures/Dropship_05_MetallicSmooth.png");

		m_Model->GetMeshes()[0].GetMaterial()->SetTextureOfType(TextureType::ALBEDO, albedoTexture01);
		m_Model->GetMeshes()[0].GetMaterial()->SetTextureOfType(TextureType::NORMAL, normalTexture01);
		m_Model->GetMeshes()[0].GetMaterial()->SetTextureOfType(TextureType::METALLIC_SMOOTHNESS, metallicSmoothTexture01);
		m_Model->GetMeshes()[0].GetMaterial()->SetTextureOfType(TextureType::AMBIENT_OCCLUSION, occlusionTexture01);

		m_Model->GetMeshes()[1].GetMaterial()->SetTextureOfType(TextureType::ALBEDO, albedoTexture02);
		m_Model->GetMeshes()[1].GetMaterial()->SetTextureOfType(TextureType::NORMAL, normalTexture02);
		m_Model->GetMeshes()[1].GetMaterial()->SetTextureOfType(TextureType::METALLIC_SMOOTHNESS, metallicSmoothTexture02);
		m_Model->GetMeshes()[1].GetMaterial()->SetTextureOfType(TextureType::AMBIENT_OCCLUSION, occlusionTexture02);

		m_Model->GetMeshes()[2].GetMaterial()->SetTextureOfType(TextureType::ALBEDO, albedoTexture03);
		m_Model->GetMeshes()[2].GetMaterial()->SetTextureOfType(TextureType::NORMAL, normalTexture03);
		m_Model->GetMeshes()[2].GetMaterial()->SetTextureOfType(TextureType::METALLIC_SMOOTHNESS, metallicSmoothTexture03);
		m_Model->GetMeshes()[2].GetMaterial()->SetTextureOfType(TextureType::AMBIENT_OCCLUSION, occlusionTexture03);

		m_Model->GetMeshes()[3].GetMaterial()->SetTextureOfType(TextureType::ALBEDO, albedoTexture04);
		m_Model->GetMeshes()[3].GetMaterial()->SetTextureOfType(TextureType::METALLIC_SMOOTHNESS, metallicSmoothTexture04);
		m_Model->GetMeshes()[3].GetMaterial()->SetTextureOfType(TextureType::AMBIENT_OCCLUSION, occlusionTexture04);

		m_Model->GetMeshes()[4].GetMaterial()->SetTextureOfType(TextureType::ALBEDO, albedoTexture05);
		m_Model->GetMeshes()[4].GetMaterial()->SetTextureOfType(TextureType::NORMAL, normalTexture05);
		m_Model->GetMeshes()[4].GetMaterial()->SetTextureOfType(TextureType::METALLIC_SMOOTHNESS, metallicSmoothTexture05);
		m_Model->GetMeshes()[0].GetMaterial()->UseTextures(true);
		m_Model->GetMeshes()[1].GetMaterial()->UseTextures(true);
		m_Model->GetMeshes()[2].GetMaterial()->UseTextures(true);
		m_Model->GetMeshes()[3].GetMaterial()->UseTextures(true);
		m_Model->GetMeshes()[4].GetMaterial()->UseTextures(true);
#endif

		m_CameraController = CreateScope<CameraController>(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	void ApplicationHandler::OnUpdate(Timestep t)
	{
		m_CameraController->OnUpdate(t);
		float time = (float)glfwGetTime();

		// Render to custom framebuffer to render the generated texture in an ImGui Window
		m_CameraController->GetCamera()->SetAspectRatio((float)m_FramebufferSize.x / (float)m_FramebufferSize.y);

		RendererAPI::SetClearColor(m_ClearColor);

		glm::mat4 modelSphere = glm::translate(glm::mat4(1.0f), { 10.0f, 0.0f, 0.0f });
		glm::mat4 modelCube = glm::translate(glm::mat4(1.0f), { 0.0f, 5.0f, 0.0f });
		glm::mat4 modelPyramid = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, -5.0f });

		modelCube = glm::rotate(modelCube, 3.14f, { 1.0f, 0.5f, 0.0f });

		LightInfo lightInfo = { m_LightPos, m_LightColor };
		Renderer::BeginScene(m_CameraController->GetCamera(), m_Cubemap, lightInfo);
		Renderer::Submit(m_Model, 0, 5);
		Renderer::Submit(m_Sphere, modelSphere);
		Renderer::Submit(m_Cube, modelCube);
		Renderer::Submit(m_Pyramid, modelPyramid);
		Renderer::EndScene();

		Renderer::InvertColor();
		Renderer::ColorGrade(m_GradingColor);

		RendererAPI::SetViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	}

	void ApplicationHandler::OnImGuiRender(Timestep t)
	{
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		static bool showDockspace = true;

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

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			i++;
		}
		static glm::vec3 translation = { 1.0f, 1.0f, 1.0f };
		static glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		static glm::vec3 lastRotation = rotation;
		static glm::vec3 scale = { 0.1f, 0.1f, 0.1f };


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
		ImGui::ColorEdit4("Grading Color", (float*)&m_GradingColor);
		ImGui::End();

		m_Model->SetTranslation(translation);
		m_Model->SetRotation(rotation - lastRotation);
		m_Model->SetScale(scale);
		m_Model->CalculateModelMatrix();
		lastRotation = rotation;

		// Render Stats
		ImGui::Begin("Render Stats");

		const RendererStats& stats = Renderer::GetStatistics();
		std::stringstream ss;
		ss << "Frametime: " << t.GetMilliseconds() << ", FPS: " << 1000.0f / t.GetMilliseconds();
		ImGui::Text(ss.str().c_str());
		ss.str(std::string());
		ss << "Vertex Count: " << stats.VertexCount;
		ImGui::Text(ss.str().c_str());
		ss.str(std::string());
		ss << "Face Count: " << stats.FaceCount;
		ImGui::Text(ss.str().c_str());
		ss.str(std::string());
		ss << "Draw Calls: " << stats.DrawCalls;
		ImGui::Text(ss.str().c_str());

		ImGui::End();


		// Viewport (rendered scene)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("Viewport");

		ImVec2 size = ImGui::GetContentRegionAvail();
		if (m_FramebufferSize.x != size.x || m_FramebufferSize.y != size.y && size.x > 0 && size.y > 0)
		{
			Renderer::OnResize((uint32_t)size.x, (uint32_t)size.y);

			m_FramebufferSize.x = size.x;
			m_FramebufferSize.y = size.y;
		}
		ImGui::Image((void*)Renderer::GetFrameTextureId(), size, { 0, 1 }, { 1, 0 });

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
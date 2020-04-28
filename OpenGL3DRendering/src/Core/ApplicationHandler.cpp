#include "ApplicationHandler.h"

#include "Core.h"
#include "Log.h"

#include "Core/Input/Input.h"

#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace OpenGLRendering {

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

	ApplicationHandler* ApplicationHandler::s_Instance = nullptr;

	ApplicationHandler::ApplicationHandler()
		: m_Running(false)
	{
		OGL_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = new Window({ "Niels Pressel" });
		m_Window->SetEventCallback(BIND_EVENT_FN(ApplicationHandler::OnEvent));

#ifdef OGL_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
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
			
			float time = glfwGetTime();
			float lastTime = time;

			m_Running = true;
			while (m_Running)
			{
				float time = glfwGetTime();
				Timestep step = time - lastTime;
				lastTime = time;

				OnUpdate(step);
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

		m_Shader = std::make_unique<Shader>("src/ShaderSource/vertex.glsl", "src/ShaderSource/fragment.glsl");
		m_Shader->Bind();

		m_VertexArray = std::make_unique<VertexArray>();


		std::shared_ptr<OpenGLRendering::VertexBuffer> vertexBuffer = std::make_shared<OpenGLRendering::VertexBuffer>(vertices, sizeof(vertices));
		vertexBuffer->SetLayout(
			{
				{ OpenGLRendering::ShaderDataType::Float3, "a_Position" },
			});

		std::shared_ptr<OpenGLRendering::IndexBuffer> indexBuffer = std::make_shared<OpenGLRendering::IndexBuffer>(indices, 36);

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_CameraController = std::make_unique<CameraController>(glm::vec3(0.0f, 0.0f, 2.0f));

		glEnable(GL_DEPTH_TEST);
		glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void ApplicationHandler::OnUpdate(Timestep t)
	{
		m_CameraController->OnUpdate(t);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (Input::IsMouseButtonPressed(OGL_MOUSE_BUTTON_LEFT))
		{
			m_Shader->SetFloat4("u_Color", { 0.2f, 0.3f, 0.8f, 1.0f });
		}
		else
		{
			m_Shader->SetFloat4("u_Color", { 0.2078f, 0.8078f, 0.902f, 1.0f });
		}

		float time = glfwGetTime();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
		//model = glm::rotate(model, glm::radians(time * 10.0f), { 0.5f, 1.0f, 1.0f });

		glm::mat4 view = m_CameraController->GetCamera().GetViewMatrix();
		glm::mat4 projection = glm::perspective(45.0f, 1.0f * m_Window->GetWidth() / m_Window->GetHeight(), 0.1f, 100.0f);

		glm::mat4 mvp = projection * view * model;

		m_Shader->SetMat4("u_MVP", mvp);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		m_Shader->SetFloat4("u_Color", { 0.8f, 0.2f, 0.3f, 1.0f });
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		m_Window->OnUpdate();
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
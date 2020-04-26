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

			m_Running = true;
			while (m_Running)
			{
				OnUpdate();
			}
		}
	}

	void ApplicationHandler::OnStartup()
	{
		float vertices[] =
		{
			 -0.5, -0.5,  0.5,
			  0.5, -0.5,  0.5,
			  0.5,  0.5,  0.5,
			 -0.5,  0.5,  0.5
		};

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0,
		};

		m_Shader = std::make_unique<Shader>("src/ShaderSource/vertex.glsl", "src/ShaderSource/fragment.glsl");
		m_Shader->Bind();


		m_VertexArray = std::make_unique<VertexArray>();


		std::shared_ptr<OpenGLRendering::VertexBuffer> vertexBuffer = std::make_shared<OpenGLRendering::VertexBuffer>(vertices, sizeof(vertices));
		vertexBuffer->SetLayout(
			{
				{ OpenGLRendering::ShaderDataType::Float3, "a_Position" },
			});

		std::shared_ptr<OpenGLRendering::IndexBuffer> indexBuffer = std::make_shared<OpenGLRendering::IndexBuffer>(indices, 6);

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		glEnable(GL_DEPTH_TEST);
	}

	void ApplicationHandler::OnUpdate()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (Input::IsMouseButtonPressed(OGL_MOUSE_BUTTON_LEFT))
		{
			m_Shader->SetFloat4("u_Color", { 0.2f, 0.3f, 0.8f, 1.0f });
		}
		else
		{
			m_Shader->SetFloat4("u_Color", { 0.8f, 0.2f, 0.3f, 1.0f });
		}
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		m_Window->OnUpdate();
	}

	void ApplicationHandler::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ApplicationHandler::OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(ApplicationHandler::OnWindowClose));

		OGL_TRACE("{0}", event.ToString());
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
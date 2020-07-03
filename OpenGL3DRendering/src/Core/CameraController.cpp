#include "oglpch.h"

#include "CameraController.h"
#include "Input/Input.h"

namespace OpenGLRendering {

	void CameraController::OnUpdate(Timestep t)
	{
		float cameraSpeed = 2.5 * t;


		// Movement
		if (Input::IsKeyPressed(OGL_KEY_W))
		{
			m_CameraPosition += cameraSpeed * m_CameraFront;
		}
		if (Input::IsKeyPressed(OGL_KEY_S))
		{
			m_CameraPosition -= cameraSpeed * m_CameraFront;
		}
		if (Input::IsKeyPressed(OGL_KEY_A))
		{
			m_CameraPosition -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
		}
		if (Input::IsKeyPressed(OGL_KEY_D))
		{
			m_CameraPosition += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
		}
		if (Input::IsKeyPressed(OGL_KEY_C))
		{
			m_CameraPosition += m_CameraUp * cameraSpeed;
		}
		if (Input::IsKeyPressed(OGL_KEY_Z))
		{
			m_CameraPosition -= m_CameraUp * cameraSpeed;
		}
		if (Input::IsKeyPressed(OGL_KEY_Q))
		{
			m_Yaw -= 10.0 * cameraSpeed;
		}
		if (Input::IsKeyPressed(OGL_KEY_E))
		{
			m_Yaw += 10.0 * cameraSpeed;
		}


		// Update underlying camera
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_CameraFront = glm::normalize(front);
		m_Camera->SetCameraFront(m_CameraFront);

		m_Camera->SetCameraPosition(m_CameraPosition);
	}

	void CameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(CameraController::OnMouseMoved));
	}

	void CameraController::LookAtPoint(const glm::vec3& position) // TODO: Fix this function (currently not working as intended)
	{
		m_CameraPosition = { position.x, position.y - 4.0f, position.z + 2.0f };
		m_CameraFront = glm::normalize(m_CameraPosition - position);

		m_Camera->SetCameraPosition(m_CameraPosition);
		m_Camera->SetCameraFront(m_CameraFront);
	}


	bool CameraController::OnMouseMoved(MouseMovedEvent& event)
	{
		// View direction control through mouse movement
#if 0	// disabled in current build
		static bool firstMouse = true;
		static float lastX = 0;
		static float lastY = 0;

		if (firstMouse)
		{
			lastX = event.GetX();
			lastY = event.GetY();
			firstMouse = false;
		}

		float xOffset = event.GetX() - lastX;
		float yOffset = lastY - event.GetY();

		lastX = event.GetX();
		lastY = event.GetY();

		m_Yaw += xOffset * 0.1f;
		m_Pitch += yOffset * 0.1f;

		if (m_Pitch > 89.0f)
		{ 
			m_Pitch = 89.0f;
		}
		if (m_Pitch < -89.0f)
		{
			m_Pitch = -89.0f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_CameraFront = glm::normalize(front);
		m_Camera.SetCameraFront(m_CameraFront);
#endif

		return true;
	}



}

#pragma once

#include "Camera.h"
#include "Event.h"
#include "Timestep.h"

namespace OpenGLRendering {

	class CameraController
	{
	public:
		CameraController(const glm::vec3& position)
			: m_Camera(position), m_CameraPosition(position), m_CameraFront({ 0.0f, 0.0f, -1.0f }), m_CameraUp({ 0.0f, 1.0f, 0.0f }) { }

		void OnUpdate(Timestep t);
		void OnEvent(Event& event);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseMoved(MouseMovedEvent& event);

	private:
		Camera m_Camera;
		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;

		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;
	};

}

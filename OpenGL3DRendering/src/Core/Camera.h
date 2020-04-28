#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OpenGLRendering {

	class Camera
	{
	public:
		Camera(const glm::vec3& position)
			: m_Position(position), m_Front({ 0.0f, 0.0f, -1.0f }), m_Up({ 0.0f, 1.0f, 0.0f }) { }

		glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }

		void SetCameraPosition(const glm::vec3& position) { m_Position = position; }
		void SetCameraFront(const glm::vec3& front) { m_Front = front; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
	};

}
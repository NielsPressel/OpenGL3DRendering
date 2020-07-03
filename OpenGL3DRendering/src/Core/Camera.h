#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OpenGLRendering {


	// Very basic camera class that provides the renderer with the view matrix
	class Camera
	{
	public:
		Camera(const glm::vec3& position)
			: m_Position(position), m_Front({ 0.0f, 0.0f, -1.0f }), m_Up({ 0.0f, 1.0f, 0.0f }), m_AspectRatio(16.0f / 9.0f) { }

		glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
		glm::mat4 GetProjectionMatrix() const { return glm::perspective(45.0f, m_AspectRatio, 0.1f, 100.0f); }
		glm::vec3 GetPosition() const { return m_Position; }

		void SetCameraPosition(const glm::vec3& position) { m_Position = position; }
		void SetCameraFront(const glm::vec3& front) { m_Front = front; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;

		float m_AspectRatio;
	};

}
#pragma once

#include <string>
#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

namespace OpenGLRendering {

	class Shader
	{
	public:
		Shader(const std::string& vertexFile, const std::string& fragmentFile);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int value) const;
		void SetIntArray(const std::string& name, int* values, uint32_t count) const;
		void SetFloat(const std::string& name, float value) const;
		void SetFloat2(const std::string& name, const glm::vec2& value) const;
		void SetFloat3(const std::string& name, const glm::vec3& value) const;
		void SetFloat4(const std::string& name, const glm::vec4& value) const;
		void SetMat3(const std::string& name, const glm::mat3& value) const;
		void SetMat4(const std::string& name, const glm::mat4& value) const;

	private:
		std::string ReadFile(const std::string& filePath);
		void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
	};
}
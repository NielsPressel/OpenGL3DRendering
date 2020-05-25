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

		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat3(const std::string& name, const glm::mat3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
		void SetQuat(const std::string& name, const glm::quat& value);

	private:
		std::string ReadFile(const std::string& filePath);
		void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);
		int32_t GetUniformLocation(const std::string& name);

	private:
		uint32_t m_RendererID;
		std::unordered_map<std::string, int32_t> m_UniformLocations;
	};
}
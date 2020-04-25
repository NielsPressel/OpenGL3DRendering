#pragma once

#include <string>
#include <unordered_map>

namespace OpenGLRendering {

	class Shader
	{
	public:
		Shader(const std::string& vertexFile, const std::string& fragmentFile);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		std::string ReadFile(const std::string& filePath);
		void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
	};
}
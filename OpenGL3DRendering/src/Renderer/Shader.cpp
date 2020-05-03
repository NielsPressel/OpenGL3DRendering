#include "Shader.h"

#include "Core/Log.h"

#include <fstream>
#include <glad/glad.h>
#include <vector>

namespace OpenGLRendering {

	Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = ReadFile(vertexFile);
		shaderSources[GL_FRAGMENT_SHADER] = ReadFile(fragmentFile);

		Compile(shaderSources);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	std::string Shader::ReadFile(const std::string& filePath)
	{
		std::string result;

		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();

			if (size != -1)
			{
				result.resize(size);

				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				OGL_ERROR("Couldn't read from file {0}", filePath);
			}
		}
		else
		{
			OGL_ERROR("Couldn't open file {0}", filePath);
		}

		return result;
	}

	void Shader::Compile(const std::unordered_map<uint32_t, std::string>& shaderSources)
	{
		uint32_t program = glCreateProgram();
		std::array<uint32_t, 2> shaderIDs;
		int shaderIDIndex = 0;

		for (auto& kv : shaderSources)
		{
			uint32_t type = kv.first;
			const std::string& source = kv.second;

			uint32_t shader = glCreateShader(type);

			const char* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				OGL_ERROR("{0}", infoLog.data());
				break;
			}

			glAttachShader(program, shader);

			shaderIDs[shaderIDIndex++] = shader;
		}

		m_RendererID = program;

		glLinkProgram(program);

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (auto id : shaderIDs)
			{
				glDeleteShader(id);
			}

			OGL_ERROR("{0}", infoLog.data());

			return;
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::SetIntArray(const std::string& name, int* values, uint32_t count) const
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value) const
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value) const
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value) const
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& value) const
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}
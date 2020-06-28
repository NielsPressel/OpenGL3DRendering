#pragma once

#include <string>

namespace OpenGLRendering {

	class Cubemap
	{
	public:
		Cubemap(const std::string& filepath);
		~Cubemap();

		void Render() const;

	private:
		void Initialize(const std::string& filepath);

	private:
		uint32_t m_CubemapTextureId;
	};

}
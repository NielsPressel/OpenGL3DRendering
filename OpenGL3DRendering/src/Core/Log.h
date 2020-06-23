#pragma once
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

// Logging helper class that wraps spdlog and defines some logging macros
// Logging is statically accessible everywhere in the code via the precompiled header

// Usage: ONLY use the macros, never use the class directly (except initialization)

namespace OpenGLRendering {

	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};


#define OGL_TRACE(...)		::OpenGLRendering::Log::GetLogger()->trace(__VA_ARGS__)
#define OGL_INFO(...)		::OpenGLRendering::Log::GetLogger()->info(__VA_ARGS__)
#define OGL_WARN(...)		::OpenGLRendering::Log::GetLogger()->warn(__VA_ARGS__)
#define OGL_ERROR(...)		::OpenGLRendering::Log::GetLogger()->error(__VA_ARGS__)
#define OGL_CRITICAL(...)		::OpenGLRendering::Log::GetLogger()->critical(__VA_ARGS__)

}
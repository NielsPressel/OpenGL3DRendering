#include "oglpch.h"

#include "Core/ApplicationHandler.h"

// Entry point

int main()
{
	OpenGLRendering::Log::Init();

	OpenGLRendering::ApplicationHandler handler;
	handler.StartLoop();
}
#include "Core/Log.h"
#include "Core/ApplicationHandler.h"

int main()
{
	OpenGLRendering::Log::Init();

	OpenGLRendering::ApplicationHandler handler;
	handler.StartLoop();
}
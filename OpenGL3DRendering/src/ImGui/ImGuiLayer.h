#pragma once

namespace OpenGLRendering {

	class ImGuiLayer
	{
	public:
		ImGuiLayer() = default;

		void OnStart();
		void OnShutdown();

		void Begin();
		void End();
	};

}
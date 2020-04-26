#pragma once
#include <stdint.h>
#include <ostream>

namespace OpenGLRendering {

	typedef enum class MouseCode : uint16_t
	{
		Button0			= 0,
		Button1			= 1,
		Button2			= 2,
		Button3			= 3,
		Button4			= 4,
		Button5			= 5,
		Button6			= 6,
		Button7			= 7,
		ButtonLast		= Button7,
		ButtonLeft		= Button0,
		ButtonRight		= Button1,
		ButtonMiddle	= Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		return os << static_cast<int32_t>(mouseCode);
	}
}

#define OGL_MOUSE_BUTTON_0			::OpenGLRendering::Mouse::Button0
#define OGL_MOUSE_BUTTON_1			::OpenGLRendering::Mouse::Button1
#define OGL_MOUSE_BUTTON_2			::OpenGLRendering::Mouse::Button2
#define OGL_MOUSE_BUTTON_3			::OpenGLRendering::Mouse::Button3
#define OGL_MOUSE_BUTTON_4			::OpenGLRendering::Mouse::Button4
#define OGL_MOUSE_BUTTON_5			::OpenGLRendering::Mouse::Button5
#define OGL_MOUSE_BUTTON_6			::OpenGLRendering::Mouse::Button6
#define OGL_MOUSE_BUTTON_7			::OpenGLRendering::Mouse::Button7
#define OGL_MOUSE_BUTTON_LAST		::OpenGLRendering::Mouse::ButtonLast
#define OGL_MOUSE_BUTTON_LEFT		::OpenGLRendering::Mouse::ButtonLeft
#define OGL_MOUSE_BUTTON_RIGHT		::OpenGLRendering::Mouse::ButtonRight
#define OGL_MOUSE_BUTTON_MIDDLE		::OpenGLRendering::Mouse::ButtonMiddle
#pragma once
#include <stdint.h>
#include <ostream>

namespace OpenGLRendering {

	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define OGL_KEY_SPACE           ::OpenGLRendering::Key::Space
#define OGL_KEY_APOSTROPHE      ::OpenGLRendering::Key::Apostrophe    /* ' */
#define OGL_KEY_COMMA           ::OpenGLRendering::Key::Comma         /* , */
#define OGL_KEY_MINUS           ::OpenGLRendering::Key::Minus         /* - */
#define OGL_KEY_PERIOD          ::OpenGLRendering::Key::Period        /* . */
#define OGL_KEY_SLASH           ::OpenGLRendering::Key::Slash         /* / */
#define OGL_KEY_0               ::OpenGLRendering::Key::D0
#define OGL_KEY_1               ::OpenGLRendering::Key::D1
#define OGL_KEY_2               ::OpenGLRendering::Key::D2
#define OGL_KEY_3               ::OpenGLRendering::Key::D3
#define OGL_KEY_4               ::OpenGLRendering::Key::D4
#define OGL_KEY_5               ::OpenGLRendering::Key::D5
#define OGL_KEY_6               ::OpenGLRendering::Key::D6
#define OGL_KEY_7               ::OpenGLRendering::Key::D7
#define OGL_KEY_8               ::OpenGLRendering::Key::D8
#define OGL_KEY_9               ::OpenGLRendering::Key::D9
#define OGL_KEY_SEMICOLON       ::OpenGLRendering::Key::Semicolon     /* ; */
#define OGL_KEY_EQUAL           ::OpenGLRendering::Key::Equal         /* = */
#define OGL_KEY_A               ::OpenGLRendering::Key::A
#define OGL_KEY_B               ::OpenGLRendering::Key::B
#define OGL_KEY_C               ::OpenGLRendering::Key::C
#define OGL_KEY_D               ::OpenGLRendering::Key::D
#define OGL_KEY_E               ::OpenGLRendering::Key::E
#define OGL_KEY_F               ::OpenGLRendering::Key::F
#define OGL_KEY_G               ::OpenGLRendering::Key::G
#define OGL_KEY_H               ::OpenGLRendering::Key::H
#define OGL_KEY_I               ::OpenGLRendering::Key::I
#define OGL_KEY_J               ::OpenGLRendering::Key::J
#define OGL_KEY_K               ::OpenGLRendering::Key::K
#define OGL_KEY_L               ::OpenGLRendering::Key::L
#define OGL_KEY_M               ::OpenGLRendering::Key::M
#define OGL_KEY_N               ::OpenGLRendering::Key::N
#define OGL_KEY_O               ::OpenGLRendering::Key::O
#define OGL_KEY_P               ::OpenGLRendering::Key::P
#define OGL_KEY_Q               ::OpenGLRendering::Key::Q
#define OGL_KEY_R               ::OpenGLRendering::Key::R
#define OGL_KEY_S               ::OpenGLRendering::Key::S
#define OGL_KEY_T               ::OpenGLRendering::Key::T
#define OGL_KEY_U               ::OpenGLRendering::Key::U
#define OGL_KEY_V               ::OpenGLRendering::Key::V
#define OGL_KEY_W               ::OpenGLRendering::Key::W
#define OGL_KEY_X               ::OpenGLRendering::Key::X
#define OGL_KEY_Y               ::OpenGLRendering::Key::Y
#define OGL_KEY_Z               ::OpenGLRendering::Key::Z
#define OGL_KEY_LEFT_BRACKET    ::OpenGLRendering::Key::LeftBracket   /* [ */
#define OGL_KEY_BACKSLASH       ::OpenGLRendering::Key::Backslash     /* \ */
#define OGL_KEY_RIGHT_BRACKET   ::OpenGLRendering::Key::RightBracket  /* ] */
#define OGL_KEY_GRAVE_ACCENT    ::OpenGLRendering::Key::GraveAccent   /* ` */
#define OGL_KEY_WORLD_1         ::OpenGLRendering::Key::World1        /* non-US #1 */
#define OGL_KEY_WORLD_2         ::OpenGLRendering::Key::World2        /* non-US #2 */

/* Function keys */
#define OGL_KEY_ESCAPE          ::OpenGLRendering::Key::Escape
#define OGL_KEY_ENTER           ::OpenGLRendering::Key::Enter
#define OGL_KEY_TAB             ::OpenGLRendering::Key::Tab
#define OGL_KEY_BACKSPACE       ::OpenGLRendering::Key::Backspace
#define OGL_KEY_INSERT          ::OpenGLRendering::Key::Insert
#define OGL_KEY_DELETE          ::OpenGLRendering::Key::Delete
#define OGL_KEY_RIGHT           ::OpenGLRendering::Key::Right
#define OGL_KEY_LEFT            ::OpenGLRendering::Key::Left
#define OGL_KEY_DOWN            ::OpenGLRendering::Key::Down
#define OGL_KEY_UP              ::OpenGLRendering::Key::Up
#define OGL_KEY_PAGE_UP         ::OpenGLRendering::Key::PageUp
#define OGL_KEY_PAGE_DOWN       ::OpenGLRendering::Key::PageDown
#define OGL_KEY_HOME            ::OpenGLRendering::Key::Home
#define OGL_KEY_END             ::OpenGLRendering::Key::End
#define OGL_KEY_CAPS_LOCK       ::OpenGLRendering::Key::CapsLock
#define OGL_KEY_SCROLL_LOCK     ::OpenGLRendering::Key::ScrollLock
#define OGL_KEY_NUM_LOCK        ::OpenGLRendering::Key::NumLock
#define OGL_KEY_PRINT_SCREEN    ::OpenGLRendering::Key::PrintScreen
#define OGL_KEY_PAUSE           ::OpenGLRendering::Key::Pause
#define OGL_KEY_F1              ::OpenGLRendering::Key::F1
#define OGL_KEY_F2              ::OpenGLRendering::Key::F2
#define OGL_KEY_F3              ::OpenGLRendering::Key::F3
#define OGL_KEY_F4              ::OpenGLRendering::Key::F4
#define OGL_KEY_F5              ::OpenGLRendering::Key::F5
#define OGL_KEY_F6              ::OpenGLRendering::Key::F6
#define OGL_KEY_F7              ::OpenGLRendering::Key::F7
#define OGL_KEY_F8              ::OpenGLRendering::Key::F8
#define OGL_KEY_F9              ::OpenGLRendering::Key::F9
#define OGL_KEY_F10             ::OpenGLRendering::Key::F10
#define OGL_KEY_F11             ::OpenGLRendering::Key::F11
#define OGL_KEY_F12             ::OpenGLRendering::Key::F12
#define OGL_KEY_F13             ::OpenGLRendering::Key::F13
#define OGL_KEY_F14             ::OpenGLRendering::Key::F14
#define OGL_KEY_F15             ::OpenGLRendering::Key::F15
#define OGL_KEY_F16             ::OpenGLRendering::Key::F16
#define OGL_KEY_F17             ::OpenGLRendering::Key::F17
#define OGL_KEY_F18             ::OpenGLRendering::Key::F18
#define OGL_KEY_F19             ::OpenGLRendering::Key::F19
#define OGL_KEY_F20             ::OpenGLRendering::Key::F20
#define OGL_KEY_F21             ::OpenGLRendering::Key::F21
#define OGL_KEY_F22             ::OpenGLRendering::Key::F22
#define OGL_KEY_F23             ::OpenGLRendering::Key::F23
#define OGL_KEY_F24             ::OpenGLRendering::Key::F24
#define OGL_KEY_F25             ::OpenGLRendering::Key::F25

/* Keypad */
#define OGL_KEY_KP_0            ::OpenGLRendering::Key::KP0
#define OGL_KEY_KP_1            ::OpenGLRendering::Key::KP1
#define OGL_KEY_KP_2            ::OpenGLRendering::Key::KP2
#define OGL_KEY_KP_3            ::OpenGLRendering::Key::KP3
#define OGL_KEY_KP_4            ::OpenGLRendering::Key::KP4
#define OGL_KEY_KP_5            ::OpenGLRendering::Key::KP5
#define OGL_KEY_KP_6            ::OpenGLRendering::Key::KP6
#define OGL_KEY_KP_7            ::OpenGLRendering::Key::KP7
#define OGL_KEY_KP_8            ::OpenGLRendering::Key::KP8
#define OGL_KEY_KP_9            ::OpenGLRendering::Key::KP9
#define OGL_KEY_KP_DECIMAL      ::OpenGLRendering::Key::KPDecimal
#define OGL_KEY_KP_DIVIDE       ::OpenGLRendering::Key::KPDivide
#define OGL_KEY_KP_MULTIPLY     ::OpenGLRendering::Key::KPMultiply
#define OGL_KEY_KP_SUBTRACT     ::OpenGLRendering::Key::KPSubtract
#define OGL_KEY_KP_ADD          ::OpenGLRendering::Key::KPAdd
#define OGL_KEY_KP_ENTER        ::OpenGLRendering::Key::KPEnter
#define OGL_KEY_KP_EQUAL        ::OpenGLRendering::Key::KPEqual

#define OGL_KEY_LEFT_SHIFT      ::OpenGLRendering::Key::LeftShift
#define OGL_KEY_LEFT_CONTROL    ::OpenGLRendering::Key::LeftControl
#define OGL_KEY_LEFT_ALT        ::OpenGLRendering::Key::LeftAlt
#define OGL_KEY_LEFT_SUPER      ::OpenGLRendering::Key::LeftSuper
#define OGL_KEY_RIGHT_SHIFT     ::OpenGLRendering::Key::RightShift
#define OGL_KEY_RIGHT_CONTROL   ::OpenGLRendering::Key::RightControl
#define OGL_KEY_RIGHT_ALT       ::OpenGLRendering::Key::RightAlt
#define OGL_KEY_RIGHT_SUPER     ::OpenGLRendering::Key::RightSuper
#define OGL_KEY_MENU            ::OpenGLRendering::Key::Menu
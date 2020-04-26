#pragma once
#include "Log.h"


#define BIT(x) (1 << x)
#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define OGL_DEBUGBREAK() __debugbreak()
#define OGL_ASSERT(x, ...) { if(!(x)) { OGL_ERROR("Assertion Failed: {0}", __VA_ARGS__); OGL_DEBUGBREAK(); } }
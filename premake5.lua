workspace "OpenGL3DRendering"
	architecture "x86_64"
	startproject "OpenGL3DRendering"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "OpenGL3DRendering/vendor/GLFW/include"
IncludeDir["Glad"] = "OpenGL3DRendering/vendor/Glad/include"
IncludeDir["ImGui"] = "OpenGL3DRendering/vendor/imgui"
IncludeDir["glm"] = "OpenGL3DRendering/vendor/glm"
IncludeDir["stb_image"] = "OpenGL3DRendering/vendor/stb_image"

group "Dependencies"
	include "OpenGL3DRendering/vendor/GLFW"
	include "OpenGL3DRendering/vendor/Glad"
	include "OpenGL3DRendering/vendor/imgui"

project "OpenGL3DRendering"
	location "OpenGL3DRendering"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "OGL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "OGL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "OGL_DIST"
		runtime "Release"
		optimize "on"
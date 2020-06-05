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
IncludeDir["Assimp"] = "OpenGL3DRendering/vendor/assimp/include"

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
	
	pchheader "oglpch.h"
	pchsource "OpenGL3DRendering/src/oglpch.cpp"

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
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.ImGui}"
	}

	

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "OGL_DEBUG"
		runtime "Debug"
		symbols "on"

		libdirs 
		{ 
			"assimp", "%{prj.name}/vendor/assimp_lib/Debug"
		}

		links
		{
			"GLFW",
			"Glad",
			"ImGui",
			"assimp-vc142-mtd.lib",
			"IrrXMLd.lib",
			"zlibstaticd.lib",
			"opengl32.lib"
		}

	filter "configurations:Release"
		defines "OGL_RELEASE"
		runtime "Release"
		optimize "on"

		libdirs 
		{ 
			"assimp", "%{prj.name}/vendor/assimp_lib/Release"
		}

		links
		{
			"GLFW",
			"Glad",
			"ImGui",
			"assimp-vc142-mt.lib",
			"IrrXML.lib",
			"zlibstatic.lib",
			"opengl32.lib"
		}


	filter "configurations:Dist"
		defines "OGL_DIST"
		runtime "Release"
		optimize "on"

		libdirs 
		{ 
			"assimp", "%{prj.name}/vendor/assimp_lib/Release"
		}

		links
		{
			"GLFW",
			"Glad",
			"ImGui",
			"assimp-vc142-mt.lib",
			"IrrXML.lib",
			"zlibstatic.lib",
			"opengl32.lib"
		}
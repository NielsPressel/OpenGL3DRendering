# OpenGL3DRendering
Building a simple 3D-Renderer using OpenGL.

## Third party libraries
* ImGui (https://github.com/ocornut/imgui)
* GLFW (https://github.com/glfw/glfw)
* glm (https://github.com/g-truc/glm)
* Assimp (https://github.com/assimp/assimp)
* spdlog (https://github.com/gabime/spdlog)
* stb_image (https://github.com/nothings/stb/blob/master/stb_image.h)
* glad (https://github.com/Dav1dde/glad)

## Platform
This project is only tested on Windows 10. The only supported architecture is x86_64 (no 32-bit support).

## Build
Building is pretty straight forward because of premake. Just generate your Visual Studio project files. Assimp is not included in the premake build files, so you have to compile this
library yourself and place it into the correct spot described in the main premake file.

## Current state
* 3D-Model loading
* 3D-Model rendering
* PBR Materials
* Editor space using ImGui and integrated viewport displaying the scene
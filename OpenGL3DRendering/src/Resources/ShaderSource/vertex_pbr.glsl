#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TextureCoords;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;

out vec3 v_WorldPos;
out vec2 v_TextureCoords;
out vec3 v_Normal;

void main()
{
	v_TextureCoords = a_TextureCoords;

	v_WorldPos = vec3(u_Model * vec4(a_Position, 1.0));
	v_Normal = mat3(u_Model) * a_Normal;

	gl_Position = u_Projection * u_View * vec4(v_WorldPos, 1.0);
}
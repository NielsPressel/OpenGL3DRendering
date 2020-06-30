#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec3 v_WorldPos;

void main()
{
	v_WorldPos = a_Position;
	gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}
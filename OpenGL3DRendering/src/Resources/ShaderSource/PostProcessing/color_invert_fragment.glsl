#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_Frame;

void main()
{
	vec4 frameColor = texture(u_Frame, v_TexCoords);
	color = vec4(1.0 - frameColor.r, 1.0 - frameColor.g, 1.0 - frameColor.b, 1.0);
}
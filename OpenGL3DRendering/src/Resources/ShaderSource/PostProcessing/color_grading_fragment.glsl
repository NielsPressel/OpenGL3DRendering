#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_Frame;
uniform vec4 u_GradingColor;

void main()
{
	vec4 frameColor = texture(u_Frame, v_TexCoords);
	color = frameColor * u_GradingColor;
}
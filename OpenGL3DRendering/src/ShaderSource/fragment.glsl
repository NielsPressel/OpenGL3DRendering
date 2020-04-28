#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform vec3 u_LightPos;

in vec3 v_FragPos;
in vec3 v_Normal;

void main()
{
	vec4 ambient = 0.1 * vec4(1.0, 1.0, 1.0, 1.0);

	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPos - v_FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

	color = u_Color * (vec4(diffuse, 1.0) + ambient);
}
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_WorldPos;

uniform samplerCube u_EnvironmentMap;

void main()
{
	vec3 envColor = texture(u_EnvironmentMap, v_WorldPos).rgb;

	envColor = envColor / (envColor + vec3(1.0));
	envColor = pow(envColor, vec3(1.0 / 2.2));

	color = vec4(envColor, 1.0);
}
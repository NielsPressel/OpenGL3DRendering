#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_WorldPos;

uniform sampler2D u_EquirectengularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));

	uv *= invAtan;
	uv += 0.5;

	return uv;
}

void main()
{
	vec2 uv = SampleSphericalMap(normalize(v_WorldPos));
	vec3 texColor = texture(u_EquirectengularMap, uv).rgb;

	color = vec4(texColor, 1.0);
}
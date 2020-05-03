#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_TextureDiffuse;
uniform sampler2D u_TextureNormal;

in vec3 v_FragPos;
in vec2 v_TextureCoords;
in vec3 v_TangentLightPos;
in vec3 v_TangentViewPos;
in vec3 v_TangentFragPos;

void main()
{
	vec3 normal = texture(u_TextureNormal, v_TextureCoords).rgb;
	vec3 col = texture(u_TextureDiffuse, v_TextureCoords).rgb;
	vec3 ambient = 0.1 * col;

	vec3 lightDir = normalize(v_TangentLightPos - v_TangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * col;

	vec3 viewDir = normalize(v_TangentViewPos - v_TangentFragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	vec3 specular = vec3(0.2) * spec;
	color = vec4(ambient + diffuse + specular, 1.0);
}
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TextureCoords;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_Projection;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

out vec3 v_FragPos;
out vec2 v_TextureCoords;
out vec3 v_TangentLightPos;
out vec3 v_TangentViewPos;
out vec3 v_TangentFragPos;

void main()
{
	v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
	v_TextureCoords = a_TextureCoords;
	
	mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
	vec3 T = normalize(normalMatrix * a_Tangent);
	vec3 N = normalize(normalMatrix * a_Normal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));
	v_TangentLightPos = TBN * u_LightPos;
	v_TangentViewPos = TBN * u_ViewPos;
	v_TangentFragPos = TBN * v_FragPos;

	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}
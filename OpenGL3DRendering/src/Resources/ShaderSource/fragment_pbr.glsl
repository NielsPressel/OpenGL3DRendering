#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_WorldPos;
in vec2 v_TextureCoords;
in vec3 v_Normal;

uniform sampler2D u_TextureAlbedo;
uniform sampler2D u_TextureNormal;
uniform sampler2D u_TextureMetallicSmooth;
uniform sampler2D u_TextureAmbient;

uniform samplerCube u_IrradianceMap;
uniform samplerCube u_PrefilterMap;
uniform sampler2D u_BrdfLutTexture;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_CameraPos;

const float PI = 3.14159265359;

vec3 GetNormalFromMap()
{
	vec3 tangentNormal = texture(u_TextureNormal, v_TextureCoords).xyz * 2.0 - 1.0;

	vec3 q1 = dFdx(v_WorldPos);
	vec3 q2 = dFdy(v_WorldPos);
	vec2 st1 = dFdx(v_TextureCoords);
	vec2 st2 = dFdy(v_TextureCoords);

	vec3 N = normalize(v_Normal);
	vec3 T = normalize(q1 * st2.t - q2 * st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;

	float denom = NdotV * (1.0 - k) + k;

	return NdotV / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	vec3 albedo = pow(texture(u_TextureAlbedo, v_TextureCoords).rgb, vec3(2.2));
	vec2 metallicSmooth = texture(u_TextureMetallicSmooth, v_TextureCoords).ra;
	float metallic = metallicSmooth.x;
	float roughness = 1.0 - metallicSmooth.y;
	float ao = texture(u_TextureAmbient, v_TextureCoords).r;

	vec3 N = GetNormalFromMap();
	vec3 V = normalize(u_CameraPos - v_WorldPos);
	vec3 R = reflect(-V, N);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);

	vec3 L = normalize(u_LightPos - v_WorldPos);
	vec3 H = normalize(V + L);
	float distance = length(u_LightPos - v_WorldPos) / 3.0;
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = u_LightColor * attenuation;

	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
	vec3 specular = nominator / denominator;

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	float NdotL = max(dot(N, L), 0.0);
	Lo += (kD * albedo / PI + specular) * radiance * NdotL;

	kS = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	kD = 1.0 - kS;
	kD *= 1.0 - metallic;
	vec3 irradiance = texture(u_IrradianceMap, N).rgb;
	vec3 diffuse = irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilteredColor = textureLod(u_PrefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
	vec2 brdf = texture(u_BrdfLutTexture, vec2(max(dot(N, V), 0.0), roughness)).rg;
	specular = prefilteredColor * (F * brdf.x + brdf.y);

	vec3 ambient = (kD * diffuse + specular) * ao;
	
	vec3 col = ambient + Lo;

	col = col / (col + vec3(1.0));
	col = pow(col, vec3(1.0 / 2.2));

	color = vec4(col, 1.0);
}
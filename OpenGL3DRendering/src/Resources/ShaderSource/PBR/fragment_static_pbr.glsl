#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_WorldPos;
in vec3 v_Normal;

uniform vec3 u_Albedo;
uniform float u_Roughness;
uniform float u_Metallic;
uniform float u_Ambient;

uniform samplerCube u_IrradianceMap;
uniform samplerCube u_PrefilterMap;
uniform sampler2D u_BrdfLutTexture;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_CameraPos;

const float PI = 3.14159265359;

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
	vec3 albedo = u_Albedo;
	float metallic = u_Metallic;
	float roughness = u_Roughness;
	float ao = u_Ambient;

	vec3 N = v_Normal;
	vec3 V = normalize(u_CameraPos - v_WorldPos);
	vec3 R = reflect(-V, N);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// light sources
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
	
	// ambient
	F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	kS = F;
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
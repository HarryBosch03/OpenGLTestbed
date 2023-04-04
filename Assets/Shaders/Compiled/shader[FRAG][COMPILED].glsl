#version 450

#line       1        1 
float Square(float v)
{
    return v * v;
}

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
#line      4        0 
#line       1        2 
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 VP;
uniform mat4 MVP;

uniform mat4 VP_I;

uniform float Time;

uniform vec3 CamPosition;

float pi = 3.14159265;
#line      5        0 
#line       1        3 
struct Surface
{
    vec3 albedo;
    vec3 normal;
    vec3 viewDir;
    
    float metallic;
    float roughness;
};
#line      6        0 
#line       1        4 
const int maxDirectionalLights = 4;

layout (std140) uniform DLightData
{
	int DLightCount;
	vec4 DLightDirections[maxDirectionalLights];
	vec4 DLightColors[maxDirectionalLights];
	vec4 AmbientLight;
};

struct DLight
{
	vec3 direction;
	vec3 color;
};

DLight GetDLight (int index)
{
	DLight light;

	light.direction = DLightDirections[index].xyz;
	light.color = DLightColors[index].xyz;

	return light;
}

uniform sampler2D glMap;

vec3 sampleAmbient (vec3 v)
{
    return texture(glMap, SampleSphericalMap(v)).rgb * AmbientLight.rgb;
}
#line      7        0 
#line       1        5 
vec3 FresnelSchlick(float HdotV, vec3 reflection)
{
    return reflection + (1.0 - reflection) * pow(clamp(1.0 - HdotV, 0.0, 1.0), 5.0);
}

float DistributionGGX(float NdotH, float roughness)
{
    float rough2 = roughness * roughness;
    float rough4 = rough2 * rough2;
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (rough4 - 1.0) + 1.0);
    denom = pi * denom * denom;
    return rough4 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    
    float denom = NdotV * (1.0 - k) + k;
    return NdotV / denom;
}

float GeometrySmith(float NdotV, float NdotL, float roughness)
{
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;
}

vec3 GetLighting (Surface surface, DLight light)
{
    float rough = surface.roughness * 0.9 + 0.1;
    vec3 h = normalize(surface.viewDir - light.direction);

    float NdotL = abs(dot(surface.normal, -light.direction));
    float NdotV = abs(dot(surface.normal, surface.viewDir));
    float NdotH = abs(dot(surface.normal, h));
    float HdotV = abs(dot(h, surface.viewDir));

    vec3 radiance = light.color * NdotL;

    vec3 reflection = mix(vec3(0.04), surface.albedo, surface.metallic);
    float NDF = DistributionGGX(NdotH, rough);
    float G = GeometrySmith(NdotV, NdotL, rough);
    vec3 F = FresnelSchlick(HdotV, reflection);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - surface.metallic;

    vec3 num = NDF * G * F;
    float denom = 4.0 * NdotV * NdotL + 0.0001;
    vec3 spec = num / denom;

    return (kD * surface.albedo / pi + spec) * radiance * NdotL;
}

vec3 sampleGL (Surface surface)
{
    DLight light;
    light.direction = -surface.normal;
    light.color = sampleAmbient(surface.normal);

    return GetLighting(surface, light);
}

vec3 GetLighting (Surface surface)
{
    vec3 color = sampleGL(surface);

    for (int i = 0; i < DLightCount; i++)
    {
        DLight light = GetDLight(i);
        color += GetLighting(surface, light);
    }

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    return color;
}
#line      8        0 

out vec4 fragColor;

struct Varyings
{
	vec4 color;
	vec4 normal;
	vec4 position;
	vec2 uv;
	vec4 tangent;
	vec4 bitangent;
};

in Varyings v;

uniform sampler2D texCol;
uniform sampler2D texMetal;
uniform sampler2D texRough;
uniform sampler2D texNormal;
uniform sampler2D texHeight;
uniform sampler2D texAO;

void main ()
{
	vec3 normal = normalize(v.normal.xyz);
	vec3 tangent = normalize(v.tangent.xyz);
	vec3 bitangent = normalize(v.bitangent.xyz);
	mat3 tbn = mat3(tangent, bitangent, normal);

	vec3 fnormal = mix(vec3(0.5, 0.5, 1.0), texture(texNormal, v.uv).rgb, 1.0);
	fnormal = tbn * (fnormal * 2.0 - 1.0);

	Surface surface;
	surface.albedo = texture(texCol, v.uv).rgb * v.color.rgb;
	surface.normal = fnormal;
	surface.viewDir = normalize(CamPosition - v.position.xyz);
	surface.metallic = texture(texMetal, v.uv).r;
	surface.roughness = texture(texRough, v.uv).r;

	float d = dot(surface.normal, surface.viewDir);

	vec3 final = GetLighting(surface);
	final *= texture(texAO, v.uv).r;

	float debug = 0.0;
	fragColor = mix(vec4(final, 1), vec4(0.5, 0.5, 1.0, 1.0), debug);
}

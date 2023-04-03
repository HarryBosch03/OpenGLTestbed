#version 450

float Square(float v)
{
    return v * v;
}
uniform mat4 _Model;
uniform mat4 _View;
uniform mat4 _Projection;
uniform mat4 _VP;
uniform mat4 _MVP;

uniform float _Time;

uniform vec3 _CamPos;

float pi = 3.14159265;
struct Surface
{
    vec3 diffuse;
    vec3 specular;
    vec3 normal;
    vec3 viewDir;
    
    float metallic;
    float roughness;
};
const int maxDirectionalLights = 4;

uniform int dLightCount;
uniform vec3 dLightDirections[maxDirectionalLights];
uniform vec3 dLightColors[maxDirectionalLights];
uniform vec3 ambientLight;

struct DLight
{
	vec3 direction;
	vec3 color;
};

DLight GetDLight (int index)
{
	DLight light;

	light.direction = dLightDirections[index];
	light.color = dLightColors[index];

	return light;
}

float GGX (float ndh, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = ndh * ndh * (a2 - 1.0) + 1.0;
    denom = pi * denom * denom;
    return a2 / max(denom, 0.00001);
}

float geometrySmith (float ndv, float ndl, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float ggx1 = ndv / (ndv * (1.0 - k) + k);
    float ggx2 = ndl / (ndl * (1.0 - k) + k);
    return ggx1 * ggx2;
}

vec3 Fresnel (float hdv, vec3 reflectivity)
{
    return reflectivity + (1.0 - reflectivity) * pow(1.0 - hdv, 5.0);
}

vec3 GetLighting (Surface surface, DLight light, vec3 reflectivity)
{
    vec3 h = normalize(surface.viewDir - light.direction);

    float ndv = max(dot(surface.normal, surface.viewDir), 0.00001);
    float ndl = max(dot(surface.normal, -light.direction), 0.00001);
    float hdv = max(dot(h, surface.viewDir), 0.00001);
    float ndh = max(dot(surface.normal, h), 0.00001);
    
    float d = GGX(ndh, surface.roughness);
    float g = geometrySmith(ndv, ndl, surface.roughness);
    vec3 f = Fresnel(hdv, reflectivity);

    vec3 spec = f * d * g;
    spec /= 4.0 * ndv * ndl;

    vec3 kd = vec3(1.0) - f;
    kd *= 1.0 - surface.metallic;

    return (kd * surface.diffuse / pi + spec) * light.color * ndl;
}

vec3 GetLighting (Surface surface)
{
    vec3 color = ambientLight;
    vec3 reflectivity = mix(vec3(0.04), surface.diffuse, surface.metallic);

    for (int i = 0; i < dLightCount; i++)
    {
        DLight light = GetDLight(i);
        color += GetLighting(surface, light, reflectivity);
    }

    return color;
}

out vec4 fragColor;

in vec4 vColor;
in vec4 vNormal;
in vec4 vPosition;
in vec2 vUV;
in vec4 vTangent;
in vec4 vBitangent;

uniform sampler2D texCol;
uniform sampler2D texMetal;
uniform sampler2D texRough;
uniform sampler2D texNormal;
uniform sampler2D texHeight;
uniform sampler2D texAO;

void main ()
{
	Surface surface;
	surface.diffuse = texture(texCol, vUV).rgb * vColor.rgb;
	surface.specular = vec3(0.0);
	surface.normal = normalize(vNormal.xyz);
	surface.viewDir = normalize(_CamPos - vPosition.xyz);
	surface.metallic = texture(texMetal, vUV).r;
	surface.roughness = texture(texRough, vUV).r;

	vec3 normal = texture(texNormal, vUV).rgb;

	float d = dot(surface.normal, surface.viewDir);

	vec3 final = GetLighting(surface);
	final *= texture(texAO, vUV).r;

	fragColor = vec4(normal.xyz, 1);
}

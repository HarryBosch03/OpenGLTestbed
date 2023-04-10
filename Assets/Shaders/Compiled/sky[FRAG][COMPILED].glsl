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
struct Surface
{
    vec3 albedo;
    vec3 normal;
    vec3 viewDir;
    vec3 position;
    
    float metallic;
    float roughness;
};
#line      5        0 
#line       1        3 
const int maxDirectionalLights = 4;
const int maxLights = 64;

uniform DLightData
{
	int DLightCount;
	vec4 DLightDirections[maxDirectionalLights];
	vec4 DLightColors[maxDirectionalLights];
	vec4 AmbientLight;
};

uniform LightData
{
	int LightCount;
	vec4 LightPositions[maxLights];
	vec4 LightColors[maxLights];
	vec4 LightDirections[maxLights];
};

struct Light
{
	vec3 direction;
	vec3 color;
	float attenuation;
};

Light GetDLight (int index)
{
	Light light;

	light.direction = DLightDirections[index].xyz;
	light.color = DLightColors[index].xyz;
	light.attenuation = 1.0;

	return light;
}

Light GetLight (int index, Surface surface)
{
	Light light;

	vec3 vec = surface.position - LightPositions[index].xyz;
	float l = length(vec);

	light.direction = vec / l;
	light.color = LightColors[index].rgb;
	light.attenuation = 1.0 / (l * l);

	return light;
}

uniform sampler2D glMap;

vec3 sampleAmbient (vec3 v)
{
    return texture(glMap, SampleSphericalMap(v)).rgb * AmbientLight.rgb;
}
#line      6        0 

out vec4 FragColor;

in vec3 localPos;

void main ()
{
    vec3 color = sampleAmbient(normalize(localPos));
    FragColor = vec4(color, 1.0);
}

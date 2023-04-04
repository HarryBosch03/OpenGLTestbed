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
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

	light.direction = -normalize(DLightDirections[index].xyz);
	light.color = DLightColors[index].xyz;
	light.attenuation = 1.0;

	return light;
}

Light GetLight (int index, Surface surface)
{
	Light light;

	vec3 vec = surface.position - LightPositions[index].xyz;
	float l = length(vec);

	light.direction = -normalize(vec) / l;
	light.color = LightColors[index].rgb;
	light.attenuation = 1.0 / (l * l);

	return light;
}

uniform sampler2D glMap;

vec3 sampleAmbient (vec3 v, float mip = 0)
{
    return textureLod(glMap, SampleSphericalMap(v), mip).rgb * AmbientLight.rgb;
}
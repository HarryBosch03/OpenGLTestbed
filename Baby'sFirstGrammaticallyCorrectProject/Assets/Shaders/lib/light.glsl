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
const int maxDirectionalLights = 4;

uniform int DLightCount;
uniform vec3 DLightDirections[maxDirectionalLights];
uniform vec3 DLightColors[maxDirectionalLights];
uniform vec3 AmbientLight;

struct DLight
{
	vec3 direction;
	vec3 color;
};

DLight GetDLight (int index)
{
	DLight light;

	light.direction = DLightDirections[index];
	light.color = DLightColors[index];

	return light;
}
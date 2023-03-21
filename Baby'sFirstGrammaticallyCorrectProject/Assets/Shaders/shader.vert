#version 450

#define MAX_DIRECTIONAL_LIGHTS 4

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

layout (binding = 1) buffer _LightData
{
	vec3 ambientColor;
	int directionalLightCount;
	DirectionalLight directionalLightData[MAX_DIRECTIONAL_LIGHTS];
};

out vec3 light;
out vec3 col;

uniform mat4 _Model;
uniform mat4 _View;
uniform mat4 _Projection;
uniform mat4 _VP;
uniform mat4 _MVP;

uniform vec3 _AmbientCol;

uniform float _Time;

uniform int _DirectionalLightCount;
uniform vec4 _DirectionalLightDirections[MAX_DIRECTIONAL_LIGHTS];
uniform vec4 _DirectionalLightColors[MAX_DIRECTIONAL_LIGHTS];

void main ()
{
	// float li = dot(normal.xyz, -lightDir);
	// light = lightCol * clamp(li, 0, 1) + _AmbientCol;

	light = _AmbientCol;

	vec4 p = position;

	col = normal.xyz;
	col = vec3(1);

	gl_Position = _MVP * p;
}
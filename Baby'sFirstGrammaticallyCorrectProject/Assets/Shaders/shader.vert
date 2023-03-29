#version 450

#include "lib/input.glsl"

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

out vec3 col;
out vec3 worldNormal;
out vec3 worldPos;

void main ()
{
	vec4 p = position;
	worldPos = (_Model * position).xyz;

	col = normal.xyz;
	col = vec3(1);

	worldNormal = (_Model * normal).xyz;

	gl_Position = _MVP * p;
}
#version 450

uniform mat4 _Model;
uniform mat4 _View;
uniform mat4 _Projection;
uniform mat4 _VP;
uniform mat4 _MVP;

uniform float _Time;

uniform vec3 _CamPos;

float pi = 3.14159265;

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
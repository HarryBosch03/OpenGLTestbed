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
layout (location = 2) in vec2 texcoord0;
layout (location = 3) in vec4 tangentIn;

out vec4 vColor;
out vec4 vNormal;
out vec4 vPosition;
out vec2 vUV;
out vec4 vTangent;
out vec4 vBitangent;

void main ()
{
	vec4 p = position;
	vPosition = _Model * position;

	vColor = vec4(1);
	vUV = texcoord0;
	vTangent = _Model * tangentIn;
	vBitangent = cross(vNormal, vTangent) * vTangent.w;

	vNormal = _Model * normal;

	gl_Position = _MVP * p;
}

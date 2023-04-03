#version 450

#line       1        1 
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 VP;
uniform mat4 MVP;

uniform mat4 VP_I;

uniform float Time;

uniform vec3 CamPosition;

float pi = 3.14159265;
#line      4        0 

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texcoord0;
layout (location = 3) in vec4 tangent;

out vec4 vColor;
out vec4 vNormal;
out vec4 vPosition;
out vec2 vUV;
out vec4 vTangent;
out vec4 vBitangent;

void main ()
{
	vec4 p = position;
	vPosition = Model * position;
	vNormal = Model * normal;

	vColor = vec4(1);
	vUV = texcoord0;

	vec3 t = tangent.xyz;
	vTangent = Model * vec4(t, 0.0);
	vBitangent = vec4(cross(vNormal.xyz, vTangent.xyz) * tangent.w, 0.0);

	gl_Position = MVP * p;
}

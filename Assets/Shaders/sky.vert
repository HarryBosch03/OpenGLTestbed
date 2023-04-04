#version 450

#include "lib/input.glsl"

layout (location = 0) in vec3 aPos;

out vec3 localPos;

void main ()
{
    localPos = (VP_I * vec4(aPos, 1.0)).xyz;
    gl_Position = vec4(aPos, 1.0);
}
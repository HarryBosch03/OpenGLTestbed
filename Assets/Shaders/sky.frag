#version 450

#include "lib/common.glsl"
#include "lib/surface.glsl"
#include "lib/light.glsl"

out vec4 FragColor;

in vec3 localPos;

void main ()
{
    vec3 color = sampleAmbient(normalize(localPos));
    FragColor = vec4(color, 1.0);
}
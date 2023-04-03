#version 450

#include "lib/common.glsl"

out vec4 FragColor;

in vec3 localPos;

uniform sampler2D glMap;

void main ()
{
    vec2 uv = SampleSphericalMap(normalize(localPos));
    vec3 color = texture(glMap, uv).rgb;

    FragColor = vec4(color.rgb, 1.0);
}
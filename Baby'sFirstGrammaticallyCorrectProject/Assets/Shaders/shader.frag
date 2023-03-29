#version 450

#include "lib/common.glsl"
#include "lib/input.glsl"
#include "lib/Surface.glsl"
#include "lib/light.glsl"
#include "lib/lighting.glsl"

out vec4 fragColor;

in vec3 col;
in vec3 worldNormal;
in vec3 worldPos;

void main ()
{
	Surface surface;
	surface.diffuse = vec3(1, 1, 1);
	surface.specular = vec3(0.0);
	surface.normal = normalize(worldNormal);
	surface.viewDir = normalize(_CamPos - worldPos);
	surface.metallic = 1.0;
	surface.roughness = 0.6;

	float d = dot(surface.normal, surface.viewDir);

	vec3 final = GetLighting(surface);

	fragColor = vec4(final, 1);
}
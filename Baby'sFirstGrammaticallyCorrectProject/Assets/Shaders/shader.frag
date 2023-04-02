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
in vec2 uv;

uniform sampler2D texCol;
uniform sampler2D texMetal;
uniform sampler2D texRough;
uniform sampler2D texNormal;
uniform sampler2D texHeight;
uniform sampler2D texAO;

void main ()
{
	vec3 temp = texture(texCol, uv).rgb;

	Surface surface;
	surface.diffuse = texture(texCol, uv).rgb;
	surface.specular = vec3(0.0);
	surface.normal = normalize(worldNormal);
	surface.viewDir = normalize(_CamPos - worldPos);
	surface.metallic = texture(texMetal, uv).r;
	surface.roughness = texture(texRough, uv).r;

	float d = dot(surface.normal, surface.viewDir);

	vec3 final = GetLighting(surface);
	final *= texture(texAO, uv).r;

	fragColor = vec4(final, 1);
}
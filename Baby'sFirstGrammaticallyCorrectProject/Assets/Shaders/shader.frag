#version 450

#include "lib/common.glsl"
#include "lib/input.glsl"
#include "lib/Surface.glsl"
#include "lib/light.glsl"
#include "lib/lighting.glsl"

out vec4 fragColor;

in vec4 vColor;
in vec4 vNormal;
in vec4 vPosition;
in vec2 vUV;
in vec4 vTangent;
in vec4 vBitangent;

uniform sampler2D texCol;
uniform sampler2D texMetal;
uniform sampler2D texRough;
uniform sampler2D texNormal;
uniform sampler2D texHeight;
uniform sampler2D texAO;

void main ()
{
	vec3 normal = normalize(vNormal.xyz);
	vec3 tangent = normalize(vTangent.xyz);
	vec3 bitangent = normalize(vBitangent.xyz);
	mat3 tbn = mat3(tangent, bitangent, normal);

	vec3 fnormal = mix(vec3(0.5, 0.5, 1.0), texture(texNormal, vUV).rgb, 1.0);
	fnormal = tbn * (fnormal * 2.0 - 1.0);

	Surface surface;
	surface.albedo = texture(texCol, vUV).rgb * vColor.rgb;
	surface.normal = fnormal;
	surface.viewDir = normalize(CamPosition - vPosition.xyz);
	surface.metallic = texture(texMetal, vUV).r;
	surface.roughness = texture(texRough, vUV).r;

	float d = dot(surface.normal, surface.viewDir);

	vec3 final = GetLighting(surface);
	final *= texture(texAO, vUV).r;

	fragColor = vec4(final, 1);
}
#version 450

#include "lib/common.glsl"
#include "lib/input.glsl"
#include "lib/Surface.glsl"
#include "lib/light.glsl"
#include "lib/lighting.glsl"

out vec4 fragColor;

struct Varyings
{
	vec4 color;
	vec4 normal;
	vec4 position;
	vec2 uv;
	vec4 tangent;
	vec4 bitangent;
};

in Varyings v;

uniform sampler2D texCol;
uniform sampler2D texMetal;
uniform sampler2D texRough;
uniform sampler2D texNormal;
uniform sampler2D texHeight;
uniform sampler2D texAO;

void main ()
{
	vec3 normal = normalize(v.normal.xyz);
	vec3 tangent = normalize(v.tangent.xyz);
	vec3 bitangent = normalize(v.bitangent.xyz);
	mat3 tbn = mat3(tangent, bitangent, normal);

	vec3 fnormal = mix(vec3(0.5, 0.5, 1.0), texture(texNormal, v.uv).rgb, 1.0);
	fnormal = tbn * (fnormal * 2.0 - 1.0);

	Surface surface;
	surface.albedo = texture(texCol, v.uv).rgb * v.color.rgb;
	surface.normal = fnormal;
	surface.viewDir = normalize(CamPosition - v.position.xyz);
	surface.metallic = texture(texMetal, v.uv).r;
	surface.roughness = texture(texRough, v.uv).r;

	float d = dot(surface.normal, surface.viewDir);

	vec3 final = GetLighting(surface);
	final *= texture(texAO, v.uv).r;

	float debug = 0.0;
	fragColor = mix(vec4(final, 1), vec4(0.5, 0.5, 1.0, 1.0), debug);
}
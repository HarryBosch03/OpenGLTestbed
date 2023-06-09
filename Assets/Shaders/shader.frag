#version 450

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

//uniform MaterialProperties
//{
//	vec4 color;
//	vec2 metalness;
//	vec2 roughness;
//	float normal;
//	float height;
//	float ao;
//} properties;

struct MaterialProperties
{
	vec4 color;
	vec2 metalness;
	vec2 roughness;
	float normal;
	float height;
	float ao;
};

void main ()
{
	MaterialProperties properties;
	properties.color = vec4(1.0);
	properties.metalness = vec2(0.0, 1.0);
	properties.roughness = vec2(0.0, 1.0);
	properties.normal = 1.0;
	properties.height = 1.0;
	properties.ao = 0.0;

	vec3 geoNormal = normalize(v.normal.xyz);
	vec3 tangent = normalize(v.tangent.xyz);
	vec3 bitangent = normalize(v.bitangent.xyz);
	mat3 tbn = mat3(tangent, bitangent, geoNormal);

	vec3 texNormal = normalize(tbn * (texture(texNormal, v.uv).rgb * 2.0 - 1.0));
	vec3 normal = mix(geoNormal, texNormal, properties.normal);

	Surface surface;
	surface.albedo = texture(texCol, v.uv).rgb * v.color.rgb * properties.color.rgb;
	surface.normal = normal;
	surface.viewDir = normalize(CamPosition - v.position.xyz);
	surface.position = v.position.xyz;

	surface.metallic = mix(properties.metalness.x, properties.metalness.y, texture(texMetal, v.uv).r);
	surface.roughness = mix(properties.roughness.x, properties.roughness.y, texture(texRough, v.uv).r);

	vec3 final = GetLighting(surface);
	final *= mix(1.0, texture(texAO, v.uv).r, properties.ao);

	fragColor = vec4(final, 1);
}
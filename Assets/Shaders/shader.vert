#version 450

in vec4 position;
in vec4 normal;
in vec2 texcoord0;
in vec4 tangent;

struct Varyings
{
	vec4 color;
	vec4 normal;
	vec4 position;
	vec2 uv;
	vec4 tangent;
	vec4 bitangent;
};

out Varyings v;

void main ()
{
	vec4 p = position;
	v.position = Model * position;
	v.normal = Model * normal;

	v.color = vec4(1);
	v.uv = texcoord0;

	vec3 t = tangent.xyz;
	v.tangent = Model * vec4(t, 0.0);
	v.bitangent = vec4(cross(v.normal.xyz, v.tangent.xyz) * tangent.w, 0.0);

	gl_Position = MVP * p;
}
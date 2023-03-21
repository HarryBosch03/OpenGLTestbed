#version 450

out vec4 fragColor;

in vec3 light;
in vec3 col;

uniform float time;

void main ()
{
	fragColor = vec4(col * light, 1);
}
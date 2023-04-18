#version 450

in vec4 position;

uniform mat4 MVP;

void main ()
{
	vec4 p = position;
	gl_Position = MVP * p;
}

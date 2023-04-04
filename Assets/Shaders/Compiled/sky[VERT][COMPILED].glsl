#version 450

#line       1        1 
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 VP;
uniform mat4 MVP;

uniform mat4 VP_I;

uniform float Time;

uniform vec3 CamPosition;

float pi = 3.14159265;
#line      4        0 

layout (location = 0) in vec3 aPos;

out vec3 localPos;

void main ()
{
    localPos = (VP_I * vec4(aPos, 1.0)).xyz;
    gl_Position = vec4(aPos, 1.0);
}

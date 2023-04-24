#version 450

layout (location = 0) in vec3 aPos;

out vec3 localPos;

void main ()
{
    localPos = (V_I * vec4((P_I * vec4(aPos, 1.0)).xyz, 0.0)).xyz;
    gl_Position = vec4(aPos, 1.0);
}
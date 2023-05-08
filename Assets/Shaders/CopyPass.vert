#version 450

in vec4 position;
in vec4 normal;
in vec2 texcoord0;
in vec4 tangent;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0, 1.0); 
    TexCoords = texcoord0;
}  
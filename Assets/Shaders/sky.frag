#version 450

out vec4 FragColor;

in vec3 localPos;

void main ()
{
    vec3 color = sampleAmbient(normalize(localPos), 0.0);
    FragColor = vec4(color, 1.0);
}
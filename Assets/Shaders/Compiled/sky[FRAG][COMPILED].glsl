#version 450

#line       1        1 
float Square(float v)
{
    return v * v;
}

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
#line      4        0 

out vec4 FragColor;

in vec3 localPos;

uniform sampler2D glMap;

void main ()
{
    vec2 uv = SampleSphericalMap(normalize(localPos));
    vec3 color = texture(glMap, uv).rgb;

    FragColor = vec4(color.rgb, 1.0);
}

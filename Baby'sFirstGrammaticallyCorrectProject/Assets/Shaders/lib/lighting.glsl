
float GGX (float ndh, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = ndh * ndh * (a2 - 1.0) + 1.0;
    denom = pi * denom * denom;
    return a2 / max(denom, 0.00001);
}

float geometrySmith (float ndv, float ndl, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float ggx1 = ndv / (ndv * (1.0 - k) + k);
    float ggx2 = ndl / (ndl * (1.0 - k) + k);
    return ggx1 * ggx2;
}

vec3 Fresnel (float hdv, vec3 reflectivity)
{
    return reflectivity + (1.0 - reflectivity) * pow(1.0 - hdv, 5.0);
}

vec3 GetLighting (Surface surface, DLight light, vec3 reflectivity)
{
    vec3 h = normalize(surface.viewDir - light.direction);

    float ndv = max(dot(surface.normal, surface.viewDir), 0.00001);
    float ndl = max(dot(surface.normal, -light.direction), 0.00001);
    float hdv = max(dot(h, surface.viewDir), 0.00001);
    float ndh = max(dot(surface.normal, h), 0.00001);
    
    float d = GGX(ndh, surface.roughness);
    float g = geometrySmith(ndv, ndl, surface.roughness);
    vec3 f = Fresnel(hdv, reflectivity);

    vec3 spec = f * d * g;
    spec /= 4.0 * ndv * ndl;

    vec3 kd = vec3(1.0) - f;
    kd *= 1.0 - surface.metallic;

    return (kd * surface.diffuse / pi + spec) * light.color * ndl;
}

vec3 GetLighting (Surface surface)
{
    vec3 color = ambientLight;
    vec3 reflectivity = mix(vec3(0.04), surface.diffuse, surface.metallic);

    for (int i = 0; i < dLightCount; i++)
    {
        DLight light = GetDLight(i);
        color += GetLighting(surface, light, reflectivity);
    }

    return color;
}
vec3 FresnelSchlick(float HdotV, vec3 reflection)
{
    return reflection + (1.0 - reflection) * pow(clamp(1.0 - HdotV, 0.0, 1.0), 5.0);
}

float DistributionGGX(float NdotH, float roughness)
{
    float rough2 = roughness * roughness;
    float rough4 = rough2 * rough2;
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (rough4 - 1.0) + 1.0);
    denom = pi * denom * denom;
    return rough4 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    
    float denom = NdotV * (1.0 - k) + k;
    return NdotV / denom;
}

float GeometrySmith(float NdotV, float NdotL, float roughness)
{
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;
}

vec3 GetLighting (Surface surface, DLight light)
{
    float rough = surface.roughness * 0.9 + 0.1;
    vec3 h = normalize(surface.viewDir - light.direction);

    float NdotL = abs(dot(surface.normal, -light.direction));
    float NdotV = abs(dot(surface.normal, surface.viewDir));
    float NdotH = abs(dot(surface.normal, h));
    float HdotV = abs(dot(h, surface.viewDir));

    vec3 radiance = light.color * NdotL;

    vec3 reflection = mix(vec3(0.04), surface.albedo, surface.metallic);
    float NDF = DistributionGGX(NdotH, rough);
    float G = GeometrySmith(NdotV, NdotL, rough);
    vec3 F = FresnelSchlick(HdotV, reflection);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - surface.metallic;

    vec3 num = NDF * G * F;
    float denom = 4.0 * NdotV * NdotL + 0.0001;
    vec3 spec = num / denom;

    return (kD * surface.albedo / pi + spec) * radiance * NdotL;
}

vec3 sampleGL (Surface surface)
{
    DLight light;
    light.direction = -surface.normal;
    light.color = sampleAmbient(surface.normal);

    return GetLighting(surface, light);
}

vec3 GetLighting (Surface surface)
{
    vec3 color = sampleGL(surface);

    for (int i = 0; i < DLightCount; i++)
    {
        DLight light = GetDLight(i);
        color += GetLighting(surface, light);
    }

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    return color;
}
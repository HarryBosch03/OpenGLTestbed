float DistributionGGX(float ndh, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH2 = ndh*ndh;
        
    float num    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);

    denom = pi * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(float ndl, float ndv, float roughness)
{
    float ggx2  = GeometrySchlickGGX(ndv, roughness);
    float ggx1  = GeometrySchlickGGX(ndl, roughness);
	
    return ggx1 * ggx2 + 0.0001;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1 - F0) * (pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0));
}  

vec3 GetLighting (Surface surface, Light light)
{
    float roughness = mix(0.0, 1.0, surface.roughness);

    vec3 h = normalize(surface.viewDir + light.direction);
    vec3 f0 = mix(vec3(0.04), surface.albedo, surface.metallic);

    float ndl = clamp(dot(surface.normal, light.direction), 0.0, 1.0);
    float ndv = clamp(dot(surface.normal, surface.viewDir), 0.0, 1.0);
    float ndh = clamp(dot(surface.normal, h), 0.0, 1.0);

    float ndf = DistributionGGX(ndh, roughness);
    float g = GeometrySmith(ndl, ndv, roughness);
    vec3 f = FresnelSchlick(ndh, f0);
    vec3 kD = vec3(1.0) - f;
        
    kD *= 1.0 - surface.metallic;

    vec3 numerator = ndf * g * f;
    float denominator = 4.0 * ndv * ndl + 0.0001;
    vec3 specular = numerator / denominator;

    return (kD * surface.albedo / pi + specular) * light.color * light.attenuation * ndl;
}

vec3 sampleGL (Surface surface)
{
    Light light;

    light.direction = surface.normal;
    light.color = sampleAmbient(surface.normal, 100.0);
    light.attenuation = 1.0;

    return GetLighting(surface, light);
}

vec3 GetLighting (Surface surface)
{
    vec3 color = sampleGL(surface);

    for (int i = 0; i < DLightCount; i++)
    {
        Light light = GetDLight(i);
        color += GetLighting(surface, light);
    }

    for (int j = 0; j < LightCount; j++)
    {
        Light light = GetLight(0, surface);
        color += GetLighting(surface, light);
    }

    return color;
}
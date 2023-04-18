float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float 
        a      = roughness*roughness,
        a2     = a*a,
        NdotH  = max(dot(N, H), 0.0),
        NdotH2 = NdotH*NdotH,
        
        num    = a2,
        denom  = (NdotH2 * (a2 - 1.0) + 1.0);

    denom = pi * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float 
        r = (roughness + 1.0),
        k = (r*r) / 8.0,

        num   = NdotV,
        denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float 
        NdotV = max(dot(N, V), 0.0),
        NdotL = max(dot(N, L), 0.0),
        ggx2  = GeometrySchlickGGX(NdotV, roughness),
        ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

vec3 GetLighting (Surface surface, Light light)
{
    vec3 
        h = normalize(surface.normal + light.direction),
        f0 = mix(vec3(0.04), surface.albedo, surface.metallic);

    float
        ndf = DistributionGGX(surface.normal, h, surface.roughness),
        g = GeometrySmith(surface.normal, surface.viewDir, light.direction, surface.roughness);

    vec3
        f = FresnelSchlick(max(dot(h, surface.viewDir), 0.0), f0),
        kD = vec3(1.0) - f;
        
    kD *= 1.0 - surface.metallic;

    vec3 numerator = ndf * g * f;
    float denominator = 4.0 * max(dot(surface.normal, surface.viewDir), 0.0) * max(dot(light.direction, surface.normal), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    float ndl = max(dot(surface.normal, light.direction), 0.0);

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
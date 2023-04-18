// Oren-Nayar
vec3 DiffuseLighting(Surface surface, Light light)
{
    float 
        ndl = clamp(dot(surface.normal, light.direction), 0.0, 1.0),
        nde = clamp(dot(surface.normal, surface.viewDir), 0.0, 1.0),

        r2 = surface.roughness * surface.roughness,

        a = 1.0 - 0.5 * r2 / (r2 + 0.33),
        b = 0.45 * r2 / (r2 + 0.09);

    vec3 
        lightProjected = normalize(light.direction - surface.normal * ndl),
        viewProjected = normalize(surface.viewDir - surface.normal * nde);

    float 
        cx = max(0.0, dot(lightProjected, viewProjected)),

        alpha = sin(max(acos(nde), acos(ndl))),
        beta = tan(min(acos(nde), acos(ndl))),
        dx = alpha * beta,

        orenNayar = ndl * (a + b * cx * dx);

    return light.color * light.attenuation * orenNayar * (1.0 - surface.metallic);
}

// Cook Torrance
vec3 SpecularLighting(Surface surface, Light light)
{
    vec3 h = normalize(surface.viewDir + light.direction);

    float
        r = max(surface.roughness, 0.08),
        r2 = r * r,
        ndh = clamp(dot(surface.normal, h), 0.0, 1.0),
        ndh2 = ndh * ndh,
        nde = clamp(dot(surface.normal, surface.viewDir), 0.1, 1.0),
        ndl = clamp(dot(surface.normal, light.direction), 0.1, 1.0),
        e = 2.71828,
        pi = 3.14159,
        reflectionCoefficient = 0.5,

        exponent = -(1 - ndh2) / (ndh2 * r2),
        d = pow(e, exponent) / (r2 * ndh2 * ndh2),

        f = reflectionCoefficient + (1 - reflectionCoefficient) * pow(1 - nde, 5),
        x = 2.0 * ndh / clamp(dot(surface.viewDir, h), 0.001, 1.0),
        g = min(1, min(x * ndl, x * nde)),

        cookTorrance = max((d * g * f) / (nde * pi), 0.0);

    return cookTorrance * light.color * light.attenuation;
}

vec3 GetLighting (Surface surface, Light light)
{
    return surface.albedo * (DiffuseLighting(surface, light) + SpecularLighting(surface, light));
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
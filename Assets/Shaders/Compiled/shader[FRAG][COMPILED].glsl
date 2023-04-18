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
#line       1        2 
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 VP;
uniform mat4 MVP;

uniform mat4 V_I;
uniform mat4 P_I;
uniform mat4 VP_I;

uniform float Time;

uniform vec3 CamPosition;

float pi = 3.14159265;
#line      5        0 
#line       1        3 
struct Surface
{
    vec3 
        albedo,
        normal,
        viewDir,
        position;
    
    float 
        metallic,
        roughness;
};
#line      6        0 
#line       1        4 
const int maxDirectionalLights = 4;
const int maxLights = 64;

uniform DLightData
{
	int DLightCount;
	vec4 DLightDirections[maxDirectionalLights];
	vec4 DLightColors[maxDirectionalLights];
	vec4 AmbientLight;
};

uniform LightData
{
	int LightCount;
	vec4 LightPositions[maxLights];
	vec4 LightColors[maxLights];
	vec4 LightDirections[maxLights];
};

struct Light
{
	vec3 direction;
	vec3 color;
	float attenuation;
};

Light GetDLight (int index)
{
	Light light;

	light.direction = -DLightDirections[index].xyz;
	light.color = DLightColors[index].xyz;
	light.attenuation = 1.0;

	return light;
}

Light GetLight (int index, Surface surface)
{
	Light light;

	vec3 vec = surface.position - LightPositions[index].xyz;
	float l = length(vec);

	light.direction = -vec / l;
	light.color = LightColors[index].rgb;
	light.attenuation = 1.0 / (l * l);

	return light;
}

uniform sampler2D glMap;

vec3 sampleAmbient (vec3 v, float mip = 0)
{
    return textureLod(glMap, SampleSphericalMap(v), mip).rgb * AmbientLight.rgb;
}
#line      7        0 
#line       1        5 
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
#line      8        0 

out vec4 fragColor;

struct Varyings
{
	vec4 color;
	vec4 normal;
	vec4 position;
	vec2 uv;
	vec4 tangent;
	vec4 bitangent;
};

in Varyings v;

uniform sampler2D texCol;
uniform sampler2D texMetal;
uniform sampler2D texRough;
uniform sampler2D texNormal;
uniform sampler2D texHeight;
uniform sampler2D texAO;

uniform MaterialProperties
{
	vec4 color;
	vec2 metalness;
	vec2 roughness;
	float normal;
	float height;
	float ao;
} properties;

void main ()
{
	vec3 normal = normalize(v.normal.xyz);
	vec3 tangent = normalize(v.tangent.xyz);
	vec3 bitangent = normalize(v.bitangent.xyz);
	mat3 tbn = mat3(tangent, bitangent, normal);

	vec3 fnormal = mix(vec3(0.5, 0.5, 1.0), texture(texNormal, v.uv).rgb, properties.normal);
	fnormal = tbn * (fnormal * 2.0 - 1.0);

	Surface surface;
	surface.albedo = texture(texCol, v.uv).rgb * v.color.rgb * properties.color.rgb;
	surface.normal = fnormal;
	surface.viewDir = normalize(CamPosition - v.position.xyz);
	surface.position = v.position.xyz;

	surface.metallic = mix(properties.metalness.x, properties.metalness.y, texture(texMetal, v.uv).r);
	surface.roughness = mix(properties.roughness.x, properties.roughness.y, texture(texRough, v.uv).r);

	vec3 final = GetLighting(surface);
	final *= mix(1.0, texture(texAO, v.uv).r, properties.ao);

	fragColor = vec4(final, 1);
}

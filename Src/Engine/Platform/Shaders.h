#pragma once



namespace Project001
{
    // making this static limit's its scope to its translation unit
    static const char* g_vertexShaderSource01_ = R"(
#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TextureCoordinate;
layout (location = 2) in vec3 in_Normal;
layout (location = 3) in vec4 in_Color;
layout (location = 4) in float in_TextureSlot;
layout (location = 5) in float in_SpecularSlot;
layout (location = 6) in float in_Shininess;
layout (location = 7) in vec3 in_Scale;
layout (location = 8) in vec3 in_Translation;
layout (location = 9) in vec4 in_Orientation;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_FragmentPosition;
out vec2 v_TextureCoordinate;
out vec3 v_Normal;
out vec4 v_Color;
flat out float v_TextureSlot;
flat out float v_SpecularSlot;
flat out float v_Shininess;

void main()
{
    vec3 scaledPosition = vec3(in_Position.x * in_Scale.x, in_Position.y * in_Scale.y, in_Position.z * in_Scale.z);
    vec3 temp = 2 * cross(in_Orientation.xyz, scaledPosition);
    v_FragmentPosition = scaledPosition + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);
    v_FragmentPosition += in_Translation;

    temp = 2 * cross(in_Orientation.xyz, in_Normal);
    v_Normal = in_Normal + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);
    v_Normal = normalize(v_Normal);

    v_Color = in_Color;

    v_TextureCoordinate = in_TextureCoordinate;

    v_TextureSlot = in_TextureSlot;

    v_SpecularSlot = in_SpecularSlot;

    v_Shininess = in_Shininess;

    gl_Position = u_Projection * u_View * vec4(v_FragmentPosition, 1.0);
}
    )";

    static const char* g_fragmentShaderSource01_ = R"(
#version 330 core

#define NUMBER_OF_TEXTURES 16

#define NUMBER_OF_POINT_LIGHTS 8

#define NUMBER_OF_SPOT_LIGHTS 4

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outercutoff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_FragmentPosition;
in vec2 v_TextureCoordinate;
in vec3 v_Normal;
in vec4 v_Color;
flat in float v_TextureSlot;
flat in float v_SpecularSlot;
flat in float v_Shininess;

uniform vec3 u_ViewPosition;

uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight u_SpotLights[NUMBER_OF_SPOT_LIGHTS];

// The type of sample corresponds to the type of texture
uniform sampler2D u_Textures[NUMBER_OF_TEXTURES];

layout (location = 0) out vec4 f_Color;

vec4 GetTextureColor(float textureSlot);
vec4 GetTextureColorAlt(float textureSlot);
float GetDiffuseMultiplier(vec3 normal, vec3 normalizedLightDirection, bool lightingBackFaces);

void main()
{
    vec3 viewDirection = normalize(u_ViewPosition - v_FragmentPosition);

    vec4 textureColor = GetTextureColor(v_TextureSlot);
    vec4 specularColor = GetTextureColor(v_SpecularSlot);

    vec3 totalAmbientAndDiffuseLightMultiplier = vec3(0.0, 0.0, 0.0);
    vec3 totalSpecularLightMultiplier = vec3(0.0, 0.0, 0.0);

    // DirectionalLightColor
    // -------------------------------------------------------------------------
    {
        vec3 normalizedLightDirection = normalize(-1 * u_DirectionalLight.direction);

        float diffuseMultiplier = max(dot(v_Normal, normalizedLightDirection), 0.0);

        vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, v_Normal);

        float specularMultiplier = 0.0;
        if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
        {
            specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
        }

        totalAmbientAndDiffuseLightMultiplier += u_DirectionalLight.ambient;
        totalAmbientAndDiffuseLightMultiplier += u_DirectionalLight.diffuse * diffuseMultiplier;
        totalSpecularLightMultiplier += u_DirectionalLight.specular * specularMultiplier;
    }

    // PointLightColors
    // -------------------------------------------------------------------------
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; ++i)
    {
        vec3 normalizedLightDirection = normalize(u_PointLights[i].position - v_FragmentPosition);

        float diffuseMultiplier = max(dot(v_Normal, normalizedLightDirection), 0.0);

        vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, v_Normal);

        float specularMultiplier = 0.0;
        if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
        {
            specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
        }

        float distanceToFragment = length(u_PointLights[i].position - v_FragmentPosition);
        float attenuation = 1.0 / (u_PointLights[i].constant + u_PointLights[i].linear * distanceToFragment + u_PointLights[i].quadratic * (distanceToFragment * distanceToFragment));

        totalAmbientAndDiffuseLightMultiplier += u_PointLights[i].ambient * attenuation;
        totalAmbientAndDiffuseLightMultiplier += u_PointLights[i].diffuse * diffuseMultiplier* attenuation;
        totalSpecularLightMultiplier += u_PointLights[i].specular * specularMultiplier * attenuation;
    }

    // SpotLightColors
    // -------------------------------------------------------------------------
    for (int i = 0; i < NUMBER_OF_SPOT_LIGHTS; ++i)
    {
        vec3 normalizedLightDirection = normalize(u_SpotLights[i].position - v_FragmentPosition);

        float diffuseMultiplier = max(dot(v_Normal, normalizedLightDirection), 0.0);

        vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, v_Normal);

        float specularMultiplier = 0.0;
        if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
        {
            specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
        }

        float distanceToFragment = length(u_SpotLights[i].position - v_FragmentPosition);
        float attenuation = 1.0 / (u_SpotLights[i].constant + u_SpotLights[i].linear * distanceToFragment + u_SpotLights[i].quadratic * (distanceToFragment * distanceToFragment));

        float theta = dot(normalizedLightDirection, normalize(-1 * u_SpotLights[i].direction));
        float epsilon = u_SpotLights[i].cutoff - u_SpotLights[i].outercutoff;

        float intensity = 0.0;
        if (epsilon > 0.0)
        {
            intensity = clamp((theta - u_SpotLights[i].outercutoff) / epsilon, 0.0, 1.0);
        }
        else if (theta > u_SpotLights[i].cutoff)
        {
            intensity = 1.0;
        }

        totalAmbientAndDiffuseLightMultiplier += u_SpotLights[i].ambient * attenuation * intensity;
        totalAmbientAndDiffuseLightMultiplier += u_SpotLights[i].diffuse * diffuseMultiplier* attenuation * intensity;
        totalSpecularLightMultiplier += u_SpotLights[i].specular * specularMultiplier * attenuation * intensity;
    }

    // result
    // -------------------------------------------------------------------------
    f_Color = textureColor * v_Color * vec4(totalAmbientAndDiffuseLightMultiplier, 1.0) + specularColor * vec4(totalSpecularLightMultiplier, 0.0);

    if (f_Color.a == 0.0)
    {
        discard;
    }
}

vec4 GetTextureColor(float textureSlot)
{
    int textureSlotInt = int(textureSlot);
    if (textureSlotInt < 0)
    {
        return vec4(1.0, 1.0, 1.0, 1.0);
    }
    else if (textureSlot >= NUMBER_OF_TEXTURES)
    {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        return texture(u_Textures[textureSlotInt], v_TextureCoordinate);
    }
}

vec4 GetTextureColorAlt(float textureSlot)
{
    vec4 textureColor = vec4(0.0, 0.0, 0.0, 1.0);
    if (textureSlot > -0.5)
    {
        for (int i = 0; i < NUMBER_OF_TEXTURES; ++i)
        {
            if (textureSlot < float(i) + 0.5)
            {
                textureColor = texture(u_Textures[i], v_TextureCoordinate);
                break;
            }
        }
    }
    else
    {
        textureColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    return textureColor;
}

float GetDiffuseMultiplier(vec3 normal, vec3 normalizedLightDirection, bool lightingBackFaces)
{
    float diffuseMultiplier = dot(normal, normalizedLightDirection);
    if (lightingBackFaces)
    {
        if (diffuseMultiplier < 0.0)
        {
            diffuseMultiplier *= -1.0;
        }
    }
    else
    {
        diffuseMultiplier = max(diffuseMultiplier, 0.0);
    }
    return diffuseMultiplier;
}
    )";
}
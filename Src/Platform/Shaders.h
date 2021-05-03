#pragma once



namespace Project001
{
    const char* g_vertexShaderSource01_ = R"(
#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TextureCoordinate;
layout (location = 2) in vec3 in_Normal;
layout (location = 3) in vec4 in_Color;
layout (location = 4) in float in_TextureIndex;
layout (location = 5) in float in_SpecularIndex;
layout (location = 6) in float in_Shininess;
layout (location = 7) in vec3 in_Scale;
layout (location = 8) in vec3 in_Translation;
layout (location = 9) in vec4 in_Orientation;

uniform mat4 view;
uniform mat4 projection;

out vec3 v_FragmentPosition;
out vec2 v_TextureCoordinate;
out vec3 v_Normal;
out vec4 v_Color;
flat out float v_TextureIndex;
flat out float v_SpecularIndex;
flat out float v_Shininess;

void main()
{
    vec3 scaledPosition = vec3(in_Position.x * in_Scale.x, in_Position.y * in_Scale.y, in_Position.z * in_Scale.z);
    vec3 temp = 2 * cross(in_Orientation.xyz, scaledPosition);
    v_FragmentPosition = scaledPosition + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);
    v_FragmentPosition += in_Translation;

    temp = 2 * cross(in_Orientation.xyz, in_Normal);
    v_Normal = in_Normal + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);

    v_Color = in_Color;

    v_TextureCoordinate = in_TextureCoordinate;

    v_TextureIndex = in_TextureIndex;

    v_SpecularIndex = in_SpecularIndex;

    v_Shininess = in_Shininess;

    gl_Position = projection * view * vec4(v_FragmentPosition, 1.0);
}
    )";

    const char* g_fragmentShaderSource01_ = R"(
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
flat in float v_TextureIndex;
flat in float v_SpecularIndex;
flat in float v_Shininess;

uniform vec3 viewPosition;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLights[NUMBER_OF_SPOT_LIGHTS];

// The type of sample corresponds to the type of texture
uniform sampler2D textures[NUMBER_OF_TEXTURES];

layout (location = 0) out vec4 f_Color;

vec4 GetTextureColor(float textureIndex);

void main()
{
    vec3 normalizedNormal = normalize(v_Normal);
    vec3 viewDirection = normalize(viewPosition - v_FragmentPosition);

    vec4 textureColor = GetTextureColor(v_TextureIndex);
    vec4 specularColor = GetTextureColor(v_SpecularIndex);

    vec3 totalAmbientAndDiffuseLightMultiplier = vec3(0.0, 0.0, 0.0);
    vec3 totalSpecularLightMultiplier = vec3(0.0, 0.0, 0.0);

    // DirectionalLightColor
    // -------------------------------------------------------------------------
    {
        vec3 normalizedLightDirection = normalize(-1 * directionalLight.direction);

        float diffuseMultiplier = max(dot(normalizedNormal, normalizedLightDirection), 0.0);

        vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, normalizedNormal);

        float specularMultiplier = 0.0;
        if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
        {
            specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
        }

        totalAmbientAndDiffuseLightMultiplier += directionalLight.ambient;
        totalAmbientAndDiffuseLightMultiplier += directionalLight.diffuse * diffuseMultiplier;
        totalSpecularLightMultiplier += directionalLight.specular * specularMultiplier;
    }

    // PointLightColors
    // -------------------------------------------------------------------------
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; ++i)
    {
        vec3 normalizedLightDirection = normalize(pointLights[i].position - v_FragmentPosition);

        float diffuseMultiplier = max(dot(normalizedNormal, normalizedLightDirection), 0.0);

        vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, normalizedNormal);

        float specularMultiplier = 0.0;
        if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
        {
            specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
        }

        float distanceToFragment = length(pointLights[i].position - v_FragmentPosition);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distanceToFragment + pointLights[i].quadratic * (distanceToFragment * distanceToFragment));

        totalAmbientAndDiffuseLightMultiplier += pointLights[i].ambient * attenuation;
        totalAmbientAndDiffuseLightMultiplier += pointLights[i].diffuse * diffuseMultiplier* attenuation;
        totalSpecularLightMultiplier += pointLights[i].specular * specularMultiplier * attenuation;
    }

    // SpotLightColors
    // -------------------------------------------------------------------------
    for (int i = 0; i < NUMBER_OF_SPOT_LIGHTS; ++i)
    {
        vec3 normalizedLightDirection = normalize(spotLights[i].position - v_FragmentPosition);

        float diffuseMultiplier = max(dot(normalizedNormal, normalizedLightDirection), 0.0);

        vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, normalizedNormal);

        float specularMultiplier = 0.0;
        if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
        {
            specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
        }

        float distanceToFragment = length(pointLights[i].position - v_FragmentPosition);
        float attenuation = 1.0 / (spotLights[i].constant + spotLights[i].linear * distanceToFragment + spotLights[i].quadratic * (distanceToFragment * distanceToFragment));

        float theta = dot(normalizedLightDirection, normalize(-1 * spotLights[i].direction));
        float epsilon = spotLights[i].cutoff - spotLights[i].outercutoff;

        float intensity = 0.0;
        if (epsilon > 0.0)
        {
            intensity = clamp((theta - spotLights[i].outercutoff) / epsilon, 0.0, 1.0);
        }
        else if (theta > spotLights[i].cutoff)
        {
            intensity = 1.0;
        }

        totalAmbientAndDiffuseLightMultiplier += spotLights[i].ambient * attenuation * intensity;
        totalAmbientAndDiffuseLightMultiplier += spotLights[i].diffuse * diffuseMultiplier* attenuation * intensity;
        totalSpecularLightMultiplier += spotLights[i].specular * specularMultiplier * attenuation * intensity;
    }

    // result
    // -------------------------------------------------------------------------
    f_Color = textureColor * v_Color * vec4(totalAmbientAndDiffuseLightMultiplier, 1.0) + specularColor * vec4(totalSpecularLightMultiplier, 1.0);
}

vec4 GetTextureColor(float textureIndex)
{
    int textureIndexInt = int(textureIndex);
    if (textureIndexInt < 0)
    {
        return vec4(1.0, 1.0, 1.0, 1.0);
    }
    else if (textureIndex >= NUMBER_OF_TEXTURES)
    {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        return texture(textures[textureIndexInt], v_TextureCoordinate);
    }
}

vec4 GetTextureColorAlt(float textureIndex)
{
    vec4 textureColor = vec4(0.0, 0.0, 0.0, 1.0);
    if (textureIndex > -0.5)
    {
        for (int i = 0; i < NUMBER_OF_TEXTURES; ++i)
        {
            if (textureIndex < float(i) + 0.5)
            {
                textureColor = texture(textures[i], v_TextureCoordinate);
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
    )";

        const char* g_fragmentShaderSource02_ = R"(
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
flat in float v_TextureIndex;
flat in float v_SpecularIndex;
flat in float v_Shininess;

uniform vec3 viewPosition;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLights[NUMBER_OF_SPOT_LIGHTS];

// The type of sample corresponds to the type of texture
uniform sampler2D textures[NUMBER_OF_TEXTURES];

layout (location = 0) out vec4 f_Color;

void main()
{
    f_Color = v_Color;
}

    )";
}
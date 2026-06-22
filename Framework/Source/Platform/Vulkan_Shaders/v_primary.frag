#version 450

// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-12-23

const int NUMBER_OF_TEXTURES = 16;

const int NUMBER_OF_POINT_LIGHTS = 8;

const int NUMBER_OF_SPOT_LIGHTS = 4;

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

layout (location = 0) in vec3 v_FragmentPosition;
layout (location = 1) in vec2 v_TextureCoordinate;
layout (location = 2) in vec3 v_Normal;
layout (location = 3) in vec4 v_Color;
layout (location = 4) flat in float v_TextureSlot;
layout (location = 5) flat in float v_SpecularSlot;
layout (location = 6) flat in float v_Shininess;
layout (location = 7) flat in float v_Lit;

layout (binding = 1) uniform FertexShaderUniformBufferObject {
    vec3 viewPosition;
    DirectionalLight directionalLight;
    PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
    SpotLight spotLights[NUMBER_OF_SPOT_LIGHTS];
} u_FSUBO;

layout (binding = 2) uniform sampler2D u_Textures[NUMBER_OF_TEXTURES];

layout (location = 0) out vec4 f_Color;

vec4 GetTextureColor(float textureSlot);

void main()
{
    vec3 viewDirection = normalize(u_FSUBO.viewPosition - v_FragmentPosition);

    vec4 textureColor = GetTextureColor(v_TextureSlot);
    vec4 specularColor = GetTextureColor(v_SpecularSlot);
    
    vec3 totalAmbientAndDiffuseLightMultiplier = vec3(0.0, 0.0, 0.0);
    vec3 totalSpecularLightMultiplier = vec3(0.0, 0.0, 0.0);

    if (v_Lit > 0.5)
    {
        // DirectionalLightColor
        // ---------------------------------------------------------------------
        {
            vec3 normalizedLightDirection = normalize(-1 * u_FSUBO.directionalLight.direction);

            float diffuseMultiplier = max(dot(v_Normal, normalizedLightDirection), 0.0);

            vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, v_Normal);

            float specularMultiplier = 0.0;
            if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
            {
                specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
            }

            totalAmbientAndDiffuseLightMultiplier += u_FSUBO.directionalLight.ambient;
            totalAmbientAndDiffuseLightMultiplier += u_FSUBO.directionalLight.diffuse * diffuseMultiplier;
            totalSpecularLightMultiplier += u_FSUBO.directionalLight.specular * specularMultiplier;
        }

        // PointLightColors
        // ---------------------------------------------------------------------
        for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; ++i)
        {
            vec3 normalizedLightDirection = normalize(u_FSUBO.pointLights[i].position - v_FragmentPosition);

            float diffuseMultiplier = max(dot(v_Normal, normalizedLightDirection), 0.0);

            vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, v_Normal);

            float specularMultiplier = 0.0;
            if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
            {
                specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
            }

            float distanceToFragment = length(u_FSUBO.pointLights[i].position - v_FragmentPosition);
            float attenuation = 1.0 / (u_FSUBO.pointLights[i].constant + u_FSUBO.pointLights[i].linear * distanceToFragment + u_FSUBO.pointLights[i].quadratic * (distanceToFragment * distanceToFragment));

            totalAmbientAndDiffuseLightMultiplier += u_FSUBO.pointLights[i].ambient * attenuation;
            totalAmbientAndDiffuseLightMultiplier += u_FSUBO.pointLights[i].diffuse * diffuseMultiplier* attenuation;
            totalSpecularLightMultiplier += u_FSUBO.pointLights[i].specular * specularMultiplier * attenuation;
        }

        // SpotLightColors
        // ---------------------------------------------------------------------
        for (int i = 0; i < NUMBER_OF_SPOT_LIGHTS; ++i)
        {
            vec3 normalizedLightDirection = normalize(u_FSUBO.spotLights[i].position - v_FragmentPosition);

            float diffuseMultiplier = max(dot(v_Normal, normalizedLightDirection), 0.0);

            vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, v_Normal);

            float specularMultiplier = 0.0;
            if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
            {
                specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
            }

            float distanceToFragment = length(u_FSUBO.spotLights[i].position - v_FragmentPosition);
            float attenuation = 1.0 / (u_FSUBO.spotLights[i].constant + u_FSUBO.spotLights[i].linear * distanceToFragment + u_FSUBO.spotLights[i].quadratic * (distanceToFragment * distanceToFragment));

            float theta = dot(normalizedLightDirection, normalize(-1 * u_FSUBO.spotLights[i].direction));
            float epsilon = u_FSUBO.spotLights[i].cutoff - u_FSUBO.spotLights[i].outercutoff;

            float intensity = 0.0;
            if (epsilon > 0.0)
            {
                intensity = clamp((theta - u_FSUBO.spotLights[i].outercutoff) / epsilon, 0.0, 1.0);
            }
            else if (theta > u_FSUBO.spotLights[i].cutoff)
            {
                intensity = 1.0;
            }

            totalAmbientAndDiffuseLightMultiplier += u_FSUBO.spotLights[i].ambient * attenuation * intensity;
            totalAmbientAndDiffuseLightMultiplier += u_FSUBO.spotLights[i].diffuse * diffuseMultiplier * attenuation * intensity;
            totalSpecularLightMultiplier += u_FSUBO.spotLights[i].specular * specularMultiplier * attenuation * intensity;
        }
    }
    else
    {
        totalAmbientAndDiffuseLightMultiplier = vec3(1.0, 1.0, 1.0);
    }

    // result
    // -------------------------------------------------------------------------
    f_Color = textureColor * v_Color * vec4(totalAmbientAndDiffuseLightMultiplier, 1.0) + specularColor * vec4(totalSpecularLightMultiplier, 0.0);

    // if (f_Color.a == 0.0)
    // {
    //     discard;
    // }
    
    // Gray Scale
    // float averageColor = (color.r + color.g + color.b) / 3.0;
    // f_Color = vec4(averageColor, averageColor, averageColor, 1.0);
}

vec4 GetTextureColor(float textureSlot)
{
    int textureSlotInt = int(textureSlot);
    if (textureSlotInt < 0)
    {
        return vec4(1.0, 1.0, 1.0, 1.0);
    }
    else if (textureSlotInt >= NUMBER_OF_TEXTURES)
    {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        return texture(u_Textures[textureSlotInt], v_TextureCoordinate);
    }
}

#version 450

const int NUMBER_OF_TEXTURES = 16;

const int NUMBER_OF_POINT_LIGHTS = 8;

const int NUMBER_OF_SPOT_LIGHTS = 4;

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
} u_FSUBO;

layout (binding = 2) uniform sampler2D u_Textures[16];

layout (location = 0) out vec4 f_Color;

vec4 GetTextureColor(float textureSlot);

void main()
{
    vec3 viewDirection = normalize(u_FSUBO.viewPosition - v_FragmentPosition);

    vec4 textureColor = GetTextureColor(v_TextureSlot);
    vec4 specularColor = GetTextureColor(v_SpecularSlot);
    
    vec3 totalAmbientAndDiffuseLightMultiplier = vec3(1.0, 1.0, 1.0);
    vec3 totalSpecularLightMultiplier = vec3(0.0, 0.0, 0.0);



    // result
    // -------------------------------------------------------------------------
    f_Color = textureColor * v_Color * vec4(totalAmbientAndDiffuseLightMultiplier, 1.0) + specularColor * vec4(totalSpecularLightMultiplier, 0.0);

    if (f_Color.a == 0.0)
    {
        discard;
    }

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

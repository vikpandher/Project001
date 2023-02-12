#version 450

// layout (binding = 1) uniform sampler2D u_ScreenTexture;

layout (location = 0) in vec3 v_FragmentPosition;
layout (location = 1) in vec2 v_TextureCoordinate;
layout (location = 2) in vec3 v_Normal;
layout (location = 3) in vec4 v_Color;
layout (location = 4) flat in float v_TextureSlot;
layout (location = 5) flat in float v_SpecularSlot;
layout (location = 6) flat in float v_Shininess;
layout (location = 7) flat in float v_Lit;

layout (location = 0) out vec4 f_Color;

void main()
{
    // f_Color = texture(u_ScreenTexture, v_TextureCoordinate);
    f_Color = vec4(1.0, 0.0, 0.0, 1.0);

    // Gray Scale
    // float averageColor = (color.r + color.g + color.b) / 3.0;
    // f_Color = vec4(averageColor, averageColor, averageColor, 1.0);
}

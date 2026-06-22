#version 330 core

// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-12-23

#if defined(GL_ARB_separate_shader_objects)
    #extension GL_ARB_separate_shader_objects : enable
#endif

layout (location = 0) in vec2 v_TextureCoordinate;

uniform sampler2D u_ScreenTexture;

layout (location = 0) out vec4 f_Color;

void main()
{
    f_Color = texture(u_ScreenTexture, v_TextureCoordinate);

    // Gray Scale
    // float averageColor = (color.r + color.g + color.b) / 3.0;
    // f_Color = vec4(averageColor, averageColor, averageColor, 1.0);

    if (f_Color.a == 0.0)
    {
        discard;
    }
}
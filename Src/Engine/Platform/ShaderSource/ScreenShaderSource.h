#pragma once



namespace Project001
{
    namespace ScreenShader
    {
        // ---------------------------------------------------------------------
        static const char* g_vertexShaderSource = R"(
#version 330 core
#if defined(GL_ARB_separate_shader_objects)
    #extension GL_ARB_separate_shader_objects : enable
#endif

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_TextureCoordinate;

layout (location = 0) out vec2 v_TextureCoordinate;

void main()
{
    v_TextureCoordinate = in_TextureCoordinate;

    gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0); 
}
    )";

        // ---------------------------------------------------------------------
        static const char* g_fragmentShaderSource = R"(
#version 330 core
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
        )";
    }
}
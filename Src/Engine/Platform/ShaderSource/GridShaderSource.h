#pragma once



namespace Project001
{
    namespace GridShader
    {
        // ---------------------------------------------------------------------
        static const char* g_vertexShaderSource = R"(
#version 330 core
#if defined(GL_ARB_separate_shader_objects)
    #extension GL_ARB_separate_shader_objects : enable
#endif

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec4 in_Color;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_ViewPosition;

out vec4 v_Color;
out float v_InQuadrant;

void main()
{
    float alphaFade = 1.0;

    if ((u_ViewPosition.x < 0.0 && in_Position.x > 0.0) ||
        (u_ViewPosition.x > 0.0 && in_Position.x < 0.0) ||
        (u_ViewPosition.y < 0.0 && in_Position.y > 0.0) ||
        (u_ViewPosition.y > 0.0 && in_Position.y < 0.0) ||
        (u_ViewPosition.z < 0.0 && in_Position.z > 0.0) ||
        (u_ViewPosition.z > 0.0 && in_Position.z < 0.0))
    {
        alphaFade = 0.2;
        v_InQuadrant = 0.0f;
    }
    else
    {
        v_InQuadrant = 1.0f;
    }

    v_Color = in_Color;
    v_Color.a *= alphaFade;
    gl_Position = u_Projection * u_View * vec4(in_Position, 1.0);
}
        )";

        // ---------------------------------------------------------------------
        static const char* g_fragmentShaderSource = R"(
#version 330 core
#if defined(GL_ARB_separate_shader_objects)
    #extension GL_ARB_separate_shader_objects : enable
#endif

in vec4 v_Color;
in float v_InQuadrant;

layout (location = 0) out vec4 f_Color;

void main()
{
    float zValue = gl_FragCoord.z / gl_FragCoord.w;
    if (zValue < 1.0) // near
    {
        f_Color = v_Color * zValue;
    }
    if (v_InQuadrant < 0.5 && zValue > 1.0) // far
    {
        f_Color = v_Color / zValue;
    }
    else
    {
        f_Color = v_Color;
    }
}
        )";
    }
}
#pragma once

static const char g_o_grid_frag[] = R"(#version 330 core
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
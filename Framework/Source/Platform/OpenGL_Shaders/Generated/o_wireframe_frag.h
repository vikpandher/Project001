#pragma once

static const char g_o_wireframe_frag[] = R"(#version 330 core
#extension GL_ARB_separate_shader_objects : enable

uniform vec4 u_Color;

layout (location = 0) out vec4 f_Color;

void main()
{
    f_Color = vec4(u_Color);
}
)";
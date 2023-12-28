#pragma once

static const char g_o_screen_vert[] = R"(#version 330 core
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
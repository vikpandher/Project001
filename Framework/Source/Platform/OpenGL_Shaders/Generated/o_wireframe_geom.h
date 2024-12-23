#pragma once

static const char g_o_wireframe_geom[] = R"(#version 330 core
#if defined(GL_ARB_separate_shader_objects)
    #extension GL_ARB_separate_shader_objects : enable
#endif

layout(triangles) in;
layout(line_strip, max_vertices=4) out;

void main()
{
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    EndPrimitive();
}
)";
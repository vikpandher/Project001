#version 330 core

// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-12-23

#if defined(GL_ARB_separate_shader_objects)
    #extension GL_ARB_separate_shader_objects : enable
#endif

layout(triangles) in;
layout(line_strip, max_vertices=6) out;

in vec4 v_NormalPosition[];

uniform vec4 u_StartColor;
uniform vec4 u_EndColor;

out vec4 v_Color;

void GenerateLine(int index);

void main()
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}

void GenerateLine(int index)
{
    v_Color = u_StartColor;
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    v_Color = u_EndColor;
    gl_Position = gl_in[index].gl_Position + v_NormalPosition[index];
    EmitVertex();
    EndPrimitive();
}
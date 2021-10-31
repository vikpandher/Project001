#pragma once



namespace Project001
{
    namespace NormalShader
    {
        // ---------------------------------------------------------------------
        static const char* g_vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TextureCoordinate;
layout (location = 2) in vec3 in_Normal;
layout (location = 3) in vec4 in_Color;
layout (location = 4) in float in_TextureSlot;
layout (location = 5) in float in_SpecularSlot;
layout (location = 6) in float in_Shininess;
layout (location = 7) in vec3 in_Scale;
layout (location = 8) in vec3 in_Translation;
layout (location = 9) in vec4 in_Orientation;
layout (location = 10) in float in_Lit;

uniform float u_Magnitude;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 v_NormalPosition;

void main()
{
    vec3 scaledPosition = vec3(in_Position.x * in_Scale.x, in_Position.y * in_Scale.y, in_Position.z * in_Scale.z);
    vec3 temp = 2 * cross(in_Orientation.xyz, scaledPosition);
    vec3 fragmentPosition = scaledPosition + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);
    fragmentPosition += in_Translation;

    temp = 2 * cross(in_Orientation.xyz, in_Normal);
    vec3 normal = in_Normal + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);
    normal = normalize(normal);

    vec3 normalPosition = fragmentPosition + normal * u_Magnitude;

    v_NormalPosition = u_Projection * u_View * vec4(normalPosition, 1.0);

    gl_Position = u_Projection * u_View * vec4(fragmentPosition, 1.0);
}
        )";

        // ---------------------------------------------------------------------
        static const char* g_geometryShaderSource = R"(
#version 330 core

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
        )";

        // ---------------------------------------------------------------------
        static const char* g_fragmentShaderSource = R"(
#version 330 core

in vec4 v_Color;

layout (location = 0) out vec4 f_Color;

void main()
{
    f_Color = vec4(v_Color);
}
        )";
    }
}
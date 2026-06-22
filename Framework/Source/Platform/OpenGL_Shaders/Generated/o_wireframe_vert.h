#pragma once

static const char g_o_wireframe_vert[] = R"(#version 330 core

// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-12-23

#if defined(GL_ARB_separate_shader_objects)
    #extension GL_ARB_separate_shader_objects : enable
#endif

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

uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec3 u_ViewPosition;

void main()
{
    vec3 scaledPosition = vec3(in_Position.x * in_Scale.x, in_Position.y * in_Scale.y, in_Position.z * in_Scale.z);
    vec3 temp = 2 * cross(in_Orientation.xyz, scaledPosition);
    vec3 fragmentPosition = scaledPosition + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);
    fragmentPosition += in_Translation;

    // move mesh slightly closer to the camera so it draws on top
    fragmentPosition += (u_ViewPosition - fragmentPosition) * 0.001;

    gl_Position = u_Projection * u_View * vec4(fragmentPosition, 1.0);
}
)";
#version 330 core

// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-12-23

#if defined(GL_ARB_separate_shader_objects)
    #extension GL_ARB_separate_shader_objects : enable
#endif

in vec4 v_Color;

layout (location = 0) out vec4 f_Color;

void main()
{
    f_Color = vec4(v_Color);
}
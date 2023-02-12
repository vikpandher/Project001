#version 450

layout (binding = 0) uniform UniformBufferObject {
    mat4 u_View;
    mat4 u_Projection;
} ubo;

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

layout (location = 0)  out vec3 v_FragmentPosition;
layout (location = 1)  out vec2 v_TextureCoordinate;
layout (location = 2)  out vec3 v_Normal;
layout (location = 3)  out vec4 v_Color;
layout (location = 4)  flat out float v_TextureSlot;
layout (location = 5)  flat out float v_SpecularSlot;
layout (location = 6)  flat out float v_Shininess;
layout (location = 7)  flat out float v_Lit;

void main()
{
    vec3 scaledPosition = vec3(in_Position.x * in_Scale.x, in_Position.y * in_Scale.y, in_Position.z * in_Scale.z);
    vec3 temp = 2 * cross(in_Orientation.xyz, scaledPosition);
    v_FragmentPosition = scaledPosition + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);
    v_FragmentPosition += in_Translation;

    temp = 2 * cross(in_Orientation.xyz, in_Normal);
    v_Normal = in_Normal + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);
    v_Normal = normalize(v_Normal);

    v_Color = in_Color;

    v_TextureCoordinate = in_TextureCoordinate;

    v_TextureSlot = in_TextureSlot;

    v_SpecularSlot = in_SpecularSlot;

    v_Shininess = in_Shininess;

    v_Lit = in_Lit;

    gl_Position = ubo.u_Projection * ubo.u_View * vec4(v_FragmentPosition, 1.0);
}

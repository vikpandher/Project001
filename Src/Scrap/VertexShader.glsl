#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec4 in_Color;
layout (location = 3) in vec2 in_TextureCoordinate;
layout (location = 4) in float in_TextureIndex;
layout (location = 5) in float in_SpecularIndex;
layout (location = 6) in float in_Shininess;
layout (location = 7) in vec3 in_Translation;
layout (location = 8) in vec4 in_Orientation;

uniform mat4 view;
uniform mat4 projection;

out vec3 v_FragmentPosition;
out vec3 v_Normal;
out vec4 v_Color;
out vec2 v_TextureCoordinate;
flat out float v_TextureIndex;
flat out float v_SpecularIndex;
flat out float v_Shininess;

void main()
{       
    vec3 temp = 2 * cross(in_Orientation.xyz, in_Position);
    v_FragmentPosition = in_Position + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);
    v_FragmentPosition += in_Translation;

    temp = 2 * cross(in_Orientation.xyz, in_Normal);
    v_Normal = in_Normal + in_Orientation.q * temp + cross(in_Orientation.xyz, temp);

    v_Color = in_Color;

    v_TextureCoordinate = in_TextureCoordinate;

    v_TextureIndex = in_TextureIndex;

    v_SpecularIndex = in_SpecularIndex;
    
    v_Shininess = in_Shininess;

    gl_Position = projection * view * vec4(v_FragmentPosition, 1.0);
}
#version 450

layout (location = 0) in vec2 v_TextureCoordinate;

layout (binding = 0) uniform sampler2D u_ScreenTexture;

layout (location = 0) out vec4 f_Color;

void main()
{
    f_Color = texture(u_ScreenTexture, v_TextureCoordinate);

    // Gray Scale
    // float averageColor = (color.r + color.g + color.b) / 3.0;
    // f_Color = vec4(averageColor, averageColor, averageColor, 1.0);

    if (f_Color.a == 0.0)
    {
        discard;
    }
}

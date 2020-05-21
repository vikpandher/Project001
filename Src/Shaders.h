
const char* g_vertexShaderSource01 = R"(#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

out vec3 vertexColor;
out vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(inPos, 1.0);
	vertexColor = inColor;
	textureCoord = vec2(inTexCoord.x, inTexCoord.y);
}
)";

const char* g_fragmentShaderSource01 = R"(#version 330 core

in vec3 vertexColor;
in vec2 textureCoord;

out vec4 fragmentColor;

// The type of sample corresponds to the type of texture
uniform sampler2D texture1;

void main()
{
	fragmentColor = texture(texture1, textureCoord);
}
)";

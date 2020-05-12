
const char* vertexShaderSource = R"(#version 330 core
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

out vec3 vertexColor;
out vec2 textureCoord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(inPos, 1.0);
	vertexColor = inColor;
	textureCoord = vec2(inTexCoord.x, inTexCoord.y);
}
)";

const char* fragmentShaderSource = R"(#version 330 core

in vec3 vertexColor;
in vec2 textureCoord;

out vec4 outFragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures
	outFragColor = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.5);
	// outFragColor = vec4(vertexColor, 1.0); //texture(texture1, inTexCoord);
}
)";
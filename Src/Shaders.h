
const char *vertexShaderSource = "#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
layout(location = 1) in vec3 aColor;\n\
layout(location = 2) in vec2 aTexCoord;\n\
\n\
out vec3 ourColor;\n\
out vec2 TexCoord;\n\
\n\
void main()\n\
{\n\
	gl_Position = vec4(aPos, 1.0);\n\
	ourColor = aColor;\n\
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n\
}";

const char *fragmentShaderSource = "#version 330 core\n\
out vec4 FragColor;\n\
\n\
in vec3 ourColor;\n\
in vec2 TexCoord;\n\
\n\
uniform sampler2D texture1;\n\
uniform sampler2D texture2;\n\
\n\
void main()\n\
{\n\
	// linearly interpolate between both textures\n\
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);\n\
}";
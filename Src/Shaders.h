// textures
// -----------------------------------------------------------------------------
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

// colors
// -----------------------------------------------------------------------------
const char* g_vertexShaderSource02 = R"(#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

const char* g_fragmentShaderSource02 = R"(#version 330 core

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
} 
)";

// lamp
// -----------------------------------------------------------------------------
const char* g_vertexShaderSource03 = R"(#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* g_fragmentShaderSource03 = R"(#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0); // set alle 4 vector values to 1.0
}
)";
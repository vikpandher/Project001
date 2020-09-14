#include "OpenGLRenderer.h"

#include "OpenGLShader.h"

#include "glad/glad.h"

#include "../Engine/Logger.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	OpenGLRenderer::OpenGLRenderer()
	{
		// configure global opengl state
		// ---------------------------------------------------------------------
		// enable using the z buffer
		glEnable(GL_DEPTH_TEST);

		// enable writing to the depth buffer
		glDepthMask(GL_TRUE);

		// draw as wireframe
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		OpenGLShader shader01(s_vertexShaderSource01_, s_fragmentShaderSource01_);

		GLuint vertexArrayId01;
		glGenVertexArrays(1, &vertexArrayId01);

		GLuint vertexBufferId01;
		glGenBuffers(1, &vertexBufferId01);



	}

	// protected ---------------------------------------------------------------

	// private -----------------------------------------------------------------

	const char* OpenGLRenderer::s_vertexShaderSource01_ = R"(#version 330 core

	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;
	layout (location = 2) in vec2 aTexCoords;

	out vec3 FragPos;
	out vec3 Normal;
	out vec2 TexCoords;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		FragPos = vec3(model * vec4(aPos, 1.0));
		Normal = mat3(transpose(inverse(model))) * aNormal;  
		TexCoords = aTexCoords;
    
		gl_Position = projection * view * vec4(FragPos, 1.0);
	}
	)";

	const char* OpenGLRenderer::s_fragmentShaderSource01_ = R"(#version 330 core

	out vec4 FragColor;

	struct Material {
		sampler2D diffuse;
		sampler2D specular;    
		float shininess;
	}; 

	struct Light {
		vec3 position;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};

	in vec3 FragPos;  
	in vec3 Normal;  
	in vec2 TexCoords;
  
	uniform vec3 viewPos;
	uniform Material material;
	uniform Light light;

	void main()
	{
		// ambient
		vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
		// diffuse 
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.position - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0);
	} 
	)";
}
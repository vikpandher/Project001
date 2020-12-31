#include "OpenGLRenderer.h"

#include "OpenGLShader.h"
//^includes "glm/glm.hpp"

#include "OpenGLTexture.h"
//^includes "glm/glm.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "glad/glad.h"

#include "../Engine/Logger.h"



namespace Project001
{
	// public ------------------------------------------------------------------

	OpenGLRenderer* OpenGLRenderer::GetInstance()
	{
		if (s_instance_ == nullptr)
		{
			s_instance_ = new OpenGLRenderer();
		}
		return s_instance_;
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
		delete shaderPtr_;
		delete[] texturePtrs_;
		
		glDeleteBuffers(1, &vertexBufferId_);
		glDeleteVertexArrays(1, &vertexArrayId_);
		glDeleteVertexArrays(1, &indexBufferId_);
	}

	void OpenGLRenderer::AddMesh(const MeshData* meshData)
	{
		const std::vector<glm::vec3>& positions = meshData->positions;
		const std::vector<glm::vec2>& textureCoordinates = meshData->textureCoordinates;
		const std::vector<glm::vec3>& normals = meshData->normals;
		const std::vector<std::vector<IndexGroup>>& faces = meshData->faces;
		
		for (int i = 0; i < faces.size(); ++i)
		{
			for (int j = 0; j < faces[i].size(); ++j)
			{
				const IndexGroup& face = faces[i][j];
				VertexDataStruct newVertexData;
				newVertexData.position = positions[face.positonIndex];
				newVertexData.textureCoordinte = textureCoordinates[face.textureCoordinateIndex];
				newVertexData.normal = normals[face.normalIndex];
				newVertexData.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				newVertexData.textureIndex = -1.0f;
				newVertexData.specularIndex = -1.0f;
				newVertexData.shininess = 0.0f;
				newVertexData.translation = glm::vec3(0.0f, 0.0f, 0.0f);
				newVertexData.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
				indexBuffer_.push_back(vertexBuffer_.size() % s_bufferCapacity_);
				vertexBuffer_.push_back(newVertexData);
			}
		}
	}

	void OpenGLRenderer::AddModel(const ModelData* modelData)
	{
		const MeshData* meshData = modelData->meshDataPtr_;
		const std::vector<glm::vec3>& positions = meshData->positions;
		const std::vector<glm::vec2>& textureCoordinates = meshData->textureCoordinates;
		const std::vector<glm::vec3>& normals = meshData->normals;
		const std::vector<std::vector<IndexGroup>>& faces = meshData->faces;

		const glm::vec4 color = modelData->color_;
		const float textureIndex = modelData->textureIndex_;
		const float specularIndex = modelData->specularIndex_;
		const float shininess = modelData->shininess_;
		const glm::vec3 translation = modelData->position_;
		const glm::quat orientation = modelData->orientation_;

		for (int i = 0; i < faces.size(); ++i)
		{
			for (int j = 0; j < faces[i].size(); ++j)
			{
				const IndexGroup& face = faces[i][j];
				VertexDataStruct newVertexData;
				newVertexData.position = positions[face.positonIndex];
				newVertexData.textureCoordinte = textureCoordinates[face.textureCoordinateIndex];
				newVertexData.normal = normals[face.normalIndex];
				newVertexData.color = color;
				newVertexData.textureIndex = textureIndex;
				newVertexData.specularIndex = specularIndex;
				newVertexData.shininess = shininess;
				newVertexData.translation = translation;
				newVertexData.orientation = orientation;
				indexBuffer_.push_back(vertexBuffer_.size() % s_bufferCapacity_);
				vertexBuffer_.push_back(newVertexData);
			}
		}
	}

	void OpenGLRenderer::AddTexture(const TextureData* textureData, glm::u32 index)
	{
		if (index >=0 && index < s_numberOfTextureSlots_)
		{
			OpenGLTexture*& texturePtr = texturePtrs_[index];
			if (texturePtr != nullptr)
			{
				delete texturePtr;
			}
			texturePtr = new OpenGLTexture(index, textureData->data, textureData->width, textureData->height, textureData->numberOfComponents);
		}
	}

	void OpenGLRenderer::Render()
	{	
		shaderPtr_->SetMat4("view", viewMatrix_);
		shaderPtr_->SetMat4("projection", projectionMatrix_);

		shaderPtr_->SetVec3("viewPosition", viewPosition_);

		// DirectionalLight
		shaderPtr_->SetVec3("directionalLight.direction", directionalLight_.direction);
		shaderPtr_->SetVec3("directionalLight.ambient", directionalLight_.ambient);
		shaderPtr_->SetVec3("directionalLight.diffuse", directionalLight_.diffuse);
		shaderPtr_->SetVec3("directionalLight.specular", directionalLight_.specular);

		// PointLights
		for (int i = 0; i < s_numberOfPointLights_; ++i)
		{
			std::string uniformName;
			uniformName.append("pointLights[");
			uniformName.append(std::to_string(i));
			uniformName.append("]");
			shaderPtr_->SetVec3((uniformName + ".position").c_str(), pointLights_[i].position);
			shaderPtr_->SetFloat((uniformName + ".constant").c_str(), pointLights_[i].constant);
			shaderPtr_->SetFloat((uniformName + ".linear").c_str(), pointLights_[i].linear);
			shaderPtr_->SetFloat((uniformName + ".quadratic").c_str(), pointLights_[i].quadratic);
			shaderPtr_->SetVec3((uniformName + ".ambient").c_str(), pointLights_[i].ambient);
			shaderPtr_->SetVec3((uniformName + ".diffuse").c_str(), pointLights_[i].diffuse);
			shaderPtr_->SetVec3((uniformName + ".specular").c_str(), pointLights_[i].specular);
		}

		// SpotLights
		for (int i = 0; i < s_numberOfSpotLights_; ++i)
		{
			std::string uniformName;
			uniformName.append("spotLights[");
			uniformName.append(std::to_string(i));
			uniformName.append("]");
			shaderPtr_->SetVec3((uniformName + ".position").c_str(), spotLights_[i].position);
			shaderPtr_->SetVec3((uniformName + ".direction").c_str(), spotLights_[i].direction);
			shaderPtr_->SetFloat((uniformName + ".cutOff").c_str(), spotLights_[i].cutOff);
			shaderPtr_->SetFloat((uniformName + ".outerCutOff").c_str(), spotLights_[i].outerCutOff);
			shaderPtr_->SetFloat((uniformName + ".constant").c_str(), spotLights_[i].constant);
			shaderPtr_->SetFloat((uniformName + ".linear").c_str(), spotLights_[i].linear);
			shaderPtr_->SetFloat((uniformName + ".quadratic").c_str(), spotLights_[i].quadratic);
			shaderPtr_->SetVec3((uniformName + ".ambient").c_str(), spotLights_[i].ambient);
			shaderPtr_->SetVec3((uniformName + ".diffuse").c_str(), spotLights_[i].diffuse);
			shaderPtr_->SetVec3((uniformName + ".specular").c_str(), spotLights_[i].specular);
		}
		
		// Bind buffers to make them active
		///glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);
		///glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		unsigned int numberOfVerticiesLeftToDraw = vertexBuffer_.size();
		unsigned int numberOfVerticiesDrawn = 0;
		while(numberOfVerticiesDrawn < vertexBuffer_.size())
		{
			int numberOfVerticiesThatWillBeDrawn = s_bufferCapacity_;
			if (numberOfVerticiesLeftToDraw < numberOfVerticiesThatWillBeDrawn)
			{
				numberOfVerticiesThatWillBeDrawn = numberOfVerticiesLeftToDraw;
			}

			// upload the vertex data and index data into their respective buffers
			// (target, offset, size, data)
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexDataStruct) * numberOfVerticiesThatWillBeDrawn, &vertexBuffer_[numberOfVerticiesDrawn]);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(glm::uint) * numberOfVerticiesThatWillBeDrawn, &indexBuffer_[numberOfVerticiesDrawn]);

			/// glBindVertexArray(vertexArrayId_);
			glDrawElements(GL_TRIANGLES, numberOfVerticiesThatWillBeDrawn, GL_UNSIGNED_INT, 0);

			numberOfVerticiesDrawn += numberOfVerticiesThatWillBeDrawn;
		}
	}

	void OpenGLRenderer::SetViewMatrix(const glm::mat4& viewMatrix)
	{
		viewMatrix_ = viewMatrix;
	}

	void OpenGLRenderer::SetViewPosition(const glm::vec3& viewPosition)
	{
		viewPosition_ = viewPosition;
	}

	void OpenGLRenderer::SetProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		projectionMatrix_ = projectionMatrix;
	}

	void OpenGLRenderer::SetDirectionalLight(const DirectionalLight& light)
	{
		directionalLight_ = light;
	}

	void OpenGLRenderer::SetPointLight(const PointLight& light, glm::u32 index)
	{
		if (index >= 0 && index < s_numberOfPointLights_)
		{
			pointLights_[index] = light;
		}
	}

	void OpenGLRenderer::SetSpotLight(const SpotLight& light, glm::u32 index)
	{
		if (index >= 0 && index < s_numberOfSpotLights_)
		{
			spotLights_[index] = light;
		}
	}

	void OpenGLRenderer::SetViewportSize(int lowerLeftX, int lowerLeftY, int width, int height)
	{
		glViewport(lowerLeftX, lowerLeftY, width, height);
		glScissor(lowerLeftX, lowerLeftY, width, height);
	}

	// private -----------------------------------------------------------------

	OpenGLRenderer::OpenGLRenderer()
		: shaderPtr_(nullptr)
		, texturePtrs_{ nullptr }
		, viewMatrix_(1.0f)
		, viewPosition_(0.0f)
		, projectionMatrix_(1.0f)
		, directionalLight_()
	{
		// configure global opengl state
		// ---------------------------------------------------------------------
		// enable using the z buffer
		glEnable(GL_DEPTH_TEST);

		// enable writing to the depth buffer
		glDepthMask(GL_TRUE);

		// draw as wireframe
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// cull backfaces
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		shaderPtr_ = new OpenGLShader(s_vertexShaderSource01_, s_fragmentShaderSource01_);

		// NOTE:
		// glBindVertex Array doesn't ALWAYS need to come before glBindBuffer,
		// but there are situations when it does.
		// 
		// OpenGL calls are executred in the order they are issued. So
		// glVertexArrtribPointer interprets parameters relative to the
		// currenlty bound buffer.
		// 
		// There is an exception to this. Binding the GL_ELEMENT_ARRAY_BUFFER is
		// saved to the current Vertex Array Object at the time it is bound. The
		// VAO must be bound first.

		// ---------------------------------------------------------------------
		// generate an id (name) for a new vertex array object
		glGenVertexArrays(1, &vertexArrayId_);

		// generate an id (name) for a vertex buffer object
		glGenBuffers(1, &vertexBufferId_);

		// generate an id (name) for the index buffer
		glGenBuffers(1, &indexBufferId_);

		// make the vertex arrat object active, create it if necessary
		glBindVertexArray(vertexArrayId_);

		// make the buffer the active array buffer, creating it if necessary
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);

		// set the size of the active array buffer
		// (target, size, data, usage)
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDataStruct)* s_bufferCapacity_, NULL, GL_DYNAMIC_DRAW);

		// attach the active buffer to the active array with the given attributes
		// (index, size, type, normalized, stride, pointer)

		GLuint positionAttributeIndex = 0;
		GLuint normalAttributeIndex = 1;
		GLuint colorAttributeIndex = 2;
		GLuint textureCoordinateAttributeIndex = 3;
		GLuint textureIndexAttributeIndex = 4;
		GLuint specularIndexAttributeIndex = 5;
		GLuint shininessAttributeIndex = 6;
		GLuint translationAttributeIndex = 7;
		GLuint orientationAttributeIndex = 8;

		unsigned long long attributeOffset = 0;

		glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataStruct), (void*)attributeOffset);
		glEnableVertexAttribArray(positionAttributeIndex);
		attributeOffset += sizeof(glm::vec3);

		glVertexAttribPointer(normalAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataStruct), (void*)attributeOffset);
		glEnableVertexAttribArray(normalAttributeIndex);
		attributeOffset += sizeof(glm::vec3);

		glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexDataStruct), (void*)attributeOffset);
		glEnableVertexAttribArray(colorAttributeIndex);
		attributeOffset += sizeof(glm::vec4);

		glVertexAttribPointer(textureCoordinateAttributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataStruct), (void*)attributeOffset);
		glEnableVertexAttribArray(textureCoordinateAttributeIndex);
		attributeOffset += sizeof(glm::vec2);

		glVertexAttribPointer(textureIndexAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexDataStruct), (void*)attributeOffset);
		glEnableVertexAttribArray(textureIndexAttributeIndex);
		attributeOffset += sizeof(float);

		glVertexAttribPointer(specularIndexAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexDataStruct), (void*)attributeOffset);
		glEnableVertexAttribArray(specularIndexAttributeIndex);
		attributeOffset += sizeof(float);

		glVertexAttribPointer(shininessAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexDataStruct), (void*)attributeOffset);
		glEnableVertexAttribArray(shininessAttributeIndex);
		attributeOffset += sizeof(float);

		glVertexAttribPointer(translationAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataStruct), (void*)attributeOffset);
		glEnableVertexAttribArray(translationAttributeIndex);
		attributeOffset += sizeof(glm::vec3);

		glVertexAttribPointer(orientationAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexDataStruct), (void*)attributeOffset);
		glEnableVertexAttribArray(orientationAttributeIndex);
		// attributeOffset += sizeof(glm::quat);

		// make the buffer the active element array buffer, create it if necessary
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

		// set the size of the active element array buffer
		// (target, size, data, usage)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uint)* s_bufferCapacity_, NULL, GL_DYNAMIC_DRAW);

		// unbind the array buffer and the vertex array
		///glBindBuffer(GL_ARRAY_BUFFER, 0);
		///glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		///glBindVertexArray(0);

		shaderPtr_->Use();


		for (int i = 0; i < s_numberOfTextureSlots_; ++i)
		{
			std::string uniformName;
			uniformName.append("textures[");
			uniformName.append(std::to_string(i));
			uniformName.append("]");
			shaderPtr_->SetInt(uniformName.c_str(), i);
		}
	}

	OpenGLRenderer* OpenGLRenderer::s_instance_ = nullptr;

	// NOTE:
	// gl_Position is intended for writing the homogenous vertex position. It
	// can be written at any time durring vertex shader execution. This value
	// will be used by primitive assembly, clipping, culling and other fixed
	// function operations, if preset, that operate on primitives after vertex
	// processing has occured.

	const char* OpenGLRenderer::s_vertexShaderSource01_ = R"(
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
	)";

	const char* OpenGLRenderer::s_fragmentShaderSource01_ = R"(
#version 330 core

#define NUMBER_OF_TEXTURES 16

#define NUMBER_OF_POINT_LIGHTS 8

#define NUMBER_OF_SPOT_LIGHTS 4

struct DirectionalLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in vec3 v_FragmentPosition;
in vec3 v_Normal;
in vec4 v_Color;
in vec2 v_TextureCoordinate;
flat in float v_TextureIndex;
flat in float v_SpecularIndex;
flat in float v_Shininess;

uniform vec3 viewPosition;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLights[NUMBER_OF_SPOT_LIGHTS];

// The type of sample corresponds to the type of texture
uniform sampler2D textures[NUMBER_OF_TEXTURES];

layout (location = 0) out vec4 f_Color;

vec4 GetTextureColor(float textureIndex);

void main()
{
	vec3 normalizedNormal = normalize(v_Normal);
	vec3 viewDirection = normalize(viewPosition - v_FragmentPosition);

	vec4 textureColor = GetTextureColor(v_TextureIndex);
	vec4 specularColor = GetTextureColor(v_SpecularIndex);

	vec3 totalAmbientAndDiffuseLightMultiplier = vec3(0.0, 0.0, 0.0);
	vec3 totalSpecularLightMultiplier = vec3(0.0, 0.0, 0.0);

	// DirectionalLightColor
	// -------------------------------------------------------------------------
	{
		vec3 normalizedLightDirection = normalize(-1 * directionalLight.direction);

		float diffuseMultiplier = max(dot(normalizedNormal, normalizedLightDirection), 0.0);

		vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, normalizedNormal);

		float specularMultiplier = 0.0;
		if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
		{
			specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
		}

		totalAmbientAndDiffuseLightMultiplier += directionalLight.ambient;
		totalAmbientAndDiffuseLightMultiplier += directionalLight.diffuse * diffuseMultiplier;
		totalSpecularLightMultiplier += directionalLight.specular * specularMultiplier;
	}

	// PointLightColors
	// -------------------------------------------------------------------------
	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; ++i)
	{
		vec3 normalizedLightDirection = normalize(pointLights[i].position - v_FragmentPosition);

		float diffuseMultiplier = max(dot(normalizedNormal, normalizedLightDirection), 0.0);

		vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, normalizedNormal);

		float specularMultiplier = 0.0;
		if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
		{
			specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
		}

		float distanceToFragment = length(pointLights[i].position - v_FragmentPosition);
		float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distanceToFragment + pointLights[i].quadratic * (distanceToFragment * distanceToFragment));

		totalAmbientAndDiffuseLightMultiplier += pointLights[i].ambient * attenuation;
		totalAmbientAndDiffuseLightMultiplier += pointLights[i].diffuse * diffuseMultiplier* attenuation;
		totalSpecularLightMultiplier += pointLights[i].specular * specularMultiplier * attenuation;
	}

	// SpotLightColors
	// -------------------------------------------------------------------------
	for (int i = 0; i < NUMBER_OF_SPOT_LIGHTS; ++i)
	{
		vec3 normalizedLightDirection = normalize(spotLights[i].position - v_FragmentPosition);

		float diffuseMultiplier = max(dot(normalizedNormal, normalizedLightDirection), 0.0);

		vec3 reflectedLightDirection = reflect(-1 * normalizedLightDirection, normalizedNormal);

		float specularMultiplier = 0.0;
		if (diffuseMultiplier > 0.0 && v_Shininess > 0.0)
		{
			specularMultiplier = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), v_Shininess);
		}

		float distanceToFragment = length(pointLights[i].position - v_FragmentPosition);
		float attenuation = 1.0 / (spotLights[i].constant + spotLights[i].linear * distanceToFragment + spotLights[i].quadratic * (distanceToFragment * distanceToFragment));

		float theta = dot(normalizedLightDirection, normalize(-1 * spotLights[i].direction)); 
		float epsilon = spotLights[i].cutOff - spotLights[i].outerCutOff;

		float intensity = 0.0;
		if (epsilon > 0.0)
		{
			intensity = clamp((theta - spotLights[i].outerCutOff) / epsilon, 0.0, 1.0);
		}
		else if (theta > spotLights[i].cutOff)
		{
			intensity = 1.0;
		}

		totalAmbientAndDiffuseLightMultiplier += spotLights[i].ambient * attenuation * intensity;
		totalAmbientAndDiffuseLightMultiplier += spotLights[i].diffuse * diffuseMultiplier* attenuation * intensity;
		totalSpecularLightMultiplier += spotLights[i].specular * specularMultiplier * attenuation * intensity;
	}

	// result
	// -------------------------------------------------------------------------
	f_Color = textureColor * v_Color * vec4(totalAmbientAndDiffuseLightMultiplier, 1.0) + specularColor * vec4(totalSpecularLightMultiplier, 1.0);
}

vec4 GetTextureColor(float textureIndex)
{
	int textureIndexInt = int(textureIndex);
	if (textureIndexInt < 0)
	{
		return vec4(1.0, 1.0, 1.0, 1.0);
	}
	else if (textureIndex >= NUMBER_OF_TEXTURES)
	{
		return vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		return texture(textures[textureIndexInt], v_TextureCoordinate);
	}
}

vec4 GetTextureColorAlt(float textureIndex)
{
	vec4 textureColor = vec4(0.0, 0.0, 0.0, 1.0);
	if (textureIndex > -0.5)
	{
		for (int i = 0; i < NUMBER_OF_TEXTURES; ++i)
		{
			if (textureIndex < float(i) + 0.5)
			{
				textureColor = texture(textures[i], v_TextureCoordinate);
				break;
			}
		}	
	}
	else
	{
		textureColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
	return textureColor;
}
	)";
}
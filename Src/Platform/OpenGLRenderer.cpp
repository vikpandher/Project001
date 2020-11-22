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
	
	OpenGLRenderer::OpenGLRenderer()
		: shaderPtr_(nullptr)
		, texturePtrs_{ nullptr }
		, vertexBufferSize_(0)
		, indexBufferSize_(0)
		, viewMatrix_(1.0f)
		, projectionMatrix_(1.0f)
		, ambiantLightStrength_(0.05f)
		, ambiantLightColor_(1.0f, 1.0f, 1.0f)
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

		vertexBufferPtr_ = new VertexDataStruct[s_vertexBufferCapacity_];
		indexBufferPtr_ = new glm::uint[s_indexBufferCapacity_];

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDataStruct) * s_vertexBufferCapacity_, NULL, GL_DYNAMIC_DRAW);

		// attach the active buffer to the active array with the given attributes
		// (index, size, type, normalized, stride, pointer)

		GLuint positionAttributeIndex = 0;
		GLuint normalAttributeIndex = 1;
		GLuint colorAttributeIndex = 2;
		GLuint textureCoordinateAttributeIndex = 3;
		GLuint textureIndexAttributeIndex = 4;
		GLuint translationAttributeIndex = 5;
		GLuint orientationAttributeIndex = 6;

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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uint) * s_indexBufferCapacity_, NULL, GL_DYNAMIC_DRAW);

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

	OpenGLRenderer::~OpenGLRenderer()
	{
		delete shaderPtr_;
		delete[] texturePtrs_;
		
		glDeleteBuffers(1, &vertexBufferId_);
		glDeleteVertexArrays(1, &vertexArrayId_);
		glDeleteVertexArrays(1, &indexBufferId_);

		delete[] vertexBufferPtr_;
		delete[] indexBufferPtr_;
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
				newVertexData.translation = glm::vec3(0.0f, 0.0f, 0.0f);
				newVertexData.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
				AddIndex(vertexBufferSize_);
				AddVertex(newVertexData);
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
				newVertexData.translation = translation;
				newVertexData.orientation = orientation;
				AddIndex(vertexBufferSize_);
				AddVertex(newVertexData);
			}
		}
	}

	void OpenGLRenderer::AddTexture(const TextureData* textureData, glm::u32 index)
	{
		if (index < 16)
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
		// Bind buffers to make them active
		///glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);
		///glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

		// upload the vertex data and index data into their respective buffers
		// (target, offset, size, data)
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexDataStruct) * vertexBufferSize_, vertexBufferPtr_);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(glm::uint) * indexBufferSize_, indexBufferPtr_);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderPtr_->SetMat4("view", viewMatrix_);
		shaderPtr_->SetMat4("projection", projectionMatrix_);

		shaderPtr_->SetVec3("ambientLightColor", ambiantLightColor_);
		shaderPtr_->SetFloat("ambientLightStrength", ambiantLightStrength_);
		shaderPtr_->SetVec3("pointLightPosition", glm::vec3(1.5f, 1.0f, 2.0f));
		shaderPtr_->SetVec3("pointLightColor", glm::vec3(1.0f, 1.0f, 1.0f));

		/// glBindVertexArray(vertexArrayId_);
		glDrawElements(GL_TRIANGLES, indexBufferSize_, GL_UNSIGNED_INT, 0);
	}

	void OpenGLRenderer::SetAbiantLightColor(const glm::vec3& ambiantLightColor)
	{
		ambiantLightColor_ = ambiantLightColor;
	}

	void OpenGLRenderer::SetAmbiantLightStrength(float ambiantLightStrength)
	{
		ambiantLightStrength_ = ambiantLightStrength;
	}

	void OpenGLRenderer::SetViewMatrix(const glm::mat4& viewMatrix)
	{
		viewMatrix_ = viewMatrix;
	}

	void OpenGLRenderer::SetProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		projectionMatrix_ = projectionMatrix;
	}

	// protected ---------------------------------------------------------------

	// NOTE:
	// gl_Position is intended for writing the homogenous vertex position. It
	// can be written at any time durring vertex shader execution. This value
	// will be used by primitive assembly, clipping, culling and other fixed
	// function operations, if preset, that operate on primitives after vertex
	// processing has occured.

	const char* OpenGLRenderer::s_vertexShaderSource01_ = R"(#version 330 core

	layout (location = 0) in vec3 in_Position;
	layout (location = 1) in vec3 in_Normal;
	layout (location = 2) in vec4 in_Color;
	layout (location = 3) in vec2 in_TextureCoordinate;
	layout (location = 4) in float in_TextureIndex;
	layout (location = 5) in vec3 in_Translation;
	layout (location = 6) in vec4 in_Orientation;

	uniform mat4 view;
	uniform mat4 projection;

	out vec3 v_FragmentPosition;
	out vec3 v_Normal;
	out vec4 v_Color;
	out vec2 v_TextureCoordinate;
	flat out float v_TextureIndex;

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

		gl_Position = projection * view * vec4(v_FragmentPosition, 1.0);
	}
	)";

	const char* OpenGLRenderer::s_fragmentShaderSource01_ = R"(#version 330 core

	in vec3 v_FragmentPosition;
	in vec3 v_Normal;
	in vec4 v_Color;
	in vec2 v_TextureCoordinate;
	flat in float v_TextureIndex;
	
	uniform vec3 ambientLightColor;
	uniform float ambientLightStrength;

	uniform vec3 pointLightPosition; 
	uniform vec3 pointLightColor;

	// The type of sample corresponds to the type of texture
	uniform sampler2D textures[16];

	layout (location = 0) out vec4 f_Color;

	void main()
	{
		// ambient
		// ---------------------------------------------------------------------
		vec3 ambientColor = ambientLightStrength * ambientLightColor;

		// diffuse
		// ---------------------------------------------------------------------
		vec3 pointLightDirection = normalize(pointLightPosition - v_FragmentPosition);
		float diffuseStrength = max(dot(v_Normal, pointLightDirection), 0.0);
		vec3 diffuseColor = diffuseStrength * pointLightColor;

		// texture
		// ---------------------------------------------------------------------
		vec4 textureColor;
		if (v_TextureIndex < -0.5)
		{
			textureColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
		else if (v_TextureIndex < 0.5)
		{
			textureColor = texture(textures[0], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 1.5)
		{
			textureColor = texture(textures[1], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 2.5)
		{
			textureColor = texture(textures[2], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 3.5)
		{
			textureColor = texture(textures[3], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 4.5)
		{
			textureColor = texture(textures[4], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 5.5)
		{
			textureColor = texture(textures[5], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 6.5)
		{
			textureColor = texture(textures[6], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 7.5)
		{
			textureColor = texture(textures[7], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 8.5)
		{
			textureColor = texture(textures[8], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 9.5)
		{
			textureColor = texture(textures[9], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 10.5)
		{
			textureColor = texture(textures[10], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 11.5)
		{
			textureColor = texture(textures[11], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 12.5)
		{
			textureColor = texture(textures[12], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 13.5)
		{
			textureColor = texture(textures[13], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 14.5)
		{
			textureColor = texture(textures[14], v_TextureCoordinate);
		}
		else if (v_TextureIndex < 15.5)
		{
			textureColor = texture(textures[15], v_TextureCoordinate);
		}
		else
		{
			textureColor = vec4(0.0, 0.0, 0.0, 1.0);
		}
		
		// result
		// ---------------------------------------------------------------------

		f_Color = textureColor * vec4(ambientColor + diffuseColor, 1.0f) * v_Color;
	} 
	)";
}
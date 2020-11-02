#include "OpenGLRenderer.h"

#include "glm/gtc/quaternion.hpp"

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
		, texturePtrs_{nullptr}
		, vertexBufferSize_(0)
		, indexBufferSize_(0)
		, modelMatrix_(1.0f)
		, viewMatrix_(1.0f)
		, projectionMatrix_(1.0f)
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

		glVertexAttribPointer(textureIndexAttributeIndex, 1, GL_INT, GL_FALSE, sizeof(VertexDataStruct), (void*)attributeOffset);
		glEnableVertexAttribArray(textureIndexAttributeIndex);
		// attributeOffset += sizeof(glm::int32);

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

		shaderPtr_->SetInt("texture00", 0);
		shaderPtr_->SetInt("texture01", 1);
		shaderPtr_->SetInt("texture02", 2);
		shaderPtr_->SetInt("texture03", 3);
		shaderPtr_->SetInt("texture04", 4);
		shaderPtr_->SetInt("texture05", 5);
		shaderPtr_->SetInt("texture06", 6);
		shaderPtr_->SetInt("texture07", 7);
		shaderPtr_->SetInt("texture08", 8);
		shaderPtr_->SetInt("texture09", 9);
		shaderPtr_->SetInt("texture10", 10);
		shaderPtr_->SetInt("texture11", 11);
		shaderPtr_->SetInt("texture12", 12);
		shaderPtr_->SetInt("texture13", 13);
		shaderPtr_->SetInt("texture14", 14);
		shaderPtr_->SetInt("texture15", 15);
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

	void OpenGLRenderer::AddMesh(const MeshData* mesh)
	{
		const std::vector<glm::vec3>& positions = mesh->positions;
		const std::vector<glm::vec2>& textureCoordinates = mesh->textureCoordinates;
		const std::vector<glm::vec3>& normals = mesh->normals;
		const std::vector<std::vector<IndexGroup>>& faces = mesh->faces;
		
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

		shaderPtr_->SetMat4("model", modelMatrix_);
		shaderPtr_->SetMat4("view", viewMatrix_);
		shaderPtr_->SetMat4("projection", projectionMatrix_);

		shaderPtr_->SetFloat("ambientStrength", 0.5f);
		shaderPtr_->SetVec3("lightPosition", glm::vec3(1.2f, 1.0f, 2.0f));
		shaderPtr_->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

		/// glBindVertexArray(vertexArrayId_);
		glDrawElements(GL_TRIANGLES, indexBufferSize_, GL_UNSIGNED_INT, 0);
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

	const unsigned int OpenGLRenderer::s_vertexBufferCapacity_ = 99;
	const unsigned int OpenGLRenderer::s_indexBufferCapacity_ = 99;

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
	layout (location = 4) in int in_TextureIndex;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	out vec3 v_FragmentPosition;
	out vec3 v_Normal;
	out vec4 v_Color;
	out vec2 v_TextureCoordinate;
	out int v_TextureIndex;

	void main()
	{   
		v_FragmentPosition = vec3(model * vec4(in_Position, 1.0));
		v_Normal = in_Normal;
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
	in int v_TextureIndex;

	uniform float ambientStrength;
	uniform vec3 lightPosition; 
	uniform vec3 lightColor;

	// The type of sample corresponds to the type of texture
	uniform sampler2D texture00;
	uniform sampler2D texture01;
	uniform sampler2D texture02;
	uniform sampler2D texture03;
	uniform sampler2D texture04;
	uniform sampler2D texture05;
	uniform sampler2D texture06;
	uniform sampler2D texture07;
	uniform sampler2D texture08;
	uniform sampler2D texture09;
	uniform sampler2D texture10;
	uniform sampler2D texture11;
	uniform sampler2D texture12;
	uniform sampler2D texture13;
	uniform sampler2D texture14;
	uniform sampler2D texture15;

	layout (location = 0) out vec4 f_Color;

	void main()
	{
		// ambient
		// ---------------------------------------------------------------------
		vec3 ambientColor = ambientStrength * lightColor;

		// diffuse
		// ---------------------------------------------------------------------
		vec3 lightDirection = normalize(lightPosition - v_FragmentPosition);
		float diffuseStrength = max(dot(v_Normal, lightDirection), 0.0);
		vec3 diffuseColor = diffuseStrength * lightColor;
		
		// result
		// ---------------------------------------------------------------------
		vec3 finalColor = ambientColor + diffuseColor;
		if (v_FragmentPosition.x >= 0 && v_FragmentPosition.y >= 0)
		{
			f_Color = texture(texture00, v_TextureCoordinate) * vec4(finalColor, 1.0) * v_Color;
		}
		else if (v_FragmentPosition.x < 0 && v_FragmentPosition.y >= 0)
		{
			f_Color = texture(texture01, v_TextureCoordinate) * vec4(finalColor, 1.0) * v_Color;
		}
		else if (v_FragmentPosition.x >= 0 && v_FragmentPosition.y < 0)
		{
			f_Color = texture(texture02, v_TextureCoordinate) * vec4(finalColor, 1.0) * v_Color;
		}
		else if (v_FragmentPosition.x < 0 && v_FragmentPosition.y < 0)
		{
			f_Color = texture(texture03, v_TextureCoordinate) * vec4(finalColor, 1.0) * v_Color;
		}
	} 
	)";
}
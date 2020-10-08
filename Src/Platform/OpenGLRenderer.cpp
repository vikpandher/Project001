#include "OpenGLRenderer.h"

#include "OpenGLShader.h"

#include "glad/glad.h"

#include "../Engine/Logger.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	OpenGLRenderer::OpenGLRenderer()
		: vertexBufferSize_(0)
		, indexBufferSize_(0)
	{
		// configure global opengl state
		// ---------------------------------------------------------------------
		// enable using the z buffer
		glEnable(GL_DEPTH_TEST);

		// enable writing to the depth buffer
		glDepthMask(GL_TRUE);

		// draw as wireframe
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDataStruct) * s_vertexBufferCapacity_, 0, GL_DYNAMIC_DRAW);

		int positionAttributeIndex = 0;

		// attach the active buffer to the active array with the given attributes
		// index, size, type, normalized, stride, pointer)
		glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataStruct), (void*)0);

		glEnableVertexAttribArray(positionAttributeIndex);

		// make the buffer the active element array buffer, create it if necessary
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

		// set the size of the active element array buffer
		// (target, size, data, usage)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uint) * s_indexBufferCapacity_, 0, GL_DYNAMIC_DRAW);


		// unbind the array buffer and the vertex array
		///glBindBuffer(GL_ARRAY_BUFFER, 0);
		///glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		///glBindVertexArray(0);
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
		glDeleteBuffers(1, &vertexBufferId_);
		glDeleteVertexArrays(1, &vertexArrayId_);
		glDeleteVertexArrays(1, &indexBufferId_);

		delete[] vertexBufferPtr_;
		delete[] indexBufferPtr_;
	}

	void OpenGLRenderer::AddMesh(const Mesh* mesh)
	{
		glm::uint currentIndex = vertexBufferSize_;
		
		const std::vector<glm::vec3>& positions = mesh->positions;
		
		for (int i = 0; i < positions.size(); ++i)
		{
			VertexDataStruct newVertexData;
			newVertexData.position.x = positions[i].x;
			newVertexData.position.y = positions[i].y;
			newVertexData.position.z = positions[i].z;
			AddVertex(newVertexData);
		}

		const std::vector<std::vector<IndexGroup>>& faces = mesh->faces;

		for (int i = 0; i < faces.size(); ++i)
		{
			for (int j = 0; j < faces[i].size(); ++j)
			{
				glm::uint indexValue = currentIndex + faces[i][j].positonIndex;
				AddIndex(indexValue);
			}
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

		shaderPtr_->Use();

		/// glBindVertexArray(vertexArrayId_);
		glDrawElements(GL_TRIANGLES, indexBufferSize_, GL_UNSIGNED_INT, 0);
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

	out vec4 v_Color;

	void main()
	{   
		gl_Position = vec4(in_Position, 1.0);
		v_Color = vec4(in_Position, 1.0);
	}
	)";

	const char* OpenGLRenderer::s_fragmentShaderSource01_ = R"(#version 330 core

	in vec4 v_Color;

	layout (location = 0) out vec4 f_Color;

	void main()
	{
		f_Color = v_Color;
	} 
	)";
}
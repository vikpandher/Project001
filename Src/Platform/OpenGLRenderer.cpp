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

		shaderPtr_ = new OpenGLShader(s_vertexShaderSource01_, s_fragmentShaderSource01_);

		vertexBuffer_.reserve(3);
		vertexBuffer_.push_back(VertexDataStruct());
		vertexBuffer_[0].position.data = { 0.5f, 0.5f, 0.5f };
		vertexBuffer_.push_back(VertexDataStruct());
		vertexBuffer_[1].position.data = { -0.5f, 0.5f, 0.5f };
		vertexBuffer_.push_back(VertexDataStruct());
		vertexBuffer_[2].position.data = { -0.5f, -0.5f, 0.5f };

		// generate an id (name) for a new buffer
		glGenBuffers(1, &vertexBufferId_);

		// make the buffer the active array buffer, creating it if necessary
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);

		// upload the data into the active array buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDataStruct) * vertexBuffer_.size(), &vertexBuffer_[0], GL_STATIC_DRAW);

		// generate an id (name) for a new array
		glGenVertexArrays(1, &vertexArrayId_);

		// make the array avtive, create it if necessary
		glBindVertexArray(vertexArrayId_);

		// attach the active buffer to the active array with the given attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexBuffer_.size() * sizeof(float), (void*)0);

		// enable the vertex attribute
		glEnableVertexAttribArray(0);

		// unbind the array buffer and the vertex array
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
		glDeleteBuffers(1, &vertexBufferId_);
		glDeleteVertexArrays(1, &vertexArrayId_);
	}

	void OpenGLRenderer::Render()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderPtr_->Use();

		glBindVertexArray(vertexArrayId_);
		glDrawArrays(GL_TRIANGLES, 0, vertexBuffer_.size());

		// glfwSwapBuffers happens in OpenGLWindow::OnUpdate()
	}

	// protected ---------------------------------------------------------------

	// private -----------------------------------------------------------------

	// Notes:
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
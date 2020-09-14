#include "OpenGLVertexArray.h"

#include "glad/glad.h"

#include "../Engine/Logger.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &id_);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &id_);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(id_);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	// protected ---------------------------------------------------------------

	// private -----------------------------------------------------------------

}
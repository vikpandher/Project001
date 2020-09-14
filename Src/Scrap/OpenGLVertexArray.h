#pragma once

#include "glm/glm.hpp"



namespace Project001
{	
	class OpenGLVertexArray
	{
	public:
		OpenGLVertexArray();

		~OpenGLVertexArray();

		void Bind() const;

		void Unbind() const;

	protected:
		glm::uint id_;

	private:
		
	};
}
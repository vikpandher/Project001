#pragma once

#include "glm/glm.hpp"



namespace Project001
{
	class OpenGLTexture
	{
	public:
		OpenGLTexture(unsigned int index, const unsigned char* data, int width, int height, unsigned int numberOfComponents);

		~OpenGLTexture();

		void Bind(unsigned int index);

		// Don't really need to unbind...
		// void Unbind();

	protected:
		glm::uint textureId_;
	};
}

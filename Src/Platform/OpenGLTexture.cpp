#include "OpenGLTexture.h"

#include "glad/glad.h"



namespace Project001
{
	// public ------------------------------------------------------------------
	
	OpenGLTexture::OpenGLTexture(const unsigned char* data, int width, int height, unsigned int numberOfComponents)
	{
		glGenTextures(1, &textureId_);
		glBindTexture(GL_TEXTURE_2D, textureId_);

		glm::uint format = 0;
		if (numberOfComponents == 1)
		{
			format = GL_RED;
		}
		else if (numberOfComponents == 3)
		{
			format = GL_RGB;
		}
		else if (numberOfComponents == 4)
		{
			format = GL_RGBA;
		}

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &textureId_);
	}

	void OpenGLTexture::Bind(unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textureId_);
	}
}
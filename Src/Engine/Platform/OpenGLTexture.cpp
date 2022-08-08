#include "OpenGLTexture.h"

#include "glad/glad.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGLTexture::OpenGLTexture(unsigned int textureUnit, const unsigned char* data, unsigned int width, unsigned int height, unsigned int bytesPerPixel)
    {
        glGenTextures(1, &textureId_);
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureId_);

        glm::uint format = 0;
        if (bytesPerPixel == 1)
        {
            format = GL_RED;
        }
        else if (bytesPerPixel == 3)
        {
            format = GL_RGB;
        }
        else if (bytesPerPixel == 4)
        {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // glGenerateMipmap(GL_TEXTURE_2D);
        // 
        // // set texture filtering parameters (using mipmaps)
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &textureId_);
    }

    void OpenGLTexture::Bind(unsigned int textureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureId_);
    }
}
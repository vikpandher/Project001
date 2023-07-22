#include "OpenGL_Texture.h"

#include "glad/glad.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGL_Texture::OpenGL_Texture(
        unsigned int textureUnit,
        const unsigned char* data,
        unsigned int width,
        unsigned int height,
        unsigned int bytesPerPixel,
        bool multisampleAntiAliasing,
        bool mipMaps)
    {
        glGenTextures(1, &textureId_);
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureId_);

        int format = 0;
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

        if (mipMaps)
        {
            if (multisampleAntiAliasing)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            }

            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            if (multisampleAntiAliasing)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
        }
    }

    OpenGL_Texture::~OpenGL_Texture()
    {
        glDeleteTextures(1, &textureId_);
    }

    void OpenGL_Texture::Bind(unsigned int textureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureId_);
    }
}
#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    class OpenGLTexture
    {
    public:
        OpenGLTexture(unsigned int textureUnit, const unsigned char* data, int width, int height, unsigned int numberOfComponents);

        ~OpenGLTexture();

        OpenGLTexture(OpenGLTexture& other) = delete;
        void operator=(const OpenGLTexture&) = delete;

        void Bind(unsigned int textureUnit);

        // Don't really need to unbind...
        // void Unbind();

    protected:
        glm::uint textureId_;
    };
}

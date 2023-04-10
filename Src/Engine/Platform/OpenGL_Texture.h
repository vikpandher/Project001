#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    class OpenGL_Texture
    {
    public:
        OpenGL_Texture(
            unsigned int textureUnit,
            const unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int bytesPerPixel,
            bool multisampleAntiAliasing,
            bool mipMaps);

        ~OpenGL_Texture();

        OpenGL_Texture(OpenGL_Texture& other) = delete;
        void operator=(const OpenGL_Texture&) = delete;

        void Bind(unsigned int textureUnit);

        // Don't really need to unbind...
        // void Unbind();

    protected:
        glm::uint textureId_;
    };
}

#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    bool TextureLoader::LoadTexture(
        TextureData& textureData,
        const std::string& filePath,
        bool convertToRGBA)
    {
        stbi_set_flip_vertically_on_load(true);

        int width;
        int height;
        int bytesPerPixel;
        if (convertToRGBA)
        {
            textureData.data = stbi_load(filePath.c_str(), &width, &height, &bytesPerPixel, STBI_rgb_alpha);
            bytesPerPixel = 4;
        }
        else
        {
            textureData.data = stbi_load(filePath.c_str(), &width, &height, &bytesPerPixel, 0);
        }
        if (textureData.data == nullptr)
        {
            return false;
        }
        textureData.width = (unsigned int)width;
        textureData.height = (unsigned int)height;
        textureData.bytesPerPixel = (unsigned int)bytesPerPixel;

        // NOTE: stbi_image_free just calls free

        return true;
    }

    bool TextureLoader::LoadTextureFromMemory(
        TextureData& textureData,
        const unsigned char* dataPtr,
        size_t dataSize,
        bool convertToRGBA)
    {
        stbi_set_flip_vertically_on_load(true);

        int width;
        int height;
        int bytesPerPixel;
        if (convertToRGBA)
        {
            textureData.data = stbi_load_from_memory(dataPtr, (int)dataSize, &width, &height, &bytesPerPixel, STBI_rgb_alpha);
            bytesPerPixel = 4;
        }
        else
        {
            textureData.data = stbi_load_from_memory(dataPtr, (int)dataSize, &width, &height, &bytesPerPixel, 0);
        }
        if (textureData.data == nullptr)
        {
            return false;
        }
        textureData.width = (unsigned int)width;
        textureData.height = (unsigned int)height;
        textureData.bytesPerPixel = (unsigned int)bytesPerPixel;

        return true;
    }

#ifdef VULKAN_BUILD
    bool TextureLoader::s_convertToRGBA = true;
#else
    bool TextureLoader::s_convertToRGBA = false;
#endif
}
// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#include "TextureUtility.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



namespace Project001
{
namespace Texture
{
#ifdef VULKAN_BUILD
    bool g_convertToRGBA = true;
#else
    bool g_convertToRGBA = false;
#endif

    bool LoadTexture(
        TextureData& textureData,
        const std::string& filePath,
        bool convertToRGBA)
    {
        stbi_set_flip_vertically_on_load(true);

        // // free existing texture data if any (to prevent memory leak)
        // if (textureData.data != nullptr)
        // {
        //     stbi_image_free(textureData.data);
        //     textureData.data = nullptr;
        // }

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
        textureData.width = static_cast<unsigned int>(width);
        textureData.height = static_cast<unsigned int>(height);
        textureData.bytesPerPixel = static_cast<unsigned int>(bytesPerPixel);

        // NOTE: stbi_image_free just calls free

        return true;
    }

    bool LoadTextureFromMemory(
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
            textureData.data = stbi_load_from_memory(dataPtr, static_cast<int>(dataSize), &width, &height, &bytesPerPixel, STBI_rgb_alpha);
            bytesPerPixel = 4;
        }
        else
        {
            textureData.data = stbi_load_from_memory(dataPtr, static_cast<int>(dataSize), &width, &height, &bytesPerPixel, 0);
        }
        if (textureData.data == nullptr)
        {
            return false;
        }
        textureData.width = static_cast<unsigned int>(width);
        textureData.height = static_cast<unsigned int>(height);
        textureData.bytesPerPixel = static_cast<unsigned int>(bytesPerPixel);

        return true;
    }
}
}
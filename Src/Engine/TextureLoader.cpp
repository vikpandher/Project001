#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



namespace Project001
{
    // public: -----------------------------------------------------------------

    bool TextureLoader::LoadTexture(TextureData& textureData, const std::string& path)
    {
        stbi_set_flip_vertically_on_load(true);

        int width;
        int height;
        int bytesPerPixel;
        textureData.data = stbi_load(path.c_str(), &width, &height, &bytesPerPixel, 0);
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
}
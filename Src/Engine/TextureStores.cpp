#include "TextureStores.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



namespace Project001
{
    TextureStores::TextureStores()
    {
        stbi_set_flip_vertically_on_load(true);
    }

    TextureStores::~TextureStores()
    {}

    void TextureStores::ClearTextures()
    {
        textureDataArray_.clear();
    }

    bool TextureStores::GetTexture(unsigned int index, TextureData*& textureVertexPtr)
    {
        if (index >= textureDataArray_.size())
        {
            return false;
        }

        TextureData* textureDataArrayPtr = textureDataArray_.data();

        textureVertexPtr = &textureDataArrayPtr[index];

        return true;
    }

    bool TextureStores::LoadTexture(const std::string& path, unsigned int& index)
    {
        TextureData newTextureData;
        newTextureData.data = stbi_load(path.c_str(), &newTextureData.width, &newTextureData.height, &newTextureData.numberOfComponents, 0);
        if (newTextureData.data == nullptr)
        {
            return false;
        }

        textureDataArray_.push_back(newTextureData);
        index = (unsigned int)(textureDataArray_.size() - 1);

        return true;
    }
}
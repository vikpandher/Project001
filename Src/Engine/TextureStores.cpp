#include "TextureStores.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



namespace Project001
{
    // public: -----------------------------------------------------------------

    TextureStores::TextureStores()
    {
        stbi_set_flip_vertically_on_load(true);
    }

    TextureStores::~TextureStores()
    {
        for (unsigned int i = 0; i < textureDataArray_.size(); ++i)
        {
            stbi_image_free(textureDataArray_[i].data);
        }
    }

    void TextureStores::ClearTextures()
    {
        for (unsigned int i = 0; i < textureDataArray_.size(); ++i)
        {
            stbi_image_free(textureDataArray_[i].data);
        }
        textureDataArray_.clear();
    }

    bool TextureStores::GetTexture(unsigned int index, TextureData& textureData) const
    {
        if (index >= textureDataArray_.size())
        {
            return false;
        }

        textureData = textureDataArray_[index];

        return true;
    }

    bool TextureStores::LoadTexture(unsigned int& index, const std::string& path)
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
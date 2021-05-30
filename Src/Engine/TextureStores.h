#pragma once

#include <string>
#include <vector>



namespace Project001
{
    struct TextureData
    {
        unsigned char* data;
        int width;
        int height;
        int numberOfComponents; // alternativly bytes per pixel
    };

    class TextureStores
    {
    public:
        TextureStores();

        ~TextureStores();

        void ClearTextures();

        bool GetTexture(unsigned int index, TextureData& textureData);

        bool LoadTexture(const std::string& path, unsigned int& index);

    protected:
        std::vector<TextureData> textureDataArray_;

    private:
    };
}
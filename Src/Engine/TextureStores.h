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

        bool LoadTexture(unsigned int& index, const std::string& path);

    protected:
        std::vector<TextureData> textureDataArray_;

    private:
    };
}
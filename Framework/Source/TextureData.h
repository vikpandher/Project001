// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-08

#pragma once

#include <cstdlib>



namespace Project001
{
    struct TextureData
    {
        TextureData();

        ~TextureData();

        TextureData(TextureData&&) = delete;
        TextureData& operator=(TextureData&&) = delete;

        void Clear();
        
        unsigned char* data;
        unsigned int width;
        unsigned int height;
        unsigned int bytesPerPixel;
    };

    inline TextureData::TextureData()
        : data(nullptr)
        , width(0)
        , height(0)
        , bytesPerPixel(0)
    {}

    inline TextureData::~TextureData()
    {
        free(data);
        data = nullptr;
    }

    inline void TextureData::Clear()
    {
        free(data);
        data = nullptr;
        width = 0;
        height = 0;
        bytesPerPixel = 0;
    }
}
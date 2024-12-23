// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "TextureData.h"

#include <string>



namespace Project001
{

    class TextureLoader
    {
    public:
        static bool LoadTexture(
            TextureData& textureData,
            const std::string& filePath,
            bool convertToRGBA = s_convertToRGBA);

        static bool LoadTextureFromMemory(
            TextureData& textureData,
            const unsigned char* dataPtr,
            size_t dataSize,
            bool convertToRGBA = s_convertToRGBA);

        static bool s_convertToRGBA;
    };
}
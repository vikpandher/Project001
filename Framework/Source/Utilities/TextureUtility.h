// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#pragma once

#include "TextureData.h"

#include <string>



namespace Project001
{
namespace Texture
{
    extern bool g_convertToRGBA;

    bool LoadTexture(
        TextureData& textureData,
        const std::string& filePath,
        bool convertToRGBA = g_convertToRGBA);

    bool LoadTextureFromMemory(
        TextureData& textureData,
        const unsigned char* dataPtr,
        size_t dataSize,
        bool convertToRGBA = g_convertToRGBA);
}
}
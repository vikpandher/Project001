// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#pragma once

#include "FontData.h"
#include "FontMeshData.h"
#include "TextureData.h"

#include <string>



namespace Project001
{
namespace Font
{
    extern bool g_triangulateMeshes;

    bool LoadFontData(
        FontData& fontData,
        const std::string& filePath);

    bool LoadFontDataFromMemory(
        FontData& fontData,
        const unsigned char* dataPtr,
        size_t dataSize);

    // alignment values:
    // 0 = left aligned lines (regular)
    // 1 = center aligned lines
    // 2 = right aligned lines
    bool GenerateMeshDataFromFontDataAndString(
        MeshData& meshData,
        const FontData& fontData,
        const std::string& text,
        float pixelSize = 0.01f,
        size_t alignemnt = 0,
        bool useAltLineSpacing = false,
        float altLineSpacing_px = 0.0f,
        bool trangulate = g_triangulateMeshes);

    bool GenerateGlpyhMeshDataFromFontDataAndCharacter(
        GlyphMeshData& glyphMeshData,
        const FontData& fontData,
        unsigned char character,
        float pixelSize = 0.01f,
        bool trangulate = g_triangulateMeshes);

    bool GenerateFontMeshDataFromFontData(
        FontMeshData& fontMeshData,
        const FontData& fontData,
        float pixelSize = 0.01f,
        bool trangulate = g_triangulateMeshes);

    // Helper Functions --------------------------------------------------------

    // Used in GenerateGlpyhMeshDataFromFontDataAndCharacter and GenerateFontMeshDataFromFontData
    void GenerateGlpyhMeshDataFromGlyphMetrics_H(
        GlyphMeshData& glyphMeshData,
        const GlyphMetrics& glyphMetrics,
        float pixelSize = 0.01f,
        bool trangulate = g_triangulateMeshes);
}
}
// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-11-17

#pragma once

#include "FontData.h"
#include "FontMeshData.h"
#include "TextureData.h"

#include <string>



namespace Project001
{
    class FontLoader
    {
    public:
        static bool LoadFontData(
            FontData& fontData,
            const std::string& filePath);

        static bool LoadFontDataFromMemory(
            FontData& fontData,
            const unsigned char* dataPtr,
            size_t dataSize);

        // alignment values:
        // 0 = left aligned lines (regular)
        // 1 = center aligned lines
        // 2 = right aligned lines
        static bool GenerateMeshDataFromFontDataAndString(
            MeshData& meshData,
            const FontData& fontData,
            const std::string& text,
            float pixelSize = 0.01f,
            size_t alignemnt = 0,
            bool useAltLineSpacing = false,
            float altLineSpacing_px = 0.0f,
            bool trangulate = s_triangulate);

        static bool GenerateGlpyhMeshDataFromFontDataAndCharacter(
            GlyphMeshData& glyphMeshData,
            const FontData& fontData,
            unsigned char character,
            float pixelSize = 0.01f,
            bool trangulate = s_triangulate);

        static bool GenerateFontMeshDataFromFontData(
            FontMeshData& fontMeshData,
            const FontData& fontData,
            float pixelSize = 0.01f,
            bool trangulate = s_triangulate);

        static bool s_triangulate;

    protected:

        // Used in GenerateGlpyhMeshDataFromFontDataAndCharacter and GenerateFontMeshDataFromFontData
        static void GenerateGlpyhMeshDataFromGlyphMetrics_H(
            GlyphMeshData& glyphMeshData,
            const GlyphMetrics& glyphMetrics,
            float pixelSize = 0.01f,
            bool trangulate = s_triangulate);
    };
}
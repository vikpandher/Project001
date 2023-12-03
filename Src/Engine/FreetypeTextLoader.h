#pragma once

#include "Engine/FontData.h"
#include "Engine/FontMeshData.h"
#include "Engine/TextureData.h"

#include <unordered_map>
#include <string>



namespace Project001
{
    class FreetypeTextLoader
    {
    public:
        static bool LoadTextureDataAndFontData(
            TextureData& textureData,
            FontData& fontData,
            const std::vector<unsigned char>& characterList,
            const char* fontPath,
            unsigned int fontHeightInPixels = 48,
            unsigned int horizontalSpacing = 1,
            unsigned int verticalSpacing = 1);

        static bool LoadMeshData(
            MeshData& meshData,
            const FontData& fontData,
            const std::string& text,
            float pixelSize = 0.01f,
            bool centeredLines = false,
            bool trangulate = s_triangulate);

        static bool LoadGlpyhMeshData(
            GlyphMeshData& glyphMeshData,
            const FontData& fontData,
            unsigned char character,
            float pixelSize = 0.01f,
            bool trangulate = s_triangulate);

        static bool LoadFontMeshData(
            FontMeshData& fontMeshData,
            const FontData& fontData,
            float pixelSize = 0.01f,
            bool trangulate = s_triangulate);

    protected:
        static unsigned int GetTextureIndex(
            unsigned int x,
            unsigned int xOffset,
            unsigned int textureWidth,
            unsigned int y,
            unsigned int yOffset,
            unsigned int textureHeight);

        static void LoadGlpyhMeshData_H(
            GlyphMeshData& glyphMeshData,
            const GlyphMetrics& glyphMetrics,
            const FontData& fontData,
            unsigned char character,
            float pixelSize = 0.01f,
            bool trangulate = s_triangulate);

        static const bool s_flipVerticalyOnLoad;
        static const bool s_triangulate;
    };
}
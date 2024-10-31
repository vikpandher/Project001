// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "MeshData.h"

#include <unordered_map>



namespace Project001
{
    struct GlyphMeshData
    {
        float horiAdvance;
        MeshData meshData;
    };

    struct FontMeshData
    {
        FontMeshData();

        void Clear();

        float lineSpacing;
        std::unordered_map<unsigned char, GlyphMeshData> glyphMeshDataMap;
    };

    inline FontMeshData::FontMeshData()
        : lineSpacing(0.0f)
    {}

    inline void FontMeshData::Clear()
    {
        glyphMeshDataMap.clear();
    }
}
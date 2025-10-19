// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-18

#pragma once

#include "MeshData.h"

#include <unordered_map>



namespace Project001
{
    struct GlyphMeshData
    {
        GlyphMeshData();

        float horiAdvance;
        MeshData meshData;
    };

    inline GlyphMeshData::GlyphMeshData()
        : horiAdvance(0.0f)
    {}

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
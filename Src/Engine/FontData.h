#pragma once

#include "glm/glm.hpp"

#include <map>



namespace Project001
{
    struct GlyphMetrics
    {
        unsigned int width_px;
        unsigned int height_px;
        unsigned int horiBearingX_px;
        unsigned int horiBearingY_px;
        unsigned int horiAdvance_px;
        unsigned int vertBearingX_px;
        unsigned int vertBearingY_px;
        unsigned int vertAdvance_px;
        glm::vec2 textureBottomLeft;
        glm::vec2 textureTopRight;
    };

    struct FontData
    {
        FontData();

        void Clear();

        unsigned int lineSpacing_px;
        std::map<unsigned char, Project001::GlyphMetrics> glyphMetricsMap;
    };

    inline FontData::FontData()
        : lineSpacing_px(0)
    {}

    inline void FontData::Clear()
    {
        glyphMetricsMap.clear();
    }
}
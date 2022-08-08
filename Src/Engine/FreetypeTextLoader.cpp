#include "FreetypeTextLoader.h"

#include "Engine/Logger.h"

// http://freetype.sourceforge.net/freetype2/docs/reference/ft2-index.html
#include "ft2build.h"
#include FT_FREETYPE_H



namespace Project001
{
    bool FreetypeTextLoader::GenerateTexture(
        TextureData& textureData,
        FontData& fontData,
        std::vector<unsigned char> characterList,
        const char* fontPath,
        unsigned int fontHeightInPixels)
    {
        unsigned int pixelWidth = 0;

        FT_Library library;
        FT_Error error = FT_Init_FreeType(&library);
        if (error)
        {
            _LOG_ERROR("Could not initalize FreeType Library");
            return false;
        }

        FT_Face face;
        error = FT_New_Face(library, fontPath, 0, &face);
        if (error) {
            _LOG_ERROR("Failed to load font");
            return false;
        }

        error = FT_Set_Pixel_Sizes(face, 0, fontHeightInPixels);
        if (error)
        {
            _LOG_ERROR("Could not set font pixel sizes");
            return false;
        }

        std::map<unsigned char, GlyphMetrics>& glyphMetricsMap = fontData.glyphMetricsMap;

        for (size_t i = 0; i < characterList.size(); ++i)
        {
            const unsigned char& c = characterList[i];
            if (fontData.glyphMetricsMap.find(c) != fontData.glyphMetricsMap.end())
            {
                _LOG_MESSAGE("Warning, reloading character glyph for: %c", c);
            }
            else
            {
                glyphMetricsMap[c];
            }
        }

        const unsigned int METRICS_UNIT_MULTIPLIER = 64; // because values are expressed in 26.6 fractional pixel format
        fontData.lineSpacing_px = face->size->metrics.height / METRICS_UNIT_MULTIPLIER;

        // load characters first time to calculate the size of the texture
        for (std::map<unsigned char, GlyphMetrics>::iterator iter = glyphMetricsMap.begin(); iter != glyphMetricsMap.end(); ++iter)
        {
            const unsigned char& c = iter->first;
            error = FT_Load_Char(face, c, FT_LOAD_RENDER);
            if (error)
            {
                _LOG_ERROR("Could not load char: %c", c);
                return false;
            }

            unsigned char& pixelMode = face->glyph->bitmap.pixel_mode;
            if (pixelMode != FT_Pixel_Mode::FT_PIXEL_MODE_GRAY)
            {
                _LOG_ERROR("Pixel Mode not supported");
                return false;
            }

            unsigned int& currentBitmapHeight = face->glyph->bitmap.rows;
            unsigned int& currentBitmapWidth = face->glyph->bitmap.width;
            if (currentBitmapHeight > textureData.height)
            {
                textureData.height = currentBitmapHeight;
            }
            textureData.width += currentBitmapWidth;
        }

        free(textureData.data);
        textureData.data = (unsigned char*)malloc(sizeof(unsigned char*) * textureData.height * textureData.width * 4);

        unsigned int nextCharacterOffsetX = 0;

        for (std::map<unsigned char, GlyphMetrics>::iterator iter = glyphMetricsMap.begin(); iter != glyphMetricsMap.end(); ++iter)
        {
            const unsigned char& c = iter->first;
            error = FT_Load_Char(face, c, FT_LOAD_RENDER);
            if (error)
            {
                _LOG_ERROR("Could not load char: %c", c);
                return false;
            }

            GlyphMetrics& glyphMetrics = iter->second;
            glyphMetrics.height_px = face->glyph->bitmap.rows;
            glyphMetrics.width_px = face->glyph->bitmap.width;
            glyphMetrics.horiBearingX_px = face->glyph->metrics.horiBearingX / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.horiBearingY_px = face->glyph->metrics.horiBearingY / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.horiAdvance_px = face->glyph->metrics.horiAdvance / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.vertBearingX_px = face->glyph->metrics.vertBearingX / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.vertBearingY_px = face->glyph->metrics.vertBearingY / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.vertAdvance_px = face->glyph->metrics.vertAdvance / METRICS_UNIT_MULTIPLIER;
            glyphMetrics.textureBottomLeft.x = (float)nextCharacterOffsetX / (float)textureData.width;
            glyphMetrics.textureTopRight.x = (float)(nextCharacterOffsetX + glyphMetrics.width_px) / (float)textureData.width;
            if (s_flipVerticalyOnLoad)
            {
                glyphMetrics.textureBottomLeft.y = (float)(textureData.height - glyphMetrics.height_px) / (float)textureData.height;
                glyphMetrics.textureTopRight.y = 1.0f;
            }
            else
            {
                glyphMetrics.textureBottomLeft.y = (float)(glyphMetrics.height_px) / (float)textureData.height;
                glyphMetrics.textureTopRight.y = 0.0f;
            }
            for (unsigned int j = 0; j < glyphMetrics.height_px; ++j)
            {
                for (unsigned int i = 0; i < glyphMetrics.width_px; ++i)
                {
                    unsigned int textureDataIndex;
                    if (s_flipVerticalyOnLoad)
                    {
                        textureDataIndex = (nextCharacterOffsetX + i + (textureData.height - 1 - j) * textureData.width) * 4;
                    }
                    else
                    {
                        textureDataIndex = (nextCharacterOffsetX + i + j * textureData.width) * 4;
                    }
#pragma warning(disable:6011)
                    textureData.data[textureDataIndex] = 255;
                    textureData.data[textureDataIndex + 1] = 255;
                    textureData.data[textureDataIndex + 2] = 255;
                    unsigned int sourceIndex = i + j * glyphMetrics.width_px;
                    textureData.data[textureDataIndex + 3] = face->glyph->bitmap.buffer[sourceIndex];
#pragma warning(default:6011)
                }
            }

            nextCharacterOffsetX += glyphMetrics.width_px;
        }

        textureData.bytesPerPixel = 4;

        return true;
    }

    bool FreetypeTextLoader::GenerateMesh(
        MeshData& meshData,
        FontData& fontData,
        float pixelSize)
    {
        return false;
    }
}
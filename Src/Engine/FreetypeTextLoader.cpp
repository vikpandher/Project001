#include "FreetypeTextLoader.h"

#include "Engine/Logger.h"

// http://freetype.sourceforge.net/freetype2/docs/reference/ft2-index.html
#include "ft2build.h"
#include FT_FREETYPE_H



namespace Project001
{
    bool FreetypeTextLoader::LoadTexture(
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

    bool FreetypeTextLoader::LoadMesh(
        MeshData& meshData,
        FontData& fontData,
        const std::string& text,
        float pixelSize,
        bool centeredLines,
        bool trangulate)
    {
        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        maxVertexPosition.z = std::max(maxVertexPosition.z, 0.0f);
        minVertexPosition.z = std::min(minVertexPosition.z, 0.0f);

        unsigned int lineStartVertexCount = (unsigned int)meshVertexArray.size();
        unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        MeshVertex topRight;
        MeshVertex topLeft;
        MeshVertex bottomLeft;
        MeshVertex bottomRight;

        topRight.normal = normal;
        topLeft.normal = normal;
        bottomLeft.normal = normal;
        bottomRight.normal = normal;

        glm::vec2 lineStartPenPosition(0.0f);
        glm::vec2 currentPenPosition(0.0f);
        for (size_t i = 0; i < text.size(); ++i)
        {
            const char& c = text[i];
            if (c == '\n')
            {
                if (centeredLines)
                {
                    glm::vec2 halfShift = (currentPenPosition - lineStartPenPosition) * 0.5f;
                    for (size_t i = lineStartVertexCount; i < currentVertexCount; ++i)
                    {
                        MeshVertex& currentMeshVertex = meshVertexArray[i];
                        currentMeshVertex.position.x -= halfShift.x;
                        currentMeshVertex.position.y -= halfShift.y;
                    }
                }
                lineStartVertexCount = currentVertexCount;
                lineStartPenPosition.y -= (float)fontData.lineSpacing_px * pixelSize;
                currentPenPosition = lineStartPenPosition;
            }
            else if (fontData.glyphMetricsMap.find(c) != fontData.glyphMetricsMap.end())
            {
                const GlyphMetrics& currentGlyphMetrics = fontData.glyphMetricsMap[c];
                float left = currentPenPosition.x + (float)currentGlyphMetrics.horiBearingX_px * pixelSize;
                float right = left + (float)currentGlyphMetrics.width_px * pixelSize;
                float top = currentPenPosition.y + (float)currentGlyphMetrics.horiBearingY_px * pixelSize;
                float bottom = top - (float)currentGlyphMetrics.height_px * pixelSize;

                topRight.position.x = right;
                topRight.position.y = top;
                topLeft.position.x = left;
                topLeft.position.y = top;
                bottomLeft.position.x = left;
                bottomLeft.position.y = bottom;
                bottomRight.position.x = right;
                bottomRight.position.y = bottom;

                topRight.textureCoordinate = currentGlyphMetrics.textureTopRight;
                topLeft.textureCoordinate.x = currentGlyphMetrics.textureBottomLeft.x;
                topLeft.textureCoordinate.y = currentGlyphMetrics.textureTopRight.y;
                bottomLeft.textureCoordinate = currentGlyphMetrics.textureBottomLeft;
                bottomRight.textureCoordinate.x = currentGlyphMetrics.textureTopRight.x;
                bottomRight.textureCoordinate.y = currentGlyphMetrics.textureBottomLeft.y;

                if (trangulate)
                {
                    meshVertexArray.push_back(topRight);
                    meshVertexArray.push_back(topLeft);
                    meshVertexArray.push_back(bottomLeft);
                    meshVertexArray.push_back(bottomLeft);
                    meshVertexArray.push_back(bottomRight);
                    meshVertexArray.push_back(topRight);

                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                }
                else
                {
                    meshVertexArray.push_back(topRight);
                    meshVertexArray.push_back(topLeft);
                    meshVertexArray.push_back(bottomLeft);
                    meshVertexArray.push_back(bottomRight);

                    meshIndexArray.push_back(currentVertexCount);
                    meshIndexArray.push_back(currentVertexCount + 1);
                    meshIndexArray.push_back(currentVertexCount + 2);
                    meshIndexArray.push_back(currentVertexCount + 2);
                    meshIndexArray.push_back(currentVertexCount + 3);
                    meshIndexArray.push_back(currentVertexCount);

                    currentVertexCount += 4;
                }

                currentPenPosition.x += (float)currentGlyphMetrics.horiAdvance_px * pixelSize;
            }
            else
            {
                _LOG_MESSAGE("Warning, failed to generate mesh for character: %c", c);
                // return false;
            }
        }
        if (centeredLines)
        {
            glm::vec2 halfShift = (currentPenPosition - lineStartPenPosition) * 0.5f;
            for (size_t i = lineStartVertexCount; i < currentVertexCount; ++i)
            {
                MeshVertex& currentMeshVertex = meshVertexArray[i];
                currentMeshVertex.position.x -= halfShift.x;
                currentMeshVertex.position.y -= halfShift.y;
            }
        }

        for (size_t i = 0; i < meshVertexArray.size(); ++i)
        {
            maxVertexPosition.x = std::max(maxVertexPosition.x, meshVertexArray[i].position.x);
            maxVertexPosition.y = std::max(maxVertexPosition.y, meshVertexArray[i].position.y);

            minVertexPosition.y = std::min(minVertexPosition.y, meshVertexArray[i].position.y);
            minVertexPosition.x = std::min(minVertexPosition.x, meshVertexArray[i].position.x);
        }

        return true;
    }

    // protected ---------------------------------------------------------------

    const bool FreetypeTextLoader::s_flipVerticalyOnLoad = true;
    const bool FreetypeTextLoader::s_triangulate = false;
}
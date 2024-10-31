// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "FontLoader.h"

#include "Math/MathUtilities.h"
#include "Logger.h"

#include <fstream>



namespace Project001
{
    // public ------------------------------------------------------------------

    bool FontLoader::LoadFontData(
        FontData& fontData,
        const std::string& filePath)
    {
        std::ifstream fileStream(filePath, std::ios::binary);

        if (!fileStream.is_open())
        {
            LOG_ERROR_F("Failed to open file");
            return false;
        }

        char signature[8] = { 0 };
        fileStream.read(signature, 8); // ----------------------------------------------- 8 bytes
        if (std::string(signature) != "FSSDATA")
        {
            LOG_ERROR_F("Invalid file signature");
            return false;
        }

        unsigned int version = 0;
        fileStream.read(reinterpret_cast<char*>(&version), 4); // ----------------------- 4 bytes
        if (version != 1)
        {
            LOG_ERROR_F("Unsupported version");
            return false;
        }

        fileStream.read(reinterpret_cast<char*>(&fontData.lineSpacing_px), 4); // ------- 4 bytes

        unsigned int glyphCount = 0;
        fileStream.read(reinterpret_cast<char*>(&glyphCount), 4); // -------------------- 4 bytes

        // fontData.glyphMetricsMap.clear();

        for (unsigned int i = 0; i < glyphCount; ++i)
        {
            unsigned char glyph;
            fileStream.read(reinterpret_cast<char*>(&glyph), 1); // --------------------- 1 byte

            GlyphMetrics metrics;
            fileStream.read(reinterpret_cast<char*>(&metrics.width_px), 4); // ---------- 4 bytes
            fileStream.read(reinterpret_cast<char*>(&metrics.height_px), 4); // --------- 4 bytes
            fileStream.read(reinterpret_cast<char*>(&metrics.horiBearingX_px), 4); // --- 4 bytes
            fileStream.read(reinterpret_cast<char*>(&metrics.horiBearingY_px), 4); // --- 4 bytes
            fileStream.read(reinterpret_cast<char*>(&metrics.horiAdvance_px), 4); // ---- 4 bytes
            fileStream.read(reinterpret_cast<char*>(&metrics.vertBearingX_px), 4); // --- 4 bytes
            fileStream.read(reinterpret_cast<char*>(&metrics.vertBearingY_px), 4); // --- 4 bytes
            fileStream.read(reinterpret_cast<char*>(&metrics.vertAdvance_px), 4); // ---- 4 bytes

            fileStream.read(reinterpret_cast<char*>(&metrics.textureBottomLeft.x), 4); // 4 bytes
            fileStream.read(reinterpret_cast<char*>(&metrics.textureTopRight.x), 4); // - 4 bytes
            fileStream.read(reinterpret_cast<char*>(&metrics.textureBottomLeft.y), 4); // 4 bytes
            fileStream.read(reinterpret_cast<char*>(&metrics.textureTopRight.y), 4); // - 4 bytes

            fontData.glyphMetricsMap[glyph] = metrics;
        }

        fileStream.close();

        if (fileStream.bad())
        {
            LOG_ERROR_F("File stream error");
            return false;
        }

        return true;
    }

    bool FontLoader::LoadFontDataFromMemory(
        FontData& fontData,
        const unsigned char* dataPtr,
        size_t dataSize)
    {
        auto ReadData = [&](void* dest, size_t bytes) -> bool
        {
            if (dataSize < bytes)
            {
                LOG_ERROR_F("Not enough data in buffer");
                return false;
            }
            std::memcpy(dest, dataPtr, bytes);
            dataPtr += bytes;
            dataSize -= bytes;
            return true;
        };

        char signature[8] = { 0 };
        if (!ReadData(signature, 8) || std::string(signature) != "FSSDATA") // 8 bytes
        {
            LOG_ERROR_F("Invalid file signature");
            return false;
        }

        unsigned int version = 0;
        if (!ReadData(&version, 4) || version != 1) // ----------------------- 4 bytes
        {
            LOG_ERROR_F("Unsupported version");
            return false;
        }

        if (!ReadData(&fontData.lineSpacing_px, 4)) // ----------------------- 4 bytes
        {
            LOG_ERROR_F("Could not read line spacing");
            return false;
        }

        unsigned int glyphCount = 0;
        if (!ReadData(&glyphCount, 4)) // ------------------------------------ 4 bytes
        {
            LOG_ERROR_F("Could not read glyph count");
            return false;
        }

        fontData.glyphMetricsMap.clear();

        for (unsigned int i = 0; i < glyphCount; ++i)
        {
            unsigned char glyph = 0;
            if (!ReadData(&glyph, 1)) // ------------------------------------- 1 byte
            {
                LOG_ERROR_F("Could not read glyph");
                return false;
            }

            GlyphMetrics metrics;
            if (!ReadData(&metrics.width_px, 4) || // ------------------------ 4 bytes
                !ReadData(&metrics.height_px, 4) || // ----------------------- 4 bytes
                !ReadData(&metrics.horiBearingX_px, 4) || // ----------------- 4 bytes
                !ReadData(&metrics.horiBearingY_px, 4) || // ----------------- 4 bytes
                !ReadData(&metrics.horiAdvance_px, 4) || // ------------------ 4 bytes
                !ReadData(&metrics.vertBearingX_px, 4) || // ----------------- 4 bytes
                !ReadData(&metrics.vertBearingY_px, 4) || // ----------------- 4 bytes
                !ReadData(&metrics.vertAdvance_px, 4) || // ------------------ 4 bytes

                !ReadData(&metrics.textureBottomLeft.x, 4) || // ------------- 4 bytes
                !ReadData(&metrics.textureTopRight.x, 4) || // --------------- 4 bytes
                !ReadData(&metrics.textureBottomLeft.y, 4) || // ------------- 4 bytes
                !ReadData(&metrics.textureTopRight.y, 4)) // ----------------- 4 bytes
            {
                LOG_ERROR_F("Could not read glyph metrics");
                return false;
            }

            fontData.glyphMetricsMap[glyph] = metrics;
        }

        return true;
    }

    bool FontLoader::GenerateMeshDataFromFontDataAndString(
        MeshData& meshData,
        const FontData& fontData,
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
            else
            {
                const std::unordered_map<unsigned char, GlyphMetrics>::const_iterator glyphMetricsMapIter =
                    fontData.glyphMetricsMap.find(c);
                if (glyphMetricsMapIter != fontData.glyphMetricsMap.end())
                {
                    const GlyphMetrics& currentGlyphMetrics = glyphMetricsMapIter->second;
                    if (currentGlyphMetrics.height_px != 0)
                    {
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
                    }
                    currentPenPosition.x += (float)currentGlyphMetrics.horiAdvance_px * pixelSize;
                }
                else
                {
                    LOG_WARNING_F("Failed to generate mesh for character: " << c << " (ASCII: " << (unsigned int)c << ")");

                    // return false;
                }
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

    bool FontLoader::GenerateGlpyhMeshDataFromFontDataAndCharacter(
        GlyphMeshData& glyphMeshData,
        const FontData& fontData,
        unsigned char character,
        float pixelSize,
        bool trangulate)
    {
        const std::unordered_map<unsigned char, GlyphMetrics>::const_iterator glyphMetricsMapIter =
            fontData.glyphMetricsMap.find(character);
        if (glyphMetricsMapIter == fontData.glyphMetricsMap.end())
        {
            LOG_ERROR_F("Character glyph not found for: " << character);
            return false;
        }

        const GlyphMetrics& currentGlyphMetrics = glyphMetricsMapIter->second;
        GenerateGlpyhMeshDataFromGlyphMetrics_H(glyphMeshData, currentGlyphMetrics, pixelSize, trangulate);

        return true;
    }

    bool FontLoader::GenerateFontMeshDataFromFontData(
        FontMeshData& fontMeshData,
        const FontData& fontData,
        float pixelSize,
        bool trangulate)
    {
        std::unordered_map<unsigned char, GlyphMetrics>::const_iterator glyphMetricsMapIter =
            fontData.glyphMetricsMap.begin();
        for (; glyphMetricsMapIter != fontData.glyphMetricsMap.end(); ++glyphMetricsMapIter)
        {
            const unsigned char& character = glyphMetricsMapIter->first;
            const GlyphMetrics& glyphMetrics = glyphMetricsMapIter->second;
            std::unordered_map<unsigned char, GlyphMeshData>::const_iterator glyphMeshDataMapIter =
                fontMeshData.glyphMeshDataMap.find(character);
            if (glyphMeshDataMapIter != fontMeshData.glyphMeshDataMap.end())
            {
                LOG_WARNING_F("Reloading character mesh data for: " << character);
                fontMeshData.glyphMeshDataMap[character].horiAdvance = 0.0f;
                fontMeshData.glyphMeshDataMap[character].meshData.Clear();
            }

            GlyphMeshData& glyphMeshData = fontMeshData.glyphMeshDataMap[character];
            GenerateGlpyhMeshDataFromGlyphMetrics_H(glyphMeshData, glyphMetrics, pixelSize, trangulate);
        }

        return true;
    }

    bool FontLoader::s_triangulate = false;

    // protected ---------------------------------------------------------------

    void FontLoader::GenerateGlpyhMeshDataFromGlyphMetrics_H(
        GlyphMeshData& glyphMeshData,
        const GlyphMetrics& glyphMetrics,
        float pixelSize,
        bool trangulate)
    {
        glyphMeshData.horiAdvance = (float)glyphMetrics.horiAdvance_px * pixelSize;
        if (glyphMetrics.height_px == 0)
        {
            // LOG_WARNING_F("Character of height_px == 0");

            return;
        }

        std::vector<MeshVertex>& meshVertexArray = glyphMeshData.meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = glyphMeshData.meshData.meshIndexArray;
        glm::vec3& maxVertexPosition = glyphMeshData.meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = glyphMeshData.meshData.minVertexPosition;

        maxVertexPosition.z = std::max(maxVertexPosition.z, 0.0f);
        minVertexPosition.z = std::min(minVertexPosition.z, 0.0f);

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

        float left = (float)glyphMetrics.horiBearingX_px * pixelSize;
        float right = left + (float)glyphMetrics.width_px * pixelSize;
        float top = (float)glyphMetrics.horiBearingY_px * pixelSize;
        float bottom = top - (float)glyphMetrics.height_px * pixelSize;

        topRight.position.x = right;
        topRight.position.y = top;
        topLeft.position.x = left;
        topLeft.position.y = top;
        bottomLeft.position.x = left;
        bottomLeft.position.y = bottom;
        bottomRight.position.x = right;
        bottomRight.position.y = bottom;

        maxVertexPosition.x = std::max(maxVertexPosition.x, topRight.position.x);
        maxVertexPosition.y = std::max(maxVertexPosition.y, topRight.position.y);
        maxVertexPosition.z = std::max(maxVertexPosition.z, 0.0f);

        minVertexPosition.x = std::min(minVertexPosition.x, bottomLeft.position.x);
        minVertexPosition.y = std::min(minVertexPosition.y, bottomLeft.position.y);
        minVertexPosition.z = std::min(minVertexPosition.z, 0.0f);

        topRight.textureCoordinate = glyphMetrics.textureTopRight;
        topLeft.textureCoordinate.x = glyphMetrics.textureBottomLeft.x;
        topLeft.textureCoordinate.y = glyphMetrics.textureTopRight.y;
        bottomLeft.textureCoordinate = glyphMetrics.textureBottomLeft;
        bottomRight.textureCoordinate.x = glyphMetrics.textureTopRight.x;
        bottomRight.textureCoordinate.y = glyphMetrics.textureBottomLeft.y;

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

            // currentVertexCount += 4;
        }

        float& maxBoundingRadius = glyphMeshData.meshData.maxBoundingRadius;

        for (size_t i = 0; i < meshVertexArray.size(); ++i)
        {
            glm::vec3& currentPosition = meshVertexArray[i].position;

            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;
        }
    }
}
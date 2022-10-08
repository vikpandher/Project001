#pragma once

#include "Engine/FontData.h"
#include "Engine/MeshData.h"
#include "Engine/TextureData.h"

#include <map>
#include <string>



namespace Project001
{
    class FreetypeTextLoader
    {
    public:
        static bool LoadTexture(
            TextureData& textureData,
            FontData& fontData,
            std::vector<unsigned char> characterList,
            const char* fontPath,
            unsigned int fontHeightInPixels = 48);

        static bool LoadMesh(
            MeshData& meshData,
            FontData& fontData,
            const std::string& text,
            float pixelSize = 0.01f,
            bool centeredLines = false,
            bool trangulate = s_triangulate);

    protected:
        static const bool s_flipVerticalyOnLoad;
        static const bool s_triangulate;

    private:
    };
}
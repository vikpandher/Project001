#pragma once

#include "Engine/FontData.h"
#include "Engine/MeshData.h"
#include "Engine/TextureData.h"

#include <map>



namespace Project001
{
    class FreetypeTextLoader
    {
    public:
        static bool GenerateTexture(
            TextureData& textureData,
            FontData& fontData,
            std::vector<unsigned char> characterList,
            const char* fontPath,
            unsigned int fontHeightInPixels = 48);

        // TODO: 
        static bool GenerateMesh(
            MeshData& meshData,
            FontData& fontData,
            float pixelSize = 0.01f);

    protected:
        static const bool s_flipVerticalyOnLoad = true;

    private:
    };
}
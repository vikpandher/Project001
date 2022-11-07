#pragma once

#include "Engine/TextureData.h"

#include <string>



namespace Project001
{

    class TextureLoader
    {
    public:
        static bool LoadTexture(TextureData& textureData, const std::string& path, bool convertToRGBA = false);

    protected:

    private:
    };
}
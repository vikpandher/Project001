#pragma once



namespace Project001
{
    struct TextureData
    {
        unsigned char* data;
        int width;
        int height;
        int numberOfComponents; // alternativly bytes per pixel
    };
}
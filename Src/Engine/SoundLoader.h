#pragma once

#include "Engine/SoundData.h"

#include <string>



namespace Project001
{
    class SoundLoader
    {
    public:
        static bool LoadSoundOGG(SoundData& soundData, const std::string& path);

        static bool LoadSoundWAV(SoundData& soundData, const std::string& path);

    protected:
        static uint32_t ConvertToUInt32(const char* buffer, size_t len);

        static bool IsBigEndian();
    };
}
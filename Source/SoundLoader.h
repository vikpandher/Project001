#pragma once

#include "SoundData.h"

#include <string>



namespace Project001
{
    class SoundLoader
    {
    public:
        static bool LoadSoundOGG(
            SoundData& soundData,
            const std::string& filePath);

        static bool LoadSoundOGGFromMemory(
            SoundData& soundData,
            const unsigned char* dataPtr,
            size_t dataSize);

        static bool LoadSoundWAV(
            SoundData& soundData,
            const std::string& filePath);

        static bool LoadSoundWAVFromMemory(
            SoundData& soundData,
            const unsigned char* dataPtr,
            size_t dataSize);

    protected:
        // Doesn't check destination memory size
        // Used in LoadSoundWAVFromMemory
        static bool Read(
            char* destinationPtr,
            const char*& incrementingSourcePtr,
            const char* pastTheEndOfSourcePtr,
            size_t copySize);

        // Used in LoadSoundWAV and LoadSoundWAVFromMemory
        static uint32_t ConvertToUInt32(
            const char* buffer,
            size_t len);

        // Used in ConvertToUInt32
        static bool IsBigEndian();
    };
}
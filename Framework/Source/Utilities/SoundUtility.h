// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#pragma once

#include "SoundData.h"

#include <string>



namespace Project001
{
namespace Sound
{
    bool LoadSoundOGG(
        SoundData& soundData,
        const std::string& filePath);

    bool LoadSoundOGGFromMemory(
        SoundData& soundData,
        const unsigned char* dataPtr,
        size_t dataSize);

    bool LoadSoundWAV(
        SoundData& soundData,
        const std::string& filePath);

    bool LoadSoundWAVFromMemory(
        SoundData& soundData,
        const unsigned char* dataPtr,
        size_t dataSize);

    // Helper Functions --------------------------------------------------------

    // Doesn't check destination memory size
    // Used in LoadSoundWAVFromMemory
    bool Read_H(
        char* destinationPtr,
        const char*& incrementingSourcePtr,
        const char* pastTheEndOfSourcePtr,
        size_t copySize);

    // Used in LoadSoundWAV and LoadSoundWAVFromMemory
    uint32_t ConvertToUInt32_H(
        const char* buffer,
        size_t len);

    // Used in ConvertToUInt32_H
    bool IsBigEndian_H();
}
}
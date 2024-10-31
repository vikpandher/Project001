// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include <cstdlib>



namespace Project001
{
    struct SoundData
    {
        SoundData();

        ~SoundData();

        void Clear();
    
        void* data;
        unsigned int sizeInBytes;
        unsigned int numberOfChannels;
        unsigned int sampleRate_Hz;
        unsigned int bitsPerSample;
        unsigned int sizeInFrames; // the number of samples per channel;
        float duration_s;
    };

    inline SoundData::SoundData()
        : data(nullptr)
        , sizeInBytes(0)
        , numberOfChannels(0)
        , sampleRate_Hz(0)
        , bitsPerSample(0)
        , sizeInFrames(0)
        , duration_s(0.0f)
    {}

    inline SoundData::~SoundData()
    {
        free(data);
        data = nullptr;
    }

    inline void SoundData::Clear()
    {
        free(data);
        data = nullptr;
        sizeInBytes = 0;
        numberOfChannels = 0;
        sampleRate_Hz = 0;
        bitsPerSample = 0;
        sizeInFrames = 0;
        duration_s = 0.0f;
    }
}
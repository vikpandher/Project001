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
        unsigned int numberOfChannels;
        unsigned int sampleRate;
        unsigned int bitsPerSample;
        unsigned int size;
    };

    inline SoundData::SoundData()
        : data(nullptr)
        , numberOfChannels(0)
        , sampleRate(0)
        , bitsPerSample(0)
        , size(0)
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
        numberOfChannels = 0;
        sampleRate = 0;
        bitsPerSample = 0;
        size = 0;
    }
}
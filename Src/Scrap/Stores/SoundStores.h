#pragma once

#include <string>
#include <vector>



namespace Project001
{
    struct SoundData
    {
        void* data;
        int numberOfChannels;
        int sampleRate;
        int bitsPerSample;
        int size;
    };

    class SoundStores
    {
    public:
        SoundStores();

        ~SoundStores();

        void ClearSounds();

        bool GetSound(unsigned int index, SoundData& soundDataPtr) const;

        bool LoadSoundOGG(unsigned int& index, const std::string& path);

        bool LoadSoundWAV(unsigned int& index, const std::string& path);

    protected:
        static int32_t ConvertToInt32(const char* buffer, size_t len);

        static bool IsBigEndian();

        std::vector<SoundData> soundDataArray_;

    private:
    };
}
#include "SoundStores.h"

#include <fstream>

#include "stb_vorbis.c"



namespace Project001
{
    // public: -----------------------------------------------------------------

    SoundStores::SoundStores()
    {}

    SoundStores::~SoundStores()
    {
        for (unsigned int i = 0; i < soundDataArray_.size(); ++i)
        {
            free(soundDataArray_[i].data);
        }
    }

    void SoundStores::ClearSounds()
    {
        for (unsigned int i = 0; i < soundDataArray_.size(); ++i)
        {
            free(soundDataArray_[i].data);
        }
        soundDataArray_.clear();
    }

    bool SoundStores::GetSound(unsigned int index, SoundData& soundDataPtr) const
    {
        if (index >= soundDataArray_.size())
        {
            return false;
        }

        soundDataPtr = soundDataArray_[index];

        return true;
    }

    bool SoundStores::LoadSoundOGG(unsigned int& index, const std::string& path)
    {
        SoundData newSoundData;
        int length = stb_vorbis_decode_filename(path.c_str(), &newSoundData.numberOfChannels, &newSoundData.sampleRate, (short**)&newSoundData.data);

        if (length < 1)
        {
            free(newSoundData.data);
            return false;
        }

        newSoundData.bitsPerSample = 16;
        newSoundData.size = length * newSoundData.numberOfChannels * sizeof(short);

        soundDataArray_.push_back(newSoundData);
        index = (unsigned int)(soundDataArray_.size() - 1);

        return true;
    }

    bool SoundStores::LoadSoundWAV(unsigned int& index, const std::string& path)
    {
        std::ifstream inputFileStream(path.c_str(), std::ios::binary);
        if (!inputFileStream.is_open())
        {
            return false;
        }

        char buffer[4];

        // The "RIFF" chunk descriptor
        // ---------------------------------------------------------------------
        // The Format concern here is "WAVE", which requires two sub-chunks:
        // "fmt" and "data"

        // ChunkId
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the ChunkId
            return false;
        }
        if (std::strncmp(buffer, "RIFF", 4) != 0)
        {
            // ERROR: file is not a valid WAVE file (ChunkId != RIFF)
            return false;
        }

        // ChunkSize
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the ChunkSize
            return false;
        }

        // Format
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the Format
            return false;
        }
        if (std::strncmp(buffer, "WAVE", 4) != 0)
        {
            // ERROR: file is not a valid WAVE file (Format != WAVE)
            return false;
        }

        // The "fmt" sub-chunk
        // ---------------------------------------------------------------------
        // describes the format of the sound information in the data sub-chunk

        // Subchunk1 ID ("fmt\0")
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the Subchunk1 ID ("fmt\0")
            return false;
        }
        if (std::strncmp(buffer, "fmt ", 4) != 0)
        {
            // ERROR: file is not a valid WAVE file (Subchunk1 ID != "fmt ")
            return false;
        }

        // Subchunk1 Size (always 16)
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the Subchunk1 Size (always 16)
            return false;
        }

        // AudioFormat
        if (!inputFileStream.read(buffer, 2))
        {
            // ERROR: could not read the AudioFormat
            return false;
        }

        SoundData newSoundData;

        // NumChannels
        if (!inputFileStream.read(buffer, 2))
        {
            // ERROR: could not read the NumChannels
            return false;
        }
        newSoundData.numberOfChannels = ConvertToInt32(buffer, 2);

        // SampleRate
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the SampleRate
            return false;
        }
        newSoundData.sampleRate = ConvertToInt32(buffer, 4);

        // ByteRate (sampleRate * bitsPerSample * channels) / 8
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the ByteRate
            return false;
        }

        // BlockAlign
        if (!inputFileStream.read(buffer, 2))
        {
            // ERROR: could not read the BlockAlign
            return false;
        }

        // BitsPerSample
        if (!inputFileStream.read(buffer, 2))
        {
            // ERROR: could not read the BitsPerSample
            return false;
        }
        newSoundData.bitsPerSample = ConvertToInt32(buffer, 2);

        // The "data" sub-chunk
        // ---------------------------------------------------------------------
        // Indicates the size of the sound information and contains the raw
        // raw sound data

        // Subchunk2 ID ("data")
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the Subchunk2 ID
            return false;
        }
        if (std::strncmp(buffer, "data", 4) != 0)
        {
            // ERROR: file is not a valid WAVE file (Subchunk2 ID != "data")
            return false;
        }

        // Subchunk2 Size
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the Subchunk2 Size
            return false;
        }
        newSoundData.size = ConvertToInt32(buffer, 4);

        if (inputFileStream.eof())
        {
            // ERROR: reached EOF on the file
            return false;
        }
        if (inputFileStream.fail())
        {
            //ERROR: fail state set on the file
            return false;
        }

        newSoundData.data = malloc((size_t)newSoundData.size);

        // Data
        inputFileStream.read((char*)newSoundData.data, (size_t)newSoundData.size);

        soundDataArray_.push_back(newSoundData);
        index = (unsigned int)(soundDataArray_.size() - 1);

        return true;
    }

    // protected: --------------------------------------------------------------

    int32_t SoundStores::ConvertToInt32(const char* buffer, size_t length)
    {
        if (length > 4)
        {
            length = 4;
        }
        union {
            int32_t i;
            char c[4];
        } result = { 0x00000000 };
        if (!IsBigEndian())
        {
            std::memcpy(&result, buffer, length);
        }
        else
        {
            for (std::size_t i = 0; i < length; ++i)
            {
                result.c[3 - i] = buffer[i];
            }
        }
        return result.i;
    }

    bool SoundStores::IsBigEndian()
    {
        union {
            uint32_t i;
            char c[4];
        } bint = { 0x01000000 };

        return bint.c[0] == 1;
    }
}
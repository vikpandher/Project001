#include "SoundLoader.h"

#include <fstream>

#include "stb_vorbis.c"



namespace Project001
{
    // public: -----------------------------------------------------------------

    bool SoundLoader::LoadSoundOGG(SoundData& soundData, const std::string& path)
    {
        int numberOfChannels;
        int sampleRate;
        int length = stb_vorbis_decode_filename(path.c_str(), &numberOfChannels, &sampleRate, (short**)&soundData.data);

        if (length < 1)
        {
            free(soundData.data);
            return false;
        }
        soundData.numberOfChannels = (unsigned int)numberOfChannels;
        soundData.sampleRate = (unsigned int)sampleRate;
        soundData.bitsPerSample = 16;
        soundData.size = length * soundData.numberOfChannels * sizeof(short);

        return true;
    }

    bool SoundLoader::LoadSoundWAV(SoundData& soundData, const std::string& path)
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

        // NumChannels
        if (!inputFileStream.read(buffer, 2))
        {
            // ERROR: could not read the NumChannels
            return false;
        }
        soundData.numberOfChannels = ConvertToUInt32(buffer, 2);

        // SampleRate
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the SampleRate
            return false;
        }
        soundData.sampleRate = ConvertToUInt32(buffer, 4);

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
        soundData.bitsPerSample = ConvertToUInt32(buffer, 2);

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
        soundData.size = ConvertToUInt32(buffer, 4);

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

        soundData.data = malloc((size_t)soundData.size);

        // Data
        inputFileStream.read((char*)soundData.data, (size_t)soundData.size);

        return true;
    }

    // protected: --------------------------------------------------------------

    uint32_t SoundLoader::ConvertToUInt32(const char* buffer, size_t length)
    {
        if (length > 4)
        {
            length = 4;
        }
        union {
            uint32_t i;
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

    bool SoundLoader::IsBigEndian()
    {
        union {
            uint32_t i;
            char c[4];
        } bint = { 0x01000000 };

        return bint.c[0] == 1;
    }
}
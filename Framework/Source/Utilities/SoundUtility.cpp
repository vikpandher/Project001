// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-19

#include "SoundUtility.h"

#include <fstream>

#include "stb_vorbis.c"



namespace Project001
{
namespace Sound
{
    bool LoadSoundOGG(
        SoundData& soundData,
        const std::string& filePath)
    {
        int numberOfChannels;
        int sampleRate_Hz;
        int numberOfSamplesPerChannel = stb_vorbis_decode_filename(filePath.c_str(), &numberOfChannels, &sampleRate_Hz, reinterpret_cast<short**>(&soundData.data));

        if (numberOfSamplesPerChannel < 1)
        {
            free(soundData.data);
            return false;
        }

        soundData.numberOfChannels = static_cast<unsigned int>(numberOfChannels);
        soundData.sampleRate_Hz = static_cast<unsigned int>(sampleRate_Hz);
        soundData.bitsPerSample = 16; // Assuming 16-bit samples.
        soundData.sizeInFrames = static_cast<unsigned int>(numberOfSamplesPerChannel);
        soundData.sizeInBytes = soundData.sizeInFrames * soundData.numberOfChannels * soundData.bitsPerSample / 8;
        soundData.duration_s = static_cast<float>(soundData.sizeInFrames) / static_cast<float>(soundData.sampleRate_Hz);

        return true;
    }

    bool LoadSoundOGGFromMemory(
        SoundData& soundData,
        const unsigned char* dataPtr,
        size_t dataSize)
    {
        int numberOfChannels;
        int sampleRate_Hz;
        int numberOfSamplesPerChannel = stb_vorbis_decode_memory(dataPtr, static_cast<int>(dataSize), &numberOfChannels, &sampleRate_Hz, reinterpret_cast<short**>(&soundData.data));

        if (numberOfSamplesPerChannel < 1)
        {
            free(soundData.data);
            return false;
        }

        soundData.numberOfChannels = static_cast<unsigned int>(numberOfChannels);
        soundData.sampleRate_Hz = static_cast<unsigned int>(sampleRate_Hz);
        soundData.bitsPerSample = 16; // Assuming 16-bit samples.
        soundData.sizeInFrames = static_cast<unsigned int>(numberOfSamplesPerChannel);
        soundData.sizeInBytes = soundData.sizeInFrames * soundData.numberOfChannels * soundData.bitsPerSample / 8;
        soundData.duration_s = static_cast<float>(soundData.sizeInFrames) / static_cast<float>(soundData.sampleRate_Hz);

        return true;
    }

    bool LoadSoundWAV(
        SoundData& soundData,
        const std::string& filePath)
    {
        std::ifstream inputFileStream(filePath.c_str(), std::ios::binary);
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

        uint32_t audioFormat = ConvertToUInt32_H(buffer, 2);

        if (audioFormat == 1)
        {
            // PCM (integer samples)
            // Handle integer sample data
        }
        else if (audioFormat == 3)
        {
            // IEEE floating-point (floating-point samples)
            // Handle floating-point sample data

            // Opting not to at this time.
            return false;
        }
        else
        {
            // Unsupported audio format
            return false;
        }

        // NumChannels
        if (!inputFileStream.read(buffer, 2))
        {
            // ERROR: could not read the NumChannels
            return false;
        }
        soundData.numberOfChannels = ConvertToUInt32_H(buffer, 2);

        // SampleRate
        if (!inputFileStream.read(buffer, 4))
        {
            // ERROR: could not read the SampleRate
            return false;
        }
        soundData.sampleRate_Hz = ConvertToUInt32_H(buffer, 4);

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
        soundData.bitsPerSample = ConvertToUInt32_H(buffer, 2);

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
        soundData.sizeInBytes = ConvertToUInt32_H(buffer, 4);

        unsigned int bytesPerFrame = (soundData.bitsPerSample / 8) * soundData.numberOfChannels;
        soundData.sizeInFrames = soundData.sizeInBytes / bytesPerFrame;
        soundData.duration_s = static_cast<float>(soundData.sizeInFrames) / static_cast<float>(soundData.sampleRate_Hz);

        if (inputFileStream.eof())
        {
            // ERROR: reached EOF on the file
            return false;
        }
        if (inputFileStream.fail())
        {
            // ERROR: fail state set on the file
            return false;
        }

        soundData.data = malloc(static_cast<size_t>(soundData.sizeInBytes));
        if (soundData.data == nullptr)
        {
            // ERROR: failed to allocate memory
            return false;
        }

        // Data
        if (!inputFileStream.read(reinterpret_cast<char*>(soundData.data), static_cast<size_t>(soundData.sizeInBytes)))
        {
            // ERROR: failed to read data memory
            return false;
        }

        return true;
    }

    bool LoadSoundWAVFromMemory(
        SoundData& soundData,
        const unsigned char* dataPtr,
        size_t dataSize)
    {
        if (dataPtr == nullptr)
        {
            return false;
        }

        const char* incrementingDataPtr = reinterpret_cast<const char*>(dataPtr);
        const char* pastTheEndOfSourcePtr = incrementingDataPtr + dataSize;

        size_t currentIndex = 0;
        char buffer[4];

        // The "RIFF" chunk descriptor
        // ---------------------------------------------------------------------
        // The Format concern here is "WAVE", which requires two sub-chunks:
        // "fmt" and "data"

        // ChunkId
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 4))
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
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 4))
        {
            // ERROR: could not read the ChunkSize
            return false;
        }

        // Format
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 4))
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
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 4))
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
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 4))
        {
            // ERROR: could not read the Subchunk1 Size (always 16)
            return false;
        }

        // AudioFormat
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 2))
        {
            // ERROR: could not read the AudioFormat
            return false;
        }

        uint32_t audioFormat = ConvertToUInt32_H(buffer, 2);

        if (audioFormat == 1)
        {
            // PCM (integer samples)
            // Handle integer sample data
        }
        else if (audioFormat == 3)
        {
            // IEEE floating-point (floating-point samples)
            // Handle floating-point sample data

            // Opting not to at this time.
            return false;
        }
        else
        {
            // Unsupported audio format
            return false;
        }

        // NumChannels
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 2))
        {
            // ERROR: could not read the NumChannels
            return false;
        }
        soundData.numberOfChannels = ConvertToUInt32_H(buffer, 2);

        // SampleRate
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 4))
        {
            // ERROR: could not read the SampleRate
            return false;
        }
        soundData.sampleRate_Hz = ConvertToUInt32_H(buffer, 4);

        // ByteRate (sampleRate * bitsPerSample * channels) / 8
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 4))
        {
            // ERROR: could not read the ByteRate
            return false;
        }

        // BlockAlign
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 2))
        {
            // ERROR: could not read the BlockAlign
            return false;
        }

        // BitsPerSample
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 2))
        {
            // ERROR: could not read the BitsPerSample
            return false;
        }
        soundData.bitsPerSample = ConvertToUInt32_H(buffer, 2);

        // The "data" sub-chunk
        // ---------------------------------------------------------------------
        // Indicates the size of the sound information and contains the raw
        // raw sound data

        // Subchunk2 ID ("data")
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 4))
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
        if (!Read_H(buffer, incrementingDataPtr, pastTheEndOfSourcePtr, 4))
        {
            // ERROR: could not read the Subchunk2 Size
            return false;
        }
        soundData.sizeInBytes = ConvertToUInt32_H(buffer, 4);

        unsigned int bytesPerFrame = (soundData.bitsPerSample / 8) * soundData.numberOfChannels;
        soundData.sizeInFrames = soundData.sizeInBytes / bytesPerFrame;
        soundData.duration_s = static_cast<float>(soundData.sizeInFrames) / static_cast<float>(soundData.sampleRate_Hz);

        soundData.data = malloc(static_cast<size_t>(soundData.sizeInBytes));
        if (soundData.data == nullptr)
        {
            // ERROR: failed to allocate memory
            return false;
        }

        // Data
        if (!Read_H(reinterpret_cast<char*>(soundData.data), incrementingDataPtr, pastTheEndOfSourcePtr, static_cast<size_t>(soundData.sizeInBytes)))
        {
            // ERROR: failed to read data memory
            return false;
        }

        return true;
    }

    // Helper Functions --------------------------------------------------------

    bool Read_H(
        char* destinationPtr,
        const char*& incrementingSourcePtr,
        const char* pastTheEndOfSourcePtr,
        size_t copySize)
    {
        if (incrementingSourcePtr + copySize <= pastTheEndOfSourcePtr)
        {
            memcpy(destinationPtr, incrementingSourcePtr, copySize);
            incrementingSourcePtr += copySize;
            return true;
        }
        return false;
    }

    uint32_t ConvertToUInt32_H(
        const char* buffer,
        size_t length)
    {
        if (length > 4)
        {
            length = 4;
        }
        union {
            uint32_t i;
            char c[4];
        } result = { 0x00000000 };
        if (!IsBigEndian_H())
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

    bool IsBigEndian_H()
    {
        union {
            uint32_t i;
            char c[4];
        } bint = { 0x01000000 };

        return bint.c[0] == 1;
    }
}
}
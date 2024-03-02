#pragma once

#include "SoundPlayer.h"

#include "AutoIdMap.h"



// Note:
// I cannot forward declare ma_audio_buffer because it contains flexable
// array members; I'm just using void.
struct ma_engine;
struct ma_sound;

namespace Project001
{
    class MiniAudio_SoundPlayer : public SoundPlayer
    {
    public:
        MiniAudio_SoundPlayer();

        ~MiniAudio_SoundPlayer();

        bool CreateSoundBuffer(
            unsigned int& soundBufferId,
            void* data,
            unsigned int sizeInBytes,
            unsigned int numberOfChannels,
            unsigned int sampleRate_Hz,
            unsigned int bitsPerSample,
            unsigned int sizeInFrames) override;

        // Deleting sound buffers that are part of existing sound sources will
        // result in undefined behaviour!
        bool DeleteSoundBuffer(unsigned int soundBufferId) override;

        void DeleteAllSoundBuffers() override;

        bool CreateSoundSource(
            unsigned int& soundSourceId,
            unsigned int soundBufferId) override;

        bool DeleteSoundSource(unsigned int soundSourceId) override;

        void DeleteAllSoundSources() override;

        // ---------------------------------------------------------------------

        bool GetIsPlayingSoundSource(unsigned int soundSourceId) override;

        bool PlaySoundSource(unsigned int soundSourceId) override;

        bool PauseSoundSource(unsigned int soundSourceId) override;

        bool StopSoundSource(unsigned int soundSourceId) override;

        // ---------------------------------------------------------------------

        bool SetSoundSourcePosition(
            unsigned int soundSourceId,
            const glm::vec3& position) override;
        bool GetSoundSourcePosition(
            unsigned int soundSourceId,
            glm::vec3& position) const override;

        bool SetSoundSourceForwardDirection(
            unsigned int soundSourceId,
            const glm::vec3& forward) override;
        bool GetSoundSourceForwardDirection(
            unsigned int soundSourceId,
            glm::vec3& forward) const override;

        bool SetSoundSourceVelocity(
            unsigned int soundSourceId,
            const glm::vec3& velocity) override;
        bool GetSoundSourceVelocity(
            unsigned int soundSourceId,
            glm::vec3& velocity) const override;

        bool SetSoundSourceCone(
            unsigned int soundSourceId,
            float innerAngleInRadians,
            float outerAngleInRadians,
            float outerGain) override;
        bool GetSoundSourceCone(
            unsigned int soundSourceId,
            float& innerAngleInRadians,
            float& outerAngleInRadians,
            float& outerGain) const override;

        bool SetSoundSourceVolume(
            unsigned int soundSourceId,
            float volume) override;
        bool GetSoundSourceVolume(
            unsigned int soundSourceId,
            float& volume) const override;

        bool SetSoundSourcePitch(
            unsigned int soundSourceId,
            float pitch) override;
        bool GetSoundSourcePitch(
            unsigned int soundSourceId,
            float& pitch) const override;

        bool SetSoundSourceRollOff(
            unsigned int soundSourceId,
            float rollOff) override;
        bool GetSoundSourceRollOff(
            unsigned int soundSourceId,
            float& rollOff) const override;

        bool SetSoundSourceMinGain(
            unsigned int soundSourceId,
            float minGain) override;
        bool GetSoundSourceMinGain(
            unsigned int soundSourceId,
            float& minGain) const override;

        bool SetSoundSourceMaxGain(
            unsigned int soundSourceId,
            float maxGain) override;
        bool GetSoundSourceMaxGain(
            unsigned int soundSourceId,
            float& maxGain) const override;

        bool SetSoundSourceMinDistance(
            unsigned int soundSourceId,
            float minDistance) override;
        bool GetSoundSourceMinDistance(
            unsigned int soundSourceId,
            float& minDistance) const override;

        bool SetSoundSourceMaxDistance(
            unsigned int soundSourceId,
            float maxDistance) override;
        bool GetSoundSourceMaxDistance(
            unsigned int soundSourceId,
            float& maxDistance) const override;

        bool SetSoundSourceLooping(
            unsigned int soundSourceId,
            bool looping) override;
        bool GetSoundSourceLooping(
            unsigned int soundSourceId,
            bool& looping) const override;

        // ---------------------------------------------------------------------

        void ResetListener() override;

        void SetListenerPosition(const glm::vec3& position) override;
        void GetListenerPosition(glm::vec3& position) const override;

        void SetListenerOientation(
            const glm::vec3& forward,
            const glm::vec3& up) override;
        void GetListenerOientation(
            glm::vec3& forward,
            glm::vec3& up) const override;

        void SetListenerForwardDirection(const glm::vec3& forward) override;
        void GetListenerForwardDirection(glm::vec3& forward) const override;

        void SetListenerUpDirection(const glm::vec3& up) override;
        void GetListenerUpDirection(glm::vec3& up) const override;

        void SetListenerVelocity(const glm::vec3& velocity) override;
        void GetListenerVelocity(glm::vec3& velocity) const override;

        void SetListenerCone(
            float innerAngleInRadians,
            float outerAngleInRadians,
            float outerGain) override;
        void GetListenerCone(
            float& innerAngleInRadians,
            float& outerAngleInRadians,
            float& outerGain) const override;

    protected:
        struct MiniAudio_SoundSource
        {
            // MiniAudio_SoundSource has it's own audio buffer because miniaudio
            // audio buffers hold the cursor that tracks the position in the
            // data while playing. Two sound sources can't simultaniously play
            // from the same audio buffer at different positions in the audio
            // data.
            // 
            // This audio buffer won't allocate or deallocate the data though,
            // it will point to data allocated by the audio buffers in 
            // soundBufferPtrMap_.
            void* audioBufferPtr;
            ma_sound* soundPtr;
        };

        static unsigned int CalculateSizeInFrames(
            unsigned int sizeInBytes,
            unsigned int numberOfChannels,
            unsigned int bitsPerSample);

        ma_engine* enginePtr_;

        AutoIdMap<void*> soundBufferPtrMap_;

        AutoIdMap<MiniAudio_SoundSource> soundSourceMap_;

        // TODO:
        // Work on deleting the associated sound sources when deleting a sound
        // buffer.
        // 
        // std::unordered_map<unsigned int, std::vector<unsigned int>> soundBufferIdSoundSourceIdsMap_;
        // std::unordered_map<unsigned int, unsigned int> soundSourceIdSoundBufferIdMap_;
    };

    // protected ---------------------------------------------------------------

    inline unsigned int MiniAudio_SoundPlayer::CalculateSizeInFrames(
        unsigned int sizeInBytes,
        unsigned int numberOfChannels,
        unsigned int bitsPerSample)
    {
        return (sizeInBytes * 8) / (numberOfChannels * bitsPerSample);
    }
}
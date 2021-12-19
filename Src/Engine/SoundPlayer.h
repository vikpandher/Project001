#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    class SoundPlayer
    {
    public:
        static SoundPlayer* Create();

        virtual ~SoundPlayer();

        virtual bool UpdateListener(
            const glm::vec3& position,
            const glm::vec3& orientationForward,
            const glm::vec3& orientationUp,
            const glm::vec3& velocity,
            float gain) = 0;

        virtual void RemoveAllSoundBuffers() = 0;

        virtual bool CreateSoundBuffer(
            unsigned int soundBufferIndex,
            void* data,
            int numberOfChannels,
            int sampleRate,
            int bitsPerSample,
            int size) = 0;

        virtual bool CreateSoundSource(unsigned int& soundSourceIndex) = 0;

        virtual bool LinkSoundBufferToSoundSource(
            unsigned int soundBufferIndex,
            unsigned int soundSourceIndex) = 0;

        virtual bool UpdateSoundSource(
            unsigned int soundSourceIndex,
            const glm::vec3& position,
            const glm::vec3& velocity,
            float pitch,
            float gain,
            bool loop) = 0;

        virtual void RemoveAllSoundSources() = 0;

        virtual bool PlaySoundSource(unsigned int soundSourceIndex) = 0;

        virtual bool PauseSoundSource(unsigned int soundSourceIndex) = 0;

        virtual bool RewindSoundSource(unsigned int soundSourceIndex) = 0;

        virtual bool StopSoundSource(unsigned int soundSourceIndex) = 0;

    protected:

    private:
    };
}
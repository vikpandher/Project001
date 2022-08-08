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

        virtual bool CreateSoundBuffer(
            unsigned int& soundBufferId,
            void* data,
            int numberOfChannels,
            int sampleRate,
            int bitsPerSample,
            int size) = 0;

        virtual bool DeleteSoundBuffer(unsigned int soundBufferId) = 0;

        virtual void DeleteAllSoundBuffers() = 0;

        virtual bool CreateSoundSource(unsigned int& soundSourceId) = 0;

        virtual bool DeleteSoundSource(unsigned int soundSourceId) = 0;

        virtual void DeleteAllSoundSources() = 0;

        virtual bool LinkSoundBufferToSoundSource(
            unsigned int soundBufferId,
            unsigned int soundSourceId) = 0;

        virtual bool UpdateSoundSource(
            unsigned int soundSourceId,
            const glm::vec3& position,
            const glm::vec3& velocity,
            float pitch,
            float gain,
            bool loop) = 0;

        virtual bool PlaySoundSource(unsigned int soundSourceId) = 0;

        virtual bool PauseSoundSource(unsigned int soundSourceId) = 0;

        virtual bool RewindSoundSource(unsigned int soundSourceId) = 0;

        virtual bool StopSoundSource(unsigned int soundSourceId) = 0;

    protected:

    private:
    };
}
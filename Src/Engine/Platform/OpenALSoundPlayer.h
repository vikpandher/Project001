#pragma once

#include <map>
#include <vector>

#include "AL/al.h"
#include "AL/alc.h"

#include "Engine/SoundPlayer.h"



namespace Project001
{
    class OpenALSoundPlayer : public SoundPlayer
    {
    public:
        OpenALSoundPlayer();

        ~OpenALSoundPlayer();

        bool UpdateListener(
            const glm::vec3& position,
            const glm::vec3& orientationForward,
            const glm::vec3& orientationUp,
            const glm::vec3& velocity,
            float gain);

        void RemoveAllSoundBuffers();

        bool CreateSoundBuffer(
            unsigned int soundBufferIndex,
            void* data,
            int numberOfChannels,
            int sampleRate,
            int bitsPerSample,
            int size);

        bool CreateSoundSource(unsigned int& soundSourceIndex);

        bool LinkSoundBufferToSoundSource(
            unsigned int soundBufferIndex,
            unsigned int soundSourceIndex);

        bool UpdateSoundSource(
            unsigned int soundSourceIndex,
            const glm::vec3& position,
            const glm::vec3& velocity,
            float pitch,
            float gain,
            bool loop);

        void RemoveAllSoundSources();

        bool PlaySoundSource(unsigned int soundSourceIndex);

        bool PauseSoundSource(unsigned int soundSourceIndex);

        bool RewindSoundSource(unsigned int soundSourceIndex);

        bool StopSoundSource(unsigned int soundSourceIndex);

    protected:
        static size_t s_instanceCount_;
        static ALCdevice* s_devicePtr_;
        static ALCcontext* s_contextPtr_;

        std::map<unsigned int, ALuint> soundBufferIdMap_;
        std::vector<ALuint> soundSourceIds_;

    private:
    };
}
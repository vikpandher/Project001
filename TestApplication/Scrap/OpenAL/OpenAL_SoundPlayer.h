#pragma once

#include "Engine/SoundPlayer.h"

#include "AL/al.h"
#include "AL/alc.h"

#include <deque>
#include <map>
#include <vector>



namespace Project001
{
    class OpenAL_SoundPlayer : public SoundPlayer
    {
    public:
        OpenAL_SoundPlayer();

        ~OpenAL_SoundPlayer();

        bool UpdateListener(
            const glm::vec3& position,
            const glm::vec3& orientationForward,
            const glm::vec3& orientationUp,
            const glm::vec3& velocity,
            float gain);

        bool CreateSoundBuffer(
            unsigned int& soundBufferId,
            void* data,
            int numberOfChannels,
            int sampleRate,
            int bitsPerSample,
            int size);

        // Buffers attached to Sources cannot be deleted!
        bool DeleteSoundBuffer(unsigned int soundBufferId);

        // Buffers attached to Sources cannot be deleted!
        void DeleteAllSoundBuffers();

        bool CreateSoundSource(unsigned int& soundSourceId);

        bool DeleteSoundSource(unsigned int soundSourceId);

        void DeleteAllSoundSources();

        bool LinkSoundBufferToSoundSource(
            unsigned int soundBufferId,
            unsigned int soundSourceId);

        bool UpdateSoundSource(
            unsigned int soundSourceId,
            const glm::vec3& position,
            const glm::vec3& velocity,
            float pitch,
            float gain,
            bool loop);

        bool PlaySoundSource(unsigned int soundSourceId);

        bool PauseSoundSource(unsigned int soundSourceId);

        bool RewindSoundSource(unsigned int soundSourceId);

        bool StopSoundSource(unsigned int soundSourceId);

    protected:
        static size_t s_instanceCount_;
        static ALCdevice* s_devicePtr_;
        static ALCcontext* s_contextPtr_;

        std::deque<unsigned int> recycledSoundBufferIds_;
        std::map<unsigned int, ALuint> soundBufferIdMap_;

        std::deque<unsigned int> recycledSoundSourceIds_;
        std::map<unsigned int, ALuint> soundSourceIdMap_;

    private:
    };
}
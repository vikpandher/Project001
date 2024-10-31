// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    class SoundPlayer
    {
    public:
        static SoundPlayer* Create();

        virtual ~SoundPlayer();

        virtual bool CreateSoundBuffer(
            unsigned int& soundBufferId,
            void* data,
            unsigned int sizeInBytes,
            unsigned int numberOfChannels,
            unsigned int sampleRate_Hz,
            unsigned int bitsPerSample,
            unsigned int sizeInFrames) = 0;

        virtual bool DeleteSoundBuffer(unsigned int soundBufferId) = 0;

        virtual void DeleteAllSoundBuffers() = 0;

        virtual bool CreateSoundSource(
            unsigned int& soundSourceId,
            unsigned int soundBufferId) = 0;

        virtual bool DeleteSoundSource(unsigned int soundSourceId) = 0;

        virtual void DeleteAllSoundSources() = 0;

        // ---------------------------------------------------------------------

        virtual bool GetIsPlayingSoundSource(unsigned int soundSourceId) = 0;

        virtual bool PlaySoundSource(unsigned int soundSourceId) = 0;

        virtual bool PauseSoundSource(unsigned int soundSourceId) = 0;

        virtual bool StopSoundSource(unsigned int soundSourceId) = 0;

        // ---------------------------------------------------------------------

        virtual bool SetSoundSourcePosition(
            unsigned int soundSourceId,
            const glm::vec3& position) = 0;
        virtual bool GetSoundSourcePosition(
            unsigned int soundSourceId,
            glm::vec3& position) const = 0;

        virtual bool SetSoundSourceForwardDirection(
            unsigned int soundSourceId,
            const glm::vec3& forward) = 0;
        virtual bool GetSoundSourceForwardDirection(
            unsigned int soundSourceId,
            glm::vec3& forward) const = 0;

        virtual bool SetSoundSourceVelocity(
            unsigned int soundSourceId,
            const glm::vec3& velocity) = 0;
        virtual bool GetSoundSourceVelocity(
            unsigned int soundSourceId,
            glm::vec3& velocity) const = 0;

        virtual bool SetSoundSourceCone(
            unsigned int soundSourceId,
            float innerAngleInRadians,
            float outerAngleInRadians,
            float outerGain) = 0;
        virtual bool GetSoundSourceCone(
            unsigned int soundSourceId,
            float& innerAngleInRadians,
            float& outerAngleInRadians,
            float& outerGain) const = 0;

        virtual bool SetSoundSourceVolume(
            unsigned int soundSourceId,
            float volume) = 0;
        virtual bool GetSoundSourceVolume(
            unsigned int soundSourceId,
            float& volume) const = 0;

        virtual bool SetSoundSourcePitch(
            unsigned int soundSourceId,
            float pitch) = 0;
        virtual bool GetSoundSourcePitch(
            unsigned int soundSourceId,
            float& pitch) const = 0;

        virtual bool SetSoundSourceRollOff(
            unsigned int soundSourceId,
            float rollOff) = 0;
        virtual bool GetSoundSourceRollOff(
            unsigned int soundSourceId,
            float& rollOff) const = 0;

        virtual bool SetSoundSourceMinGain(
            unsigned int soundSourceId,
            float minGain) = 0;
        virtual bool GetSoundSourceMinGain(
            unsigned int soundSourceId,
            float& minGain) const = 0;

        virtual bool SetSoundSourceMaxGain(
            unsigned int soundSourceId,
            float maxGain) = 0;
        virtual bool GetSoundSourceMaxGain(
            unsigned int soundSourceId,
            float& maxGain) const = 0;

        virtual bool SetSoundSourceMinDistance(
            unsigned int soundSourceId,
            float minDistance) = 0;
        virtual bool GetSoundSourceMinDistance(
            unsigned int soundSourceId,
            float& minDistance) const = 0;

        virtual bool SetSoundSourceMaxDistance(
            unsigned int soundSourceId,
            float maxDistance) = 0;
        virtual bool GetSoundSourceMaxDistance(
            unsigned int soundSourceId,
            float& maxDistance) const = 0;

        virtual bool SetSoundSourceLooping(
            unsigned int soundSourceId,
            bool looping) = 0;
        virtual bool GetSoundSourceLooping(
            unsigned int soundSourceId,
            bool& looping) const = 0;

        // ---------------------------------------------------------------------

        // By default the listener's forward vector is (0.0f, 0.0f, 1.0f).

        virtual void ResetListener() = 0;

        virtual void SetListenerPosition(const glm::vec3& position) = 0;
        virtual void GetListenerPosition(glm::vec3& position) const = 0;

        virtual void SetListenerOientation(
            const glm::vec3& forward,
            const glm::vec3& up) = 0;
        virtual void GetListenerOientation(
            glm::vec3& forward,
            glm::vec3& up) const = 0;

        virtual void SetListenerForwardDirection(const glm::vec3& forward) = 0;
        virtual void GetListenerForwardDirection(glm::vec3& position) const = 0;

        virtual void SetListenerUpDirection(const glm::vec3& up) = 0;
        virtual void GetListenerUpDirection(glm::vec3& up) const = 0;

        virtual void SetListenerVelocity(const glm::vec3& velocity) = 0;
        virtual void GetListenerVelocity(glm::vec3& velocity) const = 0;

        virtual void SetListenerCone(
            float innerAngleInRadians,
            float outerAngleInRadians,
            float outerGain) = 0;
        virtual void GetListenerCone(
            float& innerAngleInRadians,
            float& outerAngleInRadians,
            float& outerGain) const = 0;
    };
}
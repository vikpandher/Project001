#include "OpenALSoundPlayer.h"

#include "Engine/Platform/OpenALError.h"



namespace Project001
{
    // public: -----------------------------------------------------------------

    OpenALSoundPlayer::OpenALSoundPlayer()
    {
        if (s_instanceCount_ == 0)
        {
            // NOTE: Adding the following makes the sound sound all jumbled:
            // 
            // ALCdevice* device01Ptr = alcOpenDevice(nullptr);
            // alcLogError(device01Ptr);
            // ALCcontext* context01Ptr = alcCreateContext(device01Ptr, nullptr);
            // alcLogError(device01Ptr);
            // 
            // ??? I don't know why.

            // get the defualt device
            s_devicePtr_ = alcOpenDevice(nullptr);
            alcLogError(s_devicePtr_);

            s_contextPtr_ = alcCreateContext(s_devicePtr_, nullptr);
            alcLogError(s_devicePtr_);

            if (!alcMakeContextCurrent(s_contextPtr_))
            {
                alcLogError(s_devicePtr_);
            }
        }

        s_instanceCount_++;
    }

    OpenALSoundPlayer::~OpenALSoundPlayer()
    {
        RemoveAllSoundSources();
        RemoveAllSoundBuffers();

        if (s_instanceCount_ == 1)
        {
            if (!alcMakeContextCurrent(nullptr))
            {
                alcLogError(s_devicePtr_);
            }

            alcDestroyContext(s_contextPtr_);
            alcLogError(s_devicePtr_);

            if (!alcCloseDevice(s_devicePtr_))
            {
                alcLogError(s_devicePtr_);
            }
        }
        s_instanceCount_--;
    }

    bool OpenALSoundPlayer::UpdateListener(
        const glm::vec3& position,
        const glm::vec3& orientationForward,
        const glm::vec3& orientationUp,
        const glm::vec3& velocity,
        float gain)
    {
        alListener3f(AL_POSITION, position.x, position.y, position.z);
        float orientation[6] = {
            orientationForward.x,
            orientationForward.y,
            orientationForward.z,
            orientationUp.x,
            orientationUp.y,
            orientationUp.z
        };
        alListenerfv(AL_ORIENTATION, orientation);
        if (alCheckError())
        {
            return false;
        }
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        if (alCheckError())
        {
            return false;
        }
        alListenerf(AL_GAIN, gain);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    void OpenALSoundPlayer::RemoveAllSoundBuffers()
    {
        std::map<unsigned int, ALuint>::iterator iter;
        for (iter = soundBufferIdMap_.begin(); iter != soundBufferIdMap_.end(); ++iter)
        {
            ALuint currentBufferId = iter->second;

            alDeleteBuffers(1, &currentBufferId);
            alCheckError();
        }
        soundBufferIdMap_.clear();
    }

    bool OpenALSoundPlayer::CreateSoundBuffer(
        unsigned int soundBufferIndex,
        void* data,
        int numberOfChannels,
        int sampleRate,
        int bitsPerSample,
        int size)
    {
        if (soundBufferIdMap_.find(soundBufferIndex) != soundBufferIdMap_.end())
        {
            return false;
        }

        ALuint soundBufferId;
        alGenBuffers(1, &soundBufferId);
        if (alCheckError())
        {
            return false;
        }

        ALenum format;
        if (numberOfChannels == 1 && bitsPerSample == 8)
        {
            format = AL_FORMAT_MONO8;
        }
        else if (numberOfChannels == 1 && bitsPerSample == 16)
        {
            format = AL_FORMAT_MONO16;
        }
        else if (numberOfChannels == 2 && bitsPerSample == 8)
        {
            format = AL_FORMAT_STEREO8;
        }
        else if (numberOfChannels == 2 && bitsPerSample == 16)
        {
            format = AL_FORMAT_STEREO16;
        }
        // need to include "AL/alext.h"
        // else if (numberOfChannels == 3)
        // {
        //     format = AL_FORMAT_BFORMAT2D_16;
        // }
        // else if (numberOfChannels == 4)
        // {
        //     format = AL_FORMAT_BFORMAT3D_16;
        // }
        else
        {
            alDeleteBuffers(1, &soundBufferId);
            alLogError();

            return false;
        }

        alBufferData(soundBufferId, format, data, size, sampleRate);
        if (alCheckError())
        {
            return false;
        }

        soundBufferIdMap_[soundBufferIndex] = soundBufferId;

        return true;
    }

    bool OpenALSoundPlayer::CreateSoundSource(unsigned int& soundSourceIndex)
    {
        ALuint soundSourceId;
        alGenSources(1, &soundSourceId);
        if (alCheckError())
        {
            return false;
        }

        soundSourceIds_.push_back(soundSourceId);
        soundSourceIndex = (unsigned int)(soundSourceIds_.size() - 1);

        return true;
    }

    bool OpenALSoundPlayer::LinkSoundBufferToSoundSource(
        unsigned int soundBufferIndex,
        unsigned int soundSourceIndex)
    {
        if (soundSourceIndex > soundSourceIds_.size() - 1 ||
            soundBufferIdMap_.find(soundBufferIndex) == soundBufferIdMap_.end())
        {
            return false;
        }

        ALuint soundSourceId = soundSourceIds_[soundSourceIndex];
        ALuint soundBufferId = soundBufferIdMap_[soundBufferIndex];

        alSourcei(soundSourceId, AL_BUFFER, soundBufferId);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    bool OpenALSoundPlayer::UpdateSoundSource(
        unsigned int soundSourceIndex,
        const glm::vec3& position,
        const glm::vec3& velocity,
        float pitch,
        float gain,
        bool loop)
    {
        if (soundSourceIndex > soundSourceIds_.size() - 1)
        {
            return false;
        }

        ALuint soundSourceId = soundSourceIds_[soundSourceIndex];

        alSource3f(soundSourceId, AL_POSITION, position.x, position.y, position.z);
        if (alCheckError())
        {
            return false;
        }
        alSource3f(soundSourceId, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        if (alCheckError())
        {
            return false;
        }
        alSourcef(soundSourceId, AL_PITCH, pitch);
        if (alCheckError())
        {
            return false;
        }
        alSourcef(soundSourceId, AL_GAIN, gain);
        if (alCheckError())
        {
            return false;
        }
        alSourcei(soundSourceId, AL_LOOPING, loop);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    void OpenALSoundPlayer::RemoveAllSoundSources()
    {
        for (size_t i = 0; i < soundSourceIds_.size(); ++i)
        {
            ALuint soundSourceId = soundSourceIds_[i];
            alDeleteSources(1, &soundSourceId);
        }
        soundSourceIds_.clear();
    }

    bool OpenALSoundPlayer::PlaySoundSource(unsigned int soundSourceIndex)
    {
        if (soundSourceIndex > soundSourceIds_.size() - 1)
        {
            return false;
        }

        ALuint soundSourceId = soundSourceIds_[soundSourceIndex];

        alSourcePlay(soundSourceId);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    bool OpenALSoundPlayer::PauseSoundSource(unsigned int soundSourceIndex)
    {
        if (soundSourceIndex > soundSourceIds_.size() - 1)
        {
            return false;
        }

        ALuint soundSourceId = soundSourceIds_[soundSourceIndex];

        alSourcePause(soundSourceId);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    bool OpenALSoundPlayer::RewindSoundSource(unsigned int soundSourceIndex)
    {
        if (soundSourceIndex > soundSourceIds_.size() - 1)
        {
            return false;
        }

        ALuint soundSourceId = soundSourceIds_[soundSourceIndex];

        alSourceRewind(soundSourceId);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    bool OpenALSoundPlayer::StopSoundSource(unsigned int soundSourceIndex)
    {
        if (soundSourceIndex > soundSourceIds_.size() - 1)
        {
            return false;
        }

        ALuint soundSourceId = soundSourceIds_[soundSourceIndex];

        alSourceStop(soundSourceId);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    // protected: --------------------------------------------------------------

    size_t OpenALSoundPlayer::s_instanceCount_ = 0;
    ALCdevice* OpenALSoundPlayer::s_devicePtr_ = nullptr;
    ALCcontext* OpenALSoundPlayer::s_contextPtr_ = nullptr;
}
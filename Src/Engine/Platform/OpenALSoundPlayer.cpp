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
        DeleteAllSoundSources();
        DeleteAllSoundBuffers();

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

    bool OpenALSoundPlayer::CreateSoundBuffer(
        unsigned int& soundBufferId,
        void* data,
        int numberOfChannels,
        int sampleRate,
        int bitsPerSample,
        int size)
    {
        ALuint al_soundBufferId;
        alGenBuffers(1, &al_soundBufferId);
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
            alDeleteBuffers(1, &al_soundBufferId);
            alLogError();

            return false;
        }

        alBufferData(al_soundBufferId, format, data, size, sampleRate);
        if (alCheckError())
        {
            return false;
        }

        if (recycledSoundBufferIds_.empty())
        {
            soundBufferId = (unsigned int)soundBufferIdMap_.size();
        }
        else
        {
            soundBufferId = recycledSoundBufferIds_.front();
            recycledSoundBufferIds_.pop_front();
        }

        soundBufferIdMap_[soundBufferId] = al_soundBufferId;

        return true;
    }

    bool OpenALSoundPlayer::DeleteSoundBuffer(unsigned int soundBufferId)
    {
        if (soundBufferIdMap_.find(soundBufferId) != soundBufferIdMap_.end())
        {
            alDeleteBuffers(1, &soundBufferIdMap_[soundBufferId]);
            alLogError();
            recycledSoundBufferIds_.push_back(soundBufferId);
            return true;
        }
        else
        {
            return false;
        }
    }

    void OpenALSoundPlayer::DeleteAllSoundBuffers()
    {
        std::map<unsigned int, ALuint>::iterator iter;
        for (iter = soundBufferIdMap_.begin(); iter != soundBufferIdMap_.end(); ++iter)
        {
            ALuint currentSoundBufferId = iter->second;
            alDeleteBuffers(1, &currentSoundBufferId);
            alLogError();
        }
        soundBufferIdMap_.clear();
        recycledSoundBufferIds_.clear();
    }

    bool OpenALSoundPlayer::CreateSoundSource(unsigned int& soundSourceId)
    {
        ALuint al_soundSourceId;
        alGenSources(1, &al_soundSourceId);
        if (alCheckError())
        {
            return false;
        }

        if (recycledSoundSourceIds_.empty())
        {
            soundSourceId = (unsigned int)soundSourceIdMap_.size();
        }
        else
        {
            soundSourceId = recycledSoundSourceIds_.front();
            recycledSoundSourceIds_.pop_front();
        }

        soundSourceIdMap_[soundSourceId] = al_soundSourceId;

        return true;
    }

    bool OpenALSoundPlayer::LinkSoundBufferToSoundSource(
        unsigned int soundBufferId,
        unsigned int soundSourceId)
    {
        if (soundBufferIdMap_.find(soundBufferId) == soundBufferIdMap_.end() ||
            soundSourceIdMap_.find(soundSourceId) == soundSourceIdMap_.end())
        {
            return false;
        }

        ALuint al_soundSourceId = soundSourceIdMap_[soundSourceId];
        ALuint al_soundBufferId = soundBufferIdMap_[soundBufferId];

        alSourcei(al_soundSourceId, AL_BUFFER, al_soundBufferId);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    bool OpenALSoundPlayer::UpdateSoundSource(
        unsigned int soundSourceId,
        const glm::vec3& position,
        const glm::vec3& velocity,
        float pitch,
        float gain,
        bool loop)
    {
        if (soundSourceIdMap_.find(soundSourceId) == soundSourceIdMap_.end())
        {
            return false;
        }

        ALuint al_soundSourceId = soundSourceIdMap_[soundSourceId];

        alSource3f(al_soundSourceId, AL_POSITION, position.x, position.y, position.z);
        if (alCheckError())
        {
            return false;
        }
        alSource3f(al_soundSourceId, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        if (alCheckError())
        {
            return false;
        }
        alSourcef(al_soundSourceId, AL_PITCH, pitch);
        if (alCheckError())
        {
            return false;
        }
        alSourcef(al_soundSourceId, AL_GAIN, gain);
        if (alCheckError())
        {
            return false;
        }
        alSourcei(al_soundSourceId, AL_LOOPING, loop);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    bool OpenALSoundPlayer::DeleteSoundSource(unsigned int soundSourceId)
    {
        if (soundSourceIdMap_.find(soundSourceId) != soundSourceIdMap_.end())
        {
            alDeleteSources(1, &soundSourceIdMap_[soundSourceId]);
            alLogError();
            recycledSoundSourceIds_.push_back(soundSourceId);
            return true;
        }
        else
        {
            return false;
        }
    }

    void OpenALSoundPlayer::DeleteAllSoundSources()
    {
        std::map<unsigned int, ALuint>::iterator iter;
        for (iter = soundSourceIdMap_.begin(); iter != soundSourceIdMap_.end(); ++iter)
        {
            ALuint currentSoundSourceId = iter->second;
            alDeleteSources(1, &currentSoundSourceId);
            alLogError();
        }
        soundSourceIdMap_.clear();
        recycledSoundSourceIds_.clear();
    }

    bool OpenALSoundPlayer::PlaySoundSource(unsigned int soundSourceId)
    {
        if (soundSourceIdMap_.find(soundSourceId) == soundSourceIdMap_.end())
        {
            return false;
        }

        ALuint al_soundSourceId = soundSourceIdMap_[soundSourceId];

        alSourcePlay(al_soundSourceId);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    bool OpenALSoundPlayer::PauseSoundSource(unsigned int soundSourceId)
    {
        if (soundSourceIdMap_.find(soundSourceId) == soundSourceIdMap_.end())
        {
            return false;
        }

        ALuint al_soundSourceId = soundSourceIdMap_[soundSourceId];

        alSourcePause(al_soundSourceId);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    bool OpenALSoundPlayer::RewindSoundSource(unsigned int soundSourceId)
    {
        if (soundSourceIdMap_.find(soundSourceId) == soundSourceIdMap_.end())
        {
            return false;
        }

        ALuint al_soundSourceId = soundSourceIdMap_[soundSourceId];

        alSourceRewind(al_soundSourceId);
        if (alCheckError())
        {
            return false;
        }

        return true;
    }

    bool OpenALSoundPlayer::StopSoundSource(unsigned int soundSourceId)
    {
        if (soundSourceIdMap_.find(soundSourceId) == soundSourceIdMap_.end())
        {
            return false;
        }

        ALuint al_soundSourceId = soundSourceIdMap_[soundSourceId];

        alSourceStop(al_soundSourceId);
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
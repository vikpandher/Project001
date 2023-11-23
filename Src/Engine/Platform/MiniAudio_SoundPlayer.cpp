#include "MiniAudio_SoundPlayer.h"

#include "Engine/Logger.h"

#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_RESOURCE_MANAGER
#include "miniaudio.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    MiniAudio_SoundPlayer::MiniAudio_SoundPlayer()
    {
        enginePtr_ = new ma_engine;

        ma_result result = ma_engine_init(NULL, enginePtr_);
        if (result != MA_SUCCESS)
        {
            _LOG_ERROR("Failed to initialize audio engine.");

            delete enginePtr_;
            enginePtr_ = NULL;
        }
    }

    MiniAudio_SoundPlayer::~MiniAudio_SoundPlayer()
    {
        DeleteAllSoundSources();
        DeleteAllSoundBuffers();

        if (enginePtr_ != NULL)
        {
            ma_engine_uninit(enginePtr_);
            delete enginePtr_;
            enginePtr_ = NULL;
        }
    }

    bool MiniAudio_SoundPlayer::CreateSoundBuffer(
        unsigned int& soundBufferId,
        void* data,
        unsigned int sizeInBytes,
        unsigned int numberOfChannels,
        unsigned int sampleRate_Hz,
        unsigned int bitsPerSample,
        unsigned int sizeInFrames)
    {
        ma_format format = ma_format_unknown;

        if (bitsPerSample == 8)
        {
            format = ma_format_u8;
        }
        else if (bitsPerSample == 16)
        {
            format = ma_format_s16;
        }
        else if (bitsPerSample == 24)
        {
            format = ma_format_s24;
        }
        else if (bitsPerSample == 32)
        {
            format = ma_format_s32;
        }
        else
        {
            _LOG_ERROR("Unrecognized format; failed to create sound buffer.");
            return false;
        }

        ma_audio_buffer_config bufferConfig = ma_audio_buffer_config_init(
            format,
            numberOfChannels,
            sizeInFrames,
            data,
            NULL
        );

        ma_audio_buffer* audioBufferPtr = new ma_audio_buffer;
        // ma_result result = ma_audio_buffer_alloc_and_init(&bufferConfig, &audioBufferPtr);
        ma_result result = ma_audio_buffer_init(&bufferConfig, audioBufferPtr);
        if (result != MA_SUCCESS)
        {
            _LOG_ERROR("Failed to create sound buffer (ma_audio_buffer).");
            return false;
        }

        if (recycledSoundBufferIds_.empty())
        {
            soundBufferId = (unsigned int)soundBufferPtrMap_.size();
        }
        else
        {
            soundBufferId = recycledSoundBufferIds_.front();
            recycledSoundBufferIds_.pop_front();
        }

        soundBufferPtrMap_[soundBufferId] = audioBufferPtr;

        // ---------------------------------------------------------------------

        // ma_sound* soundPtr = new ma_sound;
        // result = ma_sound_init_from_data_source(enginePtr_, &audioBufferPtr->ref.ds, 0, NULL, soundPtr);
        // 
        // ma_sound_set_looping(soundPtr, true);
        // 
        // ma_sound_start(soundPtr);
        // 
        // while (true) {}
        // 
        // ma_sound_uninit(soundPtr);

        // ---------------------------------------------------------------------

        return true;
    }

    bool MiniAudio_SoundPlayer::DeleteSoundBuffer(unsigned int soundBufferId)
    {
        std::unordered_map<unsigned int, void*>::iterator iter = soundBufferPtrMap_.find(soundBufferId);
        if (iter != soundBufferPtrMap_.end())
        {
            // ma_audio_buffer_uninit_and_free((ma_audio_buffer*)iter->second);
            ma_audio_buffer_uninit((ma_audio_buffer*)iter->second);
            delete (ma_audio_buffer*)iter->second;
            recycledSoundBufferIds_.push_back(soundBufferId);
            return true;
        }
    
        _LOG_ERROR("Failed to delete sound buffer. Sound buffer not found.");
        return false;
    }

    void MiniAudio_SoundPlayer::DeleteAllSoundBuffers()
    {
        for (std::unordered_map<unsigned int, void*>::iterator iter = soundBufferPtrMap_.begin();
            iter != soundBufferPtrMap_.end(); ++iter)
        {
            // ma_audio_buffer_uninit_and_free((ma_audio_buffer*)iter->second);
            ma_audio_buffer_uninit((ma_audio_buffer*)iter->second);
            delete (ma_audio_buffer*)iter->second;
        }
        soundBufferPtrMap_.clear();
        recycledSoundBufferIds_.clear();
    }

    bool MiniAudio_SoundPlayer::CreateSoundSource(
        unsigned int& soundSourceId,
        unsigned int soundBufferId)
    {
        ma_audio_buffer* audioBufferPtr;
        std::unordered_map<unsigned int, void*>::iterator iter = soundBufferPtrMap_.find(soundBufferId);
        if (iter == soundBufferPtrMap_.end())
        {
            _LOG_ERROR("Failed to find sound buffer.");
            return false;
        }
        audioBufferPtr = (ma_audio_buffer*)iter->second;

        MiniAudio_SoundSource newSoundSource;
        newSoundSource.audioBufferPtr = new ma_audio_buffer;
        newSoundSource.soundPtr = new ma_sound;

        ma_audio_buffer_config bufferConfig = ma_audio_buffer_config_init(
            audioBufferPtr->ref.format,
            audioBufferPtr->ref.channels,
            audioBufferPtr->ref.sizeInFrames,
            audioBufferPtr->ref.pData,
            NULL
        );

        ma_result result = ma_audio_buffer_init(&bufferConfig, (ma_audio_buffer*)newSoundSource.audioBufferPtr);
        if (result != MA_SUCCESS)
        {
            _LOG_ERROR("Failed to create sound buffer (ma_audio_buffer).");
            return false;
        }

        result = ma_sound_init_from_data_source(enginePtr_, &((ma_audio_buffer*)newSoundSource.audioBufferPtr)->ref.ds, 0, NULL, newSoundSource.soundPtr);
        if (result != MA_SUCCESS)
        {
            _LOG_ERROR("Failed to create sound source (ma_sound).");
            return false;
        }

        if (recycledSoundSourceIds_.empty())
        {
            soundSourceId = (unsigned int)soundSourcePtrMap_.size();
        }
        else
        {
            soundSourceId = recycledSoundSourceIds_.front();
            recycledSoundSourceIds_.pop_front();
        }

        soundSourcePtrMap_[soundSourceId] = newSoundSource;

        return true;
    }

    bool MiniAudio_SoundPlayer::DeleteSoundSource(unsigned int soundSourceId)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            MiniAudio_SoundSource& soundSource = iter->second;
            ma_sound_stop(soundSource.soundPtr);
            ma_sound_uninit(soundSource.soundPtr);
            delete soundSource.soundPtr;
            delete soundSource.audioBufferPtr;
            recycledSoundSourceIds_.push_back(soundSourceId);
            return true;
        }

        _LOG_ERROR("Failed to delete sound source. Sound source not found.");
        return false;
    }

    void MiniAudio_SoundPlayer::DeleteAllSoundSources()
    {
        for (std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.begin();
            iter != soundSourcePtrMap_.end(); ++iter)
        {
            MiniAudio_SoundSource& soundSource = iter->second;
            ma_sound_stop(soundSource.soundPtr);
            ma_sound_uninit(soundSource.soundPtr);
            delete soundSource.soundPtr;
            delete soundSource.audioBufferPtr;
        }
        soundSourcePtrMap_.clear();
        recycledSoundSourceIds_.clear();
    }

    bool MiniAudio_SoundPlayer::GetIsPlayingSoundSource(unsigned int soundSourceId)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            return (bool)ma_sound_is_playing(iter->second.soundPtr);
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::PlaySoundSource(unsigned int soundSourceId)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_result result = ma_sound_start(iter->second.soundPtr);
            return result == ma_result::MA_SUCCESS;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::PauseSoundSource(unsigned int soundSourceId)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_result result = ma_sound_stop(iter->second.soundPtr);
            return result == ma_result::MA_SUCCESS;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::StopSoundSource(unsigned int soundSourceId)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_result result = ma_sound_stop(iter->second.soundPtr);

            if (result != ma_result::MA_SUCCESS)
            {
                return false;
            }

            result = ma_data_source_seek_to_pcm_frame(iter->second.soundPtr->pDataSource, 0);

            return result == ma_result::MA_SUCCESS;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourcePosition(
        unsigned int soundSourceId,
        const glm::vec3& position)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_position(iter->second.soundPtr, position.x, position.y, position.z);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourcePosition(
        unsigned int soundSourceId,
        glm::vec3& position) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_vec3f soundPosition = ma_sound_get_position(iter->second.soundPtr);
            position.x = soundPosition.x;
            position.y = soundPosition.y;
            position.z = soundPosition.z;
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourceForwardDirection(
        unsigned int soundSourceId,
        const glm::vec3& forward)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_direction(iter->second.soundPtr, forward.x, forward.y, forward.z);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourceForwardDirection(
        unsigned int soundSourceId,
        glm::vec3& forward) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_vec3f direction = ma_sound_get_direction(iter->second.soundPtr);
            forward.x = direction.x;
            forward.y = direction.y;
            forward.z = direction.z;
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourceVelocity(
        unsigned int soundSourceId,
        const glm::vec3& velocity)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_velocity(iter->second.soundPtr, velocity.x, velocity.y, velocity.z);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourceVelocity(
        unsigned int soundSourceId,
        glm::vec3& velocity) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_vec3f soundVelocity = ma_sound_get_velocity(iter->second.soundPtr);
            velocity.x = soundVelocity.x;
            velocity.y = soundVelocity.y;
            velocity.z = soundVelocity.z;
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourceCone(
        unsigned int soundSourceId,
        float innerAngleInRadians,
        float outerAngleInRadians,
        float outerGain)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_cone(iter->second.soundPtr, innerAngleInRadians, outerAngleInRadians, outerGain);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourceCone(
        unsigned int soundSourceId,
        float& innerAngleInRadians,
        float& outerAngleInRadians,
        float& outerGain) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_get_cone(iter->second.soundPtr, &innerAngleInRadians, &outerAngleInRadians, &outerGain);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourceVolume(
        unsigned int soundSourceId,
        float volume)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_volume(iter->second.soundPtr, volume);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourceVolume(
        unsigned int soundSourceId,
        float& volume) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            volume = ma_sound_get_volume(iter->second.soundPtr);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourcePitch(
        unsigned int soundSourceId,
        float pitch)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_pitch(iter->second.soundPtr, pitch);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourcePitch(
        unsigned int soundSourceId,
        float& pitch) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            pitch = ma_sound_get_pitch(iter->second.soundPtr);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourceRollOff(
        unsigned int soundSourceId,
        float rollOff)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_rolloff(iter->second.soundPtr, rollOff);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourceRollOff(
        unsigned int soundSourceId,
        float& rollOff) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            rollOff = ma_sound_get_rolloff(iter->second.soundPtr);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourceMinGain(
        unsigned int soundSourceId,
        float minGain)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_min_gain(iter->second.soundPtr, minGain);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourceMinGain(
        unsigned int soundSourceId,
        float& minGain) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            minGain = ma_sound_get_min_gain(iter->second.soundPtr);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourceMaxGain(
        unsigned int soundSourceId,
        float maxGain)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_max_gain(iter->second.soundPtr, maxGain);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourceMaxGain(
        unsigned int soundSourceId,
        float& maxGain) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            maxGain = ma_sound_get_max_gain(iter->second.soundPtr);
            return true;
        }
        return false;
    }
    
    bool MiniAudio_SoundPlayer::SetSoundSourceMinDistance(
        unsigned int soundSourceId,
        float minDistance)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_min_distance(iter->second.soundPtr, minDistance);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourceMinDistance(
        unsigned int soundSourceId,
        float& minDistance) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            minDistance = ma_sound_get_min_distance(iter->second.soundPtr);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourceMaxDistance(
        unsigned int soundSourceId,
        float maxDistance)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_max_distance(iter->second.soundPtr, maxDistance);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourceMaxDistance(
        unsigned int soundSourceId,
        float& maxDistance) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            maxDistance = ma_sound_get_max_distance(iter->second.soundPtr);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::SetSoundSourceLooping(
        unsigned int soundSourceId,
        bool looping)
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            ma_sound_set_looping (iter->second.soundPtr, looping);
            return true;
        }
        return false;
    }

    bool MiniAudio_SoundPlayer::GetSoundSourceLooping(
        unsigned int soundSourceId,
        bool& looping) const
    {
        std::unordered_map<unsigned int, MiniAudio_SoundSource>::const_iterator iter = soundSourcePtrMap_.find(soundSourceId);
        if (iter != soundSourcePtrMap_.end())
        {
            looping = ma_sound_is_looping(iter->second.soundPtr);
            return true;
        }
        return false;
    }
    
    void MiniAudio_SoundPlayer::SetListenerPosition(const glm::vec3& position)
    {
        ma_engine_listener_set_position(enginePtr_, 0, position.x, position.y, position.z);
    }

    void MiniAudio_SoundPlayer::GetListenerPosition(glm::vec3& position) const
    {
        ma_vec3f listenerPosition = ma_engine_listener_get_position(enginePtr_, 0);
        position.x = listenerPosition.x;
        position.y = listenerPosition.y;
        position.z = listenerPosition.z;
    }

    void MiniAudio_SoundPlayer::SetListenerOientation(
        const glm::vec3& forward,
        const glm::vec3& up)
    {
        ma_engine_listener_set_direction(enginePtr_, 0, forward.x, forward.y, forward.z);
        ma_engine_listener_set_world_up(enginePtr_, 0, up.x, up.y, up.z);
    }

    void MiniAudio_SoundPlayer::GetListenerOientation(
        glm::vec3& forward,
        glm::vec3& up) const
    {
        ma_vec3f direction = ma_engine_listener_get_direction(enginePtr_, 0);
        forward.x = direction.x;
        forward.y = direction.y;
        forward.z = direction.z;
        ma_vec3f worldUp = ma_engine_listener_get_world_up(enginePtr_, 0);
        up.x = worldUp.x;
        up.y = worldUp.y;
        up.z = worldUp.z;
    }

    void MiniAudio_SoundPlayer::SetListenerForwardDirection(const glm::vec3& forward)
    {
        ma_engine_listener_set_direction(enginePtr_, 0, forward.x, forward.y, forward.z);
    }

    void MiniAudio_SoundPlayer::GetListenerForwardDirection(glm::vec3& forward) const
    {
        ma_vec3f direction = ma_engine_listener_get_direction(enginePtr_, 0);
        forward.x = direction.x;
        forward.y = direction.y;
        forward.z = direction.z;
    }

    void MiniAudio_SoundPlayer::SetListenerUpDirection(const glm::vec3& up)
    {
        ma_engine_listener_set_world_up(enginePtr_, 0, up.x, up.y, up.z);
    }

    void MiniAudio_SoundPlayer::GetListenerUpDirection(glm::vec3& up) const
    {
        ma_vec3f worldUp = ma_engine_listener_get_world_up(enginePtr_, 0);
        up.x = worldUp.x;
        up.y = worldUp.y;
        up.z = worldUp.z;
    }

    void MiniAudio_SoundPlayer::SetListenerVelocity(const glm::vec3& velocity)
    {
        ma_engine_listener_set_velocity(enginePtr_, 0, velocity.x, velocity.y, velocity.z);
    }

    void MiniAudio_SoundPlayer::GetListenerVelocity(glm::vec3& velocity) const
    {
        ma_vec3f listenerVelocity = ma_engine_listener_get_velocity(enginePtr_, 0);
        velocity.x = listenerVelocity.x;
        velocity.y = listenerVelocity.y;
        velocity.z = listenerVelocity.z;
    }

    void MiniAudio_SoundPlayer::SetListenerCone(
        float innerAngleInRadians,
        float outerAngleInRadians,
        float outerGain)
    {
        ma_engine_listener_set_cone(enginePtr_, 0, innerAngleInRadians, outerAngleInRadians, outerGain);
    }

    void MiniAudio_SoundPlayer::GetListenerCone(
        float& innerAngleInRadians,
        float& outerAngleInRadians,
        float& outerGain) const
    {
        ma_engine_listener_get_cone(enginePtr_, 0, &innerAngleInRadians, &outerAngleInRadians, &outerGain);
    }

    // protected ---------------------------------------------------------------


}
#pragma once

#include "Engine/Components/Position.h"



namespace Project001
{
    class SoundSource : public Position
    {
    public:
        SoundSource();
        ~SoundSource();

        void SetSoundSourceIndex(unsigned int soundSourceIndex);
        unsigned int GetSoundSourceIndex() const;

        void SetVelocity(const glm::vec3& velocity);
        void SetVelocity(float x, float y, float z);
        void SetVelocityX(float x);
        void SetVelocityY(float y);
        void SetVelocityZ(float z);
        const glm::vec3& GetVelocity() const;
        float GetVelocityX() const;
        float GetVelocityY() const;
        float GetVelocityZ() const;

        void AddVelocity(const glm::vec3& velocity);
        void AddVelocity(float x, float y, float z);
        void AddVelocityX(float x);
        void AddVelocityY(float y);
        void AddVelocityZ(float z);

        void SetPitch(float pitch);
        float GetPitch() const;

        void SetGain(float gain);
        float GetGain() const;

        void SetLoop(bool loop);
        bool GetLoop() const;

     protected:
        unsigned int soundSourceIndex_;
        glm::vec3 velocity_;
        float pitch_;
        float gain_;
        bool loop_;

    private:
    };

    inline void SoundSource::SetSoundSourceIndex(unsigned int soundSourceIndex)
    {
        soundSourceIndex_ = soundSourceIndex;
    }

    inline unsigned int SoundSource::GetSoundSourceIndex() const
    {
        return soundSourceIndex_;
    }

    inline void SoundSource::SetVelocity(const glm::vec3& velocity)
    {
        velocity_ = velocity;
    }

    inline void SoundSource::SetVelocity(float x, float y, float z)
    {
        velocity_.x = x;
        velocity_.y = y;
        velocity_.z = z;
    }

    inline void SoundSource::SetVelocityX(float x)
    {
        velocity_.x = x;
    }

    inline void SoundSource::SetVelocityY(float y)
    {
        velocity_.y = y;
    }

    inline void SoundSource::SetVelocityZ(float z)
    {
        velocity_.z = z;
    }

    inline const glm::vec3& SoundSource::GetVelocity() const
    {
        return velocity_;
    }

    inline float SoundSource::GetVelocityX() const
    {
        return velocity_.x;
    }

    inline float SoundSource::GetVelocityY() const
    {
        return velocity_.y;
    }

    inline float SoundSource::GetVelocityZ() const
    {
        return velocity_.z;
    }

    inline void SoundSource::AddVelocity(const glm::vec3& velocity)
    {
        velocity_ += velocity;
    }

    inline void SoundSource::AddVelocity(float x, float y, float z)
    {
        velocity_.x += x;
        velocity_.y += y;
        velocity_.z += z;
    }

    inline void SoundSource::AddVelocityX(float x)
    {
        velocity_.x += x;
    }

    inline void SoundSource::AddVelocityY(float y)
    {
        velocity_.y += y;
    }

    inline void SoundSource::AddVelocityZ(float z)
    {
        velocity_.z += z;
    }

    inline void SoundSource::SetPitch(float pitch)
    {
        pitch_ = pitch;
    }

    inline float SoundSource::GetPitch() const
    {
        return pitch_;
    }

    inline void SoundSource::SetGain(float gain)
    {
        gain_ = gain;
    }

    inline float SoundSource::GetGain() const
    {
        return gain_;
    }

    inline void SoundSource::SetLoop(bool loop)
    {
        loop_ = loop;
    }

    inline bool SoundSource::GetLoop() const
    {
        return loop_;
    }
}
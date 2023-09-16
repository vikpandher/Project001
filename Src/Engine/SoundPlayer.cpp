#include "SoundPlayer.h"

#include "Engine/Platform/MiniAudio_SoundPlayer.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    SoundPlayer* SoundPlayer::Create()
    {
        return new MiniAudio_SoundPlayer();
    }

    SoundPlayer::~SoundPlayer()
    {}
}
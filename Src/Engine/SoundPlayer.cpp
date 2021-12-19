#include "SoundPlayer.h"

#include "Engine/Platform/OpenALSoundPlayer.h"



namespace Project001
{
    // public: -----------------------------------------------------------------

    SoundPlayer* SoundPlayer::Create()
    {
        return new OpenALSoundPlayer();
    }

    SoundPlayer::~SoundPlayer()
    {}
}
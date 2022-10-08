#include "SoundPlayer.h"

#include "Engine/Platform/OpenAL_SoundPlayer.h"



namespace Project001
{
    // public: -----------------------------------------------------------------

    SoundPlayer* SoundPlayer::Create()
    {
        return new OpenAL_SoundPlayer();
    }

    SoundPlayer::~SoundPlayer()
    {}
}
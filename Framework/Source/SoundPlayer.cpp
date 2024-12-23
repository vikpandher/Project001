// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "SoundPlayer.h"

#include "Platform/MiniAudio_SoundPlayer.h"



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
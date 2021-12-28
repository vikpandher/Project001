#include "SoundSource.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    SoundSource::SoundSource()
        : soundSourceIndex_((unsigned int)-1)
        , velocity_(0.0f, 0.0f, 0.0f)
        , pitch_(1.0f)
        , gain_(1.0f)
        , loop_(false)
    {}

    SoundSource::~SoundSource()
    {}
}
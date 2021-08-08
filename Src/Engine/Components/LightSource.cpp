#include "LightSource.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    LightSource::LightSource()
        : turnedOn_(false)
        , lightType_(LightType::LIGHT_TYPE_POINT)
        , direction_(0.0f, 0.0f, 0.0f)
        , cutoff_(0.99f)
        , outerCutoff_(0.97f)
        , constant_(1.0f)
        , linear_(0.0f)
        , quadratic_(0.0f)
        , ambient_(0.0f, 0.0f, 0.0f)
        , diffuse_(1.0f, 1.0f, 1.0f)
        , specular_(0.0f, 0.0f, 0.0f)
    {}

    LightSource::~LightSource()
    {}
}
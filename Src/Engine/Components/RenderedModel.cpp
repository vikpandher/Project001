#include "RenderedModel.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    RenderedModel::RenderedModel()
        : textureIndex_((unsigned int)-1)
        , specularIndex_((unsigned int)-1)
        , shininess_(32.0f)
        , color_(1.0f, 1.0f, 1.0f, 1.0f)
    {}

    RenderedModel::~RenderedModel()
    {}
}
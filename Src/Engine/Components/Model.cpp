#include "Model.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    Model::Model()
        : modelIndex_((unsigned int)-1)
        , scale_(1.0f, 1.0f, 1.0f)
    {}

    Model::~Model()
    {}
}
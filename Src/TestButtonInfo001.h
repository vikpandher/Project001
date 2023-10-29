#pragma once

#include "Engine/MeshData.h"

#include <string>



struct TestButtonInfo001
{
    std::string textString_;

    unsigned int aboveEntityId_ = (unsigned int)-1;
    unsigned int leftEntityId_ = (unsigned int)-1;
    unsigned int bellowEntityId_ = (unsigned int)-1;
    unsigned int rightEntityId_ = (unsigned int)-1;
};
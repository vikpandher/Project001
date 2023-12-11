#pragma once

#include "Engine/MeshData.h"

#include <string>



struct TestButtonData001
{
    std::string textString;

    unsigned int destinationSceneId = (unsigned int)-1;

    unsigned int aboveEntityId = (unsigned int)-1;
    unsigned int leftEntityId = (unsigned int)-1;
    unsigned int bellowEntityId = (unsigned int)-1;
    unsigned int rightEntityId = (unsigned int)-1;
};
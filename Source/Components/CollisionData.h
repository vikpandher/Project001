#pragma once

namespace Project001
{
    struct CollisionData
    {
        unsigned int myShapeTag;

        unsigned int otherEntityId;
        unsigned int otherShapeTag;
    };
}
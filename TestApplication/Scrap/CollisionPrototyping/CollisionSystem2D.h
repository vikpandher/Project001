#pragma once

#include "Engine/Math/Shapes2D.h"



namespace Project001
{
    class CollisionBody2D;
    class ComponentStores;

    class CollisionSystem2D
    {
    public:
        static void CalculateCollisions(ComponentStores* componentStoresPtr);

        static void CalculateCollisionsForGivenEntity(
            unsigned int entityId,
            ComponentStores* componentStoresPtr);

    protected:
        static void CalculateCollisions(
            unsigned int entityIdA,
            CollisionBody2D& collisionBodyA,
            unsigned int entityIdB,
            CollisionBody2D& collisionBodyB,
            bool recordInBodyB);

        static std::vector<CollisionBody2D*> s_tangibleCollisionBodyPtrs_;
    };
}
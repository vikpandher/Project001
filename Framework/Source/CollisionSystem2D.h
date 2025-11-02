// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "CollisionBodyQuadTree2D.h"

#include <unordered_set>



namespace Project001
{
    class CollisionBody2D;
    class ComponentStores;

    struct CollisionBodyQuadTreeNode2D;

    class CollisionSystem2D
    {
    public:
        static void ApplyMovement(ComponentStores* componentStoresPtr, float timestep_s);

        static void CalculateCollisions(ComponentStores* componentStoresPtr);

        static void CalculateOverlapForGivenEntity(
            unsigned int entityId,
            ComponentStores* componentStoresPtr);

        static void CalculateCollisionsWithQuadTree(ComponentStores* componentStoresPtr);

        static const CollisionBodyQuadTree2D& GetCollisionBodyQuadTree2D();

        static void ResetCollisionBodyQuadTree2D(
            const glm::vec2& min,
            const glm::vec2& max,
            size_t maxDepth,
            size_t maxBodiesPerNode);

        static float s_sunkenMeshSeperationSpacing;

    protected:
        struct PointerPairHashFunctor
        {
            std::size_t operator()(const std::pair<void*, void*>& obj) const {
                std::size_t hashA = std::hash<void*>{}(obj.first);
                std::size_t hashB = std::hash<void*>{}(obj.second);
                // Combine the hash values using a simple hash combining technique
                return hashA ^ (hashB << 1);
            }
        };

        struct CollisionManifold2D
        {
            CollisionBody2D* collisionBodyA_Ptr;
            CollisionBody2D* collisionBodyB_Ptr;

            glm::vec2 collisionPoint;
            glm::vec2 collisionNormal; // should point away from bodyA
            float collisionDepth;
        };

        static void CalculateCollisionsBetweenBodyPairs(ComponentStores* componentStoresPtr);

        static void CalculateCollisionsBetweenTwoBodies(
            unsigned int entityIdA,
            CollisionBody2D& collisionBodyA,
            unsigned int entityIdB,
            CollisionBody2D& collisionBodyB,
            bool recordInBodyB,
            bool collectDetailedCollisionData,
            bool resolvePhysics);

        static void ResolveCollisions();

        static std::vector<CollisionBody2D*> s_enabledCollisionBodyPtrs_;

        static std::vector< CollisionBody2D*> s_outOfBoundsTangibleCollisionBodyPtrs_;

        static CollisionBodyQuadTree2D s_tangibleCollisionBodyQuadTree2D_;

        static std::unordered_set<std::pair<CollisionBody2D*, CollisionBody2D*>, PointerPairHashFunctor> s_collisionBodyPairPtrs_;

        static std::vector<CollisionManifold2D> s_collisionManifolds_;
    };

    // public ------------------------------------------------------------------

    inline const CollisionBodyQuadTree2D& CollisionSystem2D::GetCollisionBodyQuadTree2D()
    {
        return s_tangibleCollisionBodyQuadTree2D_;
    }

    inline void CollisionSystem2D::ResetCollisionBodyQuadTree2D(
        const glm::vec2& min,
        const glm::vec2& max,
        size_t maxDepth,
        size_t maxBodiesPerNode)
    {
        s_tangibleCollisionBodyQuadTree2D_.Reset(min, max, maxDepth, maxBodiesPerNode);
    }
}
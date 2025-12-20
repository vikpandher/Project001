// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-20

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
        CollisionSystem2D();

        CollisionSystem2D(CollisionSystem2D& other) = delete;
        void operator=(const CollisionSystem2D&) = delete;

        void ApplyMovement(ComponentStores* componentStoresPtr, float timestep_s);

        void CalculateCollisions(ComponentStores* componentStoresPtr);

        void CalculateCollisionsForGivenEntity(
            unsigned int entityId,
            ComponentStores* componentStoresPtr);

        void CalculateCollisionsWithQuadTree(ComponentStores* componentStoresPtr);

        const CollisionBodyQuadTree2D& GetCollisionBodyQuadTree2D();

        void ResetCollisionBodyQuadTree2D(
            const glm::vec2& min,
            const glm::vec2& max,
            size_t maxDepth,
            size_t maxBodiesPerNode);

        void FullyLoadCollisionBodyQuadTree2D();

        float sunkenMeshOverlapAllowance;
        float sunkenMeshSeperationSpacing;

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

        void CalculateCollisionsBetweenBodyPairs(ComponentStores* componentStoresPtr);

        void CalculateCollisionsBetweenTwoBodiesAndAddManifold(
            unsigned int entityIdA,
            CollisionBody2D& collisionBodyA,
            unsigned int entityIdB,
            CollisionBody2D& collisionBodyB,
            bool recordInBodyB);

        void ResolveCollisions();

        std::vector<CollisionBody2D*> enabledCollisionBodyPtrs_;

        std::vector< CollisionBody2D*> outOfBoundsTangibleCollisionBodyPtrs_;

        CollisionBodyQuadTree2D tangibleCollisionBodyQuadTree2D_;

        std::unordered_set<std::pair<CollisionBody2D*, CollisionBody2D*>, PointerPairHashFunctor> collisionBodyPairPtrs_;

        std::vector<CollisionManifold2D> collisionManifolds_;
    };

    // public ------------------------------------------------------------------

    inline const CollisionBodyQuadTree2D& CollisionSystem2D::GetCollisionBodyQuadTree2D()
    {
        return tangibleCollisionBodyQuadTree2D_;
    }

    inline void CollisionSystem2D::ResetCollisionBodyQuadTree2D(
        const glm::vec2& min,
        const glm::vec2& max,
        size_t maxDepth,
        size_t maxBodiesPerNode)
    {
        tangibleCollisionBodyQuadTree2D_.Reset(min, max, maxDepth, maxBodiesPerNode);
    }

    inline void CollisionSystem2D::FullyLoadCollisionBodyQuadTree2D()
    {
        tangibleCollisionBodyQuadTree2D_.FullyLoadTree();
    }
}
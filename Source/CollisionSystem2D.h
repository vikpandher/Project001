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
        static void CalculateCollisions(ComponentStores* componentStoresPtr);

        static void CalculateCollisionsForGivenEntity(
            unsigned int entityId,
            ComponentStores* componentStoresPtr);

        static void CalculateCollisionsWithQuadTree(ComponentStores* componentStoresPtr);

        static CollisionBodyQuadTree2D& GetCollisionBodyQuadTree2D();

        static void ResetCollisionBodyQuadTree2D(
            const glm::vec2& min,
            const glm::vec2& max,
            size_t maxDepth,
            size_t maxBodiesPerNode);

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

        static void CalculateCollisionsBetweenTwoBodies(
            unsigned int entityIdA,
            CollisionBody2D& collisionBodyA,
            unsigned int entityIdB,
            CollisionBody2D& collisionBodyB,
            bool recordInBodyB);

        static std::vector<CollisionBody2D*> s_tangibleCollisionBodyPtrs_;

        static std::vector< CollisionBody2D*> s_outOfBoundsTangibleCollisionBodyPtrs_;

        static CollisionBodyQuadTree2D s_tangibleCollisionBodyQuadTree2D_;

        static std::unordered_set<std::pair<CollisionBody2D*, CollisionBody2D*>, PointerPairHashFunctor> s_collisionBodyPairPtrs_;
    };

    inline CollisionBodyQuadTree2D& CollisionSystem2D::GetCollisionBodyQuadTree2D()
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
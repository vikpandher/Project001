#pragma once

#include "glm/glm.hpp"

#include <vector>



namespace Project001
{
    class CollisionBody2D;

    struct CollisionBodyQuadTreeNode2D
    {
        CollisionBodyQuadTreeNode2D(size_t depth = 1, float minX = 0.0f, float minY = 0.0f, float maxX = 0.0f, float maxY = 0.0f)
            : leafNode(true)
            , depth(depth)
            , min(minX, minY)
            , max(maxX, maxY)
            , childrenPtrs{ nullptr }
        {}

        bool leafNode;

        size_t depth;

        glm::vec2 min;
        glm::vec2 max;

        std::vector<CollisionBody2D*> bodyPtrs;

        CollisionBodyQuadTreeNode2D* childrenPtrs[4]; // Four children nodes (NE, NW, SW, SE)
    };

    class CollisionBodyQuadTree2D
    {
    public:
        // A maxDepth value of 0 will be set to 1.
        // A maxBodiesPerNode value of 0 will be set to 1.
        CollisionBodyQuadTree2D(const glm::vec2& min, const glm::vec2& max, size_t maxDepth, size_t maxBodiesPerNode);
        ~CollisionBodyQuadTree2D();

        CollisionBodyQuadTree2D(CollisionBodyQuadTree2D& other) = delete;
        void operator=(const CollisionBodyQuadTree2D&) = delete;

        void Reset(const glm::vec2& min, const glm::vec2& max, size_t maxDepth, size_t maxBodiesPerNode);

        bool Insert(CollisionBody2D* bodyPtr);

        void Clear();

        CollisionBodyQuadTreeNode2D* GetRootNode();
        const CollisionBodyQuadTreeNode2D* GetRootNode() const;

    protected:
        void DeleteAllNodes();

        size_t maxDepth_;
        size_t maxBodiesPerNode_;

        CollisionBodyQuadTreeNode2D* rootNodePtr_;
    };

    inline CollisionBodyQuadTreeNode2D* CollisionBodyQuadTree2D::GetRootNode()
    {
        return rootNodePtr_;
    }

    inline const CollisionBodyQuadTreeNode2D* CollisionBodyQuadTree2D::GetRootNode() const
    {
        return rootNodePtr_;
    }
}
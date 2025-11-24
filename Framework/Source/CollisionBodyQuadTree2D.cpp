// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-23

#include "CollisionBodyQuadTree2D.h"

#include "Components/CollisionBody2D.h"
#include "Math/Overlap2D.h"

#include <stack>



namespace Project001
{
    // public ------------------------------------------------------------------

    CollisionBodyQuadTree2D::CollisionBodyQuadTree2D(const glm::vec2& min, const glm::vec2& max, size_t maxDepth, size_t maxBodiesPerNode)
    {
        maxDepth_ = maxDepth;

        if (maxBodiesPerNode == 0)
        {
            maxBodiesPerNode_ = 1;
        }
        else
        {
            maxBodiesPerNode_ = maxBodiesPerNode;
        }

        rootNodePtr_ = new CollisionBodyQuadTreeNode2D(1, min.x, min.y, max.x, max.y);
    }

    CollisionBodyQuadTree2D::~CollisionBodyQuadTree2D()
    {
        DeleteAllNodes();
    }

    void CollisionBodyQuadTree2D::FullyLoadTree()
    {
        std::stack<CollisionBodyQuadTreeNode2D*> nodePtrStack;
        nodePtrStack.push(rootNodePtr_);

        while (!nodePtrStack.empty())
        {
            CollisionBodyQuadTreeNode2D* nodePtr = nodePtrStack.top();
            nodePtrStack.pop();

            if (nodePtr->depth == maxDepth_)
            {
                continue;
            }

            // Create the child nodes, if they don't exist.
            if (nodePtr->childrenPtrs[0] == nullptr)
            {
                AllocateChildNodeMemory(nodePtr);

                nodePtrStack.push(nodePtr->childrenPtrs[0]);
                nodePtrStack.push(nodePtr->childrenPtrs[1]);
                nodePtrStack.push(nodePtr->childrenPtrs[2]);
                nodePtrStack.push(nodePtr->childrenPtrs[3]);
            }
        }
    }

    void CollisionBodyQuadTree2D::Reset(const glm::vec2& min, const glm::vec2& max, size_t maxDepth, size_t maxBodiesPerNode)
    {
        maxDepth_ = maxDepth;

        if (maxBodiesPerNode == 0)
        {
            maxBodiesPerNode_ = 1;
        }
        else
        {
            maxBodiesPerNode_ = maxBodiesPerNode;
        }

        DeleteAllNodes();

        rootNodePtr_ = new CollisionBodyQuadTreeNode2D(0, min.x, min.y, max.x, max.y);
    }

    bool CollisionBodyQuadTree2D::Insert(CollisionBody2D* bodyPtr)
    {
        const glm::vec2& bodyPosition = bodyPtr->GetPosition();
        const float& bodyBoundingRadius = bodyPtr->GetBoundingRadius();

        // If body is out of bounds, don't insert it.
        if (!Check2D_Rectangle_Circle_Overlap(
            rootNodePtr_->min,
            rootNodePtr_->max,
            bodyPosition,
            bodyBoundingRadius))
        {
            return false;
        }

        std::stack<CollisionBodyQuadTreeNode2D*> nodePtrStack;
        nodePtrStack.push(rootNodePtr_);

        while (!nodePtrStack.empty())
        {
            CollisionBodyQuadTreeNode2D* nodePtr = nodePtrStack.top();
            nodePtrStack.pop();

            // If node isn't a leaf, attempt to add the body to it's children.
            if (!nodePtr->leafNode)
            {
                for (size_t i = 0; i < 4; ++i)
                {
                    CollisionBodyQuadTreeNode2D*& childNodePtr = nodePtr->childrenPtrs[i];
                    if (Check2D_Rectangle_Circle_Overlap(
                        childNodePtr->min,
                        childNodePtr->max,
                        bodyPosition,
                        bodyBoundingRadius))
                    {
                        nodePtrStack.push(childNodePtr);
                    }
                }
                continue;
            }

            // If node has room to hold the body, or it's at the maximum depth, add the body.
            if (nodePtr->bodyPtrs.size() < maxBodiesPerNode_ || nodePtr->depth == maxDepth_)
            {
                nodePtr->bodyPtrs.push_back(bodyPtr);
                continue;
            }

            // Current node has reached its body capacity and is now nolonger a leaf node.

            nodePtr->leafNode = false;

            // Create the child nodes, if they don't exist.
            if (nodePtr->childrenPtrs[0] == nullptr)
            {
                AllocateChildNodeMemory(nodePtr);
            }

            // Add all storedBodyPtrs to child nodes.
            for (size_t i = 0; i < nodePtr->bodyPtrs.size(); ++i)
            {
                CollisionBody2D* storedBodyPtr = nodePtr->bodyPtrs[i];
                const glm::vec2& storedBodyPosition = storedBodyPtr->GetPosition();
                const float& storedBodyBoundingRadius = storedBodyPtr->GetBoundingRadius();

                for (size_t j = 0; j < 4; ++j)
                {
                    CollisionBodyQuadTreeNode2D*& childNodePtr = nodePtr->childrenPtrs[j];
                    if (Check2D_Rectangle_Circle_Overlap(
                        childNodePtr->min,
                        childNodePtr->max,
                        storedBodyPosition,
                        storedBodyBoundingRadius))
                    {
                        childNodePtr->bodyPtrs.push_back(storedBodyPtr);
                    }
                }
            }

            nodePtr->bodyPtrs.clear();

            // Attempt to add body to child nodes.
            for (size_t i = 0; i < 4; ++i)
            {
                CollisionBodyQuadTreeNode2D*& childNodePtr = nodePtr->childrenPtrs[i];
                if (Check2D_Rectangle_Circle_Overlap(
                    childNodePtr->min,
                    childNodePtr->max,
                    bodyPosition,
                    bodyBoundingRadius))
                {
                    nodePtrStack.push(childNodePtr);
                }
            }
        }

        return true;
    }

    void CollisionBodyQuadTree2D::Clear()
    {
        std::stack<CollisionBodyQuadTreeNode2D*> nodePtrStack;
        nodePtrStack.push(rootNodePtr_);

        while (!nodePtrStack.empty())
        {
            CollisionBodyQuadTreeNode2D* nodePtr = nodePtrStack.top();
            nodePtrStack.pop();

            for (size_t i = 0; i < 4; ++i)
            {
                if (nodePtr->childrenPtrs[i])
                {
                    nodePtrStack.push(nodePtr->childrenPtrs[i]);
                }
            }

            nodePtr->leafNode = true;
            nodePtr->bodyPtrs.clear();
        }
    }

    // protected ---------------------------------------------------------------

    void CollisionBodyQuadTree2D::DeleteAllNodes()
    {
        std::stack<CollisionBodyQuadTreeNode2D*> nodePtrStack;
        nodePtrStack.push(rootNodePtr_);

        while (!nodePtrStack.empty())
        {
            CollisionBodyQuadTreeNode2D* nodePtr = nodePtrStack.top();
            nodePtrStack.pop();

            for (size_t i = 0; i < 4; ++i)
            {
                if (nodePtr->childrenPtrs[i])
                {
                    nodePtrStack.push(nodePtr->childrenPtrs[i]);
                }
            }

            delete nodePtr;
        }
    }

    void CollisionBodyQuadTree2D::AllocateChildNodeMemory(CollisionBodyQuadTreeNode2D* nodePtr)
    {
        glm::vec2 mid = (nodePtr->min + nodePtr->max) / 2.0f;

        // NE
        nodePtr->childrenPtrs[0] = new CollisionBodyQuadTreeNode2D(
            nodePtr->depth + 1,
            mid.x,
            mid.y,
            nodePtr->max.x,
            nodePtr->max.y
        );
        nodePtr->childrenPtrs[0]->bodyPtrs.reserve(maxBodiesPerNode_);

        // NW
        nodePtr->childrenPtrs[1] = new CollisionBodyQuadTreeNode2D(
            nodePtr->depth + 1,
            nodePtr->min.x,
            mid.y,
            mid.x,
            nodePtr->max.y
        );
        nodePtr->childrenPtrs[1]->bodyPtrs.reserve(maxBodiesPerNode_);


        // SW
        nodePtr->childrenPtrs[2] = new CollisionBodyQuadTreeNode2D(
            nodePtr->depth + 1,
            nodePtr->min.x,
            nodePtr->min.y,
            mid.x,
            mid.y
        );
        nodePtr->childrenPtrs[2]->bodyPtrs.reserve(maxBodiesPerNode_);

        // SE
        nodePtr->childrenPtrs[3] = new CollisionBodyQuadTreeNode2D(
            nodePtr->depth + 1,
            mid.x,
            nodePtr->min.y,
            nodePtr->max.x,
            mid.y
        );
        nodePtr->childrenPtrs[3]->bodyPtrs.reserve(maxBodiesPerNode_);
    }
}
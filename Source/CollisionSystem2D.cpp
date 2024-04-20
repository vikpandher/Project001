#include "CollisionSystem2D.h"

#include "ComponentStores.h"
#include "Components/CollisionBody2D.h"
#include "Math/Overlap2D.h"

#include <stack>



namespace Project001
{
    // public ------------------------------------------------------------------

    void CollisionSystem2D::CalculateCollisions(ComponentStores* componentStoresPtr)
    {
        if (componentStoresPtr == nullptr)
        {
            return;
        }

        // Get collision bodies
        CollisionBody2D* collisionBodyPtrs = nullptr;
        size_t collisionBodyCount = 0;
        bool componentFound = componentStoresPtr->GetAllComponents<CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

        if (!componentFound)
        {
            return;
        }

        // Clear old collisions
        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            currentCollisionBody.ClearCollisions();
        }

        // Calculate bounding radii and transformed collision shapes
        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (!currentCollisionBody.BoundingRadiusUpToDate())
            {
                currentCollisionBody.CalculateBoundingRadius();
            }

            if (!currentCollisionBody.TransformedCollisionShapesUpToDate())
            {
                currentCollisionBody.CalculateTransformedCollisionShapes();
            }
        }

        // Gather together all tangible collision bodies
        s_tangibleCollisionBodyPtrs_.clear();

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (currentCollisionBody.GetTangible())
            {
                s_tangibleCollisionBodyPtrs_.push_back(&currentCollisionBody);
            }
        }

        if (s_tangibleCollisionBodyPtrs_.size() == 0)
        {
            return;
        }

        // Calculate collisions
        for (unsigned int i = 0; i < s_tangibleCollisionBodyPtrs_.size() - 1; ++i)
        {
            Project001::CollisionBody2D*& collisionBodyA_ptr = s_tangibleCollisionBodyPtrs_[i];
            unsigned int entityIdA;
            componentStoresPtr->GetComponentEntityId<CollisionBody2D>(entityIdA, collisionBodyA_ptr);
            for (unsigned int j = i + 1; j < s_tangibleCollisionBodyPtrs_.size(); ++j)
            {
                Project001::CollisionBody2D*& collisionBodyB_ptr = s_tangibleCollisionBodyPtrs_[j];
                unsigned int entityIdB;
                componentStoresPtr->GetComponentEntityId<CollisionBody2D>(entityIdB, collisionBodyB_ptr);

                if ((collisionBodyA_ptr->GetCollisionGroupMask() & collisionBodyB_ptr->GetAllowedCollisionFilterMask()) &&
                    (collisionBodyB_ptr->GetCollisionGroupMask() & collisionBodyA_ptr->GetAllowedCollisionFilterMask()) &&
                    Check2D_Circle_Circle_Overlap(
                    collisionBodyA_ptr->GetPosition(),
                    collisionBodyA_ptr->GetBoundingRadius(),
                    collisionBodyB_ptr->GetPosition(),
                    collisionBodyB_ptr->GetBoundingRadius()))
                {
                    CalculateCollisions(entityIdA, *collisionBodyA_ptr, entityIdB, *collisionBodyB_ptr, true);
                }
            }
        }
    }

    void CollisionSystem2D::CalculateCollisionsForGivenEntity(
        unsigned int entityId,
        ComponentStores* componentStoresPtr)
    {
        if (componentStoresPtr == nullptr)
        {
            return;
        }

        // Get primary entity's collision body
        CollisionBody2D* primaryCollisionBodyPtr = nullptr;
        bool primaryComponentFound = componentStoresPtr->GetComponent<CollisionBody2D>(primaryCollisionBodyPtr, entityId);

        if (!primaryComponentFound || !primaryCollisionBodyPtr->GetTangible())
        {
            return;
        }

        // Get other collision bodies (will include the primary entity's collision body)
        CollisionBody2D* collisionBodyPtrs = nullptr;
        size_t collisionBodyCount = 0;
        bool componentFound = componentStoresPtr->GetAllComponents<CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

        if (!componentFound)
        {
            return;
        }

        // Clear primary entity's collisions
        primaryCollisionBodyPtr->ClearCollisions();

        // Calculate bounding radii and transformed collision shapes
        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (!currentCollisionBody.BoundingRadiusUpToDate())
            {
                currentCollisionBody.CalculateBoundingRadius();
            }

            if (!currentCollisionBody.TransformedCollisionShapesUpToDate())
            {
                currentCollisionBody.CalculateTransformedCollisionShapes();
            }
        }

        // Sort out all tangible collision bodies
        s_tangibleCollisionBodyPtrs_.clear();

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (currentCollisionBody.GetTangible())
            {
                s_tangibleCollisionBodyPtrs_.push_back(&currentCollisionBody);
            }
        }

        if (s_tangibleCollisionBodyPtrs_.size() == 0)
        {
            return;
        }

        // Calculate collisions (only for primary entity)
        for (unsigned int j = 1; j < s_tangibleCollisionBodyPtrs_.size(); ++j)
        {
            Project001::CollisionBody2D*& currentCollisionBodyPtr = s_tangibleCollisionBodyPtrs_[j];
            unsigned int currentEntityId;
            componentStoresPtr->GetComponentEntityId<CollisionBody2D>(currentEntityId, currentCollisionBodyPtr);

            if (entityId != currentEntityId &&
                (primaryCollisionBodyPtr->GetCollisionGroupMask() & currentCollisionBodyPtr->GetAllowedCollisionFilterMask()) &&
                (currentCollisionBodyPtr->GetCollisionGroupMask() & primaryCollisionBodyPtr->GetAllowedCollisionFilterMask()) &&
                Check2D_Circle_Circle_Overlap(
                primaryCollisionBodyPtr->GetPosition(),
                primaryCollisionBodyPtr->GetBoundingRadius(),
                currentCollisionBodyPtr->GetPosition(),
                currentCollisionBodyPtr->GetBoundingRadius()))
            {
                CalculateCollisions(entityId, *primaryCollisionBodyPtr, currentEntityId, *currentCollisionBodyPtr, false);
            }
        }
    }

    void CollisionSystem2D::CalculateCollisionsWithQuadTree(ComponentStores* componentStoresPtr)
    {
        if (componentStoresPtr == nullptr)
        {
            return;
        }

        // Get collision bodies
        CollisionBody2D* collisionBodyPtrs = nullptr;
        size_t collisionBodyCount = 0;
        bool componentFound = componentStoresPtr->GetAllComponents<CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

        if (!componentFound)
        {
            return;
        }

        // Clear old collisions
        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            currentCollisionBody.ClearCollisions();
        }

        // Calculate bounding radii and transformed collision shapes
        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (!currentCollisionBody.BoundingRadiusUpToDate())
            {
                currentCollisionBody.CalculateBoundingRadius();
            }

            if (!currentCollisionBody.TransformedCollisionShapesUpToDate())
            {
                currentCollisionBody.CalculateTransformedCollisionShapes();
            }
        }

        // Gather together all tangible collision bodies
        s_tangibleCollisionBodyPtrs_.clear();
        s_outOfBoundsTangibleCollisionBodyPtrs_.clear();
        s_tangibleCollisionBodyQuadTree2D_.Clear();
        s_collisionBodyPairs_.clear();

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (currentCollisionBody.GetTangible())
            {
                if (s_tangibleCollisionBodyQuadTree2D_.Insert(&currentCollisionBody))
                {
                    s_tangibleCollisionBodyPtrs_.push_back(&currentCollisionBody);
                }
                else
                {
                    s_outOfBoundsTangibleCollisionBodyPtrs_.push_back(&currentCollisionBody);
                }
            }
        }

        // Calculate out-of-bounds body collisions

        if (!s_outOfBoundsTangibleCollisionBodyPtrs_.empty())
        {
            for (unsigned int i = 0; i < s_outOfBoundsTangibleCollisionBodyPtrs_.size() - 1; ++i)
            {
                Project001::CollisionBody2D*& collisionBodyA_ptr = s_outOfBoundsTangibleCollisionBodyPtrs_[i];

                for (unsigned int j = i + 1; j < s_outOfBoundsTangibleCollisionBodyPtrs_.size(); ++j)
                {
                    Project001::CollisionBody2D*& collisionBodyB_ptr = s_outOfBoundsTangibleCollisionBodyPtrs_[j];

                    if ((collisionBodyA_ptr->GetCollisionGroupMask() & collisionBodyB_ptr->GetAllowedCollisionFilterMask()) &&
                        (collisionBodyB_ptr->GetCollisionGroupMask() & collisionBodyA_ptr->GetAllowedCollisionFilterMask()) &&
                        Check2D_Circle_Circle_Overlap(
                            collisionBodyA_ptr->GetPosition(),
                            collisionBodyA_ptr->GetBoundingRadius(),
                            collisionBodyB_ptr->GetPosition(),
                            collisionBodyB_ptr->GetBoundingRadius()))
                    {
                        s_collisionBodyPairs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
                    }
                }
            }
        }

        // Calculate out-of-bounds body with in-tree body collisions

        for (unsigned int i = 0; i < s_outOfBoundsTangibleCollisionBodyPtrs_.size(); ++i)
        {
            Project001::CollisionBody2D*& collisionBodyA_ptr = s_outOfBoundsTangibleCollisionBodyPtrs_[i];

            for (unsigned int j = 0; j < s_tangibleCollisionBodyPtrs_.size(); ++j)
            {
                Project001::CollisionBody2D*& collisionBodyB_ptr = s_tangibleCollisionBodyPtrs_[j];

                if ((collisionBodyA_ptr->GetCollisionGroupMask() & collisionBodyB_ptr->GetAllowedCollisionFilterMask()) &&
                    (collisionBodyB_ptr->GetCollisionGroupMask() & collisionBodyA_ptr->GetAllowedCollisionFilterMask()) &&
                    Check2D_Circle_Circle_Overlap(
                        collisionBodyA_ptr->GetPosition(),
                        collisionBodyA_ptr->GetBoundingRadius(),
                        collisionBodyB_ptr->GetPosition(),
                        collisionBodyB_ptr->GetBoundingRadius()))
                {
                    s_collisionBodyPairs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
                }
            }
        }

        // Calculate in-tree body collisions

        CollisionBodyQuadTreeNode2D* rootNodePtr = s_tangibleCollisionBodyQuadTree2D_.GetRootNode();
        if (!(rootNodePtr->leafNode && rootNodePtr->bodyPtrs.empty()))
        {
            std::stack<CollisionBodyQuadTreeNode2D*> nodePtrStack;
            nodePtrStack.push(rootNodePtr);

            while (!nodePtrStack.empty())
            {
                CollisionBodyQuadTreeNode2D* nodePtr = nodePtrStack.top();
                nodePtrStack.pop();

                if (nodePtr->leafNode)
                {
                    // Calculate collisions
                    for (unsigned int i = 0; i < nodePtr->bodyPtrs.size() - 1; ++i)
                    {
                        Project001::CollisionBody2D*& collisionBodyA_ptr = nodePtr->bodyPtrs[i];

                        for (unsigned int j = i + 1; j < nodePtr->bodyPtrs.size(); ++j)
                        {
                            Project001::CollisionBody2D*& collisionBodyB_ptr = nodePtr->bodyPtrs[j];

                            if ((collisionBodyA_ptr->GetCollisionGroupMask() & collisionBodyB_ptr->GetAllowedCollisionFilterMask()) &&
                                (collisionBodyB_ptr->GetCollisionGroupMask() & collisionBodyA_ptr->GetAllowedCollisionFilterMask()) &&
                                Check2D_Circle_Circle_Overlap(
                                    collisionBodyA_ptr->GetPosition(),
                                    collisionBodyA_ptr->GetBoundingRadius(),
                                    collisionBodyB_ptr->GetPosition(),
                                    collisionBodyB_ptr->GetBoundingRadius()))
                            {
                                s_collisionBodyPairs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
                            }
                        }
                    }
                }
                else
                {
                    for (size_t i = 0; i < 4; ++i)
                    {
                        if (nodePtr->childrenPtrs[i])
                        {
                            nodePtrStack.push(nodePtr->childrenPtrs[i]);
                        }
                    }
                }
            }
        }

        for (std::unordered_set<std::pair<CollisionBody2D*, CollisionBody2D*>, PointerPairHashFunctor>::iterator iter = s_collisionBodyPairs_.begin();
            iter != s_collisionBodyPairs_.end();
            ++iter)
        {
            CollisionBody2D* const& collisionBodyA_ptr = iter->first;
            unsigned int entityIdA;
            componentStoresPtr->GetComponentEntityId<CollisionBody2D>(entityIdA, collisionBodyA_ptr);

            CollisionBody2D* const& collisionBodyB_ptr = iter->second;
            unsigned int entityIdB;
            componentStoresPtr->GetComponentEntityId<CollisionBody2D>(entityIdB, collisionBodyB_ptr);

            CalculateCollisions(entityIdA, *collisionBodyA_ptr, entityIdB, *collisionBodyB_ptr, true);
        }
    }

    // protected ---------------------------------------------------------------

    void CollisionSystem2D::CalculateCollisions(
        unsigned int entityIdA,
        CollisionBody2D& collisionBodyA,
        unsigned int entityIdB,
        CollisionBody2D& collisionBodyB,
        bool recordInBodyB)
    {
        const std::vector<CollisionPoint2D>& transformedCollisionPointsA = collisionBodyA.GetTransformedCollisionPoints();
        const std::vector<CollisionLine2D>& transformedCollisionLinesA = collisionBodyA.GetTransformedCollisionLines();
        const std::vector<CollisionRay2D>& transformedCollisionRaysA = collisionBodyA.GetTransformedCollisionRays();
        const std::vector<CollisionLineSegment2D>& transformedCollisionLineSegmentsA = collisionBodyA.GetTransformedCollisionLineSegments();
        const std::vector<CollisionRectangle2D>& transformedCollisionRectanglesA = collisionBodyA.GetTransformedCollisionRectangles();
        const std::vector<CollisionOrientedRectangle2D>& transformedCollisionOrientedRectanglesA = collisionBodyA.GetTransformedCollisionOrientedRectangles();
        const std::vector<CollisionCircle2D>& transformedCollisionCirclesA = collisionBodyA.GetTransformedCollisionCircles();
        const std::vector<CollisionCapsule2D>& transformedCollisionCapsulesA = collisionBodyA.GetTransformedCollisionCapsules();
        const std::vector<CollisionTriangle2D>& transformedCollisionTrianglesA = collisionBodyA.GetTransformedCollisionTriangles();
        const std::vector<CollisionPolygon2D>& transformedCollisionPolygonsA = collisionBodyA.GetTransformedCollisionPolygons();
        const std::vector<CollisionConvexPolygon2D>& transformedCollisionConvexPolygonsA = collisionBodyA.GetTransformedCollisionConvexPolygons();

        const std::vector<CollisionPoint2D>& transformedCollisionPointsB = collisionBodyB.GetTransformedCollisionPoints();
        const std::vector<CollisionLine2D>& transformedCollisionLinesB = collisionBodyB.GetTransformedCollisionLines();
        const std::vector<CollisionRay2D>& transformedCollisionRaysB = collisionBodyB.GetTransformedCollisionRays();
        const std::vector<CollisionLineSegment2D>& transformedCollisionLineSegmentsB = collisionBodyB.GetTransformedCollisionLineSegments();
        const std::vector<CollisionRectangle2D>& transformedCollisionRectanglesB = collisionBodyB.GetTransformedCollisionRectangles();
        const std::vector<CollisionOrientedRectangle2D>& transformedCollisionOrientedRectanglesB = collisionBodyB.GetTransformedCollisionOrientedRectangles();
        const std::vector<CollisionCircle2D>& transformedCollisionCirclesB = collisionBodyB.GetTransformedCollisionCircles();
        const std::vector<CollisionCapsule2D>& transformedCollisionCapsulesB = collisionBodyB.GetTransformedCollisionCapsules();
        const std::vector<CollisionTriangle2D>& transformedCollisionTrianglesB = collisionBodyB.GetTransformedCollisionTriangles();
        const std::vector<CollisionPolygon2D>& transformedCollisionPolygonsB = collisionBodyB.GetTransformedCollisionPolygons();
        const std::vector<CollisionConvexPolygon2D>& transformedCollisionConvexPolygonsB = collisionBodyB.GetTransformedCollisionConvexPolygons();

        CollisionData2D collisionA;
        collisionA.otherEntityId = entityIdB;

        CollisionData2D collisionB;
        collisionB.otherEntityId = entityIdA;

        // point A & point B ---------------------------------------------------
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            if (!pointA.tangible)
            {
                continue;
            }

            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];
                if (!pointB.tangible)
                {
                    continue;
                }

                bool collisionFound = Check2D_Point_Point_Overlap(
                    pointA.position,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & line B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            if (!pointA.tangible)
            {
                continue;
            }

            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];
                if (!lineB.tangible)
                {
                    continue;
                }

                bool collisionFound = Check2D_Point_Line_Overlap(
                    pointA.position,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & ray B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            if (!pointA.tangible)
            {
                continue;
            }

            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];
                if (!rayB.tangible)
                {
                    continue;
                }

                bool collisionFound = Check2D_Point_Ray_Overlap(
                    pointA.position,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & lineSegment B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            if (!pointA.tangible)
            {
                continue;
            }

            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];
                if (!lineSegmentB.tangible)
                {
                    continue;
                }

                bool collisionFound = Check2D_Point_LineSegment_Overlap(
                    pointA.position,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & rectangle B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            if (!pointA.tangible)
            {
                continue;
            }

            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];
                if (!rectangleB.tangible)
                {
                    continue;
                }

                bool collisionFound = Check2D_Point_Rectangle_Overlap(
                    pointA.position,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            if (!pointA.tangible)
            {
                continue;
            }

            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];
                if (!orientedRectangleB.tangible)
                {
                    continue;
                }

                bool collisionFound = Check2D_Point_OrientedRectangle_Overlap(
                    pointA.position,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & circle B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            if (!pointA.tangible)
            {
                continue;
            }

            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];
                if (!circleB.tangible)
                {
                    continue;
                }

                bool collisionFound = Check2D_Point_Circle_Overlap(
                    pointA.position,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & capsule B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_Point_Capsule_Overlap(
                    pointA.position,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & triangle B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_Point_Triangle_Overlap(
                    pointA.position,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & polygon B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_Point_Polygon_Overlap(
                    pointA.position,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_Point_ConvexPolygon_Overlap(
                    pointA.position,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = pointA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & point B ----------------------------------------------------
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];

                bool collisionFound = Check2D_Line_Point_Overlap(
                    lineA.position,
                    lineA.slope,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & line B
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_Line_Line_Overlap(
                    lineA.position,
                    lineA.slope,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & ray B
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];

                bool collisionFound = Check2D_Line_Ray_Overlap(
                    lineA.position,
                    lineA.slope,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & lineSegment B
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];

                bool collisionFound = Check2D_Line_LineSegment_Overlap(
                    lineA.position,
                    lineA.slope,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & rectangle B
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_Line_Rectangle_Overlap(
                    lineA.position,
                    lineA.slope,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_Line_OrientedRectangle_Overlap(
                    lineA.position,
                    lineA.slope,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & circle B
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_Line_Circle_Overlap(
                    lineA.position,
                    lineA.slope,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & capsule B
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_Line_Capsule_Overlap(
                    lineA.position,
                    lineA.slope,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & triangle B
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_Line_Triangle_Overlap(
                    lineA.position,
                    lineA.slope,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & polygon B
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_Line_Polygon_Overlap(
                    lineA.position,
                    lineA.slope,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionLinesA.size(); ++i)
        {
            const CollisionLine2D& lineA = transformedCollisionLinesA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_Line_ConvexPolygon_Overlap(
                    lineA.position,
                    lineA.slope,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & point B -----------------------------------------------------
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];

                bool collisionFound = Check2D_Ray_Point_Overlap(
                    rayA.position,
                    rayA.direction,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & line B
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_Ray_Line_Overlap(
                    rayA.position,
                    rayA.direction,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & ray B
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];

                bool collisionFound = Check2D_Ray_Ray_Overlap(
                    rayA.position,
                    rayA.direction,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & lineSegment B
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];

                bool collisionFound = Check2D_Ray_LineSegment_Overlap(
                    rayA.position,
                    rayA.direction,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & rectangle B
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_Ray_Rectangle_Overlap(
                    rayA.position,
                    rayA.direction,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_Ray_OrientedRectangle_Overlap(
                    rayA.position,
                    rayA.direction,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & circle B
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_Ray_Circle_Overlap(
                    rayA.position,
                    rayA.direction,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & capsule B
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_Ray_Capsule_Overlap(
                    rayA.position,
                    rayA.direction,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & triangle B
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_Ray_Triangle_Overlap(
                    rayA.position,
                    rayA.direction,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & polygon B
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_Ray_Polygon_Overlap(
                    rayA.position,
                    rayA.direction,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionRaysA.size(); ++i)
        {
            const CollisionRay2D& rayA = transformedCollisionRaysA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_Ray_ConvexPolygon_Overlap(
                    rayA.position,
                    rayA.direction,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = rayA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & point B ---------------------------------------------
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];

                bool collisionFound = Check2D_LineSegment_Point_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & line B
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_LineSegment_Line_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & ray B
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];

                bool collisionFound = Check2D_LineSegment_Ray_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & lineSegment B
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];

                bool collisionFound = Check2D_LineSegment_LineSegment_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & rectangle B
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_LineSegment_Rectangle_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_LineSegment_OrientedRectangle_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & circle B
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_LineSegment_Circle_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & capsule B
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_LineSegment_Capsule_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & triangle B
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_LineSegment_Triangle_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & polygon B
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_LineSegment_Polygon_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionLineSegmentsA.size(); ++i)
        {
            const CollisionLineSegment2D& lineSegmentA = transformedCollisionLineSegmentsA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_LineSegment_ConvexPolygon_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = lineSegmentA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & point B -----------------------------------------------
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];

                bool collisionFound = Check2D_Rectangle_Point_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & line B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_Rectangle_Line_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & ray B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];

                bool collisionFound = Check2D_Rectangle_Ray_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & lineSegment B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsA.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsA[j];

                bool collisionFound = Check2D_Rectangle_LineSegment_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & rectangle B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_Rectangle_Rectangle_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_Rectangle_OrientedRectangle_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & circle B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_Rectangle_Circle_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & capsule B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_Rectangle_Capsule_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & triangle B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_Rectangle_Triangle_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & polygon B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_Rectangle_Polygon_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_Rectangle_ConvexPolygon_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = rectangleA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & point B ---------------------------------------
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];

                bool collisionFound = Check2D_OrientedRectangle_Point_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & line B
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_OrientedRectangle_Line_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & ray B
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];

                bool collisionFound = Check2D_OrientedRectangle_Ray_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & lineSegment B
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];

                bool collisionFound = Check2D_OrientedRectangle_LineSegment_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & rectangle B
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_OrientedRectangle_Rectangle_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_OrientedRectangle_OrientedRectangle_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & circle B
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_OrientedRectangle_Circle_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & capsule B
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_OrientedRectangle_Capsule_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & triangle B
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_OrientedRectangle_Triangle_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & polygon B
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_OrientedRectangle_Polygon_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
        {
            const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_OrientedRectangle_ConvexPolygon_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = orientedRectangleA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & point B --------------------------------------------------
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];

                bool collisionFound = Check2D_Circle_Point_Overlap(
                    circleA.position,
                    circleA.radius,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & line B
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_Circle_Line_Overlap(
                    circleA.position,
                    circleA.radius,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & ray B
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];

                bool collisionFound = Check2D_Circle_Ray_Overlap(
                    circleA.position,
                    circleA.radius,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & lineSegment B
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];

                bool collisionFound = Check2D_Circle_LineSegment_Overlap(
                    circleA.position,
                    circleA.radius,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & rectangle B
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_Circle_Rectangle_Overlap(
                    circleA.position,
                    circleA.radius,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_Circle_OrientedRectangle_Overlap(
                    circleA.position,
                    circleA.radius,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & circle B
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_Circle_Circle_Overlap(
                    circleA.position,
                    circleA.radius,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & capsule B
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_Circle_Capsule_Overlap(
                    circleA.position,
                    circleA.radius,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & triangle B
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_Circle_Triangle_Overlap(
                    circleA.position,
                    circleA.radius,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & polygon B
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_Circle_Polygon_Overlap(
                    circleA.position,
                    circleA.radius,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
        {
            const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_Circle_ConvexPolygon_Overlap(
                    circleA.position,
                    circleA.radius,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = circleA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & point B -------------------------------------------------
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];

                bool collisionFound = Check2D_Capsule_Point_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & line B
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_Capsule_Line_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & ray B
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];

                bool collisionFound = Check2D_Capsule_Ray_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & lineSegment B
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];

                bool collisionFound = Check2D_Capsule_LineSegment_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule & other rectangle
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_Capsule_Rectangle_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_Capsule_OrientedRectangle_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & circle B
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_Capsule_Circle_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & capsule B
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_Capsule_Capsule_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & triangle B
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_Capsule_Triangle_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule & other polygon
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_Capsule_Polygon_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionCapsulesA.size(); ++i)
        {
            const CollisionCapsule2D& capsuleA = transformedCollisionCapsulesA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_Capsule_ConvexPolygon_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = capsuleA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & point B ------------------------------------------------
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];

                bool collisionFound = Check2D_Triangle_Point_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & line B
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_Triangle_Line_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & ray B
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];

                bool collisionFound = Check2D_Triangle_Ray_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & lineSegment B
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];

                bool collisionFound = Check2D_Triangle_LineSegment_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & rectangle B
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_Triangle_Rectangle_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_Triangle_OrientedRectangle_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & circle B
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_Triangle_Circle_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & capsule B
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_Triangle_Capsule_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & triangle B
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_Triangle_Triangle_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & polygon B
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_Triangle_Polygon_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
        {
            const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_Triangle_ConvexPolygon_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = triangleA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & point B -------------------------------------------------
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];

                bool collisionFound = Check2D_Polygon_Point_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & line B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_Polygon_Line_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & ray B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];

                bool collisionFound = Check2D_Polygon_Ray_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & lineSegment B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];

                bool collisionFound = Check2D_Polygon_LineSegment_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & rectangle B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_Polygon_Rectangle_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_Polygon_OrientedRectangle_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & circle B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_Polygon_Circle_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & capsule B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_Polygon_Capsule_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & triangle B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_Polygon_Triangle_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & polygon B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_Polygon_Polygon_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_Polygon_ConvexPolygon_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = polygonA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & point B -------------------------------------------
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionPointsB.size(); ++j)
            {
                const CollisionPoint2D& pointB = transformedCollisionPointsB[j];

                bool collisionFound = Check2D_ConvexPolygon_Point_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = pointB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & line B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Line_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = lineB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & ray B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionRaysB.size(); ++j)
            {
                const CollisionRay2D& rayB = transformedCollisionRaysB[j];

                bool collisionFound = Check2D_ConvexPolygon_Ray_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = rayB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & lineSegment B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];

                bool collisionFound = Check2D_ConvexPolygon_LineSegment_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = lineSegmentB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & rectangle B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Rectangle_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = rectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_ConvexPolygon_OrientedRectangle_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = orientedRectangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & circle B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Circle_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = circleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & capsule B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionCapsulesB.size(); ++j)
            {
                const CollisionCapsule2D& capsuleB = transformedCollisionCapsulesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Capsule_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = capsuleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & triangle B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
            {
                const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Triangle_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = triangleB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & polygon B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionPolygonsB.size(); ++j)
            {
                const CollisionPolygon2D& polygonB = transformedCollisionPolygonsB[j];

                bool collisionFound = Check2D_ConvexPolygon_Polygon_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = polygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & convexPolygon B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
            {
                const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                bool collisionFound = Check2D_ConvexPolygon_ConvexPolygon_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeTag = convexPolygonA.tag;
                    collisionA.otherShapeTag = convexPolygonB.tag;

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeTag = collisionA.otherShapeTag;
                        collisionB.otherShapeTag = collisionA.myShapeTag;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }
    }

    std::vector<CollisionBody2D*> CollisionSystem2D::s_tangibleCollisionBodyPtrs_;

    std::vector<CollisionBody2D*> CollisionSystem2D::s_outOfBoundsTangibleCollisionBodyPtrs_;

    CollisionBodyQuadTree2D CollisionSystem2D::s_tangibleCollisionBodyQuadTree2D_(glm::vec2(-8.0f, -6.0f), glm::vec2(8.0f, 6.0f), 4, 16);

    std::unordered_set<std::pair<CollisionBody2D*, CollisionBody2D*>, CollisionSystem2D::PointerPairHashFunctor> CollisionSystem2D::s_collisionBodyPairs_;
}
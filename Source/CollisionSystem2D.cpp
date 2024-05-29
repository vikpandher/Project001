#include "CollisionSystem2D.h"

#include "ComponentStores.h"
#include "Components/CollisionBody2D.h"
#include "Math/Overlap2D.h"

#include <functional>
#include <stack>



namespace Project001
{
    // public ------------------------------------------------------------------

    void CollisionSystem2D::ApplyMovement(ComponentStores* componentStoresPtr, float timestep_s)
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

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            currentCollisionBody.SetVelocity(currentCollisionBody.GetVelocity() + currentCollisionBody.GetAcceleration() * timestep_s);
            currentCollisionBody.SetPosition(currentCollisionBody.GetPosition() + currentCollisionBody.GetVelocity() * timestep_s);
            currentCollisionBody.SetAngularVelocity(currentCollisionBody.GetAngularVelocity() + currentCollisionBody.GetAngularAcceleration() * timestep_s);
            currentCollisionBody.SetRotation(currentCollisionBody.GetRotation() + currentCollisionBody.GetAngularVelocity() * timestep_s);
        }
    }

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
                    CalculateCollisionsBetweenTwoBodies(entityIdA, *collisionBodyA_ptr, entityIdB, *collisionBodyB_ptr, true, true);
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
                CalculateCollisionsBetweenTwoBodies(entityId, *primaryCollisionBodyPtr, currentEntityId, *currentCollisionBodyPtr, false, false);
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
        s_collisionBodyPairPtrs_.clear();

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
                        s_collisionBodyPairPtrs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
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
                    s_collisionBodyPairPtrs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
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
                                // Note that the order of the collision bodies
                                // should stay consistent because they are
                                // inserted into the quad tree in the order they
                                // are in the component container.
                                // 
                                // If we got the pair (A, B) one time through.
                                // We should again get the pair (A, B), not
                                // (B, A).
                                // 
                                // So if the (A, B) pair occurs twice (because
                                // both A and B are in multiple leaf nodes) then
                                // the pair will be (A, B) in all leaf nodes,
                                // never (B, A), because A will always be
                                // inserted first.
                                s_collisionBodyPairPtrs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
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

        for (std::unordered_set<std::pair<CollisionBody2D*, CollisionBody2D*>, PointerPairHashFunctor>::iterator iter = s_collisionBodyPairPtrs_.begin();
            iter != s_collisionBodyPairPtrs_.end();
            ++iter)
        {
            CollisionBody2D* const& collisionBodyA_ptr = iter->first;
            unsigned int entityIdA;
            componentStoresPtr->GetComponentEntityId<CollisionBody2D>(entityIdA, collisionBodyA_ptr);

            CollisionBody2D* const& collisionBodyB_ptr = iter->second;
            unsigned int entityIdB;
            componentStoresPtr->GetComponentEntityId<CollisionBody2D>(entityIdB, collisionBodyB_ptr);

            CalculateCollisionsBetweenTwoBodies(entityIdA, *collisionBodyA_ptr, entityIdB, *collisionBodyB_ptr, true, true);
        }
    }

    // protected ---------------------------------------------------------------

    void CollisionSystem2D::CalculateCollisionsBetweenTwoBodies(
        unsigned int entityIdA,
        CollisionBody2D& collisionBodyA,
        unsigned int entityIdB,
        CollisionBody2D& collisionBodyB,
        bool recordInBodyB,
        bool resolvePhysics)
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

        std::function<void(unsigned int tagA, unsigned int tagB)> AddCollisionData =
            [&](unsigned int tagA, unsigned int tagB) {

            CollisionData2D collisionA;
            collisionA.otherEntityId = entityIdB;
            collisionA.myShapeTag = tagA;
            collisionA.otherShapeTag = tagB;

            collisionBodyA.AddCollision(collisionA);

            if (recordInBodyB)
            {
                CollisionData2D collisionB;
                collisionB.otherEntityId = entityIdA;
                collisionB.myShapeTag = collisionA.otherShapeTag;
                collisionB.otherShapeTag = collisionA.myShapeTag;

                collisionBodyB.AddCollision(collisionB);
            }
        };

        // ---------------------------------------------------------------------
        // point A & rectangle B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
            {
                const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                bool collisionFound = Check2D_Point_Rectangle_Overlap(
                    pointA.position,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    AddCollisionData(pointA.tag, rectangleB.tag);
                }
            }
        }

        // point A & orientedRectangle B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
            {
                const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                bool collisionFound = Check2D_Point_OrientedRectangle_Overlap(
                    pointA.position,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    AddCollisionData(pointA.tag, orientedRectangleB.tag);
                }
            }
        }

        // point A & circle B
        for (size_t i = 0; i < transformedCollisionPointsA.size(); ++i)
        {
            const CollisionPoint2D& pointA = transformedCollisionPointsA[i];
            for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
            {
                const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                bool collisionFound = Check2D_Point_Circle_Overlap(
                    pointA.position,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    AddCollisionData(pointA.tag, circleB.tag);
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
                    AddCollisionData(pointA.tag, capsuleB.tag);
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
                    AddCollisionData(pointA.tag, triangleB.tag);
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(pointA.tag, polygonB.tag);
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
                    convexPolygonB.corners.data(),
                    convexPolygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(pointA.tag, convexPolygonB.tag);
                }
            }
        }

        // ---------------------------------------------------------------------
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
                    AddCollisionData(lineA.tag, lineB.tag);
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
                    AddCollisionData(lineA.tag, rayB.tag);
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
                    AddCollisionData(lineA.tag, lineSegmentB.tag);
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
                    AddCollisionData(lineA.tag, rectangleB.tag);
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
                    AddCollisionData(lineA.tag, orientedRectangleB.tag);
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
                    AddCollisionData(lineA.tag, circleB.tag);
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
                    AddCollisionData(lineA.tag, capsuleB.tag);
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
                    AddCollisionData(lineA.tag, triangleB.tag);
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(lineA.tag, polygonB.tag);
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
                    convexPolygonB.corners.data(),
                    convexPolygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(lineA.tag, convexPolygonB.tag);
                }
            }
        }

        // ---------------------------------------------------------------------
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
                    AddCollisionData(rayA.tag, lineB.tag);
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
                    AddCollisionData(rayA.tag, rayB.tag);
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
                    AddCollisionData(rayA.tag, lineSegmentB.tag);
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
                    AddCollisionData(rayA.tag, rectangleB.tag);
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
                    AddCollisionData(rayA.tag, orientedRectangleB.tag);
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
                    AddCollisionData(rayA.tag, circleB.tag);
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
                    AddCollisionData(rayA.tag, capsuleB.tag);
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
                    AddCollisionData(rayA.tag, triangleB.tag);
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(rayA.tag, polygonB.tag);
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
                    convexPolygonB.corners.data(),
                    convexPolygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(rayA.tag, convexPolygonB.tag);
                }
            }
        }

        // ---------------------------------------------------------------------
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
                    AddCollisionData(lineSegmentA.tag, lineB.tag);
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
                    AddCollisionData(lineSegmentA.tag, rayB.tag);
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
                    AddCollisionData(lineSegmentA.tag, lineSegmentB.tag);
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
                    AddCollisionData(lineSegmentA.tag, rectangleB.tag);
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
                    AddCollisionData(lineSegmentA.tag, orientedRectangleB.tag);
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
                    AddCollisionData(lineSegmentA.tag, circleB.tag);
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
                    AddCollisionData(lineSegmentA.tag, capsuleB.tag);
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
                    AddCollisionData(lineSegmentA.tag, triangleB.tag);
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(lineSegmentA.tag, polygonB.tag);
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
                    convexPolygonB.corners.data(),
                    convexPolygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(lineSegmentA.tag, convexPolygonB.tag);
                }
            }
        }

        // ---------------------------------------------------------------------
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
                    AddCollisionData(rectangleA.tag, lineB.tag);
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
                    AddCollisionData(rectangleA.tag, rayB.tag);
                }
            }
        }

        // rectangle A & lineSegment B
        for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
        {
            const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
            for (size_t j = 0; j < transformedCollisionLineSegmentsB.size(); ++j)
            {
                const CollisionLineSegment2D& lineSegmentB = transformedCollisionLineSegmentsB[j];

                bool collisionFound = Check2D_Rectangle_LineSegment_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    AddCollisionData(rectangleA.tag, lineSegmentB.tag);
                }
            }
        }

        // rectangle A & rectangle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
            {
                const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
                for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
                {
                    const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Rectangle_Rectangle_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = rectangleA.tag;
                        collisionA.otherShapeTag = rectangleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
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
                        AddCollisionData(rectangleA.tag, rectangleB.tag);
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
                    AddCollisionData(rectangleA.tag, orientedRectangleB.tag);
                }
            }
        }

        // rectangle A & circle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionRectanglesA.size(); ++i)
            {
                const CollisionRectangle2D& rectangleA = transformedCollisionRectanglesA[i];
                for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
                {
                    const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Rectangle_Circle_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        circleB.position,
                        circleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = rectangleA.tag;
                        collisionA.otherShapeTag = circleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
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
                        AddCollisionData(rectangleA.tag, circleB.tag);
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
                    AddCollisionData(rectangleA.tag, capsuleB.tag);
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
                    AddCollisionData(rectangleA.tag, triangleB.tag);
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(rectangleA.tag, polygonB.tag);
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
                    convexPolygonB.corners.data(),
                    convexPolygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(rectangleA.tag, convexPolygonB.tag);
                }
            }
        }

        // ---------------------------------------------------------------------
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
                    AddCollisionData(orientedRectangleA.tag, lineB.tag);
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
                    AddCollisionData(orientedRectangleA.tag, rayB.tag);
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
                    AddCollisionData(orientedRectangleA.tag, lineSegmentB.tag);
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
                    AddCollisionData(orientedRectangleA.tag, rectangleB.tag);
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
                    AddCollisionData(orientedRectangleA.tag, orientedRectangleB.tag);
                }
            }
        }

        // orientedRectangle A & circle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionOrientedRectanglesA.size(); ++i)
            {
                const CollisionOrientedRectangle2D& orientedRectangleA = transformedCollisionOrientedRectanglesA[i];
                for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
                {
                    const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_OrientedRectangle_Circle_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        circleB.position,
                        circleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = orientedRectangleA.tag;
                        collisionA.otherShapeTag = circleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
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
                        AddCollisionData(orientedRectangleA.tag, circleB.tag);
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
                    AddCollisionData(orientedRectangleA.tag, capsuleB.tag);
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
                    AddCollisionData(orientedRectangleA.tag, triangleB.tag);
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(orientedRectangleA.tag, polygonB.tag);
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
                    convexPolygonB.corners.data(),
                    convexPolygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(orientedRectangleA.tag, convexPolygonB.tag);
                }
            }
        }

        // ---------------------------------------------------------------------
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
                    AddCollisionData(circleA.tag, lineB.tag);
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
                    AddCollisionData(circleA.tag, rayB.tag);
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
                    AddCollisionData(circleA.tag, lineSegmentB.tag);
                }
            }
        }

        // circle A & rectangle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
            {
                const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
                for (size_t j = 0; j < transformedCollisionRectanglesB.size(); ++j)
                {
                    const CollisionRectangle2D& rectangleB = transformedCollisionRectanglesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Circle_Rectangle_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = circleA.tag;
                        collisionA.otherShapeTag = rectangleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
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
                        AddCollisionData(circleA.tag, rectangleB.tag);
                    }
                }
            }
        }

        // circle A & orientedRectangle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
            {
                const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
                for (size_t j = 0; j < transformedCollisionOrientedRectanglesB.size(); ++j)
                {
                    const CollisionOrientedRectangle2D& orientedRectangleB = transformedCollisionOrientedRectanglesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Circle_OrientedRectangle_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = circleA.tag;
                        collisionA.otherShapeTag = orientedRectangleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
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
                        AddCollisionData(circleA.tag, orientedRectangleB.tag);
                    }
                }
            }
        }

        // circle A & circle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
            {
                const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
                for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
                {
                    const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Circle_Circle_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        circleB.position,
                        circleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = circleA.tag;
                        collisionA.otherShapeTag = circleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
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
                        AddCollisionData(circleA.tag, circleB.tag);
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
                    AddCollisionData(circleA.tag, capsuleB.tag);
                }
            }
        }

        // circle A & triangle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
            {
                const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
                for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
                {
                    const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Circle_Triangle_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = circleA.tag;
                        collisionA.otherShapeTag = triangleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
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
                        AddCollisionData(circleA.tag, triangleB.tag);
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(circleA.tag, polygonB.tag);
                }
            }
        }

        // circle A & convexPolygon B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
            {
                const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
                for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
                {
                    const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Circle_ConvexPolygon_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = circleA.tag;
                        collisionA.otherShapeTag = convexPolygonB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
            for (size_t i = 0; i < transformedCollisionCirclesA.size(); ++i)
            {
                const CollisionCircle2D& circleA = transformedCollisionCirclesA[i];
                for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
                {
                    const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                    bool collisionFound = Check2D_Circle_ConvexPolygon_Overlap(
                        circleA.position,
                        circleA.radius,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size());

                    if (collisionFound)
                    {
                        AddCollisionData(circleA.tag, convexPolygonB.tag);
                    }
                }
            }
        }

        // ---------------------------------------------------------------------
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
                    AddCollisionData(capsuleA.tag, lineB.tag);
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
                    AddCollisionData(capsuleA.tag, rayB.tag);
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
                    AddCollisionData(capsuleA.tag, lineSegmentB.tag);
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
                    AddCollisionData(capsuleA.tag, rectangleB.tag);
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
                    AddCollisionData(capsuleA.tag, orientedRectangleB.tag);
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
                    AddCollisionData(capsuleA.tag, circleB.tag);
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
                    AddCollisionData(capsuleA.tag, capsuleB.tag);
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
                    AddCollisionData(capsuleA.tag, triangleB.tag);
                }
            }
        }

        // capsule A & polygon B
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(capsuleA.tag, polygonB.tag);
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
                    convexPolygonB.corners.data(),
                    convexPolygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(capsuleA.tag, convexPolygonB.tag);
                }
            }
        }

        // ---------------------------------------------------------------------
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
                    AddCollisionData(triangleA.tag, lineB.tag);
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
                    AddCollisionData(triangleA.tag, rayB.tag);
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
                    AddCollisionData(triangleA.tag, lineSegmentB.tag);
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
                    AddCollisionData(triangleA.tag, rectangleB.tag);
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
                    AddCollisionData(triangleA.tag, orientedRectangleB.tag);
                }
            }
        }

        // triangle A & circle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
            {
                const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
                for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
                {
                    const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Triangle_Circle_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        circleB.position,
                        circleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = triangleA.tag;
                        collisionA.otherShapeTag = circleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
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
                        AddCollisionData(triangleA.tag, circleB.tag);
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
                    AddCollisionData(triangleA.tag, capsuleB.tag);
                }
            }
        }

        // triangle A & triangle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
            {
                const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
                for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
                {
                    const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Triangle_Triangle_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = triangleA.tag;
                        collisionA.otherShapeTag = triangleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
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
                        AddCollisionData(triangleA.tag, triangleB.tag);
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(triangleA.tag, polygonB.tag);
                }
            }
        }

        // triangle A & convexPolygon B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionTrianglesA.size(); ++i)
            {
                const CollisionTriangle2D& triangleA = transformedCollisionTrianglesA[i];
                for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
                {
                    const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Triangle_ConvexPolygon_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = triangleA.tag;
                        collisionA.otherShapeTag = convexPolygonB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
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
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size());

                    if (collisionFound)
                    {
                        AddCollisionData(triangleA.tag, convexPolygonB.tag);
                    }
                }
            }
        }

        // ---------------------------------------------------------------------
        // polygon A & line B
        for (size_t i = 0; i < transformedCollisionPolygonsA.size(); ++i)
        {
            const CollisionPolygon2D& polygonA = transformedCollisionPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_Polygon_Line_Overlap(
                    polygonA.corners.data(),
                    polygonA.corners.size(),
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    AddCollisionData(polygonA.tag, lineB.tag);
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
                    polygonA.corners.data(),
                    polygonA.corners.size(),
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    AddCollisionData(polygonA.tag, rayB.tag);
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
                    polygonA.corners.data(),
                    polygonA.corners.size(),
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    AddCollisionData(polygonA.tag, lineSegmentB.tag);
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
                    polygonA.corners.data(),
                    polygonA.corners.size(),
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    AddCollisionData(polygonA.tag, rectangleB.tag);
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
                    polygonA.corners.data(),
                    polygonA.corners.size(),
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    AddCollisionData(polygonA.tag, orientedRectangleB.tag);
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
                    polygonA.corners.data(),
                    polygonA.corners.size(),
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    AddCollisionData(polygonA.tag, circleB.tag);
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
                    polygonA.corners.data(),
                    polygonA.corners.size(),
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    AddCollisionData(polygonA.tag, capsuleB.tag);
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
                    polygonA.corners.data(),
                    polygonA.corners.size(),
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    AddCollisionData(polygonA.tag, triangleB.tag);
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
                    polygonA.corners.data(),
                    polygonA.corners.size(),
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(polygonA.tag, polygonB.tag);
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
                    polygonA.corners.data(),
                    polygonA.corners.size(),
                    convexPolygonB.corners.data(),
                    convexPolygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(polygonA.tag, convexPolygonB.tag);
                }
            }
        }

        // ---------------------------------------------------------------------
        // convexPolygon A & line B
        for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
        {
            const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCollisionLinesB.size(); ++j)
            {
                const CollisionLine2D& lineB = transformedCollisionLinesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Line_Overlap(
                    convexPolygonA.corners.data(),
                    convexPolygonA.corners.size(),
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    AddCollisionData(convexPolygonA.tag, lineB.tag);
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
                    convexPolygonA.corners.data(),
                    convexPolygonA.corners.size(),
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    AddCollisionData(convexPolygonA.tag, rayB.tag);
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
                    convexPolygonA.corners.data(),
                    convexPolygonA.corners.size(),
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    AddCollisionData(convexPolygonA.tag, lineSegmentB.tag);
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
                    convexPolygonA.corners.data(),
                    convexPolygonA.corners.size(),
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    AddCollisionData(convexPolygonA.tag, rectangleB.tag);
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
                    convexPolygonA.corners.data(),
                    convexPolygonA.corners.size(),
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    AddCollisionData(convexPolygonA.tag, orientedRectangleB.tag);
                }
            }
        }

        // convexPolygon A & circle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
            {
                const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
                for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
                {
                    const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_ConvexPolygon_Circle_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        circleB.position,
                        circleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = convexPolygonA.tag;
                        collisionA.otherShapeTag = circleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
            for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
            {
                const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
                for (size_t j = 0; j < transformedCollisionCirclesB.size(); ++j)
                {
                    const CollisionCircle2D& circleB = transformedCollisionCirclesB[j];

                    bool collisionFound = Check2D_ConvexPolygon_Circle_Overlap(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        circleB.position,
                        circleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionData(convexPolygonA.tag, circleB.tag);
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
                    convexPolygonA.corners.data(),
                    convexPolygonA.corners.size(),
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    AddCollisionData(convexPolygonA.tag, capsuleB.tag);
                }
            }
        }

        // convexPolygon A & triangle B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
            {
                const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
                for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
                {
                    const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_ConvexPolygon_Triangle_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = convexPolygonA.tag;
                        collisionA.otherShapeTag = triangleB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
            for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
            {
                const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
                for (size_t j = 0; j < transformedCollisionTrianglesB.size(); ++j)
                {
                    const CollisionTriangle2D& triangleB = transformedCollisionTrianglesB[j];

                    bool collisionFound = Check2D_ConvexPolygon_Triangle_Overlap(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3);

                    if (collisionFound)
                    {
                        AddCollisionData(convexPolygonA.tag, triangleB.tag);
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
                    convexPolygonA.corners.data(),
                    convexPolygonA.corners.size(),
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionData(convexPolygonA.tag, polygonB.tag);
                }
            }
        }

        // convexPolygon A & convexPolygon B
        if (resolvePhysics)
        {
            for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
            {
                const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
                for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
                {
                    const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        collisionA.otherEntityId = entityIdB;
                        collisionA.myShapeTag = convexPolygonA.tag;
                        collisionA.otherShapeTag = convexPolygonB.tag;

                        collisionBodyA.AddCollision(collisionA);

                        if (recordInBodyB)
                        {
                            CollisionData2D collisionB;
                            collisionB.otherEntityId = entityIdA;
                            collisionB.myShapeTag = collisionA.otherShapeTag;
                            collisionB.otherShapeTag = collisionA.myShapeTag;
                            collisionB.point = collisionA.point;
                            collisionB.normal = -collisionA.normal;
                            collisionB.depth = collisionA.depth;

                            collisionBodyB.AddCollision(collisionB);
                        }
                    }
                }
            }
        }
        else
        {
            for (size_t i = 0; i < transformedCollisionConvexPolygonsA.size(); ++i)
            {
                const CollisionConvexPolygon2D& convexPolygonA = transformedCollisionConvexPolygonsA[i];
                for (size_t j = 0; j < transformedCollisionConvexPolygonsB.size(); ++j)
                {
                    const CollisionConvexPolygon2D& convexPolygonB = transformedCollisionConvexPolygonsB[j];

                    bool collisionFound = Check2D_ConvexPolygon_ConvexPolygon_Overlap(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size());

                    if (collisionFound)
                    {
                        AddCollisionData(convexPolygonA.tag, convexPolygonB.tag);
                    }
                }
            }
        }
    }

    std::vector<CollisionBody2D*> CollisionSystem2D::s_tangibleCollisionBodyPtrs_;

    std::vector<CollisionBody2D*> CollisionSystem2D::s_outOfBoundsTangibleCollisionBodyPtrs_;

    CollisionBodyQuadTree2D CollisionSystem2D::s_tangibleCollisionBodyQuadTree2D_(glm::vec2(-8.0f, -6.0f), glm::vec2(8.0f, 6.0f), 4, 16);

    std::unordered_set<std::pair<CollisionBody2D*, CollisionBody2D*>, CollisionSystem2D::PointerPairHashFunctor> CollisionSystem2D::s_collisionBodyPairPtrs_;
}
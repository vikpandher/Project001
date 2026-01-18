// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-17

#include "CollisionSystem2D.h"

#include "ComponentStores.h"
#include "Components/CollisionBody2D.h"
#include "Utilities/Overlap2D.h"

#include <functional>
#include <stack>



namespace Project001
{
    // public ------------------------------------------------------------------

    CollisionSystem2D::CollisionSystem2D()
        : sunkenMeshOverlapAllowance(0.001f)
        , sunkenMeshSeperationSpacing(0.0f)
        , componentStoresPtr_(nullptr)
        , tangibleCollisionBodyQuadTree2D_(glm::vec2(-8.0f, -6.0f), glm::vec2(8.0f, 6.0f), 3, 16)
    {}

    void CollisionSystem2D::ApplyMovement(float timestep_s)
    {
        if (componentStoresPtr_ == nullptr)
        {
            return;
        }

        UnsinkBodies();
        ApplyImpulses();

        // Get collision bodies
        CollisionBody2D* collisionBodyPtrs = nullptr;
        size_t collisionBodyCount = 0;
        bool componentFound = componentStoresPtr_->GetAllComponents<CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

        if (!componentFound)
        {
            return;
        }

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& collisionBody = collisionBodyPtrs[i];
            const glm::vec2& acceleration = collisionBody.GetAcceleration();
            glm::vec2 velocity = collisionBody.GetVelocity();
            glm::vec2 position = collisionBody.GetPosition();
            float rotation = collisionBody.GetRotation();
            const float& velocityDamping = collisionBody.GetVelocityDamping();
            const float& angularAcceleration = collisionBody.GetAngularAcceleration();
            float angularVelocity = collisionBody.GetAngularVelocity();
            const float& angularVelocityDamping = collisionBody.GetAngularVelocityDamping();

            // Integrate acceleration
            velocity += acceleration * timestep_s;
            angularVelocity += angularAcceleration * timestep_s;

            // Apply exponential damping (frame-rate independent)
            velocity *= glm::exp(-velocityDamping * timestep_s);
            angularVelocity *= glm::exp(-angularVelocityDamping * timestep_s);

            // Integrate velocity
            position += velocity * timestep_s;
            rotation += angularVelocity * timestep_s;

            collisionBody.SetVelocity(velocity);
            collisionBody.SetPosition(position);
            collisionBody.SetAngularVelocity(angularVelocity);
            collisionBody.SetRotation(rotation);
        }
    }

    void CollisionSystem2D::CalculateCollisions()
    {
        if (componentStoresPtr_ == nullptr)
        {
            return;
        }

        // Get collision bodies
        CollisionBody2D* collisionBodyPtrs = nullptr;
        size_t collisionBodyCount = 0;
        bool componentFound = componentStoresPtr_->GetAllComponents<CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

        if (!componentFound)
        {
            return;
        }

        // Clear old collision overlaps and impulses
        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            currentCollisionBody.ClearCollisionOverlaps();
            currentCollisionBody.ClearCollisionImpulses();
        }

        // Gather together all enabled collision bodies
        enabledCollisionBodyPtrs_.clear();
        collisionBodyPairPtrs_.clear();
        collisionManifolds_.clear();

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (currentCollisionBody.GetEnabled())
            {
                enabledCollisionBodyPtrs_.push_back(&currentCollisionBody);
            }
        }

        if (enabledCollisionBodyPtrs_.size() == 0)
        {
            return;
        }

        // Find collision pairs
        for (unsigned int i = 0; i < enabledCollisionBodyPtrs_.size() - 1; ++i)
        {
            Project001::CollisionBody2D*& collisionBodyA_ptr = enabledCollisionBodyPtrs_[i];
            unsigned int entityIdA;
            componentStoresPtr_->GetComponentEntityId<CollisionBody2D>(entityIdA, collisionBodyA_ptr);
            for (unsigned int j = i + 1; j < enabledCollisionBodyPtrs_.size(); ++j)
            {
                Project001::CollisionBody2D*& collisionBodyB_ptr = enabledCollisionBodyPtrs_[j];
                unsigned int entityIdB;
                componentStoresPtr_->GetComponentEntityId<CollisionBody2D>(entityIdB, collisionBodyB_ptr);

                if ((collisionBodyA_ptr->GetCollisionGroupMask() & collisionBodyB_ptr->GetAllowedCollisionFilterMask()) &&
                    (collisionBodyB_ptr->GetCollisionGroupMask() & collisionBodyA_ptr->GetAllowedCollisionFilterMask()) &&
                    Check2D_Circle_Circle_Overlap(
                        collisionBodyA_ptr->GetPosition(),
                        collisionBodyA_ptr->GetBoundingRadius(),
                        collisionBodyB_ptr->GetPosition(),
                        collisionBodyB_ptr->GetBoundingRadius()))
                {
                    collisionBodyPairPtrs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
                }
            }
        }

        CalculateCollisionOverlapsBetweenBodyPairsAndAddManifolds();
        GatherImpulses();
    }

    void CollisionSystem2D::CalculateCollisionsWithQuadTree()
    {
        if (componentStoresPtr_ == nullptr)
        {
            return;
        }

        // Get collision bodies
        CollisionBody2D* collisionBodyPtrs = nullptr;
        size_t collisionBodyCount = 0;
        bool componentFound = componentStoresPtr_->GetAllComponents<CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

        if (!componentFound)
        {
            return;
        }

        // Clear old collision overlaps and impulses
        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            currentCollisionBody.ClearCollisionOverlaps();
            currentCollisionBody.ClearCollisionImpulses();
        }

        // Gather together all enabled collision bodies
        enabledCollisionBodyPtrs_.clear();
        outOfBoundsTangibleCollisionBodyPtrs_.clear();
        tangibleCollisionBodyQuadTree2D_.Clear();
        collisionBodyPairPtrs_.clear();
        collisionManifolds_.clear();

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (currentCollisionBody.GetEnabled())
            {
                if (tangibleCollisionBodyQuadTree2D_.Insert(&currentCollisionBody))
                {
                    enabledCollisionBodyPtrs_.push_back(&currentCollisionBody);
                }
                else
                {
                    outOfBoundsTangibleCollisionBodyPtrs_.push_back(&currentCollisionBody);
                }
            }
        }

        // Find collision pairs for out-of-bounds body with out-of-bounds body collisions

        if (!outOfBoundsTangibleCollisionBodyPtrs_.empty())
        {
            for (unsigned int i = 0; i < outOfBoundsTangibleCollisionBodyPtrs_.size() - 1; ++i)
            {
                Project001::CollisionBody2D*& collisionBodyA_ptr = outOfBoundsTangibleCollisionBodyPtrs_[i];

                for (unsigned int j = i + 1; j < outOfBoundsTangibleCollisionBodyPtrs_.size(); ++j)
                {
                    Project001::CollisionBody2D*& collisionBodyB_ptr = outOfBoundsTangibleCollisionBodyPtrs_[j];

                    if ((collisionBodyA_ptr->GetCollisionGroupMask() & collisionBodyB_ptr->GetAllowedCollisionFilterMask()) &&
                        (collisionBodyB_ptr->GetCollisionGroupMask() & collisionBodyA_ptr->GetAllowedCollisionFilterMask()) &&
                        Check2D_Circle_Circle_Overlap(
                            collisionBodyA_ptr->GetPosition(),
                            collisionBodyA_ptr->GetBoundingRadius(),
                            collisionBodyB_ptr->GetPosition(),
                            collisionBodyB_ptr->GetBoundingRadius()))
                    {
                        collisionBodyPairPtrs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
                    }
                }
            }
        }

        // Find collision pairs for out-of-bounds body with in-tree body collisions

        for (unsigned int i = 0; i < outOfBoundsTangibleCollisionBodyPtrs_.size(); ++i)
        {
            Project001::CollisionBody2D*& collisionBodyA_ptr = outOfBoundsTangibleCollisionBodyPtrs_[i];

            for (unsigned int j = 0; j < enabledCollisionBodyPtrs_.size(); ++j)
            {
                Project001::CollisionBody2D*& collisionBodyB_ptr = enabledCollisionBodyPtrs_[j];

                if ((collisionBodyA_ptr->GetCollisionGroupMask() & collisionBodyB_ptr->GetAllowedCollisionFilterMask()) &&
                    (collisionBodyB_ptr->GetCollisionGroupMask() & collisionBodyA_ptr->GetAllowedCollisionFilterMask()) &&
                    Check2D_Circle_Circle_Overlap(
                        collisionBodyA_ptr->GetPosition(),
                        collisionBodyA_ptr->GetBoundingRadius(),
                        collisionBodyB_ptr->GetPosition(),
                        collisionBodyB_ptr->GetBoundingRadius()))
                {
                    collisionBodyPairPtrs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
                }
            }
        }

        // Find collision pairs for in-tree body with in-tree body collisions

        CollisionBodyQuadTreeNode2D* rootNodePtr = tangibleCollisionBodyQuadTree2D_.GetRootNode();
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
                    if (nodePtr->bodyPtrs.empty())
                    {
                        continue;
                    }

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
                                collisionBodyPairPtrs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
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

        CalculateCollisionOverlapsBetweenBodyPairsAndAddManifolds();
        GatherImpulses();
    }

    void CollisionSystem2D::CalculateOverlapsForGivenEntity(unsigned int entityId)
    {
        if (componentStoresPtr_ == nullptr)
        {
            return;
        }

        // Get primary entity's collision body
        CollisionBody2D* primaryCollisionBodyPtr = nullptr;
        bool primaryComponentFound = componentStoresPtr_->GetComponent<CollisionBody2D>(primaryCollisionBodyPtr, entityId);

        if (!primaryComponentFound || !primaryCollisionBodyPtr->GetEnabled())
        {
            return;
        }

        // Get other collision bodies (will include the primary entity's collision body)
        CollisionBody2D* collisionBodyPtrs = nullptr;
        size_t collisionBodyCount = 0;
        bool componentFound = componentStoresPtr_->GetAllComponents<CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

        if (!componentFound)
        {
            return;
        }

        // Clear primary entity's collision overlaps
        primaryCollisionBodyPtr->ClearCollisionOverlaps();

        // Gather together all enabled collision bodies
        enabledCollisionBodyPtrs_.clear();

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (currentCollisionBody.GetEnabled())
            {
                enabledCollisionBodyPtrs_.push_back(&currentCollisionBody);
            }
        }

        if (enabledCollisionBodyPtrs_.empty())
        {
            return;
        }

        // Calculate collisions (only for the given entity)
        for (unsigned int j = 1; j < enabledCollisionBodyPtrs_.size(); ++j)
        {
            Project001::CollisionBody2D*& currentCollisionBodyPtr = enabledCollisionBodyPtrs_[j];
            unsigned int currentEntityId;
            componentStoresPtr_->GetComponentEntityId<CollisionBody2D>(currentEntityId, currentCollisionBodyPtr);

            if (entityId != currentEntityId &&
                (primaryCollisionBodyPtr->GetCollisionGroupMask() & currentCollisionBodyPtr->GetAllowedCollisionFilterMask()) &&
                (currentCollisionBodyPtr->GetCollisionGroupMask() & primaryCollisionBodyPtr->GetAllowedCollisionFilterMask()) &&
                Check2D_Circle_Circle_Overlap(
                    primaryCollisionBodyPtr->GetPosition(),
                    primaryCollisionBodyPtr->GetBoundingRadius(),
                    currentCollisionBodyPtr->GetPosition(),
                    currentCollisionBodyPtr->GetBoundingRadius()))
            {
                CalculateCollisionOverlapsBetweenTwoBodiesAndAddManifold(entityId, *primaryCollisionBodyPtr, currentEntityId, *currentCollisionBodyPtr, true);
            }
        }
    }

    // protected ---------------------------------------------------------------

    void CollisionSystem2D::CalculateCollisionOverlapsBetweenBodyPairsAndAddManifolds()
    {
        // if (componentStoresPtr_ == nullptr)
        // {
        //     return;
        // }

        for (std::unordered_set<std::pair<CollisionBody2D*, CollisionBody2D*>, PointerPairHashFunctor>::iterator iter = collisionBodyPairPtrs_.begin();
            iter != collisionBodyPairPtrs_.end();
            ++iter)
        {
            CollisionBody2D* const& collisionBodyA_ptr = iter->first;
            unsigned int entityIdA;
            componentStoresPtr_->GetComponentEntityId<CollisionBody2D>(entityIdA, collisionBodyA_ptr);

            CollisionBody2D* const& collisionBodyB_ptr = iter->second;
            unsigned int entityIdB;
            componentStoresPtr_->GetComponentEntityId<CollisionBody2D>(entityIdB, collisionBodyB_ptr);

            CalculateCollisionOverlapsBetweenTwoBodiesAndAddManifold(entityIdA, *collisionBodyA_ptr, entityIdB, *collisionBodyB_ptr, false);
        }
    }

    void CollisionSystem2D::CalculateCollisionOverlapsBetweenTwoBodiesAndAddManifold(
        unsigned int entityIdA,
        CollisionBody2D& collisionBodyA,
        unsigned int entityIdB,
        CollisionBody2D& collisionBodyB,
        bool detectOnlyOverlapForEntityIdAandDontAddManifolds)
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

        CollisionManifold2D newCollisionManifold =
        {
            entityIdA,
            entityIdB,
            &collisionBodyA,
            &collisionBodyB,
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            0.0f
        };

        std::function<void(unsigned int tagA, unsigned int tagB)> AddCollisionOverlapData =
            [&](unsigned int tagA, unsigned int tagB)
            {
                CollisionOverlapData2D collisionOverlapA;
                collisionOverlapA.otherEntityId = entityIdB;
                collisionOverlapA.myShapeTag = tagA;
                collisionOverlapA.otherShapeTag = tagB;

                collisionBodyA.AddCollisionOverlap(collisionOverlapA);

                if (!detectOnlyOverlapForEntityIdAandDontAddManifolds)
                {
                    CollisionOverlapData2D collisionOverlapB;
                    collisionOverlapB.otherEntityId = entityIdA;
                    collisionOverlapB.myShapeTag = collisionOverlapA.otherShapeTag;
                    collisionOverlapB.otherShapeTag = collisionOverlapA.myShapeTag;

                    collisionBodyB.AddCollisionOverlap(collisionOverlapB);
                }
            };

        std::function<void(CollisionOverlapData2D& collisionA, unsigned int tagA, unsigned int tagB)> AddCollisionOverlapData_v2 =
            [&](CollisionOverlapData2D& collisionOverlapA, unsigned int tagA, unsigned int tagB)
            {
                collisionOverlapA.otherEntityId = entityIdB;
                collisionOverlapA.myShapeTag = tagA;
                collisionOverlapA.otherShapeTag = tagB;

                collisionBodyA.AddCollisionOverlap(collisionOverlapA);

                if (!detectOnlyOverlapForEntityIdAandDontAddManifolds)
                {
                    CollisionOverlapData2D collisionOverlapB;
                    collisionOverlapB.otherEntityId = entityIdA;
                    collisionOverlapB.myShapeTag = collisionOverlapA.otherShapeTag;
                    collisionOverlapB.otherShapeTag = collisionOverlapA.myShapeTag;
                    collisionOverlapB.point = collisionOverlapA.point;
                    collisionOverlapB.normal = -collisionOverlapA.normal;
                    collisionOverlapB.depth = collisionOverlapA.depth;

                    collisionBodyB.AddCollisionOverlap(collisionOverlapB);
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
                    AddCollisionOverlapData(pointA.tag, rectangleB.tag);
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
                    AddCollisionOverlapData(pointA.tag, orientedRectangleB.tag);
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
                    AddCollisionOverlapData(pointA.tag, circleB.tag);
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
                    AddCollisionOverlapData(pointA.tag, capsuleB.tag);
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
                    AddCollisionOverlapData(pointA.tag, triangleB.tag);
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
                    AddCollisionOverlapData(pointA.tag, polygonB.tag);
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
                    AddCollisionOverlapData(pointA.tag, convexPolygonB.tag);
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
                    AddCollisionOverlapData(lineA.tag, lineB.tag);
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
                    AddCollisionOverlapData(lineA.tag, rayB.tag);
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
                    AddCollisionOverlapData(lineA.tag, lineSegmentB.tag);
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
                    AddCollisionOverlapData(lineA.tag, rectangleB.tag);
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
                    AddCollisionOverlapData(lineA.tag, orientedRectangleB.tag);
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
                    AddCollisionOverlapData(lineA.tag, circleB.tag);
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
                    AddCollisionOverlapData(lineA.tag, capsuleB.tag);
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
                    AddCollisionOverlapData(lineA.tag, triangleB.tag);
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
                    AddCollisionOverlapData(lineA.tag, polygonB.tag);
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
                    AddCollisionOverlapData(lineA.tag, convexPolygonB.tag);
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
                    AddCollisionOverlapData(rayA.tag, lineB.tag);
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
                    AddCollisionOverlapData(rayA.tag, rayB.tag);
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
                    AddCollisionOverlapData(rayA.tag, lineSegmentB.tag);
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
                    AddCollisionOverlapData(rayA.tag, rectangleB.tag);
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
                    AddCollisionOverlapData(rayA.tag, orientedRectangleB.tag);
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
                    AddCollisionOverlapData(rayA.tag, circleB.tag);
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
                    AddCollisionOverlapData(rayA.tag, capsuleB.tag);
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
                    AddCollisionOverlapData(rayA.tag, triangleB.tag);
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
                    AddCollisionOverlapData(rayA.tag, polygonB.tag);
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
                    AddCollisionOverlapData(rayA.tag, convexPolygonB.tag);
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
                    AddCollisionOverlapData(lineSegmentA.tag, lineB.tag);
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
                    AddCollisionOverlapData(lineSegmentA.tag, rayB.tag);
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
                    AddCollisionOverlapData(lineSegmentA.tag, lineSegmentB.tag);
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
                    AddCollisionOverlapData(lineSegmentA.tag, rectangleB.tag);
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
                    AddCollisionOverlapData(lineSegmentA.tag, orientedRectangleB.tag);
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
                    AddCollisionOverlapData(lineSegmentA.tag, circleB.tag);
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
                    AddCollisionOverlapData(lineSegmentA.tag, capsuleB.tag);
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
                    AddCollisionOverlapData(lineSegmentA.tag, triangleB.tag);
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
                    AddCollisionOverlapData(lineSegmentA.tag, polygonB.tag);
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
                    AddCollisionOverlapData(lineSegmentA.tag, convexPolygonB.tag);
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
                    AddCollisionOverlapData(rectangleA.tag, lineB.tag);
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
                    AddCollisionOverlapData(rectangleA.tag, rayB.tag);
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
                    AddCollisionOverlapData(rectangleA.tag, lineSegmentB.tag);
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

                if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Rectangle_Rectangle_Overlap(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        rectangleB.bottomLeft,
                        rectangleB.topRight);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(rectangleA.tag, rectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Rectangle_Rectangle_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, rectangleA.tag, rectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Rectangle_OrientedRectangle_Overlap(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(rectangleA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Rectangle_OrientedRectangle_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, rectangleA.tag, orientedRectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Rectangle_Circle_Overlap(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        circleB.position,
                        circleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(rectangleA.tag, circleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Rectangle_Circle_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        circleB.position,
                        circleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, rectangleA.tag, circleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Rectangle_Capsule_Overlap(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(rectangleA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Rectangle_Capsule_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, rectangleA.tag, capsuleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Rectangle_Triangle_Overlap(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(rectangleA.tag, triangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Rectangle_Triangle_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, rectangleA.tag, triangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionOverlapData(rectangleA.tag, polygonB.tag);
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

                if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Rectangle_ConvexPolygon_Overlap(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size());

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(rectangleA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Rectangle_ConvexPolygon_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, rectangleA.tag, convexPolygonB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
                    }
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
                    AddCollisionOverlapData(orientedRectangleA.tag, lineB.tag);
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
                    AddCollisionOverlapData(orientedRectangleA.tag, rayB.tag);
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
                    AddCollisionOverlapData(orientedRectangleA.tag, lineSegmentB.tag);
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

                if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_OrientedRectangle_Rectangle_Overlap(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        rectangleB.bottomLeft,
                        rectangleB.topRight);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(orientedRectangleA.tag, rectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_OrientedRectangle_Rectangle_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, orientedRectangleA.tag, rectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_OrientedRectangle_OrientedRectangle_Overlap(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(orientedRectangleA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_OrientedRectangle_OrientedRectangle_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, orientedRectangleA.tag, orientedRectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_OrientedRectangle_Circle_Overlap(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        circleB.position,
                        circleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(orientedRectangleA.tag, circleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_OrientedRectangle_Circle_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        circleB.position,
                        circleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, orientedRectangleA.tag, circleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_OrientedRectangle_Capsule_Overlap(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(orientedRectangleA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_OrientedRectangle_Capsule_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, orientedRectangleA.tag, capsuleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_OrientedRectangle_Triangle_Overlap(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(orientedRectangleA.tag, triangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_OrientedRectangle_Triangle_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, orientedRectangleA.tag, triangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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
                    polygonB.corners.data(),
                    polygonB.corners.size());

                if (collisionFound)
                {
                    AddCollisionOverlapData(orientedRectangleA.tag, polygonB.tag);
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

                if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_OrientedRectangle_ConvexPolygon_Overlap(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size());

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(orientedRectangleA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_OrientedRectangle_ConvexPolygon_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, orientedRectangleA.tag, convexPolygonB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
                    }
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
                    AddCollisionOverlapData(circleA.tag, lineB.tag);
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
                    AddCollisionOverlapData(circleA.tag, rayB.tag);
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
                    AddCollisionOverlapData(circleA.tag, lineSegmentB.tag);
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

                if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Circle_Rectangle_Overlap(
                        circleA.position,
                        circleA.radius,
                        rectangleB.bottomLeft,
                        rectangleB.topRight);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(circleA.tag, rectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Circle_Rectangle_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, circleA.tag, rectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Circle_OrientedRectangle_Overlap(
                        circleA.position,
                        circleA.radius,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(circleA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Circle_OrientedRectangle_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, circleA.tag, orientedRectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Circle_Circle_Overlap(
                        circleA.position,
                        circleA.radius,
                        circleB.position,
                        circleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(circleA.tag, circleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Circle_Circle_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        circleB.position,
                        circleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, circleA.tag, circleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Circle_Capsule_Overlap(
                        circleA.position,
                        circleA.radius,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(circleA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Circle_Capsule_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, circleA.tag, capsuleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Circle_Triangle_Overlap(
                        circleA.position,
                        circleA.radius,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(circleA.tag, triangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Circle_Triangle_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, circleA.tag, triangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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
                    AddCollisionOverlapData(circleA.tag, polygonB.tag);
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

                if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Circle_ConvexPolygon_Overlap(
                        circleA.position,
                        circleA.radius,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size());

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(circleA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Circle_ConvexPolygon_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, circleA.tag, convexPolygonB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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
                    AddCollisionOverlapData(capsuleA.tag, lineB.tag);
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
                    AddCollisionOverlapData(capsuleA.tag, rayB.tag);
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
                    AddCollisionOverlapData(capsuleA.tag, lineSegmentB.tag);
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

                if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Capsule_Rectangle_Overlap(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        rectangleB.bottomLeft,
                        rectangleB.topRight);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(capsuleA.tag, rectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Capsule_Rectangle_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, capsuleA.tag, rectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Capsule_OrientedRectangle_Overlap(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(capsuleA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Capsule_OrientedRectangle_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, capsuleA.tag, orientedRectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Capsule_Circle_Overlap(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        circleB.position,
                        circleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(capsuleA.tag, circleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Capsule_Circle_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        circleB.position,
                        circleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, capsuleA.tag, circleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Capsule_Capsule_Overlap(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(capsuleA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Capsule_Capsule_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, capsuleA.tag, capsuleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Capsule_Triangle_Overlap(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(capsuleA.tag, triangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Capsule_Triangle_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, capsuleA.tag, triangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
                    }
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
                    AddCollisionOverlapData(capsuleA.tag, polygonB.tag);
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

                if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Capsule_ConvexPolygon_Overlap(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size());

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(capsuleA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Capsule_ConvexPolygon_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, capsuleA.tag, convexPolygonB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
                    }
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
                    AddCollisionOverlapData(triangleA.tag, lineB.tag);
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
                    AddCollisionOverlapData(triangleA.tag, rayB.tag);
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
                    AddCollisionOverlapData(triangleA.tag, lineSegmentB.tag);
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

                if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Triangle_Rectangle_Overlap(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        rectangleB.bottomLeft,
                        rectangleB.topRight);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(triangleA.tag, rectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Triangle_Rectangle_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, triangleA.tag, rectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Triangle_OrientedRectangle_Overlap(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(triangleA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Triangle_OrientedRectangle_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, triangleA.tag, orientedRectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Triangle_Circle_Overlap(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        circleB.position,
                        circleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(triangleA.tag, circleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Triangle_Circle_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        circleB.position,
                        circleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, triangleA.tag, circleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Triangle_Capsule_Overlap(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(triangleA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Triangle_Capsule_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, triangleA.tag, capsuleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Triangle_Triangle_Overlap(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(triangleA.tag, triangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Triangle_Triangle_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, triangleA.tag, triangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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
                    AddCollisionOverlapData(triangleA.tag, polygonB.tag);
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

                if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_Triangle_ConvexPolygon_Overlap(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size());

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(triangleA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_Triangle_ConvexPolygon_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, triangleA.tag, convexPolygonB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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
                    AddCollisionOverlapData(polygonA.tag, lineB.tag);
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
                    AddCollisionOverlapData(polygonA.tag, rayB.tag);
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
                    AddCollisionOverlapData(polygonA.tag, lineSegmentB.tag);
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
                    AddCollisionOverlapData(polygonA.tag, rectangleB.tag);
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
                    AddCollisionOverlapData(polygonA.tag, orientedRectangleB.tag);
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
                    AddCollisionOverlapData(polygonA.tag, circleB.tag);
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
                    AddCollisionOverlapData(polygonA.tag, capsuleB.tag);
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
                    AddCollisionOverlapData(polygonA.tag, triangleB.tag);
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
                    AddCollisionOverlapData(polygonA.tag, polygonB.tag);
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
                    AddCollisionOverlapData(polygonA.tag, convexPolygonB.tag);
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
                    AddCollisionOverlapData(convexPolygonA.tag, lineB.tag);
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
                    AddCollisionOverlapData(convexPolygonA.tag, rayB.tag);
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
                    AddCollisionOverlapData(convexPolygonA.tag, lineSegmentB.tag);
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

                if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_ConvexPolygon_Rectangle_Overlap(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        rectangleB.bottomLeft,
                        rectangleB.topRight);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(convexPolygonA.tag, rectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_ConvexPolygon_Rectangle_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, convexPolygonA.tag, rectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_ConvexPolygon_OrientedRectangle_Overlap(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(convexPolygonA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_ConvexPolygon_OrientedRectangle_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, convexPolygonA.tag, orientedRectangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_ConvexPolygon_Circle_Overlap(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        circleB.position,
                        circleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(convexPolygonA.tag, circleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_ConvexPolygon_Circle_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        circleB.position,
                        circleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, convexPolygonA.tag, circleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_ConvexPolygon_Capsule_Overlap(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(convexPolygonA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_ConvexPolygon_Capsule_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, convexPolygonA.tag, capsuleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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

                if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_ConvexPolygon_Triangle_Overlap(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(convexPolygonA.tag, triangleB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_ConvexPolygon_Triangle_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, convexPolygonA.tag, triangleB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
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
                    AddCollisionOverlapData(convexPolygonA.tag, polygonB.tag);
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

                if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR ||
                    convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR)
                {
                    bool collisionFound = Check2D_ConvexPolygon_ConvexPolygon_Overlap(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size());

                    if (collisionFound)
                    {
                        AddCollisionOverlapData(convexPolygonA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
                    CollisionOverlapData2D collisionOverlapA;
                    bool collisionFound = Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionOverlapA.point,
                        collisionOverlapA.normal,
                        collisionOverlapA.depth);

                    if (collisionFound)
                    {
                        AddCollisionOverlapData_v2(collisionOverlapA, convexPolygonA.tag, convexPolygonB.tag);

                        if (!detectOnlyOverlapForEntityIdAandDontAddManifolds &&
                            convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionOverlapA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionOverlapA.point;
                                newCollisionManifold.collisionNormal = collisionOverlapA.normal;
                                newCollisionManifold.collisionDepth = collisionOverlapA.depth;
                            }
                        }
                    }
                }
            }
        }

        if (newCollisionManifold.collisionDepth != 0.0f &&
            !(newCollisionManifold.collisionNormal.x == 0.0f && newCollisionManifold.collisionNormal.y == 0.0f) &&
            !std::isnan(newCollisionManifold.collisionPoint.x) &&
            !std::isnan(newCollisionManifold.collisionPoint.y) &&
            !std::isnan(newCollisionManifold.collisionNormal.x) &&
            !std::isnan(newCollisionManifold.collisionNormal.y) &&
            !std::isnan(newCollisionManifold.collisionDepth))
        {
            collisionManifolds_.push_back(newCollisionManifold);
        }
    }

    void CollisionSystem2D::UnsinkBodies()
    {
        for (size_t i = 0; i < collisionManifolds_.size(); ++i)
        {
            CollisionManifold2D& collisionManifold = collisionManifolds_[i];

            CollisionBody2D& collisionBodyA = *collisionManifold.collisionBodyA_Ptr;
            CollisionBody2D& collisionBodyB = *collisionManifold.collisionBodyB_Ptr;

            const glm::vec2& collisionPoint = collisionManifold.collisionPoint;
            const glm::vec2& collisionNormal = collisionManifold.collisionNormal; // points away from bodyA
            float collisionDepth = collisionManifold.collisionDepth + sunkenMeshSeperationSpacing;

            const bool& fixedTranslationA = collisionBodyA.GetFixedTranslation();
            const bool& fixedTranslationB = collisionBodyB.GetFixedTranslation();

            const bool& fixedRotationA = collisionBodyA.GetFixedRotation();
            const bool& fixedRotationB = collisionBodyB.GetFixedRotation();

            const glm::vec2& positionA = collisionBodyA.GetPosition();
            const glm::vec2& positionB = collisionBodyB.GetPosition();

            float massA = collisionBodyA.GetMass();
            float massB = collisionBodyB.GetMass();

            float momentOfInertiaA = collisionBodyA.GetMomentOfInertia();
            float momentOfInertiaB = collisionBodyB.GetMomentOfInertia();

            // CollisionBody should ensure that its mass is never less than or
            // equal to 0.0f. Same goes for momentOfInertia.
            // 
            // if (massA <= 0.0f || massB <= 0.0f)
            // {
            //     continue;
            // }

            // Unsink bodies from eachother
            // -----------------------------------------------------------------

            if (collisionDepth > sunkenMeshOverlapAllowance)
            {
                float collisionSeperation = collisionDepth; // - sunkenMeshOverlapAllowance;

                bool bodyA_notMoving = std::isinf(massA) || std::isinf(momentOfInertiaA) || fixedTranslationA || fixedRotationA;
                bool bodyB_notMoving = std::isinf(massB) || std::isinf(momentOfInertiaB) || fixedTranslationB || fixedRotationB;

                if (bodyA_notMoving)
                {
                    if (bodyB_notMoving)
                    {
                        if (std::isinf(massA) || std::isinf(massB))
                        {
                            if (massA > massB)
                            {
                                // A is more immovable
                                collisionBodyB.SetPosition(positionB + collisionNormal * collisionSeperation);
                            }
                            else if (massB > massA)
                            {
                                // B is more immovable
                                collisionBodyA.SetPosition(positionA + collisionNormal * collisionSeperation * -1.0f);
                            }
                            else
                            {
                                // equally immovable
                                collisionBodyA.SetPosition(positionA + collisionNormal * collisionSeperation * -0.5f);
                                collisionBodyB.SetPosition(positionB + collisionNormal * collisionSeperation * 0.5f);
                            }
                        }
                        else
                        {
                            float combinedMass = massA + massB;
                            float massRatioA = massA / combinedMass;
                            float massRatioB = massB / combinedMass;

                            collisionBodyA.SetPosition(positionA + collisionNormal * collisionSeperation * -1.0f * massRatioA);
                            collisionBodyB.SetPosition(positionB + collisionNormal * collisionSeperation * massRatioB);
                        }
                    }
                    else // if (bodyA_notMoving)
                    {
                        collisionBodyB.SetPosition(positionB + collisionNormal * collisionSeperation);
                    }
                }
                else if (bodyB_notMoving)
                {
                    collisionBodyA.SetPosition(positionA + collisionNormal * collisionSeperation * -1.0f);
                }
                else
                {
                    float combinedMass = massA + massB;
                    float massRatioA = massA / combinedMass;
                    float massRatioB = massB / combinedMass;

                    collisionBodyA.SetPosition(positionA + collisionNormal * collisionSeperation * -1.0f * massRatioA);
                    collisionBodyB.SetPosition(positionB + collisionNormal * collisionSeperation * massRatioB);
                }
            }
        }
    }

    void CollisionSystem2D::GatherImpulses()
    {
        for (size_t i = 0; i < collisionManifolds_.size(); ++i)
        {
            CollisionManifold2D& collisionManifold = collisionManifolds_[i];

            unsigned int& entityIdA = collisionManifold.entityIdA;
            unsigned int& entityIdB = collisionManifold.entityIdB;

            CollisionBody2D& collisionBodyA = *collisionManifold.collisionBodyA_Ptr;
            CollisionBody2D& collisionBodyB = *collisionManifold.collisionBodyB_Ptr;

            const glm::vec2& collisionPoint = collisionManifold.collisionPoint;
            const glm::vec2& collisionNormal = collisionManifold.collisionNormal; // points away from bodyA
            float collisionDepth = collisionManifold.collisionDepth + sunkenMeshSeperationSpacing;

            const bool& fixedTranslationA = collisionBodyA.GetFixedTranslation();
            const bool& fixedTranslationB = collisionBodyB.GetFixedTranslation();

            const bool& fixedRotationA = collisionBodyA.GetFixedRotation();
            const bool& fixedRotationB = collisionBodyB.GetFixedRotation();

            const glm::vec2& positionA = collisionBodyA.GetPosition();
            const glm::vec2& positionB = collisionBodyB.GetPosition();

            const glm::vec2& velocityA = collisionBodyA.GetVelocity();
            const glm::vec2& velocityB = collisionBodyB.GetVelocity();

            const float& angularVelocityA = collisionBodyA.GetAngularVelocity();
            const float& angularVelocityB = collisionBodyB.GetAngularVelocity();

            const float& restitutionA = collisionBodyA.GetRestitution();
            const float& restitutionB = collisionBodyB.GetRestitution();

            const float& frictionA = collisionBodyA.GetFriction();
            const float& frictionB = collisionBodyB.GetFriction();

            float massA = collisionBodyA.GetMass();
            float massB = collisionBodyB.GetMass();

            float momentOfInertiaA = collisionBodyA.GetMomentOfInertia();
            float momentOfInertiaB = collisionBodyB.GetMomentOfInertia();

            if (fixedTranslationA)
            {
                massA = std::numeric_limits<float>::infinity();
            }

            if (fixedTranslationB)
            {
                massB = std::numeric_limits<float>::infinity();
            }

            if (fixedRotationA)
            {
                momentOfInertiaA = std::numeric_limits<float>::infinity();
            }

            if (fixedRotationB)
            {
                momentOfInertiaB = std::numeric_limits<float>::infinity();
            }

            // Gather collision impulses
            // -----------------------------------------------------------------

            glm::vec2 rA = collisionPoint - positionA; // center of mass A to collision point
            glm::vec2 rB = collisionPoint - positionB; // center of mass B to collision point

            glm::vec2 rA_p(-rA.y, rA.x); // perpendicular
            glm::vec2 rB_p(-rB.y, rB.x); // perpendicular

            // Add angular velocity contribution
            glm::vec2 vA = velocityA + angularVelocityA * rA_p;
            glm::vec2 vB = velocityB + angularVelocityB * rB_p;

            // Relative velocity at collision point
            glm::vec2 relativeVelocity = vB - vA;

            // Normal component of rel. velocity along the collision normal length
            float relativeVelocity_n_length = glm::dot(relativeVelocity, collisionNormal);

            if (relativeVelocity_n_length >= 0.0f)
            {
                // The collision bodies are not moving towards eachother
                continue;
            }

            // Use minimum restitution value
            float restitution = std::min(restitutionA, restitutionB);

            float invMassA = 1.0f / massA;
            float invMassB = 1.0f / massB;
            float invMomentOfInertiaA = 1.0f / momentOfInertiaA;
            float invMomentOfInertiaB = 1.0f / momentOfInertiaB;

            glm::vec2 collisionTangent = glm::vec2(-collisionNormal.y, collisionNormal.x);

            // Scalar projections of rA and rB onto collisionTangent
            float leverArmA = glm::dot(rA, collisionTangent);
            float leverArmB = glm::dot(rB, collisionTangent);

            float impulseScalar_denominator = invMassA + invMassB +
                (leverArmA * leverArmA * invMomentOfInertiaA) + (leverArmB * leverArmB * invMomentOfInertiaB);

            float impulseScalar = -(1.0f + restitution) * relativeVelocity_n_length / impulseScalar_denominator;

            glm::vec2 impulse = impulseScalar * collisionNormal;
            glm::vec2 impulseA = impulse / massA;
            glm::vec2 impulseB = impulse / massB;

            glm::vec2 addedVelocityA(0.0f, 0.0f);
            glm::vec2 addedVelocityB(0.0f, 0.0f);

            if (!fixedTranslationA)
            {
                if (!fixedTranslationB)
                {
                    addedVelocityA -= impulseA;
                    addedVelocityB += impulseB;
                }
                else
                {
                    addedVelocityA -= impulseA;
                    addedVelocityA -= impulseB;
                }
            }
            else if (!fixedTranslationB)
            {
                addedVelocityB += impulseA;
                addedVelocityB += impulseB;
            }

            glm::vec2 impulsePerpendicular = glm::vec2(-impulse.y, impulse.x);
            float angularImpulseA = glm::dot(rA, impulsePerpendicular) / momentOfInertiaA;
            float angularImpulseB = glm::dot(rB, impulsePerpendicular) / momentOfInertiaB;

            float addedAngularVelocityA = 0.0f;
            float addedAngularVelocityB = 0.0f;

            if (!fixedRotationA)
            {
                if (!fixedRotationB)
                {
                    addedAngularVelocityA += angularImpulseA;
                    addedAngularVelocityB -= angularImpulseB;
                }
                else
                {
                    addedAngularVelocityA += angularImpulseA;
                    addedAngularVelocityA += angularImpulseB;
                }
            }
            else if (!fixedRotationB)
            {
                addedAngularVelocityB -= angularImpulseA;
                addedAngularVelocityB -= angularImpulseB;
            }

            // Gather friction impulses
            // -----------------------------------------------------------------

            float rA_dot_p = glm::dot(rA, collisionNormal);
            float rB_dot_p = glm::dot(rB, collisionNormal);

            float frictionImpulseScalar_denominator = invMassA + invMassB +
                (rA_dot_p * rA_dot_p * invMomentOfInertiaA) + (rB_dot_p * rB_dot_p * invMomentOfInertiaB);

            // Use minimum friction value
            float friction = std::min(frictionA, frictionB);

            float relativeVelocity_t_length = glm::dot(relativeVelocity, collisionTangent);

            float frictionImpulseScalar = -relativeVelocity_t_length / frictionImpulseScalar_denominator;

            frictionImpulseScalar = glm::clamp(frictionImpulseScalar, -friction * impulseScalar, friction * impulseScalar);

            glm::vec2 frictionImpulse = frictionImpulseScalar * collisionTangent;
            glm::vec2 frictionImpulseA = frictionImpulse / massA;
            glm::vec2 frictionImpulseB = frictionImpulse / massB;

            if (!fixedTranslationA)
            {
                if (!fixedTranslationB)
                {
                    addedVelocityA -= frictionImpulseA;
                    addedVelocityB += frictionImpulseB;
                }
                else
                {
                    addedVelocityA -= frictionImpulseA;
                    addedVelocityA -= frictionImpulseB;
                }
            }
            else if (!fixedTranslationB)
            {
                addedVelocityB += frictionImpulseA;
                addedVelocityB += frictionImpulseB;
            }

            glm::vec2 frictionImpulsePerpendicular = glm::vec2(-frictionImpulse.y, frictionImpulse.x);
            float frictionAngularImpulseA = glm::dot(rA, frictionImpulsePerpendicular) / momentOfInertiaA;
            float frictionAngularImpulseB = glm::dot(rB, frictionImpulsePerpendicular) / momentOfInertiaB;

            if (!fixedRotationA)
            {
                if (!fixedRotationB)
                {
                    addedAngularVelocityA += frictionAngularImpulseA;
                    addedAngularVelocityB -= frictionAngularImpulseB;
                }
                else
                {
                    addedAngularVelocityA += frictionAngularImpulseA;
                    addedAngularVelocityA += frictionAngularImpulseB;
                }
            }
            else if (!fixedRotationB)
            {
                addedAngularVelocityB -= frictionAngularImpulseA;
                addedAngularVelocityB -= frictionAngularImpulseB;
            }

            // -----------------------------------------------------------------

            CollisionImpulseData2D collisionImpulseDataA = {};
            collisionImpulseDataA.otherEntityId = entityIdB;
            collisionImpulseDataA.impulse = addedVelocityA;
            collisionImpulseDataA.angularImpulse = addedAngularVelocityA;
            collisionBodyA.AddCollisionImpulse(collisionImpulseDataA);

            CollisionImpulseData2D collisionImpulseDataB = {};
            collisionImpulseDataB.otherEntityId = entityIdA;
            collisionImpulseDataB.impulse = addedVelocityB;
            collisionImpulseDataB.angularImpulse = addedAngularVelocityB;
            collisionBodyB.AddCollisionImpulse(collisionImpulseDataB);
        }
    }

    void CollisionSystem2D::ApplyImpulses()
    {
        CollisionBody2D* collisionBodyPtrs = nullptr;
        size_t collisionBodyCount = 0;
        bool componentFound = componentStoresPtr_->GetAllComponents<CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

        if (!componentFound)
        {
            return;
        }

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& collisionBody = collisionBodyPtrs[i];
            const std::vector<CollisionImpulseData2D>& collisionImpulses = collisionBody.GetCollisionImpulses();

            for (size_t j = 0; j < collisionImpulses.size(); ++j)
            {
                const CollisionImpulseData2D& collisionImpulse = collisionImpulses[j];

                collisionBody.SetVelocity(collisionBody.GetVelocity() + collisionImpulse.impulse);
                collisionBody.SetAngularVelocity(collisionBody.GetAngularVelocity() + collisionImpulse.angularImpulse);
            }
        }
    }
}
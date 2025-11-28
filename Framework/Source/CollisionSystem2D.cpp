// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-27

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

        // Gather together all enabled collision bodies
        s_enabledCollisionBodyPtrs_.clear();
        s_collisionBodyPairPtrs_.clear();
        s_collisionManifolds_.clear();

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (currentCollisionBody.GetEnabled())
            {
                s_enabledCollisionBodyPtrs_.push_back(&currentCollisionBody);
            }
        }

        if (s_enabledCollisionBodyPtrs_.size() == 0)
        {
            return;
        }

        // Find collision pairs
        for (unsigned int i = 0; i < s_enabledCollisionBodyPtrs_.size() - 1; ++i)
        {
            Project001::CollisionBody2D*& collisionBodyA_ptr = s_enabledCollisionBodyPtrs_[i];
            unsigned int entityIdA;
            componentStoresPtr->GetComponentEntityId<CollisionBody2D>(entityIdA, collisionBodyA_ptr);
            for (unsigned int j = i + 1; j < s_enabledCollisionBodyPtrs_.size(); ++j)
            {
                Project001::CollisionBody2D*& collisionBodyB_ptr = s_enabledCollisionBodyPtrs_[j];
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
                    s_collisionBodyPairPtrs_.insert(std::make_pair(collisionBodyA_ptr, collisionBodyB_ptr));
                }
            }
        }

        CalculateCollisionsBetweenBodyPairs(componentStoresPtr);
        ResolveCollisions();
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

        if (!primaryComponentFound || !primaryCollisionBodyPtr->GetEnabled())
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

        // Gather together all enabled collision bodies
        s_enabledCollisionBodyPtrs_.clear();

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (currentCollisionBody.GetEnabled())
            {
                s_enabledCollisionBodyPtrs_.push_back(&currentCollisionBody);
            }
        }

        if (s_enabledCollisionBodyPtrs_.empty())
        {
            return;
        }

        // Calculate collisions (only for the given entity)
        for (unsigned int j = 1; j < s_enabledCollisionBodyPtrs_.size(); ++j)
        {
            Project001::CollisionBody2D*& currentCollisionBodyPtr = s_enabledCollisionBodyPtrs_[j];
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
                CalculateCollisionsBetweenTwoBodiesAndAddManifold(entityId, *primaryCollisionBodyPtr, currentEntityId, *currentCollisionBodyPtr, false);
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

        // Gather together all enabled collision bodies
        s_enabledCollisionBodyPtrs_.clear();
        s_outOfBoundsTangibleCollisionBodyPtrs_.clear();
        s_tangibleCollisionBodyQuadTree2D_.Clear();
        s_collisionBodyPairPtrs_.clear();
        s_collisionManifolds_.clear();

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            if (currentCollisionBody.GetEnabled())
            {
                if (s_tangibleCollisionBodyQuadTree2D_.Insert(&currentCollisionBody))
                {
                    s_enabledCollisionBodyPtrs_.push_back(&currentCollisionBody);
                }
                else
                {
                    s_outOfBoundsTangibleCollisionBodyPtrs_.push_back(&currentCollisionBody);
                }
            }
        }

        // Find collision pairs for out-of-bounds body with out-of-bounds body collisions

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

        // Find collision pairs for out-of-bounds body with in-tree body collisions

        for (unsigned int i = 0; i < s_outOfBoundsTangibleCollisionBodyPtrs_.size(); ++i)
        {
            Project001::CollisionBody2D*& collisionBodyA_ptr = s_outOfBoundsTangibleCollisionBodyPtrs_[i];

            for (unsigned int j = 0; j < s_enabledCollisionBodyPtrs_.size(); ++j)
            {
                Project001::CollisionBody2D*& collisionBodyB_ptr = s_enabledCollisionBodyPtrs_[j];

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

        // Find collision pairs for in-tree body with in-tree body collisions

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

        CalculateCollisionsBetweenBodyPairs(componentStoresPtr);
        ResolveCollisions();
    }

    float CollisionSystem2D::s_sunkenMeshOverlapAllowance = 0.001f;
    float CollisionSystem2D::s_sunkenMeshSeperationSpacing = 0.0f;

    // protected ---------------------------------------------------------------

    void CollisionSystem2D::CalculateCollisionsBetweenBodyPairs(ComponentStores* componentStoresPtr)
    {
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

            CalculateCollisionsBetweenTwoBodiesAndAddManifold(entityIdA, *collisionBodyA_ptr, entityIdB, *collisionBodyB_ptr, true);
        }
    }

    void CollisionSystem2D::CalculateCollisionsBetweenTwoBodiesAndAddManifold(
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

        CollisionManifold2D newCollisionManifold =
        {
            &collisionBodyA,
            &collisionBodyB,
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            0.0f
        };

        std::function<void(unsigned int tagA, unsigned int tagB)> AddCollisionData =
            [&](unsigned int tagA, unsigned int tagB)
            {
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

        std::function<void(CollisionData2D& collisionA, unsigned int tagA, unsigned int tagB)> AddCollisionData2 =
            [&](CollisionData2D& collisionA, unsigned int tagA, unsigned int tagB)
            {
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
                    collisionB.point = collisionA.point;
                    collisionB.normal = -collisionA.normal;
                    collisionB.depth = collisionA.depth;

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
                        AddCollisionData(rectangleA.tag, rectangleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, rectangleA.tag, rectangleB.tag);

                        if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(rectangleA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Rectangle_OrientedRectangle_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, rectangleA.tag, orientedRectangleB.tag);

                        if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(rectangleA.tag, circleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, rectangleA.tag, circleB.tag);

                        if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(rectangleA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Rectangle_Capsule_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, rectangleA.tag, capsuleB.tag);

                        if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(rectangleA.tag, triangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Rectangle_Triangle_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, rectangleA.tag, triangleB.tag);

                        if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(rectangleA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Rectangle_ConvexPolygon_CollisionPointNormalDepth(
                        rectangleA.bottomLeft,
                        rectangleA.topRight,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, rectangleA.tag, convexPolygonB.tag);

                        if (rectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(orientedRectangleA.tag, rectangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_OrientedRectangle_Rectangle_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, orientedRectangleA.tag, rectangleB.tag);

                        if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(orientedRectangleA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_OrientedRectangle_OrientedRectangle_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, orientedRectangleA.tag, orientedRectangleB.tag);

                        if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(orientedRectangleA.tag, circleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, orientedRectangleA.tag, circleB.tag);

                        if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(orientedRectangleA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_OrientedRectangle_Capsule_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, orientedRectangleA.tag, capsuleB.tag);

                        if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(orientedRectangleA.tag, triangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_OrientedRectangle_Triangle_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, orientedRectangleA.tag, triangleB.tag);

                        if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(orientedRectangleA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_OrientedRectangle_ConvexPolygon_CollisionPointNormalDepth(
                        orientedRectangleA.halfSize,
                        orientedRectangleA.position,
                        orientedRectangleA.rotation,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, orientedRectangleA.tag, convexPolygonB.tag);

                        if (orientedRectangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(circleA.tag, rectangleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, circleA.tag, rectangleB.tag);

                        if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(circleA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, circleA.tag, orientedRectangleB.tag);

                        if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(circleA.tag, circleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, circleA.tag, circleB.tag);

                        if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(circleA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Circle_Capsule_CollisionPointNormalDepth(
                        circleA.position,
                        circleA.radius,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, circleA.tag, capsuleB.tag);

                        if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(circleA.tag, triangleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, circleA.tag, triangleB.tag);

                        if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                    AddCollisionData(circleA.tag, polygonB.tag);
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
                        AddCollisionData(circleA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, circleA.tag, convexPolygonB.tag);

                        if (circleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(capsuleA.tag, rectangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Capsule_Rectangle_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, capsuleA.tag, rectangleB.tag);

                        if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(capsuleA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Capsule_OrientedRectangle_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, capsuleA.tag, orientedRectangleB.tag);

                        if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(capsuleA.tag, circleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Capsule_Circle_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        circleB.position,
                        circleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, capsuleA.tag, circleB.tag);

                        if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(capsuleA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Capsule_Capsule_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, capsuleA.tag, capsuleB.tag);

                        if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(capsuleA.tag, triangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Capsule_Triangle_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        triangleB.corner1,
                        triangleB.corner2,
                        triangleB.corner3,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, capsuleA.tag, triangleB.tag);

                        if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(capsuleA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Capsule_ConvexPolygon_CollisionPointNormalDepth(
                        capsuleA.start,
                        capsuleA.end,
                        capsuleA.radius,
                        convexPolygonB.corners.data(),
                        convexPolygonB.corners.size(),
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, capsuleA.tag, convexPolygonB.tag);

                        if (capsuleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(triangleA.tag, rectangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Triangle_Rectangle_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, triangleA.tag, rectangleB.tag);

                        if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(triangleA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Triangle_OrientedRectangle_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, triangleA.tag, orientedRectangleB.tag);

                        if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(triangleA.tag, circleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, triangleA.tag, circleB.tag);

                        if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(triangleA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_Triangle_Capsule_CollisionPointNormalDepth(
                        triangleA.corner1,
                        triangleA.corner2,
                        triangleA.corner3,
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, triangleA.tag, capsuleB.tag);

                        if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(triangleA.tag, triangleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, triangleA.tag, triangleB.tag);

                        if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                    AddCollisionData(triangleA.tag, polygonB.tag);
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
                        AddCollisionData(triangleA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, triangleA.tag, convexPolygonB.tag);

                        if (triangleA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(convexPolygonA.tag, rectangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_ConvexPolygon_Rectangle_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        rectangleB.bottomLeft,
                        rectangleB.topRight,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, convexPolygonA.tag, rectangleB.tag);

                        if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            rectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(convexPolygonA.tag, orientedRectangleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_ConvexPolygon_OrientedRectangle_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        orientedRectangleB.halfSize,
                        orientedRectangleB.position,
                        orientedRectangleB.rotation,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, convexPolygonA.tag, orientedRectangleB.tag);

                        if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            orientedRectangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(convexPolygonA.tag, circleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, convexPolygonA.tag, circleB.tag);

                        if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            circleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(convexPolygonA.tag, capsuleB.tag);
                    }
                }
                else
                {
                    CollisionData2D collisionA;
                    bool collisionFound = Get2D_ConvexPolygon_Capsule_CollisionPointNormalDepth(
                        convexPolygonA.corners.data(),
                        convexPolygonA.corners.size(),
                        capsuleB.start,
                        capsuleB.end,
                        capsuleB.radius,
                        collisionA.point,
                        collisionA.normal,
                        collisionA.depth);

                    if (collisionFound)
                    {
                        AddCollisionData2(collisionA, convexPolygonA.tag, capsuleB.tag);

                        if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            capsuleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                        AddCollisionData(convexPolygonA.tag, triangleB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, convexPolygonA.tag, triangleB.tag);

                        if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            triangleB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
                    AddCollisionData(convexPolygonA.tag, polygonB.tag);
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
                        AddCollisionData(convexPolygonA.tag, convexPolygonB.tag);
                    }
                }
                else
                {
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
                        AddCollisionData2(collisionA, convexPolygonA.tag, convexPolygonB.tag);

                        if (convexPolygonA.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY &&
                            convexPolygonB.physicsType == CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                        {
                            if (collisionA.depth > newCollisionManifold.collisionDepth)
                            {
                                newCollisionManifold.collisionPoint = collisionA.point;
                                newCollisionManifold.collisionNormal = collisionA.normal;
                                newCollisionManifold.collisionDepth = collisionA.depth;
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
            s_collisionManifolds_.push_back(newCollisionManifold);
        }
    }

    void CollisionSystem2D::ResolveCollisions()
    {
        for (size_t i = 0; i < s_collisionManifolds_.size(); ++i)
        {
            CollisionManifold2D& collisionManifold = s_collisionManifolds_[i];
            const glm::vec2& collisionPoint = collisionManifold.collisionPoint;
            const glm::vec2& collisionNormal = collisionManifold.collisionNormal; // points away from bodyA
            float collisionDepth = collisionManifold.collisionDepth + s_sunkenMeshSeperationSpacing;

            CollisionBody2D& collisionBodyA = *collisionManifold.collisionBodyA_Ptr;
            CollisionBody2D& collisionBodyB = *collisionManifold.collisionBodyB_Ptr;

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

            // CollisionBody should ensure that its mass is never less than or
            // equal to 0.0f. Same goes for momentOfInertia.
            // 
            // if (massA <= 0.0f || massB <= 0.0f)
            // {
            //     continue;
            // }

            // Unsink bodies from eachother
            // -----------------------------------------------------------------

            if (collisionDepth > s_sunkenMeshOverlapAllowance)
            {
                float collisionSeperation = collisionDepth - s_sunkenMeshOverlapAllowance;

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

            // Apply collision impulses
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

            glm::vec2 newVelocityA = velocityA;
            glm::vec2 newVelocityB = velocityB;

            if (!fixedTranslationA)
            {
                if (!fixedTranslationB)
                {
                    newVelocityA -= impulseA;
                    newVelocityB += impulseB;
                }
                else
                {
                    newVelocityA -= impulseA;
                    newVelocityA -= impulseB;
                }
            }
            else if (!fixedTranslationB)
            {
                newVelocityB += impulseA;
                newVelocityB += impulseB;
            }

            glm::vec2 impulsePerpendicular = glm::vec2(-impulse.y, impulse.x);
            float angularImpulseA = glm::dot(rA, impulsePerpendicular) / momentOfInertiaA;
            float angularImpulseB = glm::dot(rB, impulsePerpendicular) / momentOfInertiaB;

            float newAngularVelocityA = angularVelocityA;
            float newAngularVelocityB = angularVelocityB;

            if (!fixedRotationA)
            {
                if (!fixedRotationB)
                {
                    newAngularVelocityA += angularImpulseA;
                    newAngularVelocityB -= angularImpulseB;
                }
                else
                {
                    newAngularVelocityA += angularImpulseA;
                    newAngularVelocityA += angularImpulseB;
                }
            }
            else if (!fixedRotationB)
            {
                newAngularVelocityB -= angularImpulseA;
                newAngularVelocityB -= angularImpulseB;
            }

            // Apply friction impulses
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
                    newVelocityA -= frictionImpulseA;
                    newVelocityB += frictionImpulseB;
                }
                else
                {
                    newVelocityA -= frictionImpulseA;
                    newVelocityA -= frictionImpulseB;
                }
            }
            else if (!fixedTranslationB)
            {
                newVelocityB += frictionImpulseA;
                newVelocityB += frictionImpulseB;
            }

            glm::vec2 frictionImpulsePerpendicular = glm::vec2(-frictionImpulse.y, frictionImpulse.x);
            float frictionAngularImpulseA = glm::dot(rA, frictionImpulsePerpendicular) / momentOfInertiaA;
            float frictionAngularImpulseB = glm::dot(rB, frictionImpulsePerpendicular) / momentOfInertiaB;

            if (!fixedRotationA)
            {
                if (!fixedRotationB)
                {
                    newAngularVelocityA += frictionAngularImpulseA;
                    newAngularVelocityB -= frictionAngularImpulseB;
                }
                else
                {
                    newAngularVelocityA += frictionAngularImpulseA;
                    newAngularVelocityA += frictionAngularImpulseB;
                }
            }
            else if (!fixedRotationB)
            {
                newAngularVelocityB -= frictionAngularImpulseA;
                newAngularVelocityB -= frictionAngularImpulseB;
            }

            // -----------------------------------------------------------------

            collisionBodyA.SetVelocity(newVelocityA);
            collisionBodyB.SetVelocity(newVelocityB);

            collisionBodyA.SetAngularVelocity(newAngularVelocityA);
            collisionBodyB.SetAngularVelocity(newAngularVelocityB);
        }
    }

    std::vector<CollisionBody2D*> CollisionSystem2D::s_enabledCollisionBodyPtrs_;

    std::vector<CollisionBody2D*> CollisionSystem2D::s_outOfBoundsTangibleCollisionBodyPtrs_;

    CollisionBodyQuadTree2D CollisionSystem2D::s_tangibleCollisionBodyQuadTree2D_(glm::vec2(-8.0f, -6.0f), glm::vec2(8.0f, 6.0f), 3, 16);

    std::unordered_set<std::pair<CollisionBody2D*, CollisionBody2D*>, CollisionSystem2D::PointerPairHashFunctor> CollisionSystem2D::s_collisionBodyPairPtrs_;

    std::vector<CollisionSystem2D::CollisionManifold2D> CollisionSystem2D::s_collisionManifolds_;
}
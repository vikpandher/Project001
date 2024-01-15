#include "CollisionSystem2D.h"

#include "Engine/ComponentStores.h"
#include "Engine/Components/CollisionBody2D.h"
#include "Engine/Math/Overlap2D.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    void CollisionSystem2D::CalculateCollisions(ComponentStores* componentStoresPtr)
    {
        CollisionBody2D* collisionBodyPtrs = nullptr;
        size_t collisionBodyCount = 0;
        componentStoresPtr->GetAllComponents<CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

        for (size_t i = 0; i < collisionBodyCount; ++i)
        {
            CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

            currentCollisionBody.ClearCollisions();

            if (!currentCollisionBody.BoundingRadiusUpToDate())
            {
                currentCollisionBody.CalculateBoundingRadius();
            }

            if (!currentCollisionBody.TransformedShapesUpToDate())
            {
                currentCollisionBody.CalculateTransformedShapes();
            }
        }

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

        for (unsigned int i = 0; i < s_tangibleCollisionBodyPtrs_.size() - 1; ++i)
        {
            Project001::CollisionBody2D*& collisionBodyA = s_tangibleCollisionBodyPtrs_[i];
            unsigned int entityIdA;
            componentStoresPtr->GetComponentEntityId<CollisionBody2D>(collisionBodyA, entityIdA);
            for (unsigned int j = i + 1; j < s_tangibleCollisionBodyPtrs_.size(); ++j)
            {
                Project001::CollisionBody2D*& collisionBodyB = s_tangibleCollisionBodyPtrs_[j];
                unsigned int entityIdB;
                componentStoresPtr->GetComponentEntityId<CollisionBody2D>(collisionBodyB, entityIdB);

                if (Check2D_Circle_Circle_Overlap(
                    collisionBodyA->GetPosition(),
                    collisionBodyA->GetBoundingRadius(),
                    collisionBodyB->GetPosition(),
                    collisionBodyB->GetBoundingRadius()) &&
                    (collisionBodyA->GetCollisionGroupMask() & collisionBodyB->GetCollisionGroupMask()))
                {
                    CalculateCollisions(entityIdA, *collisionBodyA, entityIdB, *collisionBodyB, true);
                }
            }
        }
    }

    void CollisionSystem2D::CalculateCollisionsForGivenEntity(
        unsigned int entityId,
        ComponentStores* componentStoresPtr)
    {
        CollisionBody2D* collisionBodyPtr = nullptr;
        bool componentFound = componentStoresPtr->GetComponent<CollisionBody2D>(entityId, collisionBodyPtr);

        if (!componentFound || !collisionBodyPtr->GetTangible())
        {
            return;
        }

        collisionBodyPtr->ClearCollisions();

        if (!collisionBodyPtr->BoundingRadiusUpToDate())
        {
            collisionBodyPtr->CalculateBoundingRadius();
        }

        if (!collisionBodyPtr->TransformedShapesUpToDate())
        {
            collisionBodyPtr->CalculateTransformedShapes();
        }

        CollisionBody2D* otherCollisionBodyPtrs = nullptr;
        size_t otherCollisionBodyCount = 0;
        componentStoresPtr->GetAllComponents<CollisionBody2D>(otherCollisionBodyPtrs, otherCollisionBodyCount);

        s_tangibleCollisionBodyPtrs_.clear();

        for (size_t i = 0; i < otherCollisionBodyCount; ++i)
        {
            CollisionBody2D& otherCollisionBody = otherCollisionBodyPtrs[i];

            unsigned int otherEntityId;
            componentStoresPtr->GetComponentEntityId<CollisionBody2D>(&otherCollisionBody, otherEntityId);

            if (entityId != otherEntityId && otherCollisionBody.GetTangible())
            {
                s_tangibleCollisionBodyPtrs_.push_back(&otherCollisionBody);
            }
        }

        for (unsigned int i = 0; i < s_tangibleCollisionBodyPtrs_.size(); ++i)
        {
            Project001::CollisionBody2D*& otherCollisionBodyPtr = s_tangibleCollisionBodyPtrs_[i];

            unsigned int otherEntityId;
            componentStoresPtr->GetComponentEntityId<CollisionBody2D>(otherCollisionBodyPtr, otherEntityId);

            if (Check2D_Circle_Circle_Overlap(
                collisionBodyPtr->GetPosition(),
                collisionBodyPtr->GetBoundingRadius(),
                otherCollisionBodyPtr->GetPosition(),
                otherCollisionBodyPtr->GetBoundingRadius()) &&
                (collisionBodyPtr->GetCollisionGroupMask() & otherCollisionBodyPtr->GetCollisionGroupMask()))
            {
                CalculateCollisions(entityId, *collisionBodyPtr, otherEntityId, *otherCollisionBodyPtr, false);
            }
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
        const std::vector<Point2D>& transformedPointsA = collisionBodyA.GetTransformedPoints();
        const std::vector<Line2D>& transformedLinesA = collisionBodyA.GetTransformedLines();
        const std::vector<Ray2D>& transformedRaysA = collisionBodyA.GetTransformedRays();
        const std::vector<LineSegment2D>& transformedLineSegmentsA = collisionBodyA.GetTransformedLineSegments();
        const std::vector<Rectangle2D>& transformedRectanglesA = collisionBodyA.GetTransformedRectangles();
        const std::vector<OrientedRectangle2D>& transformedOrientedRectanglesA = collisionBodyA.GetTransformedOrientedRectangles();
        const std::vector<Circle2D>& transformedCirclesA = collisionBodyA.GetTransformedCircles();
        const std::vector<Capsule2D>& transformedCapsulesA = collisionBodyA.GetTransformedCapsules();
        const std::vector<Triangle2D>& transformedTrianglesA = collisionBodyA.GetTransformedTriangles();
        const std::vector<Polygon2D>& transformedPolygonsA = collisionBodyA.GetTransformedPolygons();
        const std::vector<ConvexPolygon2D>& transformedConvexPolygonsA = collisionBodyA.GetTransformedConvexPolygons();

        const std::vector<unsigned int>& transformedPointIdsA = collisionBodyA.GetTransformedPointIds();
        const std::vector<unsigned int>& transformedLineIdsA = collisionBodyA.GetTransformedLineIds();
        const std::vector<unsigned int>& transformedRayIdsA = collisionBodyA.GetTransformedRayIds();
        const std::vector<unsigned int>& transformedLineSegmentIdsA = collisionBodyA.GetTransformedLineSegmentIds();
        const std::vector<unsigned int>& transformedRectangleIdsA = collisionBodyA.GetTransformedRectangleIds();
        const std::vector<unsigned int>& transformedOrientedRectangleIdsA = collisionBodyA.GetTransformedOrientedRectangleIds();
        const std::vector<unsigned int>& transformedCircleIdsA = collisionBodyA.GetTransformedCircleIds();
        const std::vector<unsigned int>& transformedCapsuleIdsA = collisionBodyA.GetTransformedCapsuleIds();
        const std::vector<unsigned int>& transformedTriangleIdsA = collisionBodyA.GetTransformedTriangleIds();
        const std::vector<unsigned int>& transformedPolygonIdsA = collisionBodyA.GetTransformedPolygonIds();
        const std::vector<unsigned int>& transformedConvexPolygonIdsA = collisionBodyA.GetTransformedConvexPolygonIds();

        const std::vector<Point2D>& transformedPointsB = collisionBodyB.GetTransformedPoints();
        const std::vector<Line2D>& transformedLinesB = collisionBodyB.GetTransformedLines();
        const std::vector<Ray2D>& transformedRaysB = collisionBodyB.GetTransformedRays();
        const std::vector<LineSegment2D>& transformedLineSegmentsB = collisionBodyB.GetTransformedLineSegments();
        const std::vector<Rectangle2D>& transformedRectanglesB = collisionBodyB.GetTransformedRectangles();
        const std::vector<OrientedRectangle2D>& transformedOrientedRectanglesB = collisionBodyB.GetTransformedOrientedRectangles();
        const std::vector<Circle2D>& transformedCirclesB = collisionBodyB.GetTransformedCircles();
        const std::vector<Capsule2D>& transformedCapsulesB = collisionBodyB.GetTransformedCapsules();
        const std::vector<Triangle2D>& transformedTrianglesB = collisionBodyB.GetTransformedTriangles();
        const std::vector<Polygon2D>& transformedPolygonsB = collisionBodyB.GetTransformedPolygons();
        const std::vector<ConvexPolygon2D>& transformedConvexPolygonsB = collisionBodyB.GetTransformedConvexPolygons();

        const std::vector<unsigned int>& transformedPointIdsB = collisionBodyB.GetTransformedPointIds();
        const std::vector<unsigned int>& transformedLineIdsB = collisionBodyB.GetTransformedLineIds();
        const std::vector<unsigned int>& transformedRayIdsB = collisionBodyB.GetTransformedRayIds();
        const std::vector<unsigned int>& transformedLineSegmentIdsB = collisionBodyB.GetTransformedLineSegmentIds();
        const std::vector<unsigned int>& transformedRectangleIdsB = collisionBodyB.GetTransformedRectangleIds();
        const std::vector<unsigned int>& transformedOrientedRectangleIdsB = collisionBodyB.GetTransformedOrientedRectangleIds();
        const std::vector<unsigned int>& transformedCircleIdsB = collisionBodyB.GetTransformedCircleIds();
        const std::vector<unsigned int>& transformedCapsuleIdsB = collisionBodyB.GetTransformedCapsuleIds();
        const std::vector<unsigned int>& transformedTriangleIdsB = collisionBodyB.GetTransformedTriangleIds();
        const std::vector<unsigned int>& transformedPolygonIdsB = collisionBodyB.GetTransformedPolygonIds();
        const std::vector<unsigned int>& transformedConvexPolygonIdsB = collisionBodyB.GetTransformedConvexPolygonIds();

        CollisionData collisionA;
        collisionA.otherEntityId = entityIdB;

        CollisionData collisionB;
        collisionB.otherEntityId = entityIdA;

        // point A & point B ---------------------------------------------------
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointA = transformedPointsA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_Point_Point_Overlap(
                    pointA.position,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & line B
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointA = transformedPointsA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_Point_Line_Overlap(
                    pointA.position,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & ray B
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointA = transformedPointsA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_Point_Ray_Overlap(
                    pointA.position,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & lineSegment B
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointA = transformedPointsA[i];
            for (size_t j = 0; j < transformedLineSegmentsB.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsB[j];

                bool collisionFound = Check2D_Point_LineSegment_Overlap(
                    pointA.position,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & rectangle B
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointA = transformedPointsA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_Point_Rectangle_Overlap(
                    pointA.position,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & orientedRectangle B
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointA = transformedPointsA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& orientedRectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_Point_OrientedRectangle_Overlap(
                    pointA.position,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & circle B
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointA = transformedPointsA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_Point_Circle_Overlap(
                    pointA.position,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & capsule B
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointA = transformedPointsA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_Point_Capsule_Overlap(
                    pointA.position,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & triangle B
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointA = transformedPointsA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_Point_Triangle_Overlap(
                    pointA.position,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & polygon B
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointA = transformedPointsA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_Point_Polygon_Overlap(
                    pointA.position,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // point A & convexPolygon B
        for (size_t i = 0; i < transformedPointsA.size(); ++i)
        {
            const Point2D& pointB = transformedPointsA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_Point_ConvexPolygon_Overlap(
                    pointB.position,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPointIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & point B ----------------------------------------------------
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_Line_Point_Overlap(
                    lineA.position,
                    lineA.slope,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & line B
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_Line_Line_Overlap(
                    lineA.position,
                    lineA.slope,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & ray B
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_Line_Ray_Overlap(
                    lineA.position,
                    lineA.slope,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & lineSegment B
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedLineSegmentsB.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsB[j];

                bool collisionFound = Check2D_Line_LineSegment_Overlap(
                    lineA.position,
                    lineA.slope,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & rectangle B
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_Line_Rectangle_Overlap(
                    lineA.position,
                    lineA.slope,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & orientedRectangle B
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& rectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_Line_OrientedRectangle_Overlap(
                    lineA.position,
                    lineA.slope,
                    rectangleB.halfSize,
                    rectangleB.position,
                    rectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & circle B
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_Line_Circle_Overlap(
                    lineA.position,
                    lineA.slope,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & capsule B
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_Line_Capsule_Overlap(
                    lineA.position,
                    lineA.slope,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & triangle B
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_Line_Triangle_Overlap(
                    lineA.position,
                    lineA.slope,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & polygon B
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_Line_Polygon_Overlap(
                    lineA.position,
                    lineA.slope,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // line A & convexPolygon B
        for (size_t i = 0; i < transformedLinesA.size(); ++i)
        {
            const Line2D& lineA = transformedLinesA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_Line_ConvexPolygon_Overlap(
                    lineA.position,
                    lineA.slope,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & point B -----------------------------------------------------
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_Ray_Point_Overlap(
                    rayA.position,
                    rayA.direction,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & line B
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_Ray_Line_Overlap(
                    rayA.position,
                    rayA.direction,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & ray B
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_Ray_Ray_Overlap(
                    rayA.position,
                    rayA.direction,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & lineSegment B
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedLineSegmentsB.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsB[j];

                bool collisionFound = Check2D_Ray_LineSegment_Overlap(
                    rayA.position,
                    rayA.direction,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & rectangle B
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_Ray_Rectangle_Overlap(
                    rayA.position,
                    rayA.direction,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & orientedRectangle B
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& orientedRectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_Ray_OrientedRectangle_Overlap(
                    rayA.position,
                    rayA.direction,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & circle B
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_Ray_Circle_Overlap(
                    rayA.position,
                    rayA.direction,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & capsule B
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_Ray_Capsule_Overlap(
                    rayA.position,
                    rayA.direction,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & triangle B
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_Ray_Triangle_Overlap(
                    rayA.position,
                    rayA.direction,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & polygon B
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_Ray_Polygon_Overlap(
                    rayA.position,
                    rayA.direction,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // ray A & convexPolygon B
        for (size_t i = 0; i < transformedRaysA.size(); ++i)
        {
            const Ray2D& rayA = transformedRaysA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_Ray_ConvexPolygon_Overlap(
                    rayA.position,
                    rayA.direction,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRayIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & point B ---------------------------------------------
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_LineSegment_Point_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & line B
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_LineSegment_Line_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & ray B
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_LineSegment_Ray_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & lineSegment B
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedLineSegmentsB.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsB[j];

                bool collisionFound = Check2D_LineSegment_LineSegment_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & rectangle B
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_LineSegment_Rectangle_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & orientedRectangle B
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& orientedRectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_LineSegment_OrientedRectangle_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & circle B
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_LineSegment_Circle_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & capsule B
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_LineSegment_Capsule_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & triangle B
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_LineSegment_Triangle_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & polygon B
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_LineSegment_Polygon_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // lineSegment A & convexPolygon B
        for (size_t i = 0; i < transformedLineSegmentsA.size(); ++i)
        {
            const LineSegment2D& lineSegmentA = transformedLineSegmentsA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_LineSegment_ConvexPolygon_Overlap(
                    lineSegmentA.start,
                    lineSegmentA.end,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedLineSegmentIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & point B -----------------------------------------------
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_Rectangle_Point_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & line B
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_Rectangle_Line_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & ray B
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_Rectangle_Ray_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & lineSegment B
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedLineSegmentsA.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsA[j];

                bool collisionFound = Check2D_Rectangle_LineSegment_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & rectangle B
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_Rectangle_Rectangle_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & orientedRectangle B
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& orientedRectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_Rectangle_OrientedRectangle_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & circle B
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_Rectangle_Circle_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & capsule B
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_Rectangle_Capsule_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & triangle B
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_Rectangle_Triangle_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & polygon B
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_Rectangle_Polygon_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // rectangle A & convexPolygon B
        for (size_t i = 0; i < transformedRectanglesA.size(); ++i)
        {
            const Rectangle2D& rectangleA = transformedRectanglesA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_Rectangle_ConvexPolygon_Overlap(
                    rectangleA.bottomLeft,
                    rectangleA.topRight,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & point B ---------------------------------------
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_OrientedRectangle_Point_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & line B
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_OrientedRectangle_Line_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & ray B
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_OrientedRectangle_Ray_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & lineSegment B
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedLineSegmentsB.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsB[j];

                bool collisionFound = Check2D_OrientedRectangle_LineSegment_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & rectangle B
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_OrientedRectangle_Rectangle_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & orientedRectangle B
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& orientedRectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_OrientedRectangle_OrientedRectangle_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & circle B
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_OrientedRectangle_Circle_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & capsule B
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_OrientedRectangle_Capsule_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & triangle B
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_OrientedRectangle_Triangle_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & polygon B
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_OrientedRectangle_Polygon_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // orientedRectangle A & convexPolygon B
        for (size_t i = 0; i < transformedOrientedRectanglesA.size(); ++i)
        {
            const OrientedRectangle2D& orientedRectangleA = transformedOrientedRectanglesA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_OrientedRectangle_ConvexPolygon_Overlap(
                    orientedRectangleA.halfSize,
                    orientedRectangleA.position,
                    orientedRectangleA.rotation,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedOrientedRectangleIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & point B --------------------------------------------------
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_Circle_Point_Overlap(
                    circleA.position,
                    circleA.radius,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & line B
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_Circle_Line_Overlap(
                    circleA.position,
                    circleA.radius,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & ray B
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_Circle_Ray_Overlap(
                    circleA.position,
                    circleA.radius,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & lineSegment B
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedLineSegmentsB.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsB[j];

                bool collisionFound = Check2D_Circle_LineSegment_Overlap(
                    circleA.position,
                    circleA.radius,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & rectangle B
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_Circle_Rectangle_Overlap(
                    circleA.position,
                    circleA.radius,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & orientedRectangle B
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& orientedRectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_Circle_OrientedRectangle_Overlap(
                    circleA.position,
                    circleA.radius,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & circle B
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_Circle_Circle_Overlap(
                    circleA.position,
                    circleA.radius,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & capsule B
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_Circle_Capsule_Overlap(
                    circleA.position,
                    circleA.radius,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & triangle B
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_Circle_Triangle_Overlap(
                    circleA.position,
                    circleA.radius,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & polygon B
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_Circle_Polygon_Overlap(
                    circleA.position,
                    circleA.radius,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // circle A & convexPolygon B
        for (size_t i = 0; i < transformedCirclesA.size(); ++i)
        {
            const Circle2D& circleA = transformedCirclesA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_Circle_ConvexPolygon_Overlap(
                    circleA.position,
                    circleA.radius,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCircleIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & point B -------------------------------------------------
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_Capsule_Point_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & line B
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_Capsule_Line_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & ray B
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_Capsule_Ray_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & lineSegment B
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedLineSegmentsB.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsB[j];

                bool collisionFound = Check2D_Capsule_LineSegment_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule & other rectangle
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_Capsule_Rectangle_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & orientedRectangle B
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& orientedRectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_Capsule_OrientedRectangle_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & circle B
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_Capsule_Circle_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & capsule B
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_Capsule_Capsule_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & triangle B
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_Capsule_Triangle_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule & other polygon
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_Capsule_Polygon_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // capsule A & convexPolygon B
        for (size_t i = 0; i < transformedCapsulesA.size(); ++i)
        {
            const Capsule2D& capsuleA = transformedCapsulesA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_Capsule_ConvexPolygon_Overlap(
                    capsuleA.start,
                    capsuleA.end,
                    capsuleA.radius,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedCapsuleIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & point B ------------------------------------------------
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_Triangle_Point_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & line B
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_Triangle_Line_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & ray B
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_Triangle_Ray_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & lineSegment B
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedLineSegmentsB.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsB[j];

                bool collisionFound = Check2D_Triangle_LineSegment_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & rectangle B
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_Triangle_Rectangle_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & orientedRectangle B
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& orientedRectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_Triangle_OrientedRectangle_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & circle B
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_Triangle_Circle_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & capsule B
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_Triangle_Capsule_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & triangle B
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_Triangle_Triangle_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & polygon B
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_Triangle_Polygon_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // triangle A & convexPolygon B
        for (size_t i = 0; i < transformedTrianglesA.size(); ++i)
        {
            const Triangle2D& triangleA = transformedTrianglesA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_Triangle_ConvexPolygon_Overlap(
                    triangleA.corner1,
                    triangleA.corner2,
                    triangleA.corner3,
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedTriangleIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & point B -------------------------------------------------
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_Polygon_Point_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & line B
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_Polygon_Line_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & ray B
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_Polygon_Ray_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & lineSegment B
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedLineSegmentsB.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsB[j];

                bool collisionFound = Check2D_Polygon_LineSegment_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & rectangle B
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_Polygon_Rectangle_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & orientedRectangle B
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& orientedRectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_Polygon_OrientedRectangle_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & circle B
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_Polygon_Circle_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & capsule B
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_Polygon_Capsule_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & triangle B
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_Polygon_Triangle_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & polygon B
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_Polygon_Polygon_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // polygon A & convexPolygon B
        for (size_t i = 0; i < transformedPolygonsA.size(); ++i)
        {
            const Polygon2D& polygonA = transformedPolygonsA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_Polygon_ConvexPolygon_Overlap(
                    polygonA.positions.data(),
                    polygonA.positions.size(),
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedPolygonIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & point B -------------------------------------------
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedPointsB.size(); ++j)
            {
                const Point2D& pointB = transformedPointsB[j];

                bool collisionFound = Check2D_ConvexPolygon_Point_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    pointB.position);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedPointIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & line B
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedLinesB.size(); ++j)
            {
                const Line2D& lineB = transformedLinesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Line_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    lineB.position,
                    lineB.slope);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedLineIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & ray B
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedRaysB.size(); ++j)
            {
                const Ray2D& rayB = transformedRaysB[j];

                bool collisionFound = Check2D_ConvexPolygon_Ray_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    rayB.position,
                    rayB.direction);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedRayIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & lineSegment B
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedLineSegmentsB.size(); ++j)
            {
                const LineSegment2D& lineSegmentB = transformedLineSegmentsB[j];

                bool collisionFound = Check2D_ConvexPolygon_LineSegment_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    lineSegmentB.start,
                    lineSegmentB.end);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedLineSegmentIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & rectangle B
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedRectanglesB.size(); ++j)
            {
                const Rectangle2D& rectangleB = transformedRectanglesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Rectangle_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    rectangleB.bottomLeft,
                    rectangleB.topRight);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & orientedRectangle B
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedOrientedRectanglesB.size(); ++j)
            {
                const OrientedRectangle2D& orientedRectangleB = transformedOrientedRectanglesB[j];

                bool collisionFound = Check2D_ConvexPolygon_OrientedRectangle_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    orientedRectangleB.halfSize,
                    orientedRectangleB.position,
                    orientedRectangleB.rotation);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedOrientedRectangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & circle B
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCirclesB.size(); ++j)
            {
                const Circle2D& circleB = transformedCirclesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Circle_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    circleB.position,
                    circleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedCircleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & capsule B
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedCapsulesB.size(); ++j)
            {
                const Capsule2D& capsuleB = transformedCapsulesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Capsule_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    capsuleB.start,
                    capsuleB.end,
                    capsuleB.radius);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedCapsuleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & triangle B
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedTrianglesB.size(); ++j)
            {
                const Triangle2D& triangleB = transformedTrianglesB[j];

                bool collisionFound = Check2D_ConvexPolygon_Triangle_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    triangleB.corner1,
                    triangleB.corner2,
                    triangleB.corner3);

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedTriangleIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & polygon B
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedPolygonsB.size(); ++j)
            {
                const Polygon2D& polygonB = transformedPolygonsB[j];

                bool collisionFound = Check2D_ConvexPolygon_Polygon_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    polygonB.positions.data(),
                    polygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }

        // convexPolygon A & convexPolygon B
        for (size_t i = 0; i < transformedConvexPolygonsA.size(); ++i)
        {
            const ConvexPolygon2D& convexPolygonA = transformedConvexPolygonsA[i];
            for (size_t j = 0; j < transformedConvexPolygonsB.size(); ++j)
            {
                const ConvexPolygon2D& convexPolygonB = transformedConvexPolygonsB[j];

                bool collisionFound = Check2D_ConvexPolygon_ConvexPolygon_Overlap(
                    convexPolygonA.positions.data(),
                    convexPolygonA.positions.size(),
                    convexPolygonB.positions.data(),
                    convexPolygonB.positions.size());

                if (collisionFound)
                {
                    collisionA.myShapeId = transformedConvexPolygonIdsA[i];
                    collisionA.otherShapeId = transformedConvexPolygonIdsB[j];

                    collisionBodyA.AddCollision(collisionA);

                    if (recordInBodyB)
                    {
                        collisionB.myShapeId = collisionA.otherShapeId;
                        collisionB.otherShapeId = collisionA.myShapeId;

                        collisionBodyB.AddCollision(collisionB);
                    }
                }
            }
        }
    }

    std::vector<CollisionBody2D*> CollisionSystem2D::s_tangibleCollisionBodyPtrs_;
}
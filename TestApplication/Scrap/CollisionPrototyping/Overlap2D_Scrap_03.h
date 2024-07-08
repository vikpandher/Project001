#include "Math/MathUtilities.h"

// 
// Collision Point And Normal And Depth Functions:
//                | Rec | OrR | Cir | Cap | Tri | CoP |
// Rectangle      |  \  |  \  |     |  \  |  \  |  \  |
// O. Rectangle         |  \  |     |  \  |  \  |  \  |
// Circle                     |     |     |     |     |
// Capsule                          |  \  |  \  |  \  |
// Triangle                               |  \  |  \  |
// Convex Polygon                               |  \  |
// 
// Collision Point And Normal And Depth Function Notes:
// * collision normals point out from the first shape



namespace Project001
{
    // Rectangle Collision Point And Normal And Depth Functions ---------------

    bool Get2D_Rectangle_Rectangle_CollisionPointNormalDepth(
        const glm::vec2& rectangleA_bottomLeft,
        const glm::vec2& rectangleA_topRight,
        const glm::vec2& rectangleB_bottomLeft,
        const glm::vec2& rectangleB_topRight,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Rectangle_OrientedRectangle_CollisionPointNormalDepth(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Rectangle_Capsule_CollisionPointNormalDepth(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Rectangle_Triangle_CollisionPointNormalDepth(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Rectangle_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // Oriented Rectangle Collision Point And Normal And Depth Functions -------

    bool Get2D_OrientedRectangle_OrientedRectangle_CollisionPointNormalDepth(
        const glm::vec2& orientedRectangleA_halfSize,
        const glm::vec2& orientedRectangleA_position,
        const float& orientedRectangleA_rotation,
        const glm::vec2& orientedRectangleB_halfSize,
        const glm::vec2& orientedRectangleB_position,
        const float& orientedRectangleB_rotation,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_OrientedRectangle_Capsule_CollisionPointNormalDepth(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_OrientedRectangle_Triangle_CollisionPointNormalDepth(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_OrientedRectangle_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // Capsule Collision Point And Normal And Depth Functions ------------------

    bool Get2D_Capsule_Capsule_CollisionPointNormalDepth(
        const glm::vec2& capsuleA_start,
        const glm::vec2& capsuleA_end,
        const float& capsuleA_radius,
        const glm::vec2& capsuleB_start,
        const glm::vec2& capsuleB_end,
        const float& capsuleB_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Capsule_Triangle_CollisionPointNormalDepth(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Capsule_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // Triangle Collision Point And Normal And Depth Functions -----------------

    bool Get2D_Triangle_Triangle_CollisionPointNormalDepth(
        const glm::vec2& triangleA_corner1,
        const glm::vec2& triangleA_corner2,
        const glm::vec2& triangleA_corner3,
        const glm::vec2& triangleB_corner1,
        const glm::vec2& triangleB_corner2,
        const glm::vec2& triangleB_corner3,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Triangle_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // ConvexPolygon Collision Point And Normal And Depth Functions ------------

    bool Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // -------------------------------------------------------------------------

    // If an axis is (0.0f, 0.0f) then the max and min will be 0.0f.
    void ProjectPolygonOntoAxis_2(
        const glm::vec2& axis,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        float& max,
        float& min,
        size_t& maxIndex,
        size_t& minIndex);

    bool Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H1(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H2(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolyhonA_axes,
        const size_t& convexPolygonA_axisCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H3(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolyhonA_axes,
        const size_t& convexPolygonA_axisCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        const glm::vec2* const& convexPolyhonB_axes,
        const size_t& convexPolygonB_axisCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    void ConvexPolygon_ConvexPolygon_FixNormalAndSelectCollisionPoint_H(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        const bool& projection_max_is_from_polygonA,
        const bool& projection_min_is_from_polygonA,
        const size_t& projection_max_index,
        const size_t& projection_min_index,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal);
}

#include "Overlap2D_Scrap_03.inl"
#pragma once

#include "Math/FloatComparators.h"
#include "Math/VectorUtilities.h"



// Collision Point And Normal Functions: (TODO)
// * These assume the shapes are overlapping
//                      | Lin | Ray | LiS | Rec | OrR | Cir | Cap | Tri | Pol |
// Line                                   |  \  |  \  |  \  |     |     |     |
// Ray                                    |  \  |  \  |  \  |     |     |     |
// LineSegment                            |  \  |  \  |  \  |     |     |     |
// Rectangle            | --- | --- | --- |  \  |  \  |  \  |     |     |     |
// O. Rectangle         | --- | --- | --- | --- |  \  |  \  |     |     |     |
// Circle               | --- | --- | --- | --- | --- |  \  |     |     |     |
// Capsule              |     |     |     |     |     |     |     |     |     |
// Triangle             |     |     |     |     |     |     |     |     |     |
// Polygon              |     |     |     |     |     |     |     |     |     | 45

// Get Line Collision Point And Normal Functions ---------------------------

namespace Project001
{
    void Get2D_Line_Rectangle_CollisionPointAndNormal(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_Line_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_Line_Circle_CollisionPointAndNormal(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // Get Ray Collision Point And Normal Functions ----------------------------

    // ray_direction needs to be a unit vector
    void Get2D_Ray_Rectangle_CollisionPointAndNormal(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // ray_direction needs to be a unit vector
    void Get2D_Ray_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // ray_direction needs to be a unit vector
    void Get2D_Ray_Circle_CollisionPointAndNormal(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // Get LineSegment Collision Point And Normal Functions --------------------

    void Get2D_LineSegment_Rectangle_CollisionPointAndNormal(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_LineSegment_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_LineSegment_Circle_CollisionPointAndNormal(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // Get Rectangle Collision Point And Normal Functions ----------------------

    void Get2D_Rectangle_Line_CollisionPointAndNormal(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // ray_direction needs to be a unit vector
    void Get2D_Rectangle_Ray_CollisionPointAndNormal(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_Rectangle_LineSegment_CollisionPointAndNormal(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_Rectangle_Rectangle_CollisionPointAndNormal(
        const glm::vec2& rectangleA_bottomLeft,
        const glm::vec2& rectangleA_topRight,
        const glm::vec2& rectangleB_bottomLeft,
        const glm::vec2& rectangleB_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_Rectangle_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_Rectangle_Circle_CollisionPointAndNormal(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // Get OrientedRectangle Collision Point And Normal Functions --------------

    void Get2D_OrientedRectangle_Line_CollisionPointAndNormal(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_OrientedRectangle_Ray_CollisionPointAndNormal(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_OrientedRectangle_LineSegment_CollisionPointAndNormal(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_OrientedRectangle_Rectangle_CollisionPointAndNormal(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_OrientedRectangle_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& orientedRectangleA_halfSize,
        const glm::vec2& orientedRectangleA_position,
        const float& orientedRectangleA_rotation,
        const glm::vec2& orientedRectangleB_halfSize,
        const glm::vec2& orientedRectangleB_position,
        const float& orientedRectangleB_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_OrientedRectangle_Circle_CollisionPointAndNormal(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // Get Circle Collision Point And Normal Functions -------------------------

    void Get2D_Circle_Line_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // ray_direction needs to be a unit vector
    void Get2D_Circle_Ray_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_Circle_LineSegment_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_Circle_Rectangle_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_Circle_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    void Get2D_Circle_Circle_CollisionPointAndNormal(
        const glm::vec2& circleA_position,
        const float& circleA_radius,
        const glm::vec2& circleB_position,
        const float& circleB_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);
}

#include "Overlap2D_Scrap.inl"
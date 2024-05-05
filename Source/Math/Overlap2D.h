#pragma once

#include "Math/FloatComparators.h"
#include "Math/VectorUtilities.h"



// 2D Shape List ---------------------------------------------------------------
// * Point
// * Line
// * Ray
// * LineSegment
// * Rectangle
// * OrientedRectangle
// * Circle
// * Capsule
// * Triangle
// * Polygon
// * Convex Polygon
// 
// Overlap Functions:
//                | Poi | Lin | Ray | LiS | Rec | OrR | Cir | Cap | Tri | Pol | CoP |
// Point          |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// Line           | --- |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// Ray            | --- | --- |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// LineSegment    | --- | --- | --- |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// Rectangle      | --- | --- | --- | --- |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// O. Rectangle   | --- | --- | --- | --- | --- |  \  |  \  |  \  |  \  |  \  |  \  |
// Circle         | --- | --- | --- | --- | --- | --- |  \  |  \  |  \  |  \  |  \  |
// Capsule        | --- | --- | --- | --- | --- | --- | --- |  \  |  \  |  \  |  \  |
// Triangle       | --- | --- | --- | --- | --- | --- | --- | --- |  \  |  \  |  \  |
// Polygon        | --- | --- | --- | --- | --- | --- | --- | --- | --- |  \  |  \  |
// Convex Polygon | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |  \  |
// 
// Closest Point Functions:
//                      | Lin | Ray | LiS | Rec | OrR | Cir | Cap |
// Point                |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// 
// Distance Squared Functions:
//                | Poi | Lin | Ray | LiS |
// Point          |  \  |  \  |  \  |  \  |
// 
// Distance Functions:
//                                                    | Cir | Cap |
// Point                                              |  \  |  \  |
// 
// Closest Point And Distance Squared Functions:
//                      | Lin | Ray | LiS | Rec | OrR |           | Tri | Pol |
// Point                |  \  |  \  |  \  |  \  |  \  |           |  \  |  \  |
// 
// Collision Point And Normal Functions: (TODO)
// * These assume the shapes are overlapping
//                      | Lin | Ray | LiS | Rec | OrR | Cir | Cap | Tri | Pol |
// Line                                   |  \  |  \  |  \  |     |     |     |
// Ray                                    |  \  |  \  |     |     |     |     |
// LineSegment                            |  \  |  \  |     |     |     |     |
// Rectangle            | --- | --- | --- |  \  |  \  |     |     |     |     |
// O. Rectangle         | --- | --- | --- | --- |  \  |     |     |     |     |
// Circle               | --- |     |     |     |     |     |     |     |     |
// Capsule              |     |     |     |     |     |     |     |     |     |
// Triangle             |     |     |     |     |     |     |     |     |     |
// Polygon              |     |     |     |     |     |     |     |     |     | 54
// 
// Intersection Functions:
//                      | Lin |
// Line                 |  \  |
// 
// NOTE: Positive rotations are counter-clockwise

namespace Project001
{
    // Checking Point overlap --------------------------------------------------

    bool Check2D_Point_Point_Overlap(
        const glm::vec2& pointA_position,
        const glm::vec2& pointB_position);

    bool Check2D_Point_Line_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope);

    // ray_direction needs to be a unit vector
    bool Check2D_Point_Ray_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    bool Check2D_Point_LineSegment_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    bool Check2D_Point_Rectangle_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    bool Check2D_Point_OrientedRectangle_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    bool Check2D_Point_Circle_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& circle_position,
        const float& circle_radius);

    bool Check2D_Point_Capsule_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    bool Check2D_Point_Triangle_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    bool Check2D_Point_Polygon_Overlap(
        const glm::vec2& point_position,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    bool Check2D_Point_ConvexPolygon_Overlap(
        const glm::vec2& point_position,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Checking Line overlap ---------------------------------------------------

    bool Check2D_Line_Point_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& point_position);

    // one slope of negative infinity and the other of infinity will falsely
    // indicate a overlap
    bool Check2D_Line_Line_Overlap(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope);

    // ray_direction needs to be a unit vector
    bool Check2D_Line_Ray_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    bool Check2D_Line_LineSegment_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    bool Check2D_Line_Rectangle_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    bool Check2D_Line_OrientedRectangle_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    bool Check2D_Line_Circle_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& circle_position,
        const float& circle_radius);

    bool Check2D_Line_Capsule_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    bool Check2D_Line_Triangle_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    bool Check2D_Line_Polygon_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    bool Check2D_Line_ConvexPolygon_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Checking Ray overlap ----------------------------------------------------

    // ray_direction needs to be a unit vector
    bool Check2D_Ray_Point_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& point_position);

    // ray_direction needs to be a unit vector
    bool Check2D_Ray_Line_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& line_position,
        const float& line_slope);

    // rayA_direction and rayB_direction need to be a unit vectors
    bool Check2D_Ray_Ray_Overlap(
        const glm::vec2& rayA_position,
        const glm::vec2& rayA_direction,
        const glm::vec2& rayB_position,
        const glm::vec2& rayB_direction);

    // ray_direction needs to be a unit vector
    bool Check2D_Ray_LineSegment_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    // ray_direction needs to be a unit vector
    bool Check2D_Ray_Rectangle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    // ray_direction needs to be a unit vector
    bool Check2D_Ray_OrientedRectangle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    // ray_direction needs to be a unit vector
    bool Check2D_Ray_Circle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& circle_position,
        const float& circle_radius);

    // ray_direction needs to be a unit vector
    bool Check2D_Ray_Capsule_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    // ray_direction needs to be a unit vector
    bool Check2D_Ray_Triangle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    // ray_direction needs to be a unit vector
    bool Check2D_Ray_Polygon_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    // ray_direction needs to be a unit vector
    bool Check2D_Ray_ConvexPolygon_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Checking LineSegment overlap --------------------------------------------

    bool Check2D_LineSegment_Point_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& point_position);

    bool Check2D_LineSegment_Line_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& line_position,
        const float& line_slope);

    // ray_direction needs to be a unit vector
    bool Check2D_LineSegment_Ray_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    bool Check2D_LineSegment_LineSegment_Overlap(
        const glm::vec2& lineSegmentA_start,
        const glm::vec2& lineSegmentA_end,
        const glm::vec2& lineSegmentB_start,
        const glm::vec2& lineSegmentB_end);

    bool Check2D_LineSegment_Rectangle_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    bool Check2D_LineSegment_OrientedRectangle_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    bool Check2D_LineSegment_Circle_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& circle_position,
        const float& circle_radius);

    bool Check2D_LineSegment_Capsule_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    bool Check2D_LineSegment_Triangle_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    bool Check2D_LineSegment_Polygon_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    bool Check2D_LineSegment_ConvexPolygon_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Checking Rectangle overlap ----------------------------------------------

    bool Check2D_Rectangle_Point_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& point_position);

    bool Check2D_Rectangle_Line_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& line_position,
        const float& line_slope);

    // ray_direction needs to be a unit vector
    bool Check2D_Rectangle_Ray_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    bool Check2D_Rectangle_LineSegment_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    bool Check2D_Rectangle_Rectangle_Overlap(
        const glm::vec2& rectangleA_bottomLeft,
        const glm::vec2& rectangleA_topRight,
        const glm::vec2& rectangleB_bottomLeft,
        const glm::vec2& rectangleB_topRight);

    bool Check2D_Rectangle_OrientedRectangle_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    bool Check2D_Rectangle_Circle_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& circle_position,
        const float& circle_radius);

    bool Check2D_Rectangle_Capsule_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    bool Check2D_Rectangle_Triangle_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    bool Check2D_Rectangle_Polygon_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    bool Check2D_Rectangle_ConvexPolygon_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Checking OrientedRectangle overlap --------------------------------------

    bool Check2D_OrientedRectangle_Point_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& point_position);

    bool Check2D_OrientedRectangle_Line_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& line_position,
        const float& line_slope);

    // ray_direction needs to be a unit vector
    bool Check2D_OrientedRectangle_Ray_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    bool Check2D_OrientedRectangle_LineSegment_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    bool Check2D_OrientedRectangle_Rectangle_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    bool Check2D_OrientedRectangle_OrientedRectangle_Overlap(
        const glm::vec2& orientedRectangleA_halfSize,
        const glm::vec2& orientedRectangleA_position,
        const float& orientedRectangleA_rotation,
        const glm::vec2& orientedRectangleB_halfSize,
        const glm::vec2& orientedRectangleB_position,
        const float& orientedRectangleB_rotation);

    bool Check2D_OrientedRectangle_Circle_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& circle_position,
        const float& circle_radius);

    bool Check2D_OrientedRectangle_Capsule_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    bool Check2D_OrientedRectangle_Triangle_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    bool Check2D_OrientedRectangle_Polygon_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    bool Check2D_OrientedRectangle_ConvexPolygon_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Checking Circle overlap -------------------------------------------------

    bool Check2D_Circle_Point_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& point_position);

    bool Check2D_Circle_Line_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& line_position,
        const float& line_slope);

    // ray_direction needs to be a unit vector
    bool Check2D_Circle_Ray_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    bool Check2D_Circle_LineSegment_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    bool Check2D_Circle_Rectangle_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    bool Check2D_Circle_OrientedRectangle_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    bool Check2D_Circle_Circle_Overlap(
        const glm::vec2& circleA_position,
        const float& circleA_radius,
        const glm::vec2& circleB_position,
        const float& circleB_radius);

    bool Check2D_Circle_Capsule_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    bool Check2D_Circle_Triangle_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    bool Check2D_Circle_Polygon_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    bool Check2D_Circle_ConvexPolygon_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Checking Capsule overlap ------------------------------------------------

    bool Check2D_Capsule_Point_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& point_position);

    bool Check2D_Capsule_Line_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& line_position,
        const float& line_slope);

    // ray_direction needs to be a unit vector
    bool Check2D_Capsule_Ray_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    bool Check2D_Capsule_LineSegment_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    bool Check2D_Capsule_Rectangle_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    bool Check2D_Capsule_OrientedRectangle_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    bool Check2D_Capsule_Circle_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& circle_position,
        const float& circle_radius);

    bool Check2D_Capsule_Capsule_Overlap(
        const glm::vec2& capsuleA_start,
        const glm::vec2& capsuleA_end,
        const float& capsuleA_radius,
        const glm::vec2& capsuleB_start,
        const glm::vec2& capsuleB_end,
        const float& capsuleB_radius);

    bool Check2D_Capsule_Triangle_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    bool Check2D_Capsule_Polygon_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    bool Check2D_Capsule_ConvexPolygon_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Checking Triangle overlap -----------------------------------------------

    bool Check2D_Triangle_Point_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& point_position);

    bool Check2D_Triangle_Line_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& line_position,
        const float& line_slope);

    // ray_direction needs to be a unit vector
    bool Check2D_Triangle_Ray_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    bool Check2D_Triangle_LineSegment_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    bool Check2D_Triangle_Rectangle_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    bool Check2D_Triangle_OrientedRectangle_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    bool Check2D_Triangle_Circle_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& circle_position,
        const float& circle_radius);

    bool Check2D_Triangle_Capsule_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    bool Check2D_Triangle_Triangle_Overlap(
        const glm::vec2& triangleA_corner1,
        const glm::vec2& triangleA_corner2,
        const glm::vec2& triangleA_corner3,
        const glm::vec2& triangleB_corner1,
        const glm::vec2& triangleB_corner2,
        const glm::vec2& triangleB_corner3);

    bool Check2D_Triangle_Polygon_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    bool Check2D_Triangle_ConvexPolygon_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Checking Polygon overlap ------------------------------------------------

    bool Check2D_Polygon_Point_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& point_position);

    bool Check2D_Polygon_Line_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& line_position,
        const float& line_slope);

    // ray_direction needs to be a unit vector
    bool Check2D_Polygon_Ray_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    bool Check2D_Polygon_LineSegment_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    bool Check2D_Polygon_Rectangle_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    bool Check2D_Polygon_OrientedRectangle_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    bool Check2D_Polygon_Circle_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& circle_position,
        const float& circle_radius);

    bool Check2D_Polygon_Capsule_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    bool Check2D_Polygon_Triangle_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    bool Check2D_Polygon_Polygon_Overlap(
        const glm::vec2* const& polygonA_corners,
        const size_t& polygonA_cornerCount,
        const glm::vec2* const& polygonB_corners,
        const size_t& polygonB_cornerCount);

    bool Check2D_Polygon_ConvexPolygon_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Checking Convex Polygon overlap -----------------------------------------

    bool Check2D_ConvexPolygon_Point_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& point_position);

    bool Check2D_ConvexPolygon_Line_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& line_position,
        const float& line_slope);

    // ray_direction needs to be a unit vector
    bool Check2D_ConvexPolygon_Ray_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    bool Check2D_ConvexPolygon_LineSegment_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    bool Check2D_ConvexPolygon_Rectangle_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    bool Check2D_ConvexPolygon_OrientedRectangle_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    bool Check2D_ConvexPolygon_Circle_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& circle_position,
        const float& circle_radius);

    bool Check2D_ConvexPolygon_Capsule_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    bool Check2D_ConvexPolygon_Triangle_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    bool Check2D_ConvexPolygon_Polygon_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    bool Check2D_ConvexPolygon_ConvexPolygon_Overlap(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount);

    // Closest Point Functions -------------------------------------------------

    void Get2D_Point_Line_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& closestPoint_position);

    void Get2D_Point_Ray_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        glm::vec2& closestPoint_position);

    void Get2D_Point_LineSegment_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        glm::vec2& closestPoint_position);

    void Get2D_Point_Rectangle_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& closestPoint_position);

    void Get2D_Point_OrientedRectangle_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& closestPoint_position);

    void Get2D_Point_Circle_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& closestPoint_position);

    void Get2D_Point_Capsule_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& closestPoint_position);

    // Distance Squared Functions ----------------------------------------------

    float Get2D_Point_Point_DistanceSquared(
        const glm::vec2& pointA_position,
        const glm::vec2& pointB_position);

    float Get2D_Point_Line_DistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope);

    float Get2D_Point_Ray_DistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction);

    float Get2D_Point_LineSegment_DistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    // Distance Functions ------------------------------------------------------

    float Get2D_Point_Circle_Distance(
        const glm::vec2& point_position,
        const glm::vec2& circle_position,
        const float& circle_radius);

    float Get2D_Point_Capsule_Distance(
        const glm::vec2& point_position,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    // Closest Point And Distance Squared Functions ----------------------------

    float Get2D_Point_Line_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& closestPoint_position);

    float Get2D_Point_Ray_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        glm::vec2& closestPoint_position);

    float Get2D_Point_LineSegment_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        glm::vec2& closestPoint_position);

    float Get2D_Point_Rectangle_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& closestPoint_position);

    float Get2D_Point_OrientedRectangle_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& closestPoint_position);

    float Get2D_Point_Triangle_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        glm::vec2& closestPoint_position);

    float Get2D_Point_Polygon_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        glm::vec2& closestPoint_position);

    // Get Line Collision Point And Normal Functions ---------------------------

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

    // Get Circle Collision Point And Normal Functions -------------------------

    void Get2D_Circle_Line_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // Intersection Functions --------------------------------------------------

    bool Get2D_Line_Line_Intersection(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope,
        glm::vec2& intersection_position);

    // Helper Functions --------------------------------------------------------

    // Unused
    float Get2D_Point_Line_DistanceSquared_Alt(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope);

    // This version doesn't care which 2 cornors AS LONG AS they are opposite
    bool Check2D_Point_Rectangle_NoOverlap_H(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_oppositeCorner1,
        const glm::vec2& rectangle_oppositeCorner2);

    // This version doesn't care which 2 cornors AS LONG AS they are opposite
    bool Check2D_Point_Rectangle_Overlap_H(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_oppositeCorner1,
        const glm::vec2& rectangle_oppositeCorner2);

    // Unused
    bool Check2D_Point_Triangle_Overlap_Alt(
        const glm::vec2& point_position,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    // If lineA_slope == lineB_slope, intersection_position == (nan, nan)
    void Get2D_Line_Line_Intersection_H(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope,
        glm::vec2& intersection_position);

    unsigned int Get2D_Line_Circle_IntersectionDirectionScalars(
        const glm::vec2& line_position,
        const glm::vec2& line_direction,
        const glm::vec2& circle_position,
        const float& circle_radius,
        float& intersection_directionScalar1,
        float& intersection_directionScalar2);

    // Unused
    // Doesn't handle a slope of infinity
    unsigned int Get2D_Line_Circle_Intersections_Alt(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& intersection_position1,
        glm::vec2& intersection_position2);

    // This version assumes that the ray_position is inside the rectangle
    void Get2D_Ray_Rectangle_CollisionPointAndNormal_H(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal);

    // Unused
    bool Check2D_Rectangle_Rectangle_Overlap_Alt(
        const glm::vec2& rectangleA_bottomLeft,
        const glm::vec2& rectangleA_topRight,
        const glm::vec2& rectangleB_bottomLeft,
        const glm::vec2& rectangleB_topRight);

    // Unused
    bool Check2D_Rectangle_Circle_Overlap_Alt(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& circle_position,
        const float& circle_radius);

    // This version doesn't check if the line segment is inside the rectangle
    // it only cares it if collides with the sides constructing it
    bool Check2D_RectangleFrame_LineSegment_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    // This version doesn't check if the circle is inside the rectangle
    // it only cares it if collides with the sides constructing it
    bool Check2D_RectangleFrame_Circle_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& circle_position,
        const float& circle_radius);

    float Get2D_Slope(const glm::vec2& start, const glm::vec2& end);

    // returns 0 if colinear
    // returns 1 if clockwise
    // returns 2 if counter-clockwise
    int Get2D_Triangle_ApparentFacingDirection(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    float Get2D_Triangle_Area(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    // Preforms have of the seperate axis theorm check, using perpendiculars
    // from only convexPolygonA to generate and check seperation axis. Returns
    // false if a axis of seperation has been found.
    bool Check2D_ConvexPolygon_ConvexPolygon_HalfSeparatedAxisTheorem(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount);

    // Unused
    float RotateSlope(float slope, float rotationInRadians);

    glm::vec2 Get2D_DirectionFromSlope(float slope);

    float Get2D_SlopeFromDirection(const glm::vec2& direction);

    float FastInverseSquareRoot(float number);
}

#include "Overlap2D.inl"
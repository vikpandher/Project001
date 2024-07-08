#pragma once

#include "Math/MathUtilities.h"



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
// Point                                  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// Line                 |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// Ray                  | --- |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// LineSegment          | --- | --- |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// Rectangle      | --- | --- | --- | --- |  \  |  \  |  \  |  \  |  \  |  \  |  \  |
// O. Rectangle   | --- | --- | --- | --- | --- |  \  |  \  |  \  |  \  |  \  |  \  |
// Circle         | --- | --- | --- | --- | --- | --- |  \  |  \  |  \  |  \  |  \  |
// Capsule        | --- | --- | --- | --- | --- | --- | --- |  \  |  \  |  \  |  \  |
// Triangle       | --- | --- | --- | --- | --- | --- | --- | --- |  \  |  \  |  \  |
// Polygon        | --- | --- | --- | --- | --- | --- | --- | --- | --- |  \  |  \  |
// Convex Polygon | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |  \  |
// 
// Overlap Function Notes:
// * touching edges or corners don't count as an overlap
// * ray_direction needs to be a unit vector
// * it may be possible for a ray to pass from one corner to another corner of
//   some shapes without colliding, (thread the needle) but it's unlikely
// * polygon shapes without at least 3 corners will not overlap with anything
// * polygon shapes with 2 or more corners at the same point will give stange
//   results
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
// Collision Point And Normal And Depth Functions:
//                | Rec | OrR | Cir | Cap | Tri | CoP |
// Rectangle      |  \  |  \  |  \  |  \  |  \  |  \  |
// O. Rectangle   | --- |  \  |  \  |  \  |  \  |  \  |
// Circle         | --- | --- |  \  |  \  |  \  |  \  |
// Capsule        | --- | --- | --- |  \  |  \  |  \  |
// Triangle       | --- | --- | --- | --- |  \  |  \  |
// Convex Polygon | --- | --- | --- | --- | --- |  \  |
// 
// Collision Point And Normal And Depth Function Notes:
// * collision normals point out from the first shape
// 
// Area Functions:
// Rectangle      |  \  |
// Circle         |  \  |
// Capsule        |  \  |
// Triangle       |  \  |
// Convex Polygon |  \  |
// 
// Moment Of Inertia Functions:
// Rectangle      |  \  |
// Circle         |  \  |
// Capsule        |  \  |
// Triangle       |  \  |
// Convex Polygon |  \  |
// 
// Moment Of Inertia Function Notes:
// * the momemt of inertia is calculated assuming the center of mass is at
// (0.0f, 0.0f), even if the shape is offset.
// 
// Intersection Functions:
//                      | Lin |
// Line                 |  \  |
// 
// NOTE: Positive rotations are counter-clockwise

namespace Project001
{
    // Overlap Functions (Point) -----------------------------------------------

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

    // Overlap Functions (Line) ------------------------------------------------

    bool Check2D_Line_Line_Overlap(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope);

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

    // Overlap Functions (Ray) -------------------------------------------------

    bool Check2D_Ray_Line_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& line_position,
        const float& line_slope);

    bool Check2D_Ray_Ray_Overlap(
        const glm::vec2& rayA_position,
        const glm::vec2& rayA_direction,
        const glm::vec2& rayB_position,
        const glm::vec2& rayB_direction);

    bool Check2D_Ray_LineSegment_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    bool Check2D_Ray_Rectangle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    bool Check2D_Ray_OrientedRectangle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    bool Check2D_Ray_Circle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& circle_position,
        const float& circle_radius);

    bool Check2D_Ray_Capsule_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    bool Check2D_Ray_Triangle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    bool Check2D_Ray_Polygon_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    bool Check2D_Ray_ConvexPolygon_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Overlap Functions (LineSegment) -----------------------------------------

    bool Check2D_LineSegment_Line_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& line_position,
        const float& line_slope);

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

    // Overlap Functions (Rectangle) -------------------------------------------

    bool Check2D_Rectangle_Point_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& point_position);

    bool Check2D_Rectangle_Line_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& line_position,
        const float& line_slope);

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

    // Overlap Functions (OrientedRectangle) -----------------------------------

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

    // Overlap Functions (Circle) ----------------------------------------------

    bool Check2D_Circle_Point_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& point_position);

    bool Check2D_Circle_Line_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& line_position,
        const float& line_slope);

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

    // Overlap Functions (Capsule) ---------------------------------------------

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

    // Overlap Functions (Triangle) --------------------------------------------

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

    // Overlap Functions (Polygon) ---------------------------------------------

    bool Check2D_Polygon_Point_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& point_position);

    bool Check2D_Polygon_Line_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& line_position,
        const float& line_slope);

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

    // Overlap Functions (ConvexPolygon) ---------------------------------------

    bool Check2D_ConvexPolygon_Point_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& point_position);

    bool Check2D_ConvexPolygon_Line_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& line_position,
        const float& line_slope);

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

    bool Get2D_Rectangle_Circle_CollisionPointNormalDepth(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& circle_position,
        const float& circle_radius,
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

    bool Get2D_OrientedRectangle_Rectangle_CollisionPointNormalDepth(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

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

    bool Get2D_OrientedRectangle_Circle_CollisionPointNormalDepth(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& circle_position,
        const float& circle_radius,
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

    // Circle Collision Point And Normal And Depth Functions -------------------

    bool Get2D_Circle_Rectangle_CollisionPointNormalDepth(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Circle_OrientedRectangle_CollisionPointNormalDepth(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Circle_Circle_CollisionPointNormalDepth(
        const glm::vec2& circleA_position,
        const float& circleA_radius,
        const glm::vec2& circleB_position,
        const float& circleB_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Circle_Capsule_CollisionPointNormalDepth(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Circle_Triangle_CollisionPointNormalDepth(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Circle_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // Capsule Collision Point And Normal And Depth Functions ------------------

    bool Get2D_Capsule_Rectangle_CollisionPointNormalDepth(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Capsule_OrientedRectangle_CollisionPointNormalDepth(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Capsule_Circle_CollisionPointNormalDepth(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

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

    bool Get2D_Triangle_Rectangle_CollisionPointNormalDepth(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Triangle_OrientedRectangle_CollisionPointNormalDepth(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Triangle_Circle_CollisionPointNormalDepth(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_Triangle_Capsule_CollisionPointNormalDepth(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

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

    bool Get2D_ConvexPolygon_Rectangle_CollisionPointNormalDepth(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_ConvexPolygon_OrientedRectangle_CollisionPointNormalDepth(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_ConvexPolygon_Circle_CollisionPointNormalDepth(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_ConvexPolygon_Capsule_CollisionPointNormalDepth(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_ConvexPolygon_Triangle_CollisionPointNormalDepth(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    bool Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // Area Functions ----------------------------------------------------------

    float Get2D_Rectangle_Area(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    float Get2D_Rectangle_Area_2(
        const glm::vec2& rectangle_halfSize);

    float Get2D_Circle_Area(
        const float& circle_radius);

    float Get2D_Capsule_Area(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    float Get2D_Triangle_Area(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    float Get2D_ConvexPolygon_Area(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Moment Of Inertia Functions ---------------------------------------------

    float Get2D_Rectangle_MomentOfInertia(
        const float& rectangle_mass,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight);

    float Get2D_Rectangle_MomentOfInertia_2(
        const float& rectangle_mass,
        const glm::vec2& rectangle_position,
        const glm::vec2& rectangle_halfSize);

    float Get2D_Circle_MomentOfInertia(
        const float& circle_mass,
        const glm::vec2& circle_position,
        const float& circle_radius);

    float Get2D_Capsule_MomentOfInertia(
        const float& capsule_mass,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius);

    float Get2D_Triangle_MomentOfInertia(
        const float& triangle_mass,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    float Get2D_ConvexPolygon_MomentOfInertia(
        const float& convexPolygon_area,
        const float& convexPolygon_mass,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount);

    // Intersection Functions --------------------------------------------------

    bool Get2D_Line_Line_Intersection(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope,
        glm::vec2& intersection_position);

    // Helper Functions --------------------------------------------------------

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

    // Unused
    bool Check2D_Rectangle_OrientedRectangle_Overlap_Alt(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation);

    // Unused
    bool Check2D_Rectangle_Triangle_Overlap_Alt(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    // Unused
    bool Check2D_Circle_Triangle_Overlap_Alt(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    // Unused
    bool Check2D_Triangle_Triangle_Overlap_Alt(
        const glm::vec2& triangleA_corner1,
        const glm::vec2& triangleA_corner2,
        const glm::vec2& triangleA_corner3,
        const glm::vec2& triangleB_corner1,
        const glm::vec2& triangleB_corner2,
        const glm::vec2& triangleB_corner3);

    // Unused
    // line_direction doesn't need to be a unit vector
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

    // This version doesn't check if the line segment is inside the rectangle
    // it only cares it if collides with the sides constructing it
    bool Check2D_RectangleFrame_LineSegment_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

    // returns 0 if colinear
    // returns 1 if clockwise
    // returns 2 if counter-clockwise
    int Get2D_Triangle_ApparentFacingDirection(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3);

    // Preforms half of the seperate axis theorm check.
    // Projects the corners of both convexPolygonA and convexPolygonB onto
    // the perpendiculars from convexPolygonA to check for a seperating axis.
    // Returns false if an axis of seperation has been found.
    bool Check2D_ConvexPolygon_ConvexPolygon_HalfSeparateAxisTheorem(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount);

    // If an axis is (0.0f, 0.0f) then the max and min will be 0.0f.
    void ProjectPolygonOntoAxis(
        const glm::vec2& axis,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        float& max,
        float& min);

    // If an axis is (0.0f, 0.0f) then the max and min will be 0.0f.
    void ProjectPolygonOntoAxis_2(
        const glm::vec2& axis,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        float& max,
        float& min,
        size_t& maxIndex,
        size_t& minIndex);

    // axis vector needs to be a unit vector
    void ProjectCircleOntoAxis(
        const glm::vec2& axis,
        const glm::vec2& circle_position,
        const float& circle_radius,
        float& max,
        float& min);

    // axis vector needs to be a unit vector
    void ProjectCapsuleOntoAxis(
        const glm::vec2& axis,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        float& max,
        float& min);

    glm::vec2 GetPolygonCentroid(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount);

    float Get2D_Point_PolygonEdge_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        glm::vec2& closestPoint_position);

    bool Get2D_Circle_ConvexPolygon_CollisionPointNormalDepth_H1(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // normal doesn't always point out of convexPolygonA 
    bool Get2D_ConvexPolygon_Capsule_CollisionNormalDepth_H1(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // normal doesn't always point out of convexPolygonA 
    bool Get2D_ConvexPolygon_Capsule_CollisionNormalDepth_H2(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2* const& convexPolygon_axes,
        const size_t& convexPolygon_axisCount,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // normal doesn't always point out of convexPolygonA
    bool Get2D_ConvexPolygon_ConvexPolygon_CollisionNormalDepth_H1(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // normal doesn't always point out of convexPolygonA
    bool Get2D_ConvexPolygon_ConvexPolygon_CollisionNormalDepth_H2(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonA_axes,
        const size_t& convexPolygonA_axisCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    // normal doesn't always point out of convexPolygonA
    bool Get2D_ConvexPolygon_ConvexPolygon_CollisionNormalDepth_H3(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonA_axes,
        const size_t& convexPolygonA_axisCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        const glm::vec2* const& convexPolygonB_axes,
        const size_t& convexPolygonB_axisCount,
        glm::vec2& collisionNormal,
        float& collisionDepth);

    float RotateSlope(float slope, float rotationInRadians);

    glm::vec2 Get2D_DirectionFromSlope(float slope);

    float Get2D_SlopeFromDirection(const glm::vec2& direction);
}

#include "Overlap2D.inl"
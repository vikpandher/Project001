#pragma once

#include "glm/glm.hpp"

#include "Engine/Math/FloatComparators.h"
#include "Engine/Math/VectorAngles.h"



// 2D Shape List ---------------------------------------------------------------
// * Point
// * Line
// * LineSegment
// * Rectangle
// * OrientedRectangle
// * Circle
// * Capsule
// * Triangle
// * ConvexPolygon (TODO)
// 
// Overlap Functions:
//                | Poi | Lin | LiS | Rec | OrR | Cir | Cap | Tri | CoP |
// Point          |  \  |  \  |  \  |  \  |  \  |  \  |  \  |  \  |     |
// Line           | --- |  \  |  \  |  \  |  \  |  \  |  \  |  \  |     |
// LineSegment    | --- | --- |  \  |  \  |  \  |  \  |  \  |  \  |     |
// Rectangle      | --- | --- | --- |  \  |  \  |  \  |  \  |  \  |     |
// O. Rectangle   | --- | --- | --- | --- |  \  |  \  |  \  |  \  |     |
// Circle         | --- | --- | --- | --- | --- |  \  |  \  |  \  |     |
// Capsule        | --- | --- | --- | --- | --- | --- |  \  |  \  |     |
// Triangle       | --- | --- | --- | --- | --- | --- | --- |  \  |     |
// Convex Polygon | --- | --- | --- | --- | --- | --- | --- | --- |     |
// 
// Intersection Functions:
//                | Lin | LiS | Rec | OrR | Cir | Cap | Tri | Con |
// Line           |  \  |     |     |     |     |     |     |     |
// LineSegment    | --- |     |     |     |     |     |     |     |
// Rectangle      | --- | --- |     |     |     |     |     |     |
// O. Rectangle   | --- | --- | --- |     |     |     |     |     |
// Circle         | --- | --- | --- | --- |     |     |     |     |
// Capsule        | --- | --- | --- | --- | --- |     |     |     |
// Triangle       | --- | --- | --- | --- | --- | --- |     |     |
// Convex Polygon | --- | --- | --- | --- | --- | --- | --- |     |

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

    // Checking Line overlap ---------------------------------------------------

    bool Check2D_Line_Point_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& point_position);

    bool Check2D_Line_Line_Overlap(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope);

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

    // Getting Line Intersection -----------------------------------------------

    // If lineA_slope == lineB_slope, intersection_position == (nan, nan)
    void Get2D_Line_Line_Intersection(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope,
        glm::vec2& intersection_position);

    // Helper Functions --------------------------------------------------------

    float Get2D_Point_Line_DistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope);

    // Unused
    float Get2D_Point_Line_DistanceSquared_Alt(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope);

    float Get2D_Point_LineSegment_DistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end);

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

    // Unused
    float RotateSlope(float slope, float rotationInRadians);
}

#include "Overlap2D.inl"
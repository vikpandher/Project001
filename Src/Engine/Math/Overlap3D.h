#pragma once

#include "glm/glm.hpp"

#include "Engine/Math/FloatsEqual.h"



namespace Project001
{
    // 3D Shape List -----------------------------------------------------------
    // * Point
    // * Line
    // * LineSegment
    // * Plane
    // * Triangle (TODO)
    // * AABB (Axis Aligned Bounding Box) (TODO)
    // * OBB (Oriented Bounding Box) (TODO)
    // * Sphere (TODO)
    // 
    // Overlap Functions:
    //             | Poi | Lin | LiS | Pla | Tri | AAB | OBB | Sph |
    // Point       |  X  |  X  |  X  |  X  |     |  X  |     |  X  |
    // Line        | --- |  X  |  X  |  X  |     |     |     |     |
    // LineSegment | --- | --- |  X  |  X  |     |     |     |     |
    // Plane       | --- | --- | --- |  X  |     |     |     |     |
    // Triangle    | --- | --- | --- | --- |     |     |     |     |
    // AABB        | --- | --- | --- | --- | --- |     |     |     |
    // OBB         | --- | --- | --- | --- | --- | --- |     |     |
    // Sphere      | --- | --- | --- | --- | --- | --- | --- |     |
    // 
    // Closest Point Functions:
    //       | Pla | Tri | AAB | OBB | Sph |
    // Point |     |     |     |     |  X  |

    // Checking Point overlap --------------------------------------------------

    bool Check3D_Point_Point_Overlap(
        const glm::vec3& pointA_position,
        const glm::vec3& pointB_position);

    bool Check3D_Point_Line_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    bool Check3D_Point_LineSegment_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    bool Check3D_Point_Plane_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal);

    bool Check3D_Point_AABB_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max);

    bool Check3D_Point_Sphere_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Line overlap ---------------------------------------------------

    bool Check3D_Line_Point_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& point_position);

    bool Check3D_Line_Line_Overlap(
        const glm::vec3& lineA_position,
        const glm::vec3& lineA_direction,
        const glm::vec3& lineB_position,
        const glm::vec3& lineB_direction);

    bool Check3D_Line_LineSegment_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    bool Check3D_Line_Plane_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal);

    // Checking LineSegment overlap --------------------------------------------

    bool Check3D_LineSegment_Point_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& point_position);

    bool Check3D_LineSegment_Line_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    bool Check3D_LineSegment_LineSegment_Overlap(
        const glm::vec3& lineSegmentA_start,
        const glm::vec3& lineSegmentA_end,
        const glm::vec3& lineSegmentB_start,
        const glm::vec3& lineSegmentB_end);

    bool Check3D_LineSegment_Plane_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal);

    // Checking Plane overlap --------------------------------------------------

    bool Check3D_Plane_Point_Overlap(
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        const glm::vec3& point_position);

    bool Check3D_Plane_Line_Overlap(
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    bool Check3D_Plane_LineSegment_Overlap(
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    bool Check3D_Plane_Plane_Overlap(
        const glm::vec3& planeA_position,
        const glm::vec3& planeA_normal,
        const glm::vec3& planeB_position,
        const glm::vec3& planeB_normal);

    // Getting Closest Point ---------------------------------------------------

    // If point_position == sphere_position, closestPoint_position == (nan, nan)
    void Get3d_Point_Sphere_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        glm::vec3& closestPoint_position);

    // Helper Functions --------------------------------------------------------

    // Unused
    // This version doesn't care which 2 cornors AS LONG AS they are diagonally opposite
    bool Check3D_Point_Box_NoOverlap_H(
        const glm::vec3& point_position,
        const glm::vec3& box_oppositeCorner1,
        const glm::vec3& box_oppositeCorner2);

    // This version doesn't care which 2 cornors AS LONG AS they are diagonally opposite
    bool Check3D_Point_Box_Overlap_H(
        const glm::vec3& point_position,
        const glm::vec3& box_oppositeCorner1,
        const glm::vec3& box_oppositeCorner2);

    // Unused
    // Returns false when the lines are parallel
    bool Get3D_Line_Line_ClosestPoints_H(
        const glm::vec3& lineA_position,
        const glm::vec3& lineA_direction,
        const glm::vec3& lineB_position,
        const glm::vec3& lineB_direction,
        glm::vec3& pointA_position,
        glm::vec3& pointB_position);

    // Unused
    // Returns false when the line is parallel to the plane
    // (even if the line is directly on the plane)
    bool Get3D_Line_Plane_Intersection_H(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        glm::vec3& intersection_position);
}

#include "Overlap3D.inl"
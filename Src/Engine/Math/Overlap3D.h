#pragma once

#include "glm/glm.hpp"

#include "Engine/Math/FloatComparators.h"



// 3D Shape List ---------------------------------------------------------------
// 
// * Point (TODO)
// * Line (TODO)
// * Ray (TODO)
// * LineSegment (TODO)
// * Plane (TODO)
// * Triangle (TODO)
// * AABB (Axis Aligned Bounding Box) (TODO)
// * OBB (Oriented Bounding Box) (TODO)
// * Sphere (TODO)
// * Capsule (TODO)
// 
// |  \  | = written, but no unit test
// |  X  | = written, and has unit test
// 
// Overlap Functions:
//             | Poi | Lin | Ray | LiS | Pla | Tri | AAB | OBB | Sph | Cap |
// Point       |  X  |  X  |  X  |  X  |  X  |  X  |  X  |  X  |  X  |     |
// Line        | --- |  X  |  X  |  X  |  X  |  X  |     |     |  \  |     |
// Ray         | --- | --- |  X  |  X  |  X  |  X  |     |     |  \  |     |
// LineSegment | --- | --- | --- |  X  |  X  |  X  |     |     |  \  |     |
// Plane       | --- | --- | --- | --- |  X  |     |     |     |  \  |     |
// Triangle    | --- | --- | --- | --- | - - |     |     |     |  \  |     |
// AABB        | --- | - - | - - | - - | - - | - - |  \  |     |  \  |     |
// OBB         | --- | - - | - - | - - | - - | - - | - - |     |  \  |     |
// Sphere      | --- | --- | --- | --- | --- | --- | --- | --- |  \  |     |
// Capsule     | - - | - - | - - | - - | - - | - - | - - | - - | - - |     |
// 
// Closest Point Functions:
//                   | Lin | Ray | LiS | Pla | Tri | AAB | OBB | Sph | Cap |
// Point             |  X  |  X  |  X  |  X  |  X  |  X  |  X  |  X  |     |

namespace Project001
{
    // Checking Point overlap --------------------------------------------------

    bool Check3D_Point_Point_Overlap(
        const glm::vec3& pointA_position,
        const glm::vec3& pointB_position);

    // line_direction needs to be a unit vector
    bool Check3D_Point_Line_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    // ray_direction needs to be a unit vector
    bool Check3D_Point_Ray_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction);

    bool Check3D_Point_LineSegment_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    // plane_normal needs to be a unit vector
    bool Check3D_Point_Plane_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& plane_normal,
        const float& plane_distance);

    bool Check3D_Point_Triangle_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    bool Check3D_Point_AABB_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max);

    bool Check3D_Point_OBB_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation);

    bool Check3D_Point_Sphere_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Line overlap ---------------------------------------------------

    // line_direction needs to be a unit vector
    bool Check3D_Line_Point_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& point_position);

    // line_direction needs to be a unit vector
    // lineA_direction needs to be a unit vector
    bool Check3D_Line_Line_Overlap(
        const glm::vec3& lineA_position,
        const glm::vec3& lineA_direction,
        const glm::vec3& lineB_position,
        const glm::vec3& lineB_direction);

    // line_direction needs to be a unit vector
    // ray_direction needs to be a unit vector
    bool Check3D_Line_Ray_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction);

    // line_direction needs to be a unit vector
    bool Check3D_Line_LineSegment_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    // line_direction needs to be a unit vector
    // plane_normal needs to be a unit vector
    bool Check3D_Line_Plane_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& plane_normal,
        const float& plane_distance);

    // line_direction needs to be a unit vector
    bool Check3D_Line_Triangle_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    // line_direction needs to be a unit vector
    bool Check3D_Line_Sphere_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Ray overlap ----------------------------------------------------

    // ray_direction needs to be a unit vector
    bool Check3D_Ray_Point_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& point_position);

    // ray_direction needs to be a unit vector
    // line_direction needs to be a unit vector
    bool Check3D_Ray_Line_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    // rayA_direction needs to be a unit vector
    // rayB_direction needs to be a unit vector
    bool Check3D_Ray_Ray_Overlap(
        const glm::vec3& rayA_position,
        const glm::vec3& rayA_direction,
        const glm::vec3& rayB_position,
        const glm::vec3& rayB_direction);

    // ray_direction needs to be a unit vector
    bool Check3D_Ray_LineSegment_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    // ray_direction needs to be a unit vector
    // plane_normal needs to be a unit vector
    bool Check3D_Ray_Plane_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& plane_normal,
        const float& plane_distance);

    // ray_direction needs to be a unit vector
    bool Check3D_Ray_Triangle_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    // ray_direction needs to be a unit vector
    bool Check3D_Ray_Sphere_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking LineSegment overlap --------------------------------------------

    bool Check3D_LineSegment_Point_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& point_position);

    // line_direction needs to be a unit vector
    bool Check3D_LineSegment_Line_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    // ray_direction needs to be a unit vector
    bool Check3D_LineSegment_Ray_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction);

    bool Check3D_LineSegment_LineSegment_Overlap(
        const glm::vec3& lineSegmentA_start,
        const glm::vec3& lineSegmentA_end,
        const glm::vec3& lineSegmentB_start,
        const glm::vec3& lineSegmentB_end);

    // plane_normal needs to be a unit vector
    bool Check3D_LineSegment_Plane_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& plane_normal,
        const float& plane_distance);

    bool Check3D_LineSegment_Triangle_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    bool Check3D_LineSegment_Sphere_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Plane overlap --------------------------------------------------

    // plane_normal needs to be a unit vector
    bool Check3D_Plane_Point_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& point_position);

    // plane_normal needs to be a unit vector
    bool Check3D_Plane_Line_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    // plane_normal needs to be a unit vector
    bool Check3D_Plane_LineSegment_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    // plane_normal needs to be a unit vector
    bool Check3D_Plane_Plane_Overlap(
        const glm::vec3& planeA_normal,
        const float& planeA_distance,
        const glm::vec3& planeB_normal,
        const float& planeB_distance);

    // plane_normal needs to be a unit vector
    bool Check3D_Plane_Sphere_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Triangle overlap -----------------------------------------------

    bool Check3D_Triangle_Point_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& point_position);

    // line_direction needs to be a unit vector
    bool Check3D_Triangle_Line_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    // ray_direction needs to be a unit vector
    bool Check3D_Triangle_Ray_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction);

    bool Check3D_Triangle_LineSegment_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    bool Check3D_Triangle_Sphere_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Axis Aligned Bounding Box overlap ------------------------------

    bool Check3D_AABB_Point_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& point_position);

    bool Check3D_AABB_AABB_Overlap(
        const glm::vec3& aabbA_min,
        const glm::vec3& aabbA_max,
        const glm::vec3& aabbB_min,
        const glm::vec3& aabbB_max);

    bool Check3D_AABB_Sphere_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Oriented Bounding Box overlap ----------------------------------

    bool Check3D_OBB_Point_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& point_position);

    bool Check3D_OBB_Sphere_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Sphere overlap -------------------------------------------------

    bool Check3D_Sphere_Point_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& point_position);

    // line_direction needs to be a unit vector
    bool Check3D_Sphere_Line_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    // ray_direction needs to be a unit vector
    bool Check3D_Sphere_Ray_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction);

    bool Check3D_Sphere_LineSegment_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    // plane_normal needs to be a unit vector
    bool Check3D_Sphere_Plane_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& plane_normal,
        const float& plane_distance);

    bool Check3D_Sphere_Triangle_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    bool Check3D_Sphere_AABB_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max);

    bool Check3D_Sphere_OBB_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation);

    bool Check3D_Sphere_Sphere_Overlap(
        const glm::vec3& sphereA_position,
        const float& sphereA_radius,
        const glm::vec3& sphereB_position,
        const float& sphereB_radius);

    // Getting Closest Point ---------------------------------------------------

    // line_direction needs to be a unit vector
    void Get3D_Point_Line_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        glm::vec3& closestPoint_position);

    // ray_direction needs to be a unit vector
    void Get3D_Point_Ray_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        glm::vec3& closestPoint_position);

    void Get3D_Point_LineSegment_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        glm::vec3& closestPoint_position);

    // plane_normal needs to be a unit vector
    void Get3D_Point_Plane_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& plane_normal,
        const float& plane_distance,
        glm::vec3& closestPoint_position);

    void Get3D_Point_Triangle_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        glm::vec3& closestPoint_position);

    void Get3D_Point_AABB_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        glm::vec3& closestPoint_position);

    void Get3D_Point_OBB_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        glm::vec3& closestPoint_position);

    void Get3D_Point_Sphere_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        glm::vec3& closestPoint_position);

    // If point_position == sphere_position, closestPoint_position == (nan, nan)
    void Get3d_Point_Sphere_ClosestSurfacePoint(
        const glm::vec3& point_position,
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        glm::vec3& closestPoint_position);

    // Helper Functions --------------------------------------------------------

    // Unused
    bool Get3D_Plane_Plane_Plane_Intersection_H(
        const glm::vec3& planeA_normal,
        const float& planeA_distance,
        const glm::vec3& planeB_normal,
        const float& planeB_distance,
        const glm::vec3& planeC_normal,
        const float& planeC_distance,
        glm::vec3& intersectionPoint_position);

    // This version doesn't care which 2 cornors AS LONG AS they are diagonally opposite
    bool Check3D_Point_Box_Overlap_H(
        const glm::vec3& point_position,
        const glm::vec3& box_oppositeCorner1,
        const glm::vec3& box_oppositeCorner2);

    // Has alternative plane representation
    bool Check3D_Point_Plane_Overlap_Alt(
        const glm::vec3& point_position,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal);

    // Unused
    bool Check3D_Point_OBB_Overlap_Alt(
        const glm::vec3& point_position,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation);

    // Unused
    void Get3D_Point_OBB_ClosestPoint_Alt(
        const glm::vec3& point_position,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        glm::vec3& closestPoint_position);

    // Unused
    // Has alternative plane representation
    bool Check3D_Line_Plane_Overlap_Alt(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal);

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
    bool Check3D_Point_LineSegment_Overlap_Alt(
        const glm::vec3& point_position,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    // Unused
    // No intersection if line is parallel to the plane
    // line_direction must be a unit vector
    // plane_normal must be a unit vector
    bool Get3D_Line_Plane_Intersection_Alt(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        glm::vec3& intersection_position);

    // Unused
    // No intersection if ray is parallel to the plane
    // ray_direction must be a unit vector
    // plane_normal must be a unit vector
    bool Get3D_Ray_Plane_Intersection_Alt(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        glm::vec3& intersection_position);

    // Unused
    // Has alternative plane representation
    bool Check3D_LineSegment_Plane_Overlap_Alt(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal);

    // Unused
    // Has alternative plane representation
    bool Check3D_Plane_Plane_Overlap_Alt(
        const glm::vec3& planeA_position,
        const glm::vec3& planeA_normal,
        const glm::vec3& planeB_position,
        const glm::vec3& planeB_normal);

    // Corners cannot be the same, must be a valid triangle
    void Get3D_Triangle_ContainingPlane_H(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        glm::vec3& plane_normal,
        float& plane_distance);

    // If corners are the same (not a valid triangle),
    // normals is set to (0, 1, 0) and distance is set to length(corner1)
    void Get3D_Triangle_ContainingPlane_S(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        glm::vec3& plane_normal,
        float& plane_distance);
}

#include "Overlap3D.inl"
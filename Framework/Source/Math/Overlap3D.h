// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-05-11

#pragma once

#include "Math/MathUtilities.h"



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
// * Frustum (TODO)
// 
// |  \  | = written, but no unit test
// |  X  | = written, and has unit test
// 
// Overlap Functions:
//             | Poi | Lin | Ray | LiS | Pla | Tri | AAB | OBB | Sph | Cap | Fru |
// Point       |  X  |  X  |  X  |  X  |  X  |  X  |  X  |  X  |  X  |  X  |  \  |
// Line        | --- |  X  |  X  |  X  |  X  |  X  |  X  |  X  |  X  |     |     |
// Ray         | --- | --- |  X  |  X  |  X  |  X  |  X  |  X  |  X  |     |     |
// LineSegment | --- | --- | --- |  X  |  X  |  X  |  X  |  X  |  X  |     |     |
// Plane       | --- | --- | --- | --- |  X  |  X  |  X  |  X  |  X  |     |     |
// Triangle    | --- | --- | --- | --- | --- |  X  |  X  |  X  |  X  |     |     |
// AABB        | --- | --- | --- | --- | --- | --- |  X  |  X  |  X  |     |     |
// OBB         | --- | --- | --- | --- | --- | --- | --- |  X  |  X  |     |     |
// Sphere      | --- | --- | --- | --- | --- | --- | --- | --- |  X  |  \  |  \  |
// Capsule     | --- |     |     |     |     |     |     |     | --- |     |     |
// Frustum     | --- |     |     |     |     |     |     |     | --- |     |     |
// 
// Closest Point Functions:
//                   | Lin | Ray | LiS | Pla | Tri | AAB | OBB | Sph | Cap |
// Point             |  X  |  X  |  X  |  X  |  X  |  X  |  X  |  X  |     |
// 
// Raycast Functions:
//                                     | Pla | Tri | AAB | OBB | Sph | Cap |
// Ray                                 |  ?  |  ?  |  ?  |  ?  |  ?  |     |


namespace Project001
{
    // Checking Point Overlap --------------------------------------------------

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

    bool Check3D_Point_Capsule_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& capsule_start,
        const glm::vec3& capsule_end,
        const float& capsule_radius);

    // Frustom planes must face into its center.
    bool Check3D_Point_Frustum_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& frustum_leftPlaneNormal,
        const float& frustum_leftPlaneDistance,
        const glm::vec3& frustum_rightPlaneNormal,
        const float& frustum_rightPlaneDistance,
        const glm::vec3& frustum_bottomPlaneNormal,
        const float& frustum_bottomPlaneDistance,
        const glm::vec3& frustum_topPlaneNormal,
        const float& frustum_topPlaneDistance,
        const glm::vec3& frustum_nearPlaneNormal,
        const float& frustum_nearPlaneDistance,
        const glm::vec3& frustum_farPlaneNormal,
        const float& frustum_farPlaneDistance);

    // Checking Line Overlap ---------------------------------------------------

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
    bool Check3D_Line_AABB_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max);

    // line_direction needs to be a unit vector
    bool Check3D_Line_OBB_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation);

    // line_direction needs to be a unit vector
    bool Check3D_Line_Sphere_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Ray Overlap ----------------------------------------------------

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
    bool Check3D_Ray_AABB_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max);

    // ray_direction needs to be a unit vector
    bool Check3D_Ray_OBB_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation);

    // ray_direction needs to be a unit vector
    bool Check3D_Ray_Sphere_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking LineSegment Overlap --------------------------------------------

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

    bool Check3D_LineSegment_AABB_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max);

    bool Check3D_LineSegment_OBB_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation);

    bool Check3D_LineSegment_Sphere_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Plane Overlap --------------------------------------------------

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
    bool Check3D_Plane_Triangle_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    // plane_normal needs to be a unit vector
    bool Check3D_Plane_AABB_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max);

    // plane_normal needs to be a unit vector
    bool Check3D_Plane_OBB_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation);

    // plane_normal needs to be a unit vector
    bool Check3D_Plane_Sphere_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Triangle Overlap -----------------------------------------------

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

    // plane_normal needs to be a unit vector
    bool Check3D_Triangle_Plane_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& plane_normal,
        const float& plane_distance);

    // TODO
    bool Check3D_Triangle_Triangle_Overlap(
        const glm::vec3& triangleA_corner1,
        const glm::vec3& triangleA_corner2,
        const glm::vec3& triangleA_corner3,
        const glm::vec3& triangleB_corner1,
        const glm::vec3& triangleB_corner2,
        const glm::vec3& triangleB_corner3);

    bool Check3D_Triangle_AABB_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max);

    bool Check3D_Triangle_OBB_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation);

    bool Check3D_Triangle_Sphere_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Axis Aligned Bounding Box Overlap ------------------------------

    bool Check3D_AABB_Point_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& point_position);

    // line_direction needs to be a unit vector
    bool Check3D_AABB_Line_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    // ray_direction needs to be a unit vector
    bool Check3D_AABB_Ray_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction);

    bool Check3D_AABB_LineSegment_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    // plane_normal needs to be a unit vector
    bool Check3D_AABB_Plane_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& plane_normal,
        const float& plane_distance);

    bool Check3D_AABB_Triangle_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    bool Check3D_AABB_AABB_Overlap(
        const glm::vec3& aabbA_min,
        const glm::vec3& aabbA_max,
        const glm::vec3& aabbB_min,
        const glm::vec3& aabbB_max);

    bool Check3D_AABB_OBB_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation);

    bool Check3D_AABB_Sphere_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Oriented Bounding Box Overlap ----------------------------------

    bool Check3D_OBB_Point_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& point_position);

    // line_direction needs to be a unit vector
    bool Check3D_OBB_Line_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& line_position,
        const glm::vec3& line_direction);

    // ray_direction needs to be a unit vector
    bool Check3D_OBB_Ray_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction);

    bool Check3D_OBB_LineSegment_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end);

    // plane_normal needs to be a unit vector
    bool Check3D_OBB_Plane_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& plane_normal,
        const float& plane_distance);

    bool Check3D_OBB_Triangle_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    bool Check3D_OBB_AABB_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max);

    bool Check3D_OBB_OBB_Overlap(
        const glm::vec3& obbA_halfSize,
        const glm::vec3& obbA_position,
        const glm::quat& obbA_orientation,
        const glm::vec3& obbB_halfSize,
        const glm::vec3& obbB_position,
        const glm::quat& obbB_orientation);

    bool Check3D_OBB_Sphere_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Sphere Overlap -------------------------------------------------

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

    bool Check3D_Sphere_Capsule_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& capsule_start,
        const glm::vec3& capsule_end,
        const float& capsule_radius);

    // Frustom planes must face into its center.
    bool Check3D_Sphere_Frustum_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& frustum_leftPlaneNormal,
        const float& frustum_leftPlaneDistance,
        const glm::vec3& frustum_rightPlaneNormal,
        const float& frustum_rightPlaneDistance,
        const glm::vec3& frustum_bottomPlaneNormal,
        const float& frustum_bottomPlaneDistance,
        const glm::vec3& frustum_topPlaneNormal,
        const float& frustum_topPlaneDistance,
        const glm::vec3& frustum_nearPlaneNormal,
        const float& frustum_nearPlaneDistance,
        const glm::vec3& frustum_farPlaneNormal,
        const float& frustum_farPlaneDistance);

    // Checking Capsule Overlap ------------------------------------------------

    bool Check3D_Capsule_Point_Overlap(
        const glm::vec3& capsule_start,
        const glm::vec3& capsule_end,
        const float& capsule_radius,
        const glm::vec3& point_position);

    bool Check3D_Capsule_Sphere_Overlap(
        const glm::vec3& capsule_start,
        const glm::vec3& capsule_end,
        const float& capsule_radius,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

    // Checking Frustum Overlap ------------------------------------------------

    // Frustom planes must face into its center.
    bool Check3D_Frustum_Point_Overlap(
        const glm::vec3& frustum_leftPlaneNormal,
        const float& frustum_leftPlaneDistance,
        const glm::vec3& frustum_rightPlaneNormal,
        const float& frustum_rightPlaneDistance,
        const glm::vec3& frustum_bottomPlaneNormal,
        const float& frustum_bottomPlaneDistance,
        const glm::vec3& frustum_topPlaneNormal,
        const float& frustum_topPlaneDistance,
        const glm::vec3& frustum_nearPlaneNormal,
        const float& frustum_nearPlaneDistance,
        const glm::vec3& frustum_farPlaneNormal,
        const float& frustum_farPlaneDistance,
        const glm::vec3& point_position);

    // Frustom planes must face into its center.
    bool Check3D_Frustum_Sphere_Overlap(
        const glm::vec3& frustum_leftPlaneNormal,
        const float& frustum_leftPlaneDistance,
        const glm::vec3& frustum_rightPlaneNormal,
        const float& frustum_rightPlaneDistance,
        const glm::vec3& frustum_bottomPlaneNormal,
        const float& frustum_bottomPlaneDistance,
        const glm::vec3& frustum_topPlaneNormal,
        const float& frustum_topPlaneDistance,
        const glm::vec3& frustum_nearPlaneNormal,
        const float& frustum_nearPlaneDistance,
        const glm::vec3& frustum_farPlaneNormal,
        const float& frustum_farPlaneDistance,
        const glm::vec3& sphere_position,
        const float& sphere_radius);

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
    void Get3D_Point_Sphere_ClosestSurfacePoint(
        const glm::vec3& point_position,
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        glm::vec3& closestPoint_position);

    // Raycast Functions -------------------------------------------------------

    // hit_scalar determins where the hit occured.
    // hit_point_position = ray_position + ray_direction * hit_scalar

    // plane_normal needs to be a unit vector
    bool Raycast3D_Plane(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& plane_normal,
        const float& plane_distance,
        float& hit_scalar,
        glm::vec3& hit_normal);

    bool Raycast3D_Triangle(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        float& hit_scalar,
        glm::vec3& hit_normal,
        glm::vec3& hit_point_position);

    bool Raycast3D_AABB(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        float& hit_scalar,
        glm::vec3& hit_normal);

    bool Raycast3D_OBB(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        float& hit_scalar,
        glm::vec3& hit_normal);

    // ray_direction needs to be a unit vector
    bool Raycast3D_Sphere(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        float& hit_scalar,
        glm::vec3& hit_normal,
        glm::vec3& hit_point_position);

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

    // TODO
    bool Get3D_Plane_Plane_Intersection_H(
        const glm::vec3& planeA_normal,
        const float& planeA_distance,
        const glm::vec3& planeB_normal,
        const float& planeB_distance,
        glm::vec3& line_position,
        glm::vec3& line_direction);

    // This version doesn't care which 2 cornors AS LONG AS they are diagonally opposite
    bool Check3D_Point_Box_Overlap_H(
        const glm::vec3& point_position,
        const glm::vec3& box_oppositeCorner1,
        const glm::vec3& box_oppositeCorner2);

    // plane_normal needs to be a unit vector
    // If distance is negative, the point is below the plane, If distance is
    // positive, the point is above the plane.
    void Get3D_Point_Plane_Distance(
        const glm::vec3& point_position,
        const glm::vec3& plane_normal,
        const float& plane_distance,
        float& distance);

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

    // Doesn't check if triangle is a valid triangle with 3 unique corner points
    bool Check3D_Point_Triangle_Overlap_H(
        const glm::vec3& point_position,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    // Doesn't check if triangle is a valid triangle with 3 unique corner points
    bool Check3D_LineSegment_Triangle_Overlap_H(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    // Unused
    // Doesn't check if triangle is a valid triangle with 3 unique corner points
    bool Check3D_LineSegment_Triangle_Overlap_H_Alt(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3);

    // Corners cannot be the same, must be a valid triangle
    void Get3D_Triangle_ContainingPlane_H(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        glm::vec3& plane_normal,
        float& plane_distance);

    // Unused
    // If corners are the same (not a valid triangle),
    // normals is set to (0, 1, 0) and distance is set to length(corner1)
    void Get3D_Triangle_ContainingPlane_S(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        glm::vec3& plane_normal,
        float& plane_distance);

    void Get3D_Triangle_AxisInterval(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& axis,
        float& interval_min,
        float& interval_max);

    void Get3D_AABB_AxisInterval(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& axis,
        float& interval_min,
        float& interval_max);

    void Get3D_OBB_AxisInterval(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& axis,
        float& interval_min,
        float& interval_max);

    // includes g_floattMarginOfError
    bool Linecast3D_AABB(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        float& hit_scalar_largest_min,
        float& hit_scalar_smallest_max);

    // includes g_floattMarginOfError
    bool Linecast3D_OBB(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        float& hit_scalar_largest_min,
        float& hit_scalar_smallest_max);

    glm::vec3 SAT_Cross(
        const glm::vec3& a,
        const glm::vec3& b,
        const glm::vec3& c,
        const glm::vec3& d);
}
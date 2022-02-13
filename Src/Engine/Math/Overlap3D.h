#pragma once

#include "glm/glm.hpp"

#include "Engine/Math/FloatsEqual.h"



namespace Project001
{
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

    // Checking LineSegment overlap --------------------------------------------

    bool Check3D_LineSegment_Point_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& point_position);

    // Helper Functions --------------------------------------------------------

    // This version doesn't care which 2 cornors AS LONG AS they are opposite
    bool Check3D_Point_Rectangle_NoOverlap_H(
        const glm::vec3& point_position,
        const glm::vec3& rectangle_oppositeCorner1,
        const glm::vec3& rectangle_oppositeCorner2);

    // This version doesn't care which 2 cornors AS LONG AS they are opposite
    bool Check3D_Point_Rectangle_Overlap_H(
        const glm::vec3& point_position,
        const glm::vec3& rectangle_oppositeCorner1,
        const glm::vec3& rectangle_oppositeCorner2);

}

#include "Overlap3D.inl"
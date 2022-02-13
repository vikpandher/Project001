


namespace Project001
{
    // Checking Point overlap --------------------------------------------------

    inline bool Check3D_Point_Point_Overlap(
        const glm::vec3& pointA_position,
        const glm::vec3& pointB_position)
    {
        return FloatsEqual(pointA_position.x, pointB_position.x) &&
            FloatsEqual(pointA_position.y, pointB_position.y) &&
            FloatsEqual(pointA_position.z, pointB_position.z);
    }

    inline bool Check3D_Point_Line_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& line_position,
        const glm::vec3& line_direction)
    {
        // point_position = line_direction * t + line_position
        glm::vec3 t = (point_position - line_position) / line_direction;

        return FloatsEqual(t.x, t.y) &&
            FloatsEqual(t.x, t.z);
    }

    inline bool Check3D_Point_LineSegment_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        if (Check3D_Point_Rectangle_NoOverlap_H(point_position, lineSegment_start, lineSegment_end))
        {
            // the point is outside of the recangular range of the lineSegment
            return false;
        }

        return Check3D_Point_Line_Overlap(point_position, lineSegment_start, lineSegment_end - lineSegment_start);
    }

    // Checking Line overlap ---------------------------------------------------

    inline bool Check3D_Line_Point_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& point_position)
    {
        return Check3D_Point_Line_Overlap(point_position, line_position, line_direction);
    }

    bool Check3D_Line_Line_Overlap(
        const glm::vec3& lineA_position,
        const glm::vec3& lineA_direction,
        const glm::vec3& lineB_position,
        const glm::vec3& lineB_direction)
    {
        glm::vec3 lineAB = lineB_position - lineA_position;

        glm::vec3 n = glm::cross(lineA_direction, lineAB);
        float dotProduct = glm::dot(n, lineB_direction);

        if (!FloatsEqual(dotProduct, 0.0f))
        {
            // lines are not coplanar
            return false;
        }

        if (FloatsEqual(lineA_position.x, lineB_position.x) &&
            FloatsEqual(lineA_position.y, lineB_position.y) &&
            FloatsEqual(lineA_position.z, lineB_position.z))
        {
            // points are the same;
            return true;
        }

        glm::vec3 directionRatio = lineA_direction / lineB_direction;
        if (FloatsEqual(directionRatio.x, directionRatio.y) &&
            FloatsEqual(directionRatio.x, directionRatio.z))
        {
            // lines are parallel
            return false;
        }

        // lines are coplanar and not parallel so they must intersect
        return true;;
    }

    // Checking LineSegment overlap --------------------------------------------

    inline bool Check3D_LineSegment_Point_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& point_position)
    {
        return Check3D_Point_LineSegment_Overlap(point_position, lineSegment_start, lineSegment_end);
    }

    // Helper Functions --------------------------------------------------------

    inline bool Check3D_Point_Rectangle_NoOverlap_H(
        const glm::vec3& point_position,
        const glm::vec3& rectangle_oppositeCorner1,
        const glm::vec3& rectangle_oppositeCorner2)
    {
        return point_position.x > GetMax(rectangle_oppositeCorner1.x, rectangle_oppositeCorner2.x) ||
            point_position.x < GetMin(rectangle_oppositeCorner1.x, rectangle_oppositeCorner2.x) ||
            point_position.y > GetMax(rectangle_oppositeCorner1.y, rectangle_oppositeCorner2.y) ||
            point_position.y < GetMin(rectangle_oppositeCorner1.y, rectangle_oppositeCorner2.y) ||
            point_position.z > GetMax(rectangle_oppositeCorner1.z, rectangle_oppositeCorner2.z) ||
            point_position.z < GetMin(rectangle_oppositeCorner1.z, rectangle_oppositeCorner2.z);
    }

    inline bool Check3D_Point_Rectangle_Overlap_H(
        const glm::vec3& point_position,
        const glm::vec3& rectangle_oppositeCorner1,
        const glm::vec3& rectangle_oppositeCorner2)
    {
        return point_position.x <= GetMax(rectangle_oppositeCorner1.x, rectangle_oppositeCorner2.x) &&
            point_position.x >= GetMin(rectangle_oppositeCorner1.x, rectangle_oppositeCorner2.x) &&
            point_position.y <= GetMax(rectangle_oppositeCorner1.y, rectangle_oppositeCorner2.y) &&
            point_position.y >= GetMin(rectangle_oppositeCorner1.y, rectangle_oppositeCorner2.y) &&
            point_position.z <= GetMax(rectangle_oppositeCorner1.z, rectangle_oppositeCorner2.z) &&
            point_position.z >= GetMin(rectangle_oppositeCorner1.z, rectangle_oppositeCorner2.z);
    }

}
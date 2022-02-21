


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
        if (Check3D_Point_Box_NoOverlap_H(point_position, lineSegment_start, lineSegment_end))
        {
            // the point is outside of the recangular range of the lineSegment
            return false;
        }

        return Check3D_Point_Line_Overlap(point_position, lineSegment_start, lineSegment_end - lineSegment_start);
    }

    inline bool Check3D_Point_Plane_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal)
    {
        glm::vec3 planeToPoint = point_position - plane_position;

        float dotProduct = glm::dot(planeToPoint, plane_normal);
        return FloatsEqual(dotProduct, 0.0f);
    }

    inline bool Check3D_Point_AABB_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max)
    {
        return point_position.x <= aabb_max.x && point_position.x >= aabb_min.x &&
            point_position.y <= aabb_max.y && point_position.y >= aabb_min.y &&
            point_position.z <= aabb_max.z && point_position.z >= aabb_min.z;
    }

    inline bool Check3D_Point_Sphere_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& sphere_position,
        const float& sphere_radius)
    {
        glm::vec3 sphereToPoint = point_position - sphere_position;
        return glm::dot(sphereToPoint, sphereToPoint) <= sphere_radius * sphere_radius;
    }

    // Checking Line overlap ---------------------------------------------------

    inline bool Check3D_Line_Point_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& point_position)
    {
        return Check3D_Point_Line_Overlap(point_position, line_position, line_direction);
    }

    inline bool Check3D_Line_Line_Overlap(
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
            // points are the same
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

    inline bool Check3D_Line_LineSegment_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        glm::vec3 startToStart = line_position - lineSegment_start;

        glm::vec3 n = glm::cross(line_direction, startToStart);
        float dotProduct = glm::dot(n, line_direction);

        if (!FloatsEqual(dotProduct, 0.0f))
        {
            // lines are not coplanar
            return false;
        }

        // p = d1 * t1 + o1
        // p = d2 * t2 + o2
        const glm::vec3& o1 = line_position;
        const glm::vec3& d1 = line_direction;
        const glm::vec3& o2 = lineSegment_start;
        glm::vec3 d2 = lineSegment_end - lineSegment_start;

        glm::vec3 d1_X_d2 = glm::cross(d1, d2);
        float denominator = glm::dot(d1_X_d2, d1_X_d2);
        if (FloatsEqual(denominator, 0.0f))
        {
            // lines are parallel
            return false;
        }

        glm::vec3 o1_to_o2 = o2 - o1;
        // t1 = determinant((o2 - o1),d2,d1 X d2) / ||d1 X d2||^2
        float t1 = glm::determinant(glm::mat3(o1_to_o2, d2, d1_X_d2)) / denominator;

        glm::vec3 p = d1 * t1 + o1;

        // Check if the intersection point is within the lineSegemt's range
        return Check3D_Point_Box_Overlap_H(p, lineSegment_start, lineSegment_end);
    }

    inline bool Check3D_Line_Plane_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal)
    {
        if (Check3D_Point_Plane_Overlap(line_position, plane_position, plane_normal))
        {
            // line is on the plane
            return true;
        }

        float dotProduct = glm::dot(line_direction, plane_normal);

        // If dotProduct == 0, line is parallel to the plane
        return !FloatsEqual(dotProduct, 0.0f);
    }

    // Checking LineSegment overlap --------------------------------------------

    inline bool Check3D_LineSegment_Point_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& point_position)
    {
        return Check3D_Point_LineSegment_Overlap(point_position, lineSegment_start, lineSegment_end);
    }

    inline bool Check3D_LineSegment_Line_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& line_position,
        const glm::vec3& line_direction)
    {
        return Check3D_Line_LineSegment_Overlap(line_position, line_direction, lineSegment_start, lineSegment_end);
    }

    inline bool Check3D_LineSegment_LineSegment_Overlap(
        const glm::vec3& lineSegmentA_start,
        const glm::vec3& lineSegmentA_end,
        const glm::vec3& lineSegmentB_start,
        const glm::vec3& lineSegmentB_end)
    {
        glm::vec3 startToStart = lineSegmentB_start - lineSegmentA_start;
        glm::vec3 lineA_direction = lineSegmentA_end - lineSegmentA_start;

        glm::vec3 n = glm::cross(lineA_direction, startToStart);
        float dotProduct = glm::dot(n, lineA_direction);

        if (!FloatsEqual(dotProduct, 0.0f))
        {
            // lines are not coplanar
            return false;
        }

        // p = d1 * t1 + o1
        // p = d2 * t2 + o2
        const glm::vec3& o1 = lineSegmentA_start;
        glm::vec3 d1 = lineSegmentA_end - lineSegmentA_start;
        const glm::vec3& o2 = lineSegmentB_start;
        glm::vec3 d2 = lineSegmentB_end - lineSegmentB_start;

        glm::vec3 d1_X_d2 = glm::cross(d1, d2);
        float denominator = glm::dot(d1_X_d2, d1_X_d2);
        if (FloatsEqual(denominator, 0.0f))
        {
            // lines are parallel
            return false;
        }

        glm::vec3 o1_to_o2 = o2 - o1;
        // t1 = determinant((o2 - o1),d2,d1 X d2) / ||d1 X d2||^2
        float t1 = glm::determinant(glm::mat3(o1_to_o2, d2, d1_X_d2)) / denominator;

        glm::vec3 p = d1 * t1 + o1;

        // check if the intersection point is within the lineSegemts' ranges
        return Check3D_Point_Box_Overlap_H(p, lineSegmentA_start, lineSegmentA_end) &&
            Check3D_Point_Box_Overlap_H(p, lineSegmentB_start, lineSegmentB_end);
    }

    inline bool Check3D_LineSegment_Plane_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal)
    {
        glm::vec3 intersection_position;
        if(!Get3D_Line_Plane_Intersection_H(lineSegment_start, lineSegment_end - lineSegment_start, plane_position, plane_normal, intersection_position))
        {
            // line is parallel
            return false;
        }

        // check if the line plane intersection is within the line segment's range
        return Check3D_Point_Box_Overlap_H(intersection_position, lineSegment_start, lineSegment_end);
    }

    // Checking Plane overlap --------------------------------------------------

    inline bool Check3D_Plane_Point_Overlap(
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        const glm::vec3& point_position)
    {
        return Check3D_Point_Plane_Overlap(point_position, plane_position, plane_normal);
    }

    inline bool Check3D_Plane_Line_Overlap(
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        const glm::vec3& line_position,
        const glm::vec3& line_direction)
    {
        return Check3D_Line_Plane_Overlap(line_position, line_direction, plane_position, plane_normal);
    }

    inline bool Check3D_Plane_LineSegment_Overlap(
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        return Check3D_LineSegment_Plane_Overlap(lineSegment_start, lineSegment_end, plane_position, plane_normal);
    }

    inline bool Check3D_Plane_Plane_Overlap(
        const glm::vec3& planeA_position,
        const glm::vec3& planeA_normal,
        const glm::vec3& planeB_position,
        const glm::vec3& planeB_normal)
    {
        if (FloatsEqual(planeA_position.x, planeB_position.x) &&
            FloatsEqual(planeA_position.y, planeB_position.y) &&
            FloatsEqual(planeA_position.z, planeB_position.z))
        {
            return true;
        }

        glm::vec3 normal_ratio = planeA_normal / planeB_normal;

        // Check if planes are parallel; if they are, they don't overlap
        return !(FloatsEqual(normal_ratio.x, normal_ratio.y) &&
            FloatsEqual(normal_ratio.x, normal_ratio.z));
    }

    // Getting Closest Point ---------------------------------------------------

    inline void Get3d_Point_Sphere_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        glm::vec3& closestPoint_position)
    {
        closestPoint_position = point_position - sphere_position;
        closestPoint_position = glm::normalize(closestPoint_position);
        closestPoint_position *= sphere_radius;
        closestPoint_position += sphere_position;
    }

    // Helper Functions --------------------------------------------------------

    inline bool Check3D_Point_Box_NoOverlap_H(
        const glm::vec3& point_position,
        const glm::vec3& box_oppositeCorner1,
        const glm::vec3& box_oppositeCorner2)
    {
        return point_position.x > GetMax(box_oppositeCorner1.x, box_oppositeCorner2.x) ||
            point_position.x < GetMin(box_oppositeCorner1.x, box_oppositeCorner2.x) ||
            point_position.y > GetMax(box_oppositeCorner1.y, box_oppositeCorner2.y) ||
            point_position.y < GetMin(box_oppositeCorner1.y, box_oppositeCorner2.y) ||
            point_position.z > GetMax(box_oppositeCorner1.z, box_oppositeCorner2.z) ||
            point_position.z < GetMin(box_oppositeCorner1.z, box_oppositeCorner2.z);
    }

    inline bool Check3D_Point_Box_Overlap_H(
        const glm::vec3& point_position,
        const glm::vec3& box_oppositeCorner1,
        const glm::vec3& box_oppositeCorner2)
    {
        return point_position.x <= GetMax(box_oppositeCorner1.x, box_oppositeCorner2.x) &&
            point_position.x >= GetMin(box_oppositeCorner1.x, box_oppositeCorner2.x) &&
            point_position.y <= GetMax(box_oppositeCorner1.y, box_oppositeCorner2.y) &&
            point_position.y >= GetMin(box_oppositeCorner1.y, box_oppositeCorner2.y) &&
            point_position.z <= GetMax(box_oppositeCorner1.z, box_oppositeCorner2.z) &&
            point_position.z >= GetMin(box_oppositeCorner1.z, box_oppositeCorner2.z);
    }

    inline bool Get3D_Line_Line_ClosestPoints_H(
        const glm::vec3& lineA_position,
        const glm::vec3& lineA_direction,
        const glm::vec3& lineB_position,
        const glm::vec3& lineB_direction,
        glm::vec3& pointA_position,
        glm::vec3& pointB_position)
    {
        // p = d1 * t1 + o1
        // p = d2 * t2 + o2
        const glm::vec3& o1 = lineA_position;
        const glm::vec3& d1 = lineA_direction;
        const glm::vec3& o2 = lineB_position;
        const glm::vec3& d2 = lineB_direction;

        glm::vec3 d1_X_d2 = glm::cross(d1, d2);
        float denominator = glm::dot(d1_X_d2, d1_X_d2);
        if (FloatsEqual(denominator, 0.0f))
        {
            // if (Check3D_Point_Line_Overlap(lineA_position, lineB_position, lineB_direction))
            // {
            //     // lines are the same line
            // }

            // lines are just parallel
            return false;
        }

        glm::vec3 o1_to_o2 = o2 - o1;
        // t1 = determinant((o2 - o1),d2,d1 X d2) / ||d1 X d2||^2
        float t1 = glm::determinant(glm::mat3(o1_to_o2, d2, d1_X_d2)) / denominator;
        // t2 = determinant((o2 - o1),d1,d1 X d2) / ||d1 X d2||^2
        float t2 = glm::determinant(glm::mat3(o1_to_o2, d1, d1_X_d2)) / denominator;

        pointA_position = d1 * t1 + o1;
        pointB_position = d1 * t1 + o1;

        return true;
    }

    inline bool Get3D_Line_Plane_Intersection_H(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        glm::vec3& intersection_position)
    {
        // dot = dot operation
        // p1 = point on line
        // d = line direction vector
        // p2 = point on plane (plane position)
        // n = plane normal
        // 
        // u = unknown
        // p = intersection point
        // 
        // Equation for Plane:
        // n dot (p - p2) = 0
        // 
        // Equation for Line:
        // p = d * u + p1
        // 
        // Intersection:
        // n dot ((d * u + p1) - p2) = 0
        // n dot(d * u + p1) = n dot p2
        // u * n dot d + n dot p1 = n dot p2
        // u * n dot d = n dot p2 - n dot p1
        // u = n dot (p2 - p1) / (n dot d)

        float denominator = glm::dot(plane_normal, line_direction);
        if (FloatsEqual(denominator, 0.0f))
        {
            // line is parallel to the plane
            return false;
        }

        float numerator = glm::dot(plane_normal, line_position - plane_position);
        float u = numerator / denominator;

        intersection_position = line_direction * u + line_position;
        return true;
    }
}
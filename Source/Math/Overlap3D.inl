


namespace Project001
{
    // Checking Point Overlap --------------------------------------------------

    inline bool Check3D_Point_Point_Overlap(
        const glm::vec3& pointA_position,
        const glm::vec3& pointB_position)
    {
        return FloatEqualToFloat(pointA_position.x, pointB_position.x) &&
            FloatEqualToFloat(pointA_position.y, pointB_position.y) &&
            FloatEqualToFloat(pointA_position.z, pointB_position.z);
    }

    inline bool Check3D_Point_Line_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& line_position,
        const glm::vec3& line_direction)
    {
        if (Check3D_Point_Point_Overlap(point_position, line_position))
        {
            return true;
        }

        glm::vec3 line_to_point = glm::normalize(point_position - line_position);
        glm::vec3 point_to_line = -line_to_point;

        return (FloatEqualToFloat(line_direction.x, line_to_point.x) &&
            FloatEqualToFloat(line_direction.y, line_to_point.y) &&
            FloatEqualToFloat(line_direction.z, line_to_point.z)) ||
            (FloatEqualToFloat(line_direction.x, point_to_line.x) &&
            FloatEqualToFloat(line_direction.y, point_to_line.y) &&
            FloatEqualToFloat(line_direction.z, point_to_line.z));
    }

    inline bool Check3D_Point_Ray_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction)
    {
        if (Check3D_Point_Point_Overlap(point_position, ray_position))
        {
            return true;
        }

        glm::vec3 ray_to_point = glm::normalize(point_position - ray_position);

        return FloatEqualToFloat(ray_direction.x, ray_to_point.x) &&
            FloatEqualToFloat(ray_direction.y, ray_to_point.y) &&
            FloatEqualToFloat(ray_direction.z, ray_to_point.z);

        // float dotProduct = glm::dot(rayToPoint, ray_direction);
        // return FloatsEqual(dotProduct, 1.0f);
        // the dot product of two normal vectors pointing in the same direction
        // is 1.0
    }

    inline bool Check3D_Point_LineSegment_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        if (!Check3D_Point_Box_Overlap_H(point_position, lineSegment_start, lineSegment_end))
        {
            // the point is outside of the recangular range of the lineSegment
            return false;
        }

        glm::vec3 lineSegment_direction = glm::normalize(lineSegment_end - lineSegment_start);
        return Check3D_Point_Line_Overlap(point_position, lineSegment_start, lineSegment_direction);
    }

    inline bool Check3D_Point_Plane_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& plane_normal,
        const float& plane_distance)
    {
        float dot_product = glm::dot(point_position, plane_normal);
        // (dot_product - plane_distance) == 0.0f
        // dot_product == plane_distance
        return FloatEqualToFloat(dot_product, plane_distance);
    }

    inline bool Check3D_Point_Triangle_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner2))
        {
            if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3))
            {
                // triangle is really a point
                return Check3D_Point_Point_Overlap(point_position, triangle_corner1);
            }
            else
            {
                // triangle is really a line segment
                return Check3D_Point_LineSegment_Overlap(point_position, triangle_corner1, triangle_corner3);
            }
        }
        else if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3) ||
            Check3D_Point_Point_Overlap(triangle_corner2, triangle_corner3))
        {
            // triangle is really a line segment
            return Check3D_Point_LineSegment_Overlap(point_position, triangle_corner1, triangle_corner2);
        }
        else
        {
            return Check3D_Point_Triangle_Overlap_H(point_position, triangle_corner1, triangle_corner2, triangle_corner3);
        }
    }

    inline bool Check3D_Point_AABB_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max)
    {
        return FloatLessThanOrEqualToFloat(point_position.x, aabb_max.x) &&
            FloatGreaterThanOrEqualToFloat(point_position.x, aabb_min.x) &&
            FloatLessThanOrEqualToFloat(point_position.y, aabb_max.y) &&
            FloatGreaterThanOrEqualToFloat(point_position.y, aabb_min.y) &&
            FloatLessThanOrEqualToFloat(point_position.z, aabb_max.z) &&
            FloatGreaterThanOrEqualToFloat(point_position.z, aabb_min.z);
    }

    inline bool Check3D_Point_OBB_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation)
    {
        glm::vec3 translated_point_position = point_position - obb_position;
        glm::quat inverse_orientation = glm::inverse(obb_orientation);
        glm::vec3 rotated_point_position = inverse_orientation * translated_point_position;

        return Check3D_Point_AABB_Overlap(rotated_point_position, -1.0f * obb_halfSize, obb_halfSize);
    }

    inline bool Check3D_Point_Sphere_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& sphere_position,
        const float& sphere_radius)
    {
        glm::vec3 sphereToPoint = point_position - sphere_position;
        float distanceSquared = glm::dot(sphereToPoint, sphereToPoint);
        float sphereRadiusSquared = sphere_radius * sphere_radius;
        return FloatLessThanOrEqualToFloat(distanceSquared, sphereRadiusSquared);
    }

    inline bool Check3D_Point_Capsule_Overlap(
        const glm::vec3& point_position,
        const glm::vec3& capsule_start,
        const glm::vec3& capsule_end,
        const float& capsule_radius)
    {
        glm::vec3 closestPointOnLineSegment;
        Get3D_Point_LineSegment_ClosestPoint(point_position, capsule_start, capsule_end, closestPointOnLineSegment);
        glm::vec3 pointToClosestPoint = closestPointOnLineSegment - point_position;
        float distanceSquared = glm::dot(pointToClosestPoint, pointToClosestPoint);
        float capsuleRadiusSquared = capsule_radius * capsule_radius;
        return FloatLessThanOrEqualToFloat(distanceSquared, capsuleRadiusSquared);
    }

    inline bool Check3D_Point_Frustum_Overlap(
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
        const float& frustum_farPlaneDistance)
    {
        float distance;

        Get3D_Point_Plane_Distance(point_position, frustum_nearPlaneNormal, frustum_nearPlaneDistance, distance);
        if (distance < 0.0f)
        {
            return false;
        }

        Get3D_Point_Plane_Distance(point_position, frustum_farPlaneNormal, frustum_farPlaneDistance, distance);
        if (distance < 0.0f)
        {
            return false;
        }

        Get3D_Point_Plane_Distance(point_position, frustum_rightPlaneNormal, frustum_rightPlaneDistance, distance);
        if (distance < 0.0f)
        {
            return false;
        }

        Get3D_Point_Plane_Distance(point_position, frustum_leftPlaneNormal, frustum_leftPlaneDistance, distance);
        if (distance < 0.0f)
        {
            return false;
        }

        Get3D_Point_Plane_Distance(point_position, frustum_topPlaneNormal, frustum_topPlaneDistance, distance);
        if (distance < 0.0f)
        {
            return false;
        }

        Get3D_Point_Plane_Distance(point_position, frustum_bottomPlaneNormal, frustum_bottomPlaneDistance, distance);
        if (distance < 0.0f)
        {
            return false;
        }

        return true;
    }

    // Checking Line Overlap ---------------------------------------------------

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

        if (!FloatEqualToFloat(dotProduct, 0.0f))
        {
            // lines are not coplanar
            return false;
        }

        if (FloatEqualToFloat(lineA_position.x, lineB_position.x) &&
            FloatEqualToFloat(lineA_position.y, lineB_position.y) &&
            FloatEqualToFloat(lineA_position.z, lineB_position.z))
        {
            // points are the same
            return true;
        }

        glm::vec3 crossProduct = glm::cross(lineA_direction, lineB_direction);
        float crossProductMagnitudeSquared = glm::dot(crossProduct, crossProduct);
        if (FloatEqualToFloat(crossProductMagnitudeSquared,0.0))
        {
            // lines are parallel
            return false;
        }

        // lines are coplanar and not parallel so they must intersect
        return true;
    }

    inline bool Check3D_Line_Ray_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction)
    {
        // p = d1 * t1 + o1
        // p = d2 * t2 + o2
        const glm::vec3& o1 = line_position;
        const glm::vec3& d1 = line_direction;
        const glm::vec3& o2 = ray_position;
        const glm::vec3& d2 = ray_direction;

        glm::vec3 o1_to_o2 = o2 - o1;
        glm::vec3 n = glm::cross(d1, o1_to_o2);
        float dotProduct = glm::dot(n, d2);
        if (!FloatEqualToFloat(dotProduct, 0.0f))
        {
            // line and ray are not coplanar
            return false;
        }

        glm::vec3 d1_X_d2 = glm::cross(d1, d2);
        float denominator = glm::dot(d1_X_d2, d1_X_d2);
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            if (Check3D_Point_Line_Overlap(ray_position, line_position, line_direction))
            {
                // line and ray are on top of each other
                return true;
            }

            // line and ray are just parallel
            return false;
        }

        // t2 = determinant((o2 - o1),d1,d1 X d2) / ||d1 X d2||^2
        float t2 = glm::determinant(glm::mat3(o1_to_o2, d1, d1_X_d2)) / denominator;

        // intersectionPoint = d2 * t2 + o2;
        // a negative t2 would mean the intersection point is behind the ray,
        // in the opposite direction to the ray's direction
        return FloatGreaterThanOrEqualToFloat(t2, 0.0f);
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
        if (!FloatEqualToFloat(dotProduct, 0.0f))
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
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            if (Check3D_Point_Line_Overlap(lineSegment_start, line_position, line_direction))
            {
                // line and lineSegment are on top of each other
                return true;
            }

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
        const glm::vec3& plane_normal,
        const float& plane_distance)
    {
        if (Check3D_Point_Plane_Overlap(line_position, plane_normal, plane_distance))
        {
            // line is on the plane
            return true;
        }

        float dotProduct = glm::dot(line_direction, plane_normal);

        // If dotProduct == 0, line is parallel to the plane
        return !FloatEqualToFloat(dotProduct, 0.0f);
    }

    inline bool Check3D_Line_Triangle_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner2))
        {
            if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3))
            {
                // triangle is really a point
                return Check3D_Point_Line_Overlap(triangle_corner1, line_position, line_direction);
            }
            else
            {
                // triangle is really a line segment
                return Check3D_LineSegment_Line_Overlap(triangle_corner1, triangle_corner3, line_position, line_direction);
            }
        }
        else if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3) ||
            Check3D_Point_Point_Overlap(triangle_corner2, triangle_corner3))
        {
            // triangle is really a line segment
            return Check3D_LineSegment_Line_Overlap(triangle_corner1, triangle_corner2, line_position, line_direction);
        }

        glm::vec3 plane_normal;
        float plane_distance;
        Get3D_Triangle_ContainingPlane_H(
            triangle_corner1,
            triangle_corner2,
            triangle_corner3,
            plane_normal,
            plane_distance);

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
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            // line is parallel to the plane
            return Check3D_Line_LineSegment_Overlap(line_position, line_direction, triangle_corner1, triangle_corner2) ||
                Check3D_Line_LineSegment_Overlap(line_position, line_direction, triangle_corner2, triangle_corner3) ||
                Check3D_Line_LineSegment_Overlap(line_position, line_direction, triangle_corner3, triangle_corner1);
        }
        else
        {
            const glm::vec3 plane_position = triangle_corner1;
            float numerator = glm::dot(plane_normal, plane_position - line_position);
            float u = numerator / denominator;

            glm::vec3 intersection_position = line_direction * u + line_position;
            return Check3D_Point_Triangle_Overlap_H(
                intersection_position,
                triangle_corner1, triangle_corner2, triangle_corner3);
        }
    }

    inline bool Check3D_Line_AABB_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max)
    {
        float hit_scalar_largest_min;
        float hit_scalar_smallest_max;
        return Linecast3D_AABB(line_position, line_direction, aabb_min, aabb_max, hit_scalar_largest_min, hit_scalar_smallest_max);
    }

    inline bool Check3D_Line_OBB_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation)
    {
        float hit_scalar_largest_min;
        float hit_scalar_smallest_max;
        return Linecast3D_OBB(line_position, line_direction, obb_halfSize, obb_position, obb_orientation, hit_scalar_largest_min, hit_scalar_smallest_max);
    }

    inline bool Check3D_Line_Sphere_Overlap(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& sphere_position,
        const float& sphere_radius)
    {
        glm::vec3 closestPoint_position;
        Get3D_Point_Line_ClosestPoint(sphere_position, line_position, line_direction, closestPoint_position);
        return Check3D_Point_Sphere_Overlap(closestPoint_position, sphere_position, sphere_radius);
    }

    // Checking Ray Overlap ----------------------------------------------------

    inline bool Check3D_Ray_Point_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& point_position)
    {
        return Check3D_Point_Ray_Overlap(point_position, ray_position, ray_direction);
    }

    inline bool Check3D_Ray_Line_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& line_position,
        const glm::vec3& line_direction)
    {
        return Check3D_Line_Ray_Overlap(line_position, line_direction, ray_position, ray_direction);
    }

    inline bool Check3D_Ray_Ray_Overlap(
        const glm::vec3& rayA_position,
        const glm::vec3& rayA_direction,
        const glm::vec3& rayB_position,
        const glm::vec3& rayB_direction)
    {
        // p = d1 * t1 + o1
        // p = d2 * t2 + o2
        const glm::vec3& o1 = rayA_position;
        const glm::vec3& d1 = rayA_direction;
        const glm::vec3& o2 = rayB_position;
        const glm::vec3& d2 = rayB_direction;

        glm::vec3 o1_to_o2 = o2 - o1;
        glm::vec3 n = glm::cross(d1, o1_to_o2);
        float dotProduct = glm::dot(n, d2);
        if (!FloatEqualToFloat(dotProduct, 0.0f))
        {
            // rays are not coplanar
            return false;
        }

        glm::vec3 d1_X_d2 = glm::cross(d1, d2);
        float denominator = glm::dot(d1_X_d2, d1_X_d2);
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            // rays are parallel
            return Check3D_Point_Ray_Overlap(rayB_position, rayA_position, rayA_direction) ||
                Check3D_Point_Ray_Overlap(rayA_position, rayB_position, rayB_direction);
        }

        // t1 = determinant((o2 - o1),d2,d1 X d2) / ||d1 X d2||^2
        float t1 = glm::determinant(glm::mat3(o1_to_o2, d2, d1_X_d2)) / denominator;
        // t2 = determinant((o2 - o1),d1,d1 X d2) / ||d1 X d2||^2
        float t2 = glm::determinant(glm::mat3(o1_to_o2, d1, d1_X_d2)) / denominator;

        // intersectionPointA_position = d1 * t1 + o1;
        // intersectionPointB_position = d2 * t2 + o2;
        return FloatGreaterThanOrEqualToFloat(t1, 0.0f) && FloatGreaterThanOrEqualToFloat(t2, 0.0f);
    }

    inline bool Check3D_Ray_LineSegment_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        glm::vec3 startToStart = ray_position - lineSegment_start;

        glm::vec3 n = glm::cross(ray_direction, startToStart);
        float dotProduct = glm::dot(n, ray_direction);
        if (!FloatEqualToFloat(dotProduct, 0.0f))
        {
            // ray and lineSegment are not coplanar
            return false;
        }

        // p = d1 * t1 + o1
        // p = d2 * t2 + o2
        const glm::vec3& o1 = ray_position;
        const glm::vec3& d1 = ray_direction;
        const glm::vec3& o2 = lineSegment_start;
        glm::vec3 d2 = lineSegment_end - lineSegment_start;

        glm::vec3 d1_X_d2 = glm::cross(d1, d2);
        float denominator = glm::dot(d1_X_d2, d1_X_d2);
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            // ray and lineSegment are parallel
            return Check3D_Point_Ray_Overlap(lineSegment_start, ray_position, ray_direction) ||
                Check3D_Point_Ray_Overlap(lineSegment_end, ray_position, ray_direction);
        }

        glm::vec3 o1_to_o2 = o2 - o1;
        // t1 = determinant((o2 - o1),d2,d1 X d2) / ||d1 X d2||^2
        float t1 = glm::determinant(glm::mat3(o1_to_o2, d2, d1_X_d2)) / denominator;

        if (FloatGreaterThanOrEqualToFloat(t1, 0.0f))
        {
            glm::vec3 p = d1 * t1 + o1;

            // Check if the intersection point is within the lineSegemt's range
            return Check3D_Point_Box_Overlap_H(p, lineSegment_start, lineSegment_end);
        }
        else
        {
            // intersection point is behind the ray
            return false;
        }
    }

    inline bool Check3D_Ray_Plane_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& plane_normal,
        const float& plane_distance)
    {
        if (Check3D_Point_Plane_Overlap(ray_position, plane_normal, plane_distance))
        {
            // ray is on the plane
            return true;
        }

        float nd = glm::dot(ray_direction, plane_normal);
        float pn = glm::dot(ray_position, plane_normal);

        // if nd == 0, ray is parallel to the plane
        // if nd > 0, ray is hitting plane from below
        // if nd < 0, ray is hitting plane from above
        if (FloatEqualToFloat(nd, 0.0f))
        {
            return false;
        }

        float hit_scalar = (plane_distance - pn) / nd;

        return FloatGreaterThanOrEqualToFloat(hit_scalar, 0.0f);
    }

    inline bool Check3D_Ray_Triangle_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner2))
        {
            if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3))
            {
                // triangle is really a point
                return Check3D_Point_Ray_Overlap(triangle_corner1, ray_position, ray_direction);
            }
            else
            {
                // triangle is really a line segment
                return Check3D_LineSegment_Ray_Overlap(triangle_corner1, triangle_corner3, ray_position, ray_direction);
            }
        }
        else if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3) ||
            Check3D_Point_Point_Overlap(triangle_corner2, triangle_corner3))
        {
            // triangle is really a line segment
            return Check3D_LineSegment_Ray_Overlap(triangle_corner1, triangle_corner2, ray_position, ray_direction);
        }


        glm::vec3 plane_normal;
        float plane_distance;
        Get3D_Triangle_ContainingPlane_H(triangle_corner1, triangle_corner2, triangle_corner3, plane_normal, plane_distance);

        float nd = glm::dot(ray_direction, plane_normal);
        float pn = glm::dot(ray_position, plane_normal);

        // if nd == 0, ray is parallel to the plane
        // if nd > 0, ray is hitting plane from below
        // if nd < 0, ray is hitting plane from above
        if (FloatEqualToFloat(nd, 0.0f))
        {
            return Check3D_Ray_LineSegment_Overlap(ray_position, ray_direction, triangle_corner1, triangle_corner2) ||
                Check3D_Ray_LineSegment_Overlap(ray_position, ray_direction, triangle_corner2, triangle_corner3) ||
                Check3D_Ray_LineSegment_Overlap(ray_position, ray_direction, triangle_corner3, triangle_corner1);
        }

        float hit_scalar = (plane_distance - pn) / nd;

        if (FloatGreaterThanOrEqualToFloat(hit_scalar, 0.0f))
        {
            glm::vec3 hit_point_position = ray_position + ray_direction * hit_scalar;

            return Check3D_Point_Triangle_Overlap_H(hit_point_position, triangle_corner1, triangle_corner2, triangle_corner3);
        }

        return false;
    }

    inline bool Check3D_Ray_AABB_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max)
    {
        float hit_scalar_largest_min;
        float hit_scalar_smallest_max;
        if (Linecast3D_AABB(ray_position, ray_direction, aabb_min, aabb_max, hit_scalar_largest_min, hit_scalar_smallest_max))
        {
            return hit_scalar_smallest_max > 0.0f;
        }

        return false;
    }

    inline bool Check3D_Ray_OBB_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation)
    {
        float hit_scalar_largest_min;
        float hit_scalar_smallest_max;
        if (Linecast3D_OBB(ray_position, ray_direction, obb_halfSize, obb_position, obb_orientation, hit_scalar_largest_min, hit_scalar_smallest_max))
        {
            return hit_scalar_smallest_max > 0.0f;
        }

        return false;
    }

    inline bool Check3D_Ray_Sphere_Overlap(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& sphere_position,
        const float& sphere_radius)
    {
        glm::vec3 closestPoint_position;
        Get3D_Point_Ray_ClosestPoint(sphere_position, ray_position, ray_direction, closestPoint_position);
        return Check3D_Point_Sphere_Overlap(closestPoint_position, sphere_position, sphere_radius);
    }

    // Checking LineSegment Overlap --------------------------------------------

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

    inline bool Check3D_LineSegment_Ray_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction)
    {
        return Check3D_Ray_LineSegment_Overlap(ray_position, ray_direction, lineSegment_start, lineSegment_end);
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

        if (!FloatEqualToFloat(dotProduct, 0.0f))
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
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            // lineSegments are parallel
            return Check3D_Point_LineSegment_Overlap(lineSegmentA_start, lineSegmentB_start, lineSegmentB_end) ||
                Check3D_Point_LineSegment_Overlap(lineSegmentA_end, lineSegmentB_start, lineSegmentB_end) ||
                Check3D_Point_LineSegment_Overlap(lineSegmentB_start, lineSegmentA_start, lineSegmentA_end);
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
        const glm::vec3& plane_normal,
        const float& plane_distance)
    {
        glm::vec3 lineSegment_direction = glm::normalize(lineSegment_end - lineSegment_start);
        float denominator = glm::dot(plane_normal, lineSegment_direction);
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            // line is parallel to the plane
            return Check3D_Point_Plane_Overlap(lineSegment_start, plane_normal, plane_distance) ||
                Check3D_Point_Plane_Overlap(lineSegment_end, plane_normal, plane_distance);
        }
        else
        {
            glm::vec3 plane_position = plane_normal * plane_distance;
            float numerator = glm::dot(plane_normal, plane_position - lineSegment_start);
            float u = numerator / denominator;

            glm::vec3 intersection_position = lineSegment_direction * u + lineSegment_start;

            // check if the line plane intersection is within the line segment's range
            return Check3D_Point_Box_Overlap_H(intersection_position, lineSegment_start, lineSegment_end);
        }
    }

    inline bool Check3D_LineSegment_Triangle_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner2))
        {
            if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3))
            {
                // triangle is really a point
                return Check3D_Point_LineSegment_Overlap(triangle_corner1, lineSegment_start, lineSegment_end);
            }
            else
            {
                // triangle is really a line segment
                return Check3D_LineSegment_LineSegment_Overlap(triangle_corner1, triangle_corner3, lineSegment_start, lineSegment_end);
            }
        }
        else if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3) ||
            Check3D_Point_Point_Overlap(triangle_corner2, triangle_corner3))
        {
            // triangle is really a line segment
            return Check3D_LineSegment_LineSegment_Overlap(triangle_corner1, triangle_corner2, lineSegment_start, lineSegment_end);
        }

        return Check3D_LineSegment_Triangle_Overlap_H(lineSegment_start, lineSegment_end, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check3D_LineSegment_AABB_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max)
    {
        glm::vec3 lineSegment_direciton = lineSegment_end - lineSegment_start;
        float hit_scalar_largest_min;
        float hit_scalar_smallest_max;
        if (Linecast3D_AABB(lineSegment_start, lineSegment_direciton, aabb_min, aabb_max, hit_scalar_largest_min, hit_scalar_smallest_max))
        {
            return hit_scalar_smallest_max > 0.0f && hit_scalar_largest_min < 1.0f;
        }

        return false;
    }

    inline bool Check3D_LineSegment_OBB_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation)
    {
        glm::vec3 lineSegment_direciton = lineSegment_end - lineSegment_start;
        float hit_scalar_largest_min;
        float hit_scalar_smallest_max;
        if (Linecast3D_OBB(lineSegment_start, lineSegment_direciton, obb_halfSize, obb_position, obb_orientation, hit_scalar_largest_min, hit_scalar_smallest_max))
        {
            return hit_scalar_smallest_max > 0.0f && hit_scalar_largest_min < 1.0f;
        }

        return false;
    }

    inline bool Check3D_LineSegment_Sphere_Overlap(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& sphere_position,
        const float& sphere_radius)
    {
        glm::vec3 closestPoint_position;
        Get3D_Point_LineSegment_ClosestPoint(sphere_position, lineSegment_start, lineSegment_end, closestPoint_position);
        return Check3D_Point_Sphere_Overlap(closestPoint_position, sphere_position, sphere_radius);
    }

    // Checking Plane Overlap --------------------------------------------------

    inline bool Check3D_Plane_Point_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& point_position)
    {
        return Check3D_Point_Plane_Overlap(point_position, plane_normal, plane_distance);
    }

    inline bool Check3D_Plane_Line_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& line_position,
        const glm::vec3& line_direction)
    {
        return Check3D_Line_Plane_Overlap(line_position, line_direction, plane_normal, plane_distance);
    }

    inline bool Check3D_Plane_LineSegment_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        return Check3D_LineSegment_Plane_Overlap(lineSegment_start, lineSegment_end, plane_normal, plane_distance);
    }

    inline bool Check3D_Plane_Plane_Overlap(
        const glm::vec3& planeA_normal,
        const float& planeA_distance,
        const glm::vec3& planeB_normal,
        const float& planeB_distance)
    {
        // This works even with the divide by Zero
        glm::vec3 normal_ratio = planeA_normal / planeB_normal;
        if (FloatEqualToFloat(normal_ratio.x, normal_ratio.y) &&
            FloatEqualToFloat(normal_ratio.x, normal_ratio.z))
        {
            // Planes parallel so they only overlap if they are the same
            return FloatEqualToFloat(planeA_distance, planeB_distance);
        }

        return true;
    }

    inline bool Check3D_Plane_Triangle_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        float distance1;
        Get3D_Point_Plane_Distance(triangle_corner1, plane_normal, plane_distance, distance1);
        if (FloatEqualToFloat(distance1, 0.0f))
        {
            return true;
        }
        float distance2;
        Get3D_Point_Plane_Distance(triangle_corner2, plane_normal, plane_distance, distance2);
        if (FloatEqualToFloat(distance2, 0.0f) || std::signbit(distance1) != std::signbit(distance2))
        {
            return true;
        }
        float distance3;
        Get3D_Point_Plane_Distance(triangle_corner3, plane_normal, plane_distance, distance3);
        return FloatEqualToFloat(distance3, 0.0f) || std::signbit(distance1) != std::signbit(distance3);
    }

    inline bool Check3D_Plane_AABB_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max)
    {
        // project AABB half extents onto plane normal
        glm::vec3 aabb_halfSize = (aabb_max - aabb_min) * 0.5f;
        glm::vec3 aabb_position = (aabb_max + aabb_min) * 0.5f;
        float projection = aabb_halfSize.x * std::abs(plane_normal.x) +
            aabb_halfSize.y * std::abs(plane_normal.y) +
            aabb_halfSize.z * std::abs(plane_normal.z);

        // distance from center of AABB to plane
        float distance = std::abs(glm::dot(plane_normal, aabb_position) - plane_distance);

        return FloatLessThanOrEqualToFloat(distance, projection);
    }

    inline bool Check3D_Plane_OBB_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation)
    {
        glm::mat3 obb_oritentationMatrix = glm::mat3_cast(obb_orientation);
        // glm::mat3 obb_oritentationMatrix =
        // {
        //     obb_orientation * glm::vec3(1.0f, 0.0f, 0.0f),
        //     obb_orientation * glm::vec3(0.0f, 1.0f, 0.0f),
        //     obb_orientation * glm::vec3(0.0f, 0.0f, 1.0f)
        // };

        // project OBB half extents onto plane normal
        float projection = obb_halfSize.x * std::abs(glm::dot(plane_normal, obb_oritentationMatrix[0])) +
            obb_halfSize.y * std::abs(glm::dot(plane_normal, obb_oritentationMatrix[1])) +
            obb_halfSize.z * std::abs(glm::dot(plane_normal, obb_oritentationMatrix[2]));

        // distance from center of OBB to plane
        float distance = std::abs(glm::dot(plane_normal, obb_position) - plane_distance);

        return FloatLessThanOrEqualToFloat(distance, projection);
    }

    inline bool Check3D_Plane_Sphere_Overlap(
        const glm::vec3& plane_normal,
        const float& plane_distance,
        const glm::vec3& sphere_position,
        const float& sphere_radius)
    {
        glm::vec3 closestPoint_position;
        Get3D_Point_Plane_ClosestPoint(sphere_position, plane_normal, plane_distance, closestPoint_position);
        return Check3D_Point_Sphere_Overlap(closestPoint_position, sphere_position, sphere_radius);
    }

    // Checking Triangle Overlap -----------------------------------------------

    inline bool Check3D_Triangle_Point_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& point_position)
    {
        return Check3D_Point_Triangle_Overlap(point_position, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check3D_Triangle_Line_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& line_position,
        const glm::vec3& line_direction)
    {
        return Check3D_Line_Triangle_Overlap(line_position, line_direction, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check3D_Triangle_Ray_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction)
    {
        return Check3D_Ray_Triangle_Overlap(ray_position, ray_direction, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check3D_Triangle_LineSegment_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        return Check3D_LineSegment_Triangle_Overlap(lineSegment_start, lineSegment_end, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check3D_Triangle_Plane_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& plane_normal,
        const float& plane_distance)
    {
        return Check3D_Plane_Triangle_Overlap(plane_normal, plane_distance, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check3D_Triangle_Triangle_Overlap(
        const glm::vec3& triangleA_corner1,
        const glm::vec3& triangleA_corner2,
        const glm::vec3& triangleA_corner3,
        const glm::vec3& triangleB_corner1,
        const glm::vec3& triangleB_corner2,
        const glm::vec3& triangleB_corner3)
    {
        // Check if both triangles are actually triangles
        // ---------------------------------------------------------------------

        if (Check3D_Point_Point_Overlap(triangleA_corner1, triangleA_corner2))
        {
            if (Check3D_Point_Point_Overlap(triangleA_corner1, triangleA_corner3))
            {
                // triangleA is really a point
                return Check3D_Point_Triangle_Overlap(triangleA_corner1, triangleB_corner1, triangleB_corner2, triangleB_corner3);
            }
            else
            {
                // triangleA is really a line segment
                return Check3D_LineSegment_Triangle_Overlap(triangleA_corner1, triangleA_corner3, triangleB_corner1, triangleB_corner2, triangleB_corner3);
            }
        }
        else if (Check3D_Point_Point_Overlap(triangleA_corner1, triangleA_corner3) ||
            Check3D_Point_Point_Overlap(triangleA_corner2, triangleA_corner3))
        {
            // triangleA is really a line segment
            return Check3D_LineSegment_Triangle_Overlap(triangleA_corner1, triangleA_corner2, triangleB_corner1, triangleB_corner2, triangleB_corner3);
        }

        if (Check3D_Point_Point_Overlap(triangleB_corner1, triangleB_corner2))
        {
            if (Check3D_Point_Point_Overlap(triangleB_corner1, triangleB_corner3))
            {
                // triangleB is really a point
                return Check3D_Point_Triangle_Overlap_H(triangleB_corner1, triangleA_corner1, triangleA_corner2, triangleA_corner3);
            }
            else
            {
                // triangleB is really a line segment
                return Check3D_LineSegment_Triangle_Overlap_H(triangleB_corner1, triangleB_corner3, triangleA_corner1, triangleA_corner2, triangleA_corner3);
            }
        }
        else if (Check3D_Point_Point_Overlap(triangleB_corner1, triangleB_corner3) ||
            Check3D_Point_Point_Overlap(triangleB_corner2, triangleB_corner3))
        {
            // triangleB is really a line segment
            return Check3D_LineSegment_Triangle_Overlap_H(triangleB_corner1, triangleB_corner2, triangleA_corner1, triangleA_corner2, triangleA_corner3);
        }

        glm::vec3 testAxes[11] = {
            SAT_Cross(triangleA_corner1, triangleA_corner2, triangleA_corner2, triangleA_corner3), // Triangle A Normal
            SAT_Cross(triangleB_corner1, triangleB_corner2, triangleB_corner2, triangleB_corner3), // Triangle B Normal
            SAT_Cross(triangleB_corner1, triangleB_corner2, triangleA_corner1, triangleA_corner2),
            SAT_Cross(triangleB_corner1, triangleB_corner2, triangleA_corner2, triangleA_corner3),
            SAT_Cross(triangleB_corner1, triangleB_corner2, triangleA_corner3, triangleA_corner1),
            SAT_Cross(triangleB_corner2, triangleB_corner3, triangleA_corner1, triangleA_corner2),
            SAT_Cross(triangleB_corner2, triangleB_corner3, triangleA_corner2, triangleA_corner3),
            SAT_Cross(triangleB_corner2, triangleB_corner3, triangleA_corner3, triangleA_corner1),
            SAT_Cross(triangleB_corner3, triangleB_corner1, triangleA_corner1, triangleA_corner2),
            SAT_Cross(triangleB_corner3, triangleB_corner1, triangleA_corner2, triangleA_corner3),
            SAT_Cross(triangleB_corner3, triangleB_corner1, triangleA_corner3, triangleA_corner1)
        };

        for (size_t i = 0; i < 11; ++i)
        {
            float intervalA_min;
            float intervalA_max;
            Get3D_Triangle_AxisInterval(triangleA_corner1, triangleA_corner2, triangleA_corner3, testAxes[i], intervalA_min, intervalA_max);
            float intervalB_min;
            float intervalB_max;
            Get3D_Triangle_AxisInterval(triangleB_corner1, triangleB_corner2, triangleB_corner3, testAxes[i], intervalB_min, intervalB_max);

            if ((intervalB_min > intervalA_max) || (intervalA_min > intervalB_max))
            {
                if (!(FloatEqualToFloat(intervalB_min, intervalA_max) || FloatEqualToFloat(intervalA_min, intervalB_max)))
                {
                    return false;
                }
            }
        }

        return true;
    }

    inline bool Check3D_Triangle_AABB_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max)
    {
        if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner2))
        {
            if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3))
            {
                // triangle is really a point
                return Check3D_Point_AABB_Overlap(triangle_corner1, aabb_min, aabb_max);
            }
            else
            {
                // triangle is really a line segment
                return Check3D_LineSegment_AABB_Overlap(triangle_corner1, triangle_corner3, aabb_min, aabb_max);
            }
        }
        else if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3) ||
            Check3D_Point_Point_Overlap(triangle_corner2, triangle_corner3))
        {
            // triangle is really a line segment
            return Check3D_LineSegment_AABB_Overlap(triangle_corner1, triangle_corner2, aabb_min, aabb_max);
        }

        // triangle sides
        glm::vec3 f0 = triangle_corner2 - triangle_corner1;
        glm::vec3 f1 = triangle_corner3 - triangle_corner2;
        glm::vec3 f2 = triangle_corner1 - triangle_corner3;

        // AABB face normals
        glm::vec3 u0(1.0f, 0.0f, 0.0f);
        glm::vec3 u1(0.0f, 1.0f, 0.0f);
        glm::vec3 u2(0.0f, 0.0f, 1.0f);

        glm::vec3 testAxes[13] = {
            u0, // AABB Axis 1
            u1, // AABB Axis 2
            u2, // AABB Axis 3
            glm::cross(f0, f1), // triangle normal
            glm::cross(u0, f0),
            glm::cross(u0, f1),
            glm::cross(u0, f2),
            glm::cross(u1, f0),
            glm::cross(u1, f1),
            glm::cross(u1, f2),
            glm::cross(u2, f0),
            glm::cross(u2, f1),
            glm::cross(u2, f2)
        };

        for (size_t i = 0; i < 13; ++i)
        {
            float intervalA_min;
            float intervalA_max;
            Get3D_Triangle_AxisInterval(triangle_corner1, triangle_corner2, triangle_corner3, testAxes[i], intervalA_min, intervalA_max);
            float intervalB_min;
            float intervalB_max;
            Get3D_AABB_AxisInterval(aabb_min, aabb_max, testAxes[i], intervalB_min, intervalB_max);

            if ((intervalB_min > intervalA_max) || (intervalA_min > intervalB_max))
            {
                if (!(FloatEqualToFloat(intervalB_min, intervalA_max) || FloatEqualToFloat(intervalA_min, intervalB_max)))
                {
                    return false;
                }
            }
        }

        return true;
    }

    inline bool Check3D_Triangle_OBB_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation)
    {
        if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner2))
        {
            if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3))
            {
                // triangle is really a point
                return Check3D_Point_OBB_Overlap(triangle_corner1, obb_halfSize, obb_position, obb_orientation);
            }
            else
            {
                // triangle is really a line segment
                return Check3D_LineSegment_OBB_Overlap(triangle_corner1, triangle_corner3, obb_halfSize, obb_position, obb_orientation);
            }
        }
        else if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3) ||
            Check3D_Point_Point_Overlap(triangle_corner2, triangle_corner3))
        {
            // triangle is really a line segment
            return Check3D_LineSegment_OBB_Overlap(triangle_corner1, triangle_corner2, obb_halfSize, obb_position, obb_orientation);
        }

        // triangle sides
        glm::vec3 f0 = triangle_corner2 - triangle_corner1;
        glm::vec3 f1 = triangle_corner3 - triangle_corner2;
        glm::vec3 f2 = triangle_corner1 - triangle_corner3;

        // OBB face normals
        glm::vec3 u0 = obb_orientation * glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 u1 = obb_orientation * glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 u2 = obb_orientation * glm::vec3(0.0f, 0.0f, 1.0f);

        glm::vec3 testAxes[13] = {
            u0, // OBB Axis 1
            u1, // OBB Axis 2
            u2, // OBB Axis 3
            glm::cross(f0, f1), // triangle normal
            glm::cross(u0, f0),
            glm::cross(u0, f1),
            glm::cross(u0, f2),
            glm::cross(u1, f0),
            glm::cross(u1, f1),
            glm::cross(u1, f2),
            glm::cross(u2, f0),
            glm::cross(u2, f1),
            glm::cross(u2, f2)
        };

        for (size_t i = 0; i < 13; ++i)
        {
            float intervalA_min;
            float intervalA_max;
            Get3D_Triangle_AxisInterval(triangle_corner1, triangle_corner2, triangle_corner3, testAxes[i], intervalA_min, intervalA_max);
            float intervalB_min;
            float intervalB_max;
            Get3D_OBB_AxisInterval(obb_halfSize, obb_position, obb_orientation, testAxes[i], intervalB_min, intervalB_max);

            if ((intervalB_min > intervalA_max) || (intervalA_min > intervalB_max))
            {
                if (!(FloatEqualToFloat(intervalB_min, intervalA_max) || FloatEqualToFloat(intervalA_min, intervalB_max)))
                {
                    return false;
                }
            }
        }

        return true;
    }

    inline bool Check3D_Triangle_Sphere_Overlap(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& sphere_position,
        const float& sphere_radius)
    {
        glm::vec3 closestPoint_position;
        Get3D_Point_Triangle_ClosestPoint(sphere_position, triangle_corner1, triangle_corner2, triangle_corner3, closestPoint_position);
        return Check3D_Point_Sphere_Overlap(closestPoint_position, sphere_position, sphere_radius);
    }

    // Checking Axis Aligned Bounding Box Overlap ------------------------------

    inline bool Check3D_AABB_Point_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& point_position)
    {
        return Check3D_Point_AABB_Overlap(point_position, aabb_min, aabb_max);
    }

    inline bool Check3D_AABB_Line_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& line_position,
        const glm::vec3& line_direction)
    {
        return Check3D_Line_AABB_Overlap(line_position, line_direction, aabb_min, aabb_max);
    }

    inline bool Check3D_AABB_Ray_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction)
    {
        return Check3D_Ray_AABB_Overlap(ray_position, ray_direction, aabb_min, aabb_max);
    }

    inline bool Check3D_AABB_LineSegment_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        return Check3D_LineSegment_AABB_Overlap(lineSegment_start, lineSegment_end, aabb_min, aabb_max);
    }

    inline bool Check3D_AABB_Plane_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& plane_normal,
        const float& plane_distance)
    {
        return Check3D_Plane_AABB_Overlap(plane_normal, plane_distance, aabb_min, aabb_max);
    }

    inline bool Check3D_AABB_Triangle_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        return Check3D_Triangle_AABB_Overlap(triangle_corner1, triangle_corner2, triangle_corner3, aabb_min, aabb_max);
    }

    inline bool Check3D_AABB_AABB_Overlap(
        const glm::vec3& aabbA_min,
        const glm::vec3& aabbA_max,
        const glm::vec3& aabbB_min,
        const glm::vec3& aabbB_max)
    {
        // Note:
        // Can't decide whether to use FloatLessThanOrEqualToFloat or just '<'
        return FloatLessThanOrEqualToFloat(aabbA_min.x, aabbB_max.x) &&
            FloatGreaterThanOrEqualToFloat(aabbA_max.x, aabbB_min.x) &&
            FloatLessThanOrEqualToFloat(aabbA_min.y, aabbB_max.y) &&
            FloatGreaterThanOrEqualToFloat(aabbA_max.y, aabbB_min.y) &&
            FloatLessThanOrEqualToFloat(aabbA_min.z, aabbB_max.z) &&
            FloatGreaterThanOrEqualToFloat(aabbA_max.z, aabbB_min.z);
    }

    inline bool Check3D_AABB_OBB_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation)
    {
        glm::vec3 testAxes[15] = {
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            obb_orientation * glm::vec3(1.0f, 0.0f, 0.0f),
            obb_orientation * glm::vec3(0.0f, 1.0f, 0.0f),
            obb_orientation * glm::vec3(0.0f, 0.0f, 1.0f)
        };

        for (size_t i = 0; i < 3; ++i)
        {
            testAxes[6 + i * 3] = glm::cross(testAxes[i], testAxes[0]);
            testAxes[6 + i * 3 + 1] = glm::cross(testAxes[i], testAxes[1]);
            testAxes[6 + i * 3 + 2] = glm::cross(testAxes[i], testAxes[2]);
        }

        for (size_t i = 0; i < 15; ++i)
        {
            float intervalA_min;
            float intervalA_max;
            Get3D_AABB_AxisInterval(aabb_min, aabb_max, testAxes[i], intervalA_min, intervalA_max);
            float intervalB_min;
            float intervalB_max;
            Get3D_OBB_AxisInterval(obb_halfSize, obb_position, obb_orientation, testAxes[i], intervalB_min, intervalB_max);

            if ((intervalB_min > intervalA_max) || (intervalA_min > intervalB_max))
            {
                return false;
            }
        }

        return true;
    }

    inline bool Check3D_AABB_Sphere_Overlap(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& sphere_position,
        const float& sphere_radius)
    {
        glm::vec3 closestPoint_position;
        Get3D_Point_AABB_ClosestPoint(sphere_position, aabb_min, aabb_max, closestPoint_position);
        return Check3D_Point_Sphere_Overlap(closestPoint_position, sphere_position, sphere_radius);
    }

    // Checking Oriented Bounding Box Overlap ----------------------------------

    inline bool Check3D_OBB_Point_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& point_position)
    {
        return Check3D_Point_OBB_Overlap(point_position, obb_halfSize, obb_position, obb_orientation);
    }

    inline bool Check3D_OBB_Line_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& line_position,
        const glm::vec3& line_direction)
    {
        return Check3D_Line_OBB_Overlap(line_position, line_direction, obb_halfSize, obb_position, obb_orientation);
    }

    inline bool Check3D_OBB_Ray_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction)
    {
        return Check3D_Ray_OBB_Overlap(ray_position, ray_direction, obb_halfSize, obb_position, obb_orientation);
    }

    inline bool Check3D_OBB_LineSegment_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        return Check3D_LineSegment_OBB_Overlap(lineSegment_start, lineSegment_end, obb_halfSize, obb_position, obb_orientation);
    }

    inline bool Check3D_OBB_Plane_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& plane_normal,
        const float& plane_distance)
    {
        return Check3D_Plane_OBB_Overlap(plane_normal, plane_distance, obb_halfSize, obb_position, obb_orientation);
    }

    inline bool Check3D_OBB_Triangle_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        return Check3D_Triangle_OBB_Overlap(triangle_corner1, triangle_corner2, triangle_corner3, obb_halfSize, obb_position, obb_orientation);
    }

    inline bool Check3D_OBB_AABB_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max)
    {
        return Check3D_AABB_OBB_Overlap(aabb_min, aabb_max, obb_halfSize, obb_position, obb_orientation);
    }

    inline bool Check3D_OBB_OBB_Overlap(
        const glm::vec3& obbA_halfSize,
        const glm::vec3& obbA_position,
        const glm::quat& obbA_orientation,
        const glm::vec3& obbB_halfSize,
        const glm::vec3& obbB_position,
        const glm::quat& obbB_orientation)
    {
        glm::vec3 testAxes[15] = {
            obbA_orientation * glm::vec3(1.0f, 0.0f, 0.0f),
            obbA_orientation * glm::vec3(0.0f, 1.0f, 0.0f),
            obbA_orientation * glm::vec3(0.0f, 0.0f, 1.0f),
            obbB_orientation * glm::vec3(1.0f, 0.0f, 0.0f),
            obbB_orientation * glm::vec3(0.0f, 1.0f, 0.0f),
            obbB_orientation * glm::vec3(0.0f, 0.0f, 1.0f)
        };

        for (size_t i = 0; i < 3; ++i)
        {
            testAxes[6 + i * 3] = glm::cross(testAxes[i], testAxes[0]);
            testAxes[6 + i * 3 + 1] = glm::cross(testAxes[i], testAxes[1]);
            testAxes[6 + i * 3 + 2] = glm::cross(testAxes[i], testAxes[2]);
        }

        for (size_t i = 0; i < 15; ++i)
        {
            float intervalA_min;
            float intervalA_max;
            Get3D_OBB_AxisInterval(obbA_halfSize, obbA_position, obbA_orientation, testAxes[i], intervalA_min, intervalA_max);
            float intervalB_min;
            float intervalB_max;
            Get3D_OBB_AxisInterval(obbB_halfSize, obbB_position, obbB_orientation, testAxes[i], intervalB_min, intervalB_max);

            if ((intervalB_min > intervalA_max) || (intervalA_min > intervalB_max))
            {
                return false;
            }
        }

        return true;
    }

    inline bool Check3D_OBB_Sphere_Overlap(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& sphere_position,
        const float& sphere_radius)
    {
        glm::vec3 closestPoint_position;
        Get3D_Point_OBB_ClosestPoint(sphere_position, obb_halfSize, obb_position, obb_orientation, closestPoint_position);
        return Check3D_Point_Sphere_Overlap(closestPoint_position, sphere_position, sphere_radius);
    }

    // Checking Sphere Overlap -------------------------------------------------

    inline bool Check3D_Sphere_Point_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& point_position)
    {
        return Check3D_Point_Sphere_Overlap(point_position, sphere_position, sphere_radius);
    }

    inline bool Check3D_Sphere_Line_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& line_position,
        const glm::vec3& line_direction)
    {
        return Check3D_Line_Sphere_Overlap(line_position, line_direction, sphere_position, sphere_radius);
    }

    inline bool Check3D_Sphere_Ray_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction)
    {
        return Check3D_Ray_Sphere_Overlap(ray_position, ray_direction, sphere_position, sphere_radius);
    }

    inline bool Check3D_Sphere_LineSegment_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        return Check3D_LineSegment_Sphere_Overlap(lineSegment_start, lineSegment_end, sphere_position, sphere_radius);
    }

    inline bool Check3D_Sphere_Plane_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& plane_normal,
        const float& plane_distance)
    {
        return Check3D_Plane_Sphere_Overlap(plane_normal, plane_distance, sphere_position, sphere_radius);
    }

    inline bool Check3D_Sphere_Triangle_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        return Check3D_Triangle_Sphere_Overlap(triangle_corner1, triangle_corner2, triangle_corner3, sphere_position, sphere_radius);
    }

    inline bool Check3D_Sphere_AABB_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max)
    {
        return Check3D_AABB_Sphere_Overlap(aabb_min, aabb_max, sphere_position, sphere_radius);
    }

    inline bool Check3D_Sphere_OBB_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation)
    {
        return Check3D_OBB_Sphere_Overlap(obb_halfSize, obb_position, obb_orientation, sphere_position, sphere_radius);
    }

    inline bool Check3D_Sphere_Sphere_Overlap(
        const glm::vec3& sphereA_position,
        const float& sphereA_radius,
        const glm::vec3& sphereB_position,
        const float& sphereB_radius)
    {
        return Check3D_Point_Sphere_Overlap(sphereA_position, sphereB_position, sphereA_radius + sphereB_radius);
    }

    inline bool Check3D_Sphere_Capsule_Overlap(
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        const glm::vec3& capsule_start,
        const glm::vec3& capsule_end,
        const float& capsule_radius)
    {
        return Check3D_LineSegment_Sphere_Overlap(capsule_start, capsule_end, sphere_position, sphere_radius + capsule_radius);
    }

    inline bool Check3D_Sphere_Frustum_Overlap(
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
        const float& frustum_farPlaneDistance)
    {
        float distance;

        Get3D_Point_Plane_Distance(sphere_position, frustum_nearPlaneNormal, frustum_nearPlaneDistance, distance);
        if (distance < -sphere_radius)
        {
            return false;
        }

        Get3D_Point_Plane_Distance(sphere_position, frustum_farPlaneNormal, frustum_farPlaneDistance, distance);
        if (distance < -sphere_radius)
        {
            return false;
        }

        Get3D_Point_Plane_Distance(sphere_position, frustum_rightPlaneNormal, frustum_rightPlaneDistance, distance);
        if (distance < -sphere_radius)
        {
            return false;
        }

        Get3D_Point_Plane_Distance(sphere_position, frustum_leftPlaneNormal, frustum_leftPlaneDistance, distance);
        if (distance < -sphere_radius)
        {
            return false;
        }

        Get3D_Point_Plane_Distance(sphere_position, frustum_topPlaneNormal, frustum_topPlaneDistance, distance);
        if (distance < -sphere_radius)
        {
            return false;
        }

        Get3D_Point_Plane_Distance(sphere_position, frustum_bottomPlaneNormal, frustum_bottomPlaneDistance, distance);
        if (distance < -sphere_radius)
        {
            return false;
        }

        return true;
    }

    // Checking Capsule Overlap -------------------------------------------------

    inline bool Check3D_Capsule_Point_Overlap(
        const glm::vec3& capsule_start,
        const glm::vec3& capsule_end,
        const float& capsule_radius,
        const glm::vec3& point_position)
    {
        return Check3D_Point_Capsule_Overlap(point_position, capsule_start, capsule_end, capsule_radius);
    }

    inline bool Check3D_Capsule_Sphere_Overlap(
        const glm::vec3& capsule_start,
        const glm::vec3& capsule_end,
        const float& capsule_radius,
        const glm::vec3& sphere_position,
        const float& sphere_radius)
    {
        return Check3D_Sphere_Capsule_Overlap(sphere_position, sphere_radius, capsule_start, capsule_end, capsule_radius);
    }

    // Checking Frustum Overlap ------------------------------------------------

    inline bool Check3D_Frustum_Point_Overlap(
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
        const glm::vec3& point_position)
    {
        Check3D_Point_Frustum_Overlap(
            point_position,
            frustum_leftPlaneNormal,
            frustum_leftPlaneDistance,
            frustum_rightPlaneNormal,
            frustum_rightPlaneDistance,
            frustum_bottomPlaneNormal,
            frustum_bottomPlaneDistance,
            frustum_topPlaneNormal,
            frustum_topPlaneDistance,
            frustum_nearPlaneNormal,
            frustum_nearPlaneDistance,
            frustum_farPlaneNormal,
            frustum_farPlaneDistance
        );
    }

    inline bool Check3D_Frustum_Sphere_Overlap(
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
        const float& sphere_radius)
    {
        Check3D_Sphere_Frustum_Overlap(
            sphere_position,
            sphere_radius,
            frustum_leftPlaneNormal,
            frustum_leftPlaneDistance,
            frustum_rightPlaneNormal,
            frustum_rightPlaneDistance,
            frustum_bottomPlaneNormal,
            frustum_bottomPlaneDistance,
            frustum_topPlaneNormal,
            frustum_topPlaneDistance,
            frustum_nearPlaneNormal,
            frustum_nearPlaneDistance,
            frustum_farPlaneNormal,
            frustum_farPlaneDistance
        );
    }

    // Getting Closest Point ---------------------------------------------------

    inline void Get3D_Point_Line_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        glm::vec3& closestPoint_position)
    {
        float t = glm::dot(point_position - line_position, line_direction);

        // If line_direction isn't normalized:
        // t /= glm::dot(line_direction, line_direction);

        closestPoint_position = line_position + line_direction * t;
    }

    inline void Get3D_Point_Ray_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        glm::vec3& closestPoint_position)
    {
        float t = glm::dot(point_position - ray_position, ray_direction);

        // If ray_direction isn't normalized:
        // t /= glm::dot(ray_direction, ray_direction);

        t = fmaxf(t, 0.0f);
        closestPoint_position = ray_position + ray_direction * t;
    }

    inline void Get3D_Point_LineSegment_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        glm::vec3& closestPoint_position)
    {
        glm::vec3 lineSegment = lineSegment_end - lineSegment_start;
        float lineSegmentLengthSquared = glm::dot(lineSegment, lineSegment);
        if (FloatEqualToFloat(lineSegmentLengthSquared, 0.0f))
        {
            closestPoint_position = lineSegment_start;
        }
        else
        {
            glm::vec3 startToPoint = point_position - lineSegment_start;
            float t = glm::dot(startToPoint, lineSegment) / lineSegmentLengthSquared;
            t = std::fmaxf(t, 0.0f);
            t = std::fminf(t, 1.0f);
            closestPoint_position = lineSegment_start + lineSegment * t;
        }
    }

    inline void Get3D_Point_Plane_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& plane_normal,
        const float& plane_distance,
        glm::vec3& closestPoint_position)
    {
        float distance;
        Get3D_Point_Plane_Distance(point_position, plane_normal, plane_distance, distance);
        closestPoint_position = point_position - plane_normal * distance;
    }

    inline void Get3D_Point_Triangle_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        glm::vec3& closestPoint_position)
    {
        glm::vec3 plane_normal;
        float plane_distance;

        if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner2))
        {
            if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3))
            {
                // triangle is really a point
                closestPoint_position = triangle_corner1;
            }
            else
            {
                // triangle is really a line segment
                return Get3D_Point_LineSegment_ClosestPoint(point_position, triangle_corner1, triangle_corner3, closestPoint_position);
            }
        }
        else if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner3) ||
            Check3D_Point_Point_Overlap(triangle_corner2, triangle_corner3))
        {
            // triangle is really a line segment
            return Get3D_Point_LineSegment_ClosestPoint(point_position, triangle_corner1, triangle_corner2, closestPoint_position);
        }
        else
        {
            Get3D_Triangle_ContainingPlane_H(
                triangle_corner1,
                triangle_corner2,
                triangle_corner3,
                plane_normal,
                plane_distance);

            glm::vec3 closestPointOnPlane_position;
            Get3D_Point_Plane_ClosestPoint(
                point_position,
                plane_normal,
                plane_distance,
                closestPointOnPlane_position);

            if (Check3D_Point_Triangle_Overlap_H(
                closestPointOnPlane_position,
                triangle_corner1, triangle_corner2, triangle_corner3))
            {
                closestPoint_position = closestPointOnPlane_position;
            }
            else
            {
                glm::vec3 closestPointA_position;
                Get3D_Point_LineSegment_ClosestPoint(
                    closestPointOnPlane_position,
                    triangle_corner1,
                    triangle_corner2,
                    closestPointA_position);

                glm::vec3 closestPointB_position;
                Get3D_Point_LineSegment_ClosestPoint(
                    closestPointOnPlane_position,
                    triangle_corner2,
                    triangle_corner3,
                    closestPointB_position);

                glm::vec3 closestPointC_position;
                Get3D_Point_LineSegment_ClosestPoint(
                    closestPointOnPlane_position,
                    triangle_corner3,
                    triangle_corner1,
                    closestPointC_position);

                glm::vec3 vecA = closestPointA_position - closestPointOnPlane_position;
                glm::vec3 vecB = closestPointB_position - closestPointOnPlane_position;
                glm::vec3 vecC = closestPointC_position - closestPointOnPlane_position;

                float distanceA_squared = glm::dot(vecA, vecA);
                float distanceB_squared = glm::dot(vecB, vecB);
                float distanceC_squared = glm::dot(vecC, vecC);

                if (distanceA_squared <= distanceB_squared)
                {
                    if (distanceA_squared <= distanceC_squared)
                    {
                        // distanceA is smallest
                        closestPoint_position = closestPointA_position;
                    }
                    else
                    {
                        // distanceC is smallest
                        closestPoint_position = closestPointC_position;
                    }
                }
                else
                {
                    if (distanceB_squared <= distanceC_squared)
                    {
                        // distanceB is smallest
                        closestPoint_position = closestPointB_position;
                    }
                    else
                    {
                        // distanceC is smallest
                        closestPoint_position = closestPointC_position;
                    }
                }
            }
        }
    }

    inline void Get3D_Point_AABB_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        glm::vec3& closestPoint_position)
    {
        // clamp min
        closestPoint_position.x = (point_position.x < aabb_min.x) ? aabb_min.x : point_position.x;
        closestPoint_position.y = (point_position.y < aabb_min.y) ? aabb_min.y : point_position.y;
        closestPoint_position.z = (point_position.z < aabb_min.z) ? aabb_min.z : point_position.z;

        // clamp max
        closestPoint_position.x = (closestPoint_position.x > aabb_max.x) ? aabb_max.x : closestPoint_position.x;
        closestPoint_position.y = (closestPoint_position.y > aabb_max.y) ? aabb_max.y : closestPoint_position.y;
        closestPoint_position.z = (closestPoint_position.z > aabb_max.z) ? aabb_max.z : closestPoint_position.z;
    }

    inline void Get3D_Point_OBB_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        glm::vec3& closestPoint_position)
    {
        closestPoint_position = obb_position;
        glm::vec3 obb_to_point = point_position - obb_position;
        glm::mat3 obb_rotation = glm::mat3_cast(obb_orientation);

        for (glm::length_t i = 0; i < 3; ++i)
        {
            glm::vec3 axis = obb_rotation[i];

            float distance = glm::dot(obb_to_point, axis);

            if (distance > obb_halfSize[i]) {
                distance = obb_halfSize[i];
            }
            if (distance < -obb_halfSize[i]) {
                distance = -obb_halfSize[i];
            }

            closestPoint_position += (axis * distance);
        }
    }

    inline void Get3D_Point_Sphere_ClosestPoint(
        const glm::vec3& point_position,
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        glm::vec3& closestPoint_position)
    {
        closestPoint_position = point_position - sphere_position;
        float distanceSquared = glm::dot(closestPoint_position, closestPoint_position);
        if (distanceSquared <= sphere_radius * sphere_radius)
        {
            // The point is inside the sphere so it's as close as it can be
            closestPoint_position = point_position;
        }
        else
        {
            // Find surface point
            closestPoint_position = closestPoint_position * glm::inversesqrt(distanceSquared);
            closestPoint_position *= sphere_radius;
            closestPoint_position += sphere_position;
        }
    }

    inline void Get3D_Point_Sphere_ClosestSurfacePoint(
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

    // Raycast Functions -------------------------------------------------------

    inline bool Raycast3D_Plane(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& plane_normal,
        const float& plane_distance,
        float& hit_scalar,
        glm::vec3& hit_normal)
    {
        float nd = glm::dot(ray_direction, plane_normal);
        float pn = glm::dot(ray_position, plane_normal);

        // if nd == 0, ray is parallel to the plane
        // if nd > 0, ray is hitting plane from below
        // if nd < 0, ray is hitting plane from above
        if (FloatEqualToFloat(nd, 0.0f))
        {
            return false;
        }

        hit_scalar = (plane_distance - pn) / nd;

        if (FloatGreaterThanOrEqualToFloat(hit_scalar, 0.0f))
        {
            if (FloatLessThanOrEqualToFloat(nd, 0))
            {
                hit_normal = plane_normal;
            }
            else
            {
                hit_normal = -plane_normal;
            }
            return true;
        }

        return false;
    }

    inline bool Raycast3D_Triangle(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        float& hit_scalar,
        glm::vec3& hit_normal,
        glm::vec3& hit_point_position)
    {
        if (Check3D_Point_Point_Overlap(triangle_corner1, triangle_corner2) ||
            Check3D_Point_Point_Overlap(triangle_corner2, triangle_corner3) ||
            Check3D_Point_Point_Overlap(triangle_corner3, triangle_corner1))
        {
            return false;
        }

        glm::vec3 plane_normal;
        float plane_distance;
        Get3D_Triangle_ContainingPlane_H(triangle_corner1, triangle_corner2, triangle_corner3, plane_normal, plane_distance);

        if (Raycast3D_Plane(ray_position, ray_direction, plane_normal, plane_distance, hit_scalar, hit_normal))
        {
            hit_point_position = ray_position + ray_direction * hit_scalar;

            return Check3D_Point_Triangle_Overlap_H(hit_point_position, triangle_corner1, triangle_corner2, triangle_corner3);
        }
        
        return false;
    }

    inline bool Raycast3D_AABB(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        float& hit_scalar,
        glm::vec3& hit_normal)
    {
        // Any component of direction could be 0
        // Address this by using a small number, close to
        // 0 in case any of directions components are 0
        float t[6] = {
            (aabb_min.x - ray_position.x) / (FloatEqualToFloat(ray_direction.x, 0.0f) ? 0.00001f : ray_direction.x),
            (aabb_max.x - ray_position.x) / (FloatEqualToFloat(ray_direction.x, 0.0f) ? 0.00001f : ray_direction.x),
            (aabb_min.y - ray_position.y) / (FloatEqualToFloat(ray_direction.y, 0.0f) ? 0.00001f : ray_direction.y),
            (aabb_max.y - ray_position.y) / (FloatEqualToFloat(ray_direction.y, 0.0f) ? 0.00001f : ray_direction.y),
            (aabb_min.z - ray_position.z) / (FloatEqualToFloat(ray_direction.z, 0.0f) ? 0.00001f : ray_direction.z),
            (aabb_max.z - ray_position.z) / (FloatEqualToFloat(ray_direction.z, 0.0f) ? 0.00001f : ray_direction.z)
        };

        float largest_t_min = std::max(std::max(std::min(t[0], t[1]), std::min(t[2], t[3])), std::min(t[4], t[5]));
        float smallest_t_max = std::min(std::min(std::max(t[0], t[1]), std::max(t[2], t[3])), std::max(t[4], t[5]));

        // if smallest_t_max < 0, ray would intersect but
        // it is facing the opposite direction
        if (smallest_t_max < 0) {
            return false;
        }

        // if largest_t_min > smallest_t_max, ray doesn't intersect
        if (largest_t_min > smallest_t_max) {
            return false;
        }

        // If largest_t_min is < 0, tmax is closer
        if (largest_t_min < 0.0f) {
            hit_scalar = smallest_t_max;
        }
        else
        {
            hit_scalar = largest_t_min;
        }

        glm::vec3 normals[6] = {
            glm::vec3(-1, 0, 0),
            glm::vec3(1, 0, 0),
            glm::vec3(0, -1, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 0, -1),
            glm::vec3(0, 0, 1)
        };
        for (int i = 0; i < 6; ++i) {
            if (FloatEqualToFloat(hit_scalar, t[i])) {
                hit_normal = normals[i];
                break;
            }
        }

        return true;
    }

    inline bool Raycast3D_OBB(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        float& hit_scalar,
        glm::vec3& hit_normal)
    {
        glm::mat3 obb_rotation = glm::mat3_cast(obb_orientation);

        glm::vec3 f(
            glm::dot(obb_rotation[0], ray_direction),
            glm::dot(obb_rotation[1], ray_direction),
            glm::dot(obb_rotation[2], ray_direction)
        );

        glm::vec3 p = obb_position - ray_position;

        glm::vec3 e(
            glm::dot(obb_rotation[0], p),
            glm::dot(obb_rotation[1], p),
            glm::dot(obb_rotation[2], p)
        );

        float t[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        for (int i = 0; i < 3; ++i) {
            if (FloatEqualToFloat(f[i], 0.0f)) {
                if (-e[i] - obb_halfSize[i] > 0.0f || -e[i] + obb_halfSize[i] < 0.0f) {
                    return false;
                }
                f[i] = 0.00001f; // avoid dividing by 0
            }

            t[i * 2 + 0] = (e[i] + obb_halfSize[i]) / f[i]; // tmin[x, y, z]
            t[i * 2 + 1] = (e[i] - obb_halfSize[i]) / f[i]; // tmax[x, y, z]
        }

        float largest_t_min = std::max(std::max(std::min(t[0], t[1]), std::min(t[2], t[3])), std::min(t[4], t[5]));
        float smallest_t_max = std::min(std::min(std::max(t[0], t[1]), std::max(t[2], t[3])), std::max(t[4], t[5]));

        // if smallest_t_max < 0, ray would intersect but
        // it is facing the opposite direction
        if (smallest_t_max < 0) {
            return false;
        }

        // if largest_t_min > smallest_t_max, ray doesn't intersect
        if (largest_t_min > smallest_t_max) {
            return false;
        }

        // If largest_t_min is < 0, tmax is closer
        if (largest_t_min < 0.0f) {
            hit_scalar = smallest_t_max;
        }
        else
        {
            hit_scalar = largest_t_min;
        }

        glm::vec3 normals[6] = {
            obb_rotation[0],
            obb_rotation[0] * -1.0f,
            obb_rotation[1],
            obb_rotation[1] * -1.0f,
            obb_rotation[2],
            obb_rotation[2] * -1.0f
        };
        for (int i = 0; i < 6; ++i) {
            if (FloatEqualToFloat(hit_scalar, t[i])) {
                hit_normal = normals[i];
                break;
            }
        }

        return true;
    }

    inline bool Raycast3D_Sphere(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& sphere_position,
        const float& sphere_radius,
        float& hit_scalar,
        glm::vec3& hit_normal,
        glm::vec3& hit_point_position)
    {
        glm::vec3 rayToSphere = sphere_position - ray_position;
        float rayToSphereSq = glm::dot(rayToSphere, rayToSphere);
        float sphereRadiusSq = sphere_radius * sphere_radius;
        float a = glm::dot(rayToSphere, ray_direction);
        float aSq = a * a;
        float bSq = rayToSphereSq - (aSq);
        float f = glm::sqrt(glm::abs((sphereRadiusSq)- bSq));

        if (FloatGreaterThanOrEqualToFloat(sphereRadiusSq - (rayToSphereSq - aSq), 0.0f))
        {
            if (rayToSphereSq < sphereRadiusSq) {
                hit_scalar = a + f;
            }
            else
            {
                hit_scalar = a - f;
            }

            hit_point_position = ray_position + ray_direction * hit_scalar;
            hit_normal = glm::normalize(hit_point_position - sphere_position);

            return true;
        }

        return false;
    }

    // Helper Functions --------------------------------------------------------

    inline bool Get3D_Plane_Plane_Plane_Intersection_H(
        const glm::vec3& planeA_normal,
        const float& planeA_distance,
        const glm::vec3& planeB_normal,
        const float& planeB_distance,
        const glm::vec3& planeC_normal,
        const float& planeC_distance,
        glm::vec3& intersectionPoint_position)
    {
        glm::vec3 distances(planeA_distance, planeB_distance, planeC_distance);

        glm::vec3 u = glm::cross(planeB_normal, planeC_normal);
        glm::vec3 v = glm::cross(planeA_normal, distances);

        float denominator = glm::dot(planeA_normal, u);
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            // planes don't intersect at a point
            return false;
        }

        intersectionPoint_position.x = glm::dot(distances, u) / denominator;
        intersectionPoint_position.y = glm::dot(planeC_normal, v) / denominator;
        intersectionPoint_position.z = -glm::dot(planeB_normal, v) / denominator;

        return true;
    }

    inline bool Check3D_Point_Box_Overlap_H(
        const glm::vec3& point_position,
        const glm::vec3& box_oppositeCorner1,
        const glm::vec3& box_oppositeCorner2)
    {
        float maxX, minX, maxY, minY, maxZ, minZ;
        GetMinMax(box_oppositeCorner1.x, box_oppositeCorner2.x, minX, maxX);
        GetMinMax(box_oppositeCorner1.y, box_oppositeCorner2.y, minY, maxY);
        GetMinMax(box_oppositeCorner1.z, box_oppositeCorner2.z, minZ, maxZ);

        return FloatLessThanOrEqualToFloat(point_position.x, maxX) &&
            FloatGreaterThanOrEqualToFloat(point_position.x, minX) &&
            FloatLessThanOrEqualToFloat(point_position.y, maxY) &&
            FloatGreaterThanOrEqualToFloat(point_position.y, minY) &&
            FloatLessThanOrEqualToFloat(point_position.z, maxZ) &&
            FloatGreaterThanOrEqualToFloat(point_position.z, minZ);
    }

    inline void Get3D_Point_Plane_Distance(
        const glm::vec3& point_position,
        const glm::vec3& plane_normal,
        const float& plane_distance,
        float& distance)
    {
        float dotProduct = glm::dot(point_position, plane_normal);
        distance = dotProduct - plane_distance;
    }

    inline bool Check3D_Point_Plane_Overlap_Alt(
        const glm::vec3& point_position,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal)
    {
        glm::vec3 planeToPoint = point_position - plane_position;

        float dotProduct = glm::dot(planeToPoint, plane_normal);
        return FloatEqualToFloat(dotProduct, 0.0f);
    }

    inline bool Check3D_Point_OBB_Overlap_Alt(
        const glm::vec3& point_position,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation)
    {
        glm::vec3 obb_to_point = point_position - obb_position;
        glm::mat3 obb_rotation = glm::mat3_cast(obb_orientation);

        for (glm::length_t i = 0; i < 3; ++i) {

            glm::vec3 axis = obb_rotation[i];

            float distance = glm::dot(obb_to_point, axis);

            if (distance > obb_halfSize[i] || distance < -obb_halfSize[i])
            {
                return false;
            }
        }

        return true;
    }

    inline void Get3D_Point_OBB_ClosestPoint_Alt(
        const glm::vec3& point_position,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        glm::vec3& closestPoint_position)
    {
        glm::vec3 translated_point_position = point_position - obb_position;
        glm::quat inverse_orientation = glm::inverse(obb_orientation);
        glm::vec3 rotated_point_position = inverse_orientation * translated_point_position;

        Get3D_Point_AABB_ClosestPoint(rotated_point_position, -1.0f * obb_halfSize, obb_halfSize, closestPoint_position);

        closestPoint_position = obb_orientation * closestPoint_position;
        closestPoint_position += obb_position;
    }

    inline bool Check3D_Line_Plane_Overlap_Alt(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal)
    {
        if (Check3D_Point_Plane_Overlap_Alt(line_position, plane_position, plane_normal))
        {
            // line is on the plane
            return true;
        }

        float dotProduct = glm::dot(line_direction, plane_normal);

        // If dotProduct == 0, line is parallel to the plane
        return !FloatEqualToFloat(dotProduct, 0.0f);
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
        if (FloatEqualToFloat(denominator, 0.0f))
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
        pointB_position = d2 * t2 + o2;

        return true;
    }

    inline bool Check3D_Point_LineSegment_Overlap_Alt(
        const glm::vec3& point_position,
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end)
    {
        glm::vec3 closestLinePosition;
        Get3D_Point_LineSegment_ClosestPoint(point_position, lineSegment_start, lineSegment_end, closestLinePosition);

        glm::vec3 pointToClosest = closestLinePosition - point_position;
        float distanceSquared = glm::dot(pointToClosest, pointToClosest);
        return FloatEqualToFloat(distanceSquared, 0.0f);
    }

    inline bool Get3D_Line_Plane_Intersection_Alt(
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
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            // line is parallel to the plane
            return false;
        }

        float numerator = glm::dot(plane_normal, plane_position - line_position);
        float u = numerator / denominator;

        intersection_position = line_direction * u + line_position;
        return true;
    }

    inline bool Get3D_Ray_Plane_Intersection_Alt(
        const glm::vec3& ray_position,
        const glm::vec3& ray_direction,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal,
        glm::vec3& intersection_position)
    {
        float denominator = glm::dot(plane_normal, ray_direction);
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            // ray is parallel to the plane,
            return false;
        }

        float numerator = glm::dot(plane_normal, plane_position - ray_position);
        float u = numerator / denominator;

        if (FloatGreaterThanOrEqualToFloat(u, 0.0f))
        {
            intersection_position = ray_direction * u + ray_position;
            return true;
        }
        else
        {
            // intersection is behind
            return false;
        }
    }

    inline bool Check3D_LineSegment_Plane_Overlap_Alt(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& plane_position,
        const glm::vec3& plane_normal)
    {
        glm::vec3 intersection_position;
        glm::vec3 lineSegment_direction = glm::normalize(lineSegment_end - lineSegment_start);
        if (!Get3D_Line_Plane_Intersection_Alt(lineSegment_start, lineSegment_direction, plane_position, plane_normal, intersection_position))
        {
            // line is parallel
            return false;
        }

        // check if the line plane intersection is within the line segment's range
        return Check3D_Point_Box_Overlap_H(intersection_position, lineSegment_start, lineSegment_end);
    }

    inline bool Check3D_Plane_Plane_Overlap_Alt(
        const glm::vec3& planeA_position,
        const glm::vec3& planeA_normal,
        const glm::vec3& planeB_position,
        const glm::vec3& planeB_normal)
    {
        if (FloatEqualToFloat(planeA_position.x, planeB_position.x) &&
            FloatEqualToFloat(planeA_position.y, planeB_position.y) &&
            FloatEqualToFloat(planeA_position.z, planeB_position.z))
        {
            return true;
        }

        glm::vec3 normal_ratio = planeA_normal / planeB_normal;

        // Check if planes are parallel; if they are, they don't Overlap
        return !(FloatEqualToFloat(normal_ratio.x, normal_ratio.y) &&
            FloatEqualToFloat(normal_ratio.x, normal_ratio.z));
    }

    inline bool Check3D_Point_Triangle_Overlap_H(
        const glm::vec3& point_position,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        const glm::vec3& p = point_position;
        glm::vec3 a = triangle_corner1 - p;
        glm::vec3 b = triangle_corner2 - p;
        glm::vec3 c = triangle_corner3 - p;

        glm::vec3 u = glm::cross(b, c);
        glm::vec3 v = glm::cross(c, a);
        glm::vec3 w = glm::cross(a, b);

        float dotUV = glm::dot(u, v);
        float dotUW = glm::dot(u, w);

        return FloatGreaterThanOrEqualToFloat(dotUV, 0.0f) &&
            FloatGreaterThanOrEqualToFloat(dotUW, 0.0f);
    }

    inline bool Check3D_LineSegment_Triangle_Overlap_H(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        glm::vec3 side12 = triangle_corner2 - triangle_corner1;
        glm::vec3 side13 = triangle_corner3 - triangle_corner1;
        glm::vec3 plane_normal = glm::normalize(glm::cross(side12, side13));
        glm::vec3 lineSegment_direction = glm::normalize(lineSegment_end - lineSegment_start);
        float denominator = glm::dot(plane_normal, lineSegment_direction);
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            // line is parallel to the plane
            return Check3D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner1, triangle_corner2) ||
                Check3D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner2, triangle_corner3) ||
                Check3D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner3, triangle_corner1) ||
                Check3D_Point_Triangle_Overlap_H(lineSegment_start, triangle_corner1, triangle_corner2, triangle_corner3);
        }
        else
        {
            const glm::vec3& plane_position = triangle_corner1;
            float numerator = glm::dot(plane_normal, plane_position - lineSegment_start);
            float u = numerator / denominator;

            glm::vec3 intersection_position = lineSegment_direction * u + lineSegment_start;

            // check if the line plane intersection is within the line segment's
            // range and through the triangle
            return Check3D_Point_Box_Overlap_H(intersection_position, lineSegment_start, lineSegment_end) &&
                Check3D_Point_Triangle_Overlap_H(intersection_position, triangle_corner1, triangle_corner2, triangle_corner3);
        }
    }

    inline bool Check3D_LineSegment_Triangle_Overlap_H_Alt(
        const glm::vec3& lineSegment_start,
        const glm::vec3& lineSegment_end,
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3)
    {
        glm::vec3 plane_normal;
        float plane_distance;
        Get3D_Triangle_ContainingPlane_H(triangle_corner1, triangle_corner2, triangle_corner3, plane_normal, plane_distance);

        glm::vec3 lineSegment_vector = lineSegment_end - lineSegment_start;
        glm::vec3 lineSegment_direction = glm::normalize(lineSegment_vector);
        float denominator = glm::dot(plane_normal, lineSegment_direction);
        if (FloatEqualToFloat(denominator, 0.0f))
        {
            // line is parallel to the plane
            return Check3D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner1, triangle_corner2) ||
                Check3D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner2, triangle_corner3) ||
                Check3D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner3, triangle_corner1) ||
                Check3D_Point_Triangle_Overlap_H(lineSegment_start, triangle_corner1, triangle_corner2, triangle_corner3);
        }
        else
        {
            float t = (plane_distance - glm::dot(plane_normal, lineSegment_start)) / denominator;
            float lineSegmentLengthSq = glm::dot(lineSegment_vector, lineSegment_vector);
            return FloatGreaterThanOrEqualToFloat(t, 0.0f) &&
                FloatLessThanOrEqualToFloat(t * t, lineSegmentLengthSq);
        }
    }

    inline void Get3D_Triangle_ContainingPlane_H(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        glm::vec3& plane_normal,
        float& plane_distance)
    {
        glm::vec3 side12 = triangle_corner2 - triangle_corner1;
        glm::vec3 side13 = triangle_corner3 - triangle_corner1;
        plane_normal = glm::normalize(glm::cross(side12, side13));

        float magnitudeOfNormal = glm::length(plane_normal);

        // plane equation:
        // a * x + b * y + c * z = d
        // 
        // plane_normal = (a, b, c) = n
        // point_on_plane = (x, y, z)
        // 
        // plane_distance = |d| / |n|

        float d = glm::dot(plane_normal, triangle_corner1);

        if (d < 0.0f)
        {
            plane_normal *= -1.0f;
            d *= -1.0f;
        }

        plane_distance = d / magnitudeOfNormal;
    }

    inline void Get3D_Triangle_ContainingPlane_S(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        glm::vec3& plane_normal,
        float& plane_distance)
    {
        if ((FloatEqualToFloat(triangle_corner1.x, triangle_corner2.x) &&
            FloatEqualToFloat(triangle_corner1.y, triangle_corner2.y) &&
            FloatEqualToFloat(triangle_corner1.z, triangle_corner2.z)) ||
            (FloatEqualToFloat(triangle_corner2.x, triangle_corner3.x) &&
            FloatEqualToFloat(triangle_corner2.y, triangle_corner3.y) &&
            FloatEqualToFloat(triangle_corner2.z, triangle_corner3.z)) ||
            (FloatEqualToFloat(triangle_corner3.x, triangle_corner1.x) &&
            FloatEqualToFloat(triangle_corner3.y, triangle_corner1.y) &&
            FloatEqualToFloat(triangle_corner3.z, triangle_corner1.z)))
        {
            plane_normal = glm::vec3(0.0f, 1.0f, 0.0f);
            plane_distance = glm::length(triangle_corner1);

            if (plane_distance < 0.0f)
            {
                plane_normal *= -1;
                plane_distance *= -1;
            }
        }
        else
        {
            Get3D_Triangle_ContainingPlane_H(
                triangle_corner1,
                triangle_corner2,
                triangle_corner3,
                plane_normal,
                plane_distance);
        }
    }

    inline void Get3D_Triangle_AxisInterval(
        const glm::vec3& triangle_corner1,
        const glm::vec3& triangle_corner2,
        const glm::vec3& triangle_corner3,
        const glm::vec3& axis,
        float& interval_min,
        float& interval_max)
    {
        interval_min = glm::dot(axis, triangle_corner1);
        interval_max = interval_min;

        float scalarProjection = glm::dot(axis, triangle_corner2);
        interval_min = std::min(interval_min, scalarProjection);
        interval_max = std::max(interval_max, scalarProjection);

        scalarProjection = glm::dot(axis, triangle_corner3);
        interval_min = std::min(interval_min, scalarProjection);
        interval_max = std::max(interval_max, scalarProjection);
    }

    inline void Get3D_AABB_AxisInterval(
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        const glm::vec3& axis,
        float& interval_min,
        float& interval_max)
    {
        glm::vec3 verticies[8] = {
            glm::vec3(aabb_min.x, aabb_min.y, aabb_min.z),
            glm::vec3(aabb_min.x, aabb_min.y, aabb_max.z),
            glm::vec3(aabb_min.x, aabb_max.y, aabb_min.z),
            glm::vec3(aabb_min.x, aabb_max.y, aabb_max.z),
            glm::vec3(aabb_max.x, aabb_min.y, aabb_min.z),
            glm::vec3(aabb_max.x, aabb_min.y, aabb_max.z),
            glm::vec3(aabb_max.x, aabb_max.y, aabb_min.z),
            glm::vec3(aabb_max.x, aabb_max.y, aabb_max.z)
        };

        interval_min = glm::dot(axis, verticies[0]);
interval_max = interval_min;

for (size_t i = 1; i < 8; ++i)
{
    float scalarProjection = glm::dot(axis, verticies[i]);
    interval_min = GetMin(interval_min, scalarProjection);
    interval_max = GetMax(interval_max, scalarProjection);
}
    }

    inline void Get3D_OBB_AxisInterval(
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        const glm::vec3& axis,
        float& interval_min,
        float& interval_max)
    {
        glm::vec3 verticies[8] = {
            glm::vec3(obb_halfSize.x, obb_halfSize.y, obb_halfSize.z),
            glm::vec3(obb_halfSize.x, obb_halfSize.y, -obb_halfSize.z),
            glm::vec3(obb_halfSize.x, -obb_halfSize.y, obb_halfSize.z),
            glm::vec3(obb_halfSize.x, -obb_halfSize.y, -obb_halfSize.z),
            glm::vec3(-obb_halfSize.x, obb_halfSize.y, obb_halfSize.z),
            glm::vec3(-obb_halfSize.x, obb_halfSize.y, -obb_halfSize.z),
            glm::vec3(-obb_halfSize.x, -obb_halfSize.y, obb_halfSize.z),
            glm::vec3(-obb_halfSize.x, -obb_halfSize.y, -obb_halfSize.z)
        };

        for (size_t i = 0; i < 8; ++i)
        {
            verticies[i] = obb_position + obb_orientation * verticies[i];
        }

        interval_min = glm::dot(axis, verticies[0]);
        interval_max = interval_min;

        for (size_t i = 1; i < 8; ++i)
        {
            float scalarProjection = glm::dot(axis, verticies[i]);
            interval_min = GetMin(interval_min, scalarProjection);
            interval_max = GetMax(interval_max, scalarProjection);
        }
    }

    inline bool Linecast3D_AABB(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& aabb_min,
        const glm::vec3& aabb_max,
        float& hit_scalar_largest_min,
        float& hit_scalar_smallest_max)
    {
        // Any component of direction could be 0
        // Address this by using a small number, close to
        // 0 in case any of directions components are 0
        float t[6] = {
            (aabb_min.x - g_floatMarginOfError - line_position.x) / (line_direction.x == 0.0f ? 0.001f * g_floatMarginOfError : line_direction.x),
            (aabb_max.x + g_floatMarginOfError - line_position.x) / (line_direction.x == 0.0f ? 0.001f * g_floatMarginOfError : line_direction.x),
            (aabb_min.y - g_floatMarginOfError - line_position.y) / (line_direction.y == 0.0f ? 0.001f * g_floatMarginOfError : line_direction.y),
            (aabb_max.y + g_floatMarginOfError - line_position.y) / (line_direction.y == 0.0f ? 0.001f * g_floatMarginOfError : line_direction.y),
            (aabb_min.z - g_floatMarginOfError - line_position.z) / (line_direction.z == 0.0f ? 0.001f * g_floatMarginOfError : line_direction.z),
            (aabb_max.z + g_floatMarginOfError - line_position.z) / (line_direction.z == 0.0f ? 0.001f * g_floatMarginOfError : line_direction.z)
        };

        hit_scalar_largest_min = std::max(std::max(std::min(t[0], t[1]), std::min(t[2], t[3])), std::min(t[4], t[5]));
        hit_scalar_smallest_max = std::min(std::min(std::max(t[0], t[1]), std::max(t[2], t[3])), std::max(t[4], t[5]));

        if (hit_scalar_largest_min > hit_scalar_smallest_max) {
            return false;
        }

        return true;
    }

    inline bool Linecast3D_OBB(
        const glm::vec3& line_position,
        const glm::vec3& line_direction,
        const glm::vec3& obb_halfSize,
        const glm::vec3& obb_position,
        const glm::quat& obb_orientation,
        float& hit_scalar_largest_min,
        float& hit_scalar_smallest_max)
    {
        glm::mat3 obb_rotation = glm::mat3_cast(obb_orientation);

        glm::vec3 f(
            glm::dot(obb_rotation[0], line_direction),
            glm::dot(obb_rotation[1], line_direction),
            glm::dot(obb_rotation[2], line_direction)
        );

        glm::vec3 p = obb_position - line_position;

        glm::vec3 e(
            glm::dot(obb_rotation[0], p),
            glm::dot(obb_rotation[1], p),
            glm::dot(obb_rotation[2], p)
        );

        float t[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        for (int i = 0; i < 3; ++i) {
            if (f[i] == 0.0f) {
                // if (-e[i] - obb_halfSize[i] > 0.0f || -e[i] + obb_halfSize[i] < 0.0f) {
                //     return false;
                // }
                f[i] = 0.001f * g_floatMarginOfError; // avoid dividing by 0
            }
            t[i * 2 + 0] = (e[i] + obb_halfSize[i] + g_floatMarginOfError) / f[i]; // tmin[x, y, z]
            t[i * 2 + 1] = (e[i] - obb_halfSize[i] - g_floatMarginOfError) / f[i]; // tmax[x, y, z]
        }

        hit_scalar_largest_min = std::max(std::max(std::min(t[0], t[1]), std::min(t[2], t[3])), std::min(t[4], t[5]));
        hit_scalar_smallest_max = std::min(std::min(std::max(t[0], t[1]), std::max(t[2], t[3])), std::max(t[4], t[5]));

        if (hit_scalar_largest_min > hit_scalar_smallest_max) {
            return false;
        }

        return true;
    }

    inline glm::vec3 SAT_Cross(
        const glm::vec3& a,
        const glm::vec3& b,
        const glm::vec3& c,
        const glm::vec3& d)
    {
        glm::vec3 ab = b - a;
        glm::vec3 cd = d - c;

        glm::vec3 result = glm::cross(ab, cd);
        float magnitudeSq = glm::dot(result, result);
        if (magnitudeSq == 0.0f) // ab && cd are parallel
        {
            glm::vec3 axis = glm::cross(ab, c - a);
            result = glm::cross(ab, axis);

            // if the result is still 0:
            // a, b, c, and d are all on a line
        }

        return result;
    }
}
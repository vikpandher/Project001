


namespace Project001
{
    // Overlap Functions (Point) -----------------------------------------------

    inline bool Check2D_Point_Rectangle_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return point_position.x < rectangle_topRight.x &&
            point_position.x > rectangle_bottomLeft.x &&
            point_position.y < rectangle_topRight.y &&
            point_position.y > rectangle_bottomLeft.y;
    }

    inline bool Check2D_Point_OrientedRectangle_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        // rotate and translate the point rather then the orientedRectangle
        glm::vec2 translatedPointPosition = point_position - orientedRectangle_position;
        glm::vec2 rotatedPointPosition = Rotate2DVector(translatedPointPosition, -1.0f * orientedRectangle_rotation);

        return Check2D_Point_Rectangle_Overlap(rotatedPointPosition, -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize);
    }

    inline bool Check2D_Point_Circle_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        float pointToCircleCenterDistanceSquared = Get2D_Point_Point_DistanceSquared(point_position, circle_position);
        float circleRadiusSquared = circle_radius * circle_radius;
        return pointToCircleCenterDistanceSquared < circleRadiusSquared;
    }

    inline bool Check2D_Point_Capsule_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        float pointLineSegmentDistanceSquared = Get2D_Point_LineSegment_DistanceSquared(point_position, capsule_start, capsule_end);
        float capsuleRadiusSquared = capsule_radius * capsule_radius;
        return pointLineSegmentDistanceSquared < capsuleRadiusSquared;
    }

    inline bool Check2D_Point_Triangle_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        // Barycentric Technique
        // 
        // P = point_position
        // A = triangle_corner1, B = triangle_corner2, C = triangle_corner3
        // 
        // A = origin on the plane
        // (C - A) = an axis on the plane
        // (B - A) = an axis on the plane
        // 
        // Equation for point on a plane:
        // P = A + u * (C - A) + v * (B - A)
        // 
        // if u < 0 or v < 0, we are outside the triangle
        // if u > 1 or v > 1, we are outside the triangle
        // if (u + v) > 1, we are outside the triangle
        // 
        // P = A + u * (C - A) + v * (B - A)
        // (P - A) = u * (C - A) + v * (B - A)
        // 
        // let: AP = (P - A), AC = (C - A), AB = (B - A)
        // 
        // AP = u * AC + v * AB
        // 
        // AP . AC = (u * AC + v * AB) . AC
        // AP . AB = (u * AC + v * AB) . AB
        // 
        // AP . AC = u * (AC . AC) + v * (AB . AC)
        // AP . AB = u * (AC . AB) + v * (AB . AB)
        // 
        // u = ((AB.AB)(AP.AC)-(AB.AC)(AP.AB)) / ((AC.AC)(AB.AB) - (AC.AB)(AB.AC))
        // v = ((AC.AC)(AP.AB)-(AC.AB)(AP.AC)) / ((AC.AC)(AB.AB) - (AC.AB)(AB.AC))

        glm::vec2 AP = point_position - triangle_corner1;
        glm::vec2 AB = triangle_corner2 - triangle_corner1;
        glm::vec2 AC = triangle_corner3 - triangle_corner1;

        float dot_AC_AC = glm::dot(AC, AC); // corner1_to_corner3 distance squared
        float dot_AB_AC = glm::dot(AC, AB);
        float dot_AC_AP = glm::dot(AC, AP);
        float dot_AB_AB = glm::dot(AB, AB); // corner2_to_corner3 distance squared
        float dot_AB_AP = glm::dot(AB, AP);

        float denominator = (dot_AC_AC * dot_AB_AB - dot_AB_AC * dot_AB_AC);
        float u = (dot_AB_AB * dot_AC_AP - dot_AB_AC * dot_AB_AP) / denominator;
        float v = (dot_AC_AC * dot_AB_AP - dot_AB_AC * dot_AC_AP) / denominator;

        return (u > 0.0f) && (v > 0.0f) && ((u + v) < 1.0f);
    }

    inline bool Check2D_Point_Polygon_Overlap(
        const glm::vec2& point_position,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount < 3)
        {
            return false;
        }

        // Check how many times a ray from the point intersects
        // the polygon's line segments.
        unsigned int intersectionCount = 0;

        // Don't use an axis alinged rays. If they are aligned to the
        // x or y axis, they tend to dodge corners.
        glm::vec2 ray_direction(0.78965f, 0.613558);
        glm::vec2 ray_perpendicular(-0.613558, 0.78965f);
        const glm::vec2& ray_position = point_position;

        size_t start_index = polygon_cornerCount - 1;
        for (size_t end_index = 0; end_index < polygon_cornerCount; ++end_index)
        {
            const glm::vec2& lineSegment_start = polygon_corners[start_index];
            const glm::vec2& lineSegment_end = polygon_corners[end_index];

            glm::vec2 startToRay = ray_position - lineSegment_start;
            glm::vec2 startToEnd = lineSegment_end - lineSegment_start;

            glm::vec2 currentIntersect_position;

            float denominator = glm::dot(startToEnd, ray_perpendicular); // can be zero
            float t1 = (startToEnd.x * startToRay.y - startToRay.x * startToEnd.y) / denominator;
            float t2 = glm::dot(startToRay, ray_perpendicular) / denominator;

            if (t2 > 0.0f && t2 < 1.0f && t1 > 0.0f)
            {
                intersectionCount++;
            }

            start_index = end_index;
        }

        if (intersectionCount % 2 == 0)
        {
            return false;
        }

        return true;
    }

    inline bool Check2D_Point_ConvexPolygon_Overlap(
        const glm::vec2& point_position,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Point_Polygon_Overlap(point_position, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Overlap Functions (Line) ------------------------------------------------

    inline bool Check2D_Line_Line_Overlap(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope)
    {
        if ((std::isinf(lineA_slope) && std::isinf(lineB_slope)) || lineA_slope == lineB_slope)
        {
            return false;
        }

        return true;
    }

    inline bool Check2D_Line_Ray_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        if (std::isinf(line_slope))
        {
            return (ray_direction.x > 0.0f && line_position.x > ray_position.x) ||
                (ray_direction.x < 0.0f && line_position.x < ray_position.x);
        }

        glm::vec2 perpendicular(-ray_direction.y, ray_direction.x);
        glm::vec2 lineToRay = ray_position - line_position;

        glm::vec2 line_direction = Get2D_DirectionFromSlope(line_slope);
        float denominator = glm::dot(line_direction, perpendicular);
        if (denominator == 0.0f)
        {
            // lineSegment and ray are parallel
            return false;
        }

        float t = (line_direction.x * lineToRay.y - lineToRay.x * line_direction.y) / denominator;

        return t > 0.0f;
    }

    inline bool Check2D_Line_LineSegment_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        if (std::isinf(line_slope))
        {
            return (lineSegment_start.x < line_position.x && lineSegment_end.x > line_position.x) ||
                (lineSegment_start.x > line_position.x && lineSegment_end.x < line_position.x);
        }

        // y = mx + b
        // b = y - mx
        float line_yIntercept = line_position.y - line_slope * line_position.x;

        // y = mx + b
        // y - mx - b = 0 = yInterceptOffset
        // If yInterceptOffset < 0.0, the point is below the line
        // If yInterceptOffset > 0.0, the point is above the line
        float lineSegment_start_yInterceptOffset =
            lineSegment_start.y - line_slope * lineSegment_start.x - line_yIntercept;
        float lineSegment_end_yInterceptOffset =
            lineSegment_end.y - line_slope * lineSegment_end.x - line_yIntercept;

        // check if the endpoints are on opposite sides of the line
        return (lineSegment_start_yInterceptOffset > 0.0f && lineSegment_end_yInterceptOffset < 0.0f) ||
            (lineSegment_start_yInterceptOffset < 0.0f && lineSegment_end_yInterceptOffset > 0.0f);
    }

    inline bool Check2D_Line_Rectangle_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        if (std::isinf(line_slope))
        {
            return line_position.x > rectangle_bottomLeft.x && line_position.x < rectangle_topRight.x;
        }

        // y = mx + b
        // b = y - mx
        float line_yIntercept = line_position.y - line_slope * line_position.x;

        // y = mx + b
        // y - mx - b = 0 = yInterceptOffset
        // If yInterceptOffset < 0.0, the point is below the line
        // If yInterceptOffset > 0.0, the point is above the line
        float rectangle_bottomLeft_yInterceptOffset =
            rectangle_bottomLeft.y - line_slope * rectangle_bottomLeft.x - line_yIntercept;
        float rectangle_bottomRight_yInterceptOffset =
            rectangle_bottomLeft.y - line_slope * rectangle_topRight.x - line_yIntercept;

        // Bottom edge: from rectangle_bottomLeft to rectangle_bottomRight
        if ((rectangle_bottomLeft_yInterceptOffset > 0.0f && rectangle_bottomRight_yInterceptOffset < 0.0f) ||
            (rectangle_bottomLeft_yInterceptOffset < 0.0f && rectangle_bottomRight_yInterceptOffset > 0.0f))
        {
            return true;
        }

        float rectangle_topRight_yInterceptOffset =
            rectangle_topRight.y - line_slope * rectangle_topRight.x - line_yIntercept;

        // Right edge: from rectangle_bottomRight to rectangle_topRight
        if ((rectangle_bottomRight_yInterceptOffset > 0.0f && rectangle_topRight_yInterceptOffset < 0.0f) ||
            (rectangle_bottomRight_yInterceptOffset < 0.0f && rectangle_topRight_yInterceptOffset > 0.0f))
        {
            return true;
        }

        float rectangle_topLeft_yInterceptOffset =
            rectangle_topRight.y - line_slope * rectangle_bottomLeft.x - line_yIntercept;

        // Top edge: from rectangle_topLeft to rectangle_topRight
        if ((rectangle_topLeft_yInterceptOffset > 0.0f && rectangle_topRight_yInterceptOffset < 0.0f) ||
            (rectangle_topLeft_yInterceptOffset < 0.0f && rectangle_topRight_yInterceptOffset > 0.0f))
        {
            return true;
        }

        // Note:
        // Testing only 3 edges is enough except for the case where the the
        // line threads the needle between two edges at a corner and doesn't
        // get detected.

        // Left edge: from rectangle_bottomLeft to rectangle_topLeft
        if ((rectangle_bottomLeft_yInterceptOffset > 0.0f && rectangle_topLeft_yInterceptOffset < 0.0f) ||
            (rectangle_bottomLeft_yInterceptOffset < 0.0f && rectangle_topLeft_yInterceptOffset > 0.0f))
        {
            return true;
        }

        return false;
    }

    inline bool Check2D_Line_OrientedRectangle_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        // rotate and translate the line rather then the orientedRectangle
        glm::vec2 transformed_line_position = line_position;
        transformed_line_position -= orientedRectangle_position;
        transformed_line_position = Rotate2DVector(transformed_line_position, -1.0f * orientedRectangle_rotation);
        float transformed_line_slope = RotateSlope(line_slope, -1.0f * orientedRectangle_rotation);

        return Check2D_Line_Rectangle_Overlap(transformed_line_position, transformed_line_slope, -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize);
    }

    inline bool Check2D_Line_Circle_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        float pointLineDistanceSquared = Get2D_Point_Line_DistanceSquared(circle_position, line_position, line_slope);
        float circleRadiusSquared = circle_radius * circle_radius;
        return pointLineDistanceSquared < circleRadiusSquared;
    }

    inline bool Check2D_Line_Capsule_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        if (Check2D_Line_LineSegment_Overlap(line_position, line_slope, capsule_start, capsule_end))
        {
            return true;
        }

        float capsuleRadiusSquared = capsule_radius * capsule_radius;

        // check if line intersects capsule start
        float capsuleStartDistanceSquared = Get2D_Point_Line_DistanceSquared(capsule_start, line_position, line_slope);
        if (capsuleStartDistanceSquared < capsuleRadiusSquared)
        {
            return true;
        }

        // check if line intersects capsule end
        float capsuleEndDistanceSquared = Get2D_Point_Line_DistanceSquared(capsule_end, line_position, line_slope);
        return capsuleEndDistanceSquared < capsuleRadiusSquared;
    }

    inline bool Check2D_Line_Triangle_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        if (std::isinf(line_slope))
        {
            float maxX = std::max(std::max(triangle_corner1.x, triangle_corner2.x), triangle_corner3.x);
            float minX = std::min(std::min(triangle_corner1.x, triangle_corner2.x), triangle_corner3.x);
            return maxX > line_position.x && minX < line_position.x;
        }

        // y = mx + b
        // b = y - mx
        float line_yIntercept = line_position.y - line_slope * line_position.x;

        // y = mx + b
        // y - mx - b = 0 = yInterceptOffset
        // If yInterceptOffset < 0.0, the point is below the line
        // If yInterceptOffset > 0.0, the point is above the line
        float triangle_corner1_yInterceptOffset =
            triangle_corner1.y - line_slope * triangle_corner1.x - line_yIntercept;
        float triangle_corner2_yInterceptOffset =
            triangle_corner2.y - line_slope * triangle_corner2.x - line_yIntercept;

        // Edge 1: from triangle_corner1 to triangle_corner2
        if ((triangle_corner1_yInterceptOffset > 0.0f && triangle_corner2_yInterceptOffset < 0.0f) ||
            (triangle_corner1_yInterceptOffset < 0.0f && triangle_corner2_yInterceptOffset > 0.0f))
        {
            return true;
        }

        float triangle_corner3_yInterceptOffset =
            triangle_corner3.y - line_slope * triangle_corner3.x - line_yIntercept;

        // Edge 2: from triangle_corner2 to triangle_corner3
        if ((triangle_corner2_yInterceptOffset > 0.0f && triangle_corner3_yInterceptOffset < 0.0f) ||
            (triangle_corner2_yInterceptOffset < 0.0f && triangle_corner3_yInterceptOffset > 0.0f))
        {
            return true;
        }

        // Note:
        // Testing only 2 edges is enough except for the case where the the
        // line threads the needle between two edges at a corner and doesn't
        // get detected.

        // Edge 3: from triangle_corner3 to triangle_corner1
        if ((triangle_corner3_yInterceptOffset > 0.0f && triangle_corner1_yInterceptOffset < 0.0f) ||
            (triangle_corner3_yInterceptOffset < 0.0f && triangle_corner1_yInterceptOffset > 0.0f))
        {
            return true;
        }

        return false;
    }

    inline bool Check2D_Line_Polygon_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount < 3)
        {
            return false;
        }

        if (std::isinf(line_slope))
        {
            bool cornerLeft = false;
            bool cornerRight = false;

            for (size_t index = 0; index < polygon_cornerCount; ++index)
            {
                const glm::vec2& currentCorner = polygon_corners[index];

                if (currentCorner.x < line_position.x)
                {
                    cornerLeft = true;
                }
                else if (currentCorner.x > line_position.x)
                {
                    cornerRight = true;
                }

                if (cornerLeft && cornerRight)
                {
                    // corners on opposite sides
                    return true;
                }
            }

            return false;
        }

        // y = mx + b
        // b = y - mx
        float line_yIntercept = line_position.y - line_slope * line_position.x;

        // y = mx + b
        // y - mx - b = 0 = yInterceptOffset
        // If yInterceptOffset > 0.0, the point is above the line
        // If yInterceptOffset < 0.0, the point is below the line
        bool cornerAbove = false;
        bool cornerBelow = false;

        for (size_t index = 0; index < polygon_cornerCount; ++index)
        {
            const glm::vec2& currentCorner = polygon_corners[index];

            float currentCorner_yInterceptOffset =
                currentCorner.y - line_slope * currentCorner.x - line_yIntercept;

            if (currentCorner_yInterceptOffset > 0.0f)
            {
                cornerAbove = true;
            }
            else if (currentCorner_yInterceptOffset < 0.0f)
            {
                cornerBelow = true;
            }

            if (cornerAbove && cornerBelow)
            {
                // corners on opposite sides
                return true;
            }
        }

        return false;
    }

    inline bool Check2D_Line_ConvexPolygon_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Line_Polygon_Overlap(line_position, line_slope, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Overlap Functions (Ray) -------------------------------------------------

    inline bool Check2D_Ray_Line_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_Ray_Overlap(line_position, line_slope, ray_position, ray_direction);
    }

    inline bool Check2D_Ray_Ray_Overlap(
        const glm::vec2& rayA_position,
        const glm::vec2& rayA_direction,
        const glm::vec2& rayB_position,
        const glm::vec2& rayB_direction)
    {
        glm::vec2 perpendicular(-rayA_direction.y, rayA_direction.x);
        glm::vec2 b_to_a = rayA_position - rayB_position;

        float denominator = glm::dot(rayB_direction, perpendicular);
        if (denominator == 0.0f)
        {
            // rays are parallel
            return false;
        }

        float t1 = (rayB_direction.x * b_to_a.y - b_to_a.x * rayB_direction.y) / denominator;
        float t2 = glm::dot(b_to_a, perpendicular) / denominator;

        return t2 > 0.0f && t1 > 0.0f;
    }

    inline bool Check2D_Ray_LineSegment_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        glm::vec2 perpendicular(-ray_direction.y, ray_direction.x);
        glm::vec2 startToRay = ray_position - lineSegment_start;
        glm::vec2 startToEnd = lineSegment_end - lineSegment_start;

        float denominator = glm::dot(startToEnd, perpendicular);
        if (denominator == 0.0f)
        {
            // lineSegment and ray are parallel
            return false;
        }

        float t1 = (startToEnd.x * startToRay.y - startToRay.x * startToEnd.y) / denominator;
        float t2 = glm::dot(startToRay, perpendicular) / denominator;

        return t2 > 0.0f &&t2 < 1.0f && t1 > 0.0f;
    }

    inline bool Check2D_Ray_Rectangle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        // Check if the ray crosses the bottom or the right of the rectangle
        glm::vec2 rectangle_bottomRight(rectangle_topRight.x, rectangle_bottomLeft.y);
        if (Check2D_Ray_LineSegment_Overlap(ray_position, ray_direction, rectangle_bottomLeft, rectangle_bottomRight) ||
            Check2D_Ray_LineSegment_Overlap(ray_position, ray_direction, rectangle_bottomRight, rectangle_topRight))
        {
            return true;
        }

        // Check if the ray crosses the top or the left of the rectangle
        glm::vec2 rectangle_topLeft(rectangle_bottomLeft.x, rectangle_topRight.y);
        return Check2D_Ray_LineSegment_Overlap(ray_position, ray_direction, rectangle_topRight, rectangle_topLeft) ||
            Check2D_Ray_LineSegment_Overlap(ray_position, ray_direction, rectangle_topLeft, rectangle_bottomLeft);
    }

    inline bool Check2D_Ray_OrientedRectangle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        glm::vec2 transformed_position = ray_position - orientedRectangle_position;
        transformed_position = Rotate2DVector(transformed_position, -orientedRectangle_rotation);
        glm::vec2 transformed_direction = Rotate2DVector(ray_direction, -orientedRectangle_rotation);

        return Check2D_Ray_Rectangle_Overlap(transformed_position, transformed_direction, -orientedRectangle_halfSize, orientedRectangle_halfSize);
    }

    inline bool Check2D_Ray_Circle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        float distanceSquared = Get2D_Point_Ray_DistanceSquared(circle_position, ray_position, ray_direction);
        return distanceSquared < circle_radius * circle_radius;
    }

    inline bool Check2D_Ray_Capsule_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        if (Check2D_Ray_LineSegment_Overlap(ray_position, ray_direction, capsule_start, capsule_end))
        {
            // ray crosses the capsule
            return true;
        }

        float capsuleRadiusSquared = capsule_radius * capsule_radius;

        float pointRayDistanceSquared1 = Get2D_Point_Ray_DistanceSquared(capsule_start, ray_position, ray_direction);
        if (pointRayDistanceSquared1 < capsuleRadiusSquared)
        {
            // capsule start is near the ray
            return true;
        }

        float pointRayDistanceSquared2 = Get2D_Point_Ray_DistanceSquared(capsule_end, ray_position, ray_direction);
        if (pointRayDistanceSquared2 < capsuleRadiusSquared)
        {
            // capsule end is near the ray
            return true;
        }

        // check if ray start is near capsule
        float pointLineSegmentDistanceSquared = Get2D_Point_LineSegment_DistanceSquared(ray_position, capsule_start, capsule_end);
        return pointLineSegmentDistanceSquared < capsuleRadiusSquared;
    }

    inline bool Check2D_Ray_Triangle_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        return Check2D_Ray_LineSegment_Overlap(ray_position, ray_direction, triangle_corner1, triangle_corner2) ||
            Check2D_Ray_LineSegment_Overlap(ray_position, ray_direction, triangle_corner2, triangle_corner3) ||
            Check2D_Ray_LineSegment_Overlap(ray_position, ray_direction, triangle_corner3, triangle_corner1);
    }

    inline bool Check2D_Ray_Polygon_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount < 3)
        {
            return false;
        }

        size_t start_index = polygon_cornerCount - 1;
        for (size_t end_index = 0; end_index < polygon_cornerCount; ++end_index)
        {
            const glm::vec2& lineSegment_start = polygon_corners[start_index];
            const glm::vec2& lineSegment_end = polygon_corners[end_index];

            if (Check2D_Ray_LineSegment_Overlap(ray_position, ray_direction, lineSegment_start, lineSegment_end))
            {
                return true;
            }

            start_index = end_index;
        }

        return false;
    }

    inline bool Check2D_Ray_ConvexPolygon_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Ray_Polygon_Overlap(ray_position, ray_direction, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Overlap Functions (LineSegment) -----------------------------------------

    inline bool Check2D_LineSegment_Line_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_LineSegment_Overlap(line_position, line_slope, lineSegment_start, lineSegment_end);
    }

    inline bool Check2D_LineSegment_Ray_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        return Check2D_Ray_LineSegment_Overlap(ray_position, ray_direction, lineSegment_start, lineSegment_end);
    }

    inline bool Check2D_LineSegment_LineSegment_Overlap(
        const glm::vec2& lineSegmentA_start,
        const glm::vec2& lineSegmentA_end,
        const glm::vec2& lineSegmentB_start,
        const glm::vec2& lineSegmentB_end)
    {
        int afd1 = Get2D_Triangle_ApparentFacingDirection(lineSegmentA_start, lineSegmentA_end, lineSegmentB_start);
        int afd2 = Get2D_Triangle_ApparentFacingDirection(lineSegmentA_start, lineSegmentA_end, lineSegmentB_end);
        int afd3 = Get2D_Triangle_ApparentFacingDirection(lineSegmentB_start, lineSegmentB_end, lineSegmentA_start);
        int afd4 = Get2D_Triangle_ApparentFacingDirection(lineSegmentB_start, lineSegmentB_end, lineSegmentA_end);

        if (afd1 != afd2 && afd3 != afd4)
        {
            return true;
        }

        // lineSegmentB_start is colinear with and lies on LineSegmentA
        if (afd1 == 0 && Check2D_Point_Rectangle_Overlap_H(lineSegmentB_start, lineSegmentA_start, lineSegmentA_end))
        {
            return true;
        }

        // lineSegmentB_end is colinear with and lies on LineSegmentA
        if (afd2 == 0 && Check2D_Point_Rectangle_Overlap_H(lineSegmentB_end, lineSegmentA_start, lineSegmentA_end))
        {
            return true;
        }

        // lineSegmentA_start is colinear with and lies on LineSegmentB
        if (afd3 == 0 && Check2D_Point_Rectangle_Overlap_H(lineSegmentA_start, lineSegmentB_start, lineSegmentB_end))
        {
            return true;
        }

        // lineSegmentA_end is colinear with and lies on LineSegmentB
        if (afd4 == 0 && Check2D_Point_Rectangle_Overlap_H(lineSegmentA_end, lineSegmentB_start, lineSegmentB_end))
        {
            return true;
        }

        return false;
    }

    inline bool Check2D_LineSegment_Rectangle_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, lineSegment_start, lineSegment_end) ||
            Check2D_Point_Rectangle_Overlap(lineSegment_start, rectangle_bottomLeft, rectangle_topRight);
    }

    inline bool Check2D_LineSegment_OrientedRectangle_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        // rotate and translate the lineSegment rather then the orientedRectangle
        glm::vec2 transformedLineSegment_start = lineSegment_start - orientedRectangle_position;
        transformedLineSegment_start = Rotate2DVector(transformedLineSegment_start, -1.0f * orientedRectangle_rotation);
        glm::vec2 transformedLineSegment_end = lineSegment_end - orientedRectangle_position;
        transformedLineSegment_end = Rotate2DVector(transformedLineSegment_end, -1.0f * orientedRectangle_rotation);

        return Check2D_LineSegment_Rectangle_Overlap(
            transformedLineSegment_start, transformedLineSegment_end,
            -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize);
    }

    inline bool Check2D_LineSegment_Circle_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        return Check2D_Point_Capsule_Overlap(circle_position, lineSegment_start, lineSegment_end, circle_radius);
    }

    inline bool Check2D_LineSegment_Capsule_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        if (Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, capsule_start, capsule_end))
        {
            // lineSegment crosses the capsule
            return true;
        }

        float capsuleRadiusSquared = capsule_radius * capsule_radius;

        float pointLineSegmentDistanceSquared1 = Get2D_Point_LineSegment_DistanceSquared(capsule_start, lineSegment_start, lineSegment_end);
        if (pointLineSegmentDistanceSquared1 < capsuleRadiusSquared)
        {
            // capsule start is near lineSegment
            return true;
        }

        float pointLineSegmentDistanceSquared2 = Get2D_Point_LineSegment_DistanceSquared(capsule_end, lineSegment_start, lineSegment_end);
        if (pointLineSegmentDistanceSquared2 < capsuleRadiusSquared)
        {
            // capsule end is near lineSegment
            return true;
        }

        float pointLineSegmentDistanceSquared3 = Get2D_Point_LineSegment_DistanceSquared(lineSegment_start, capsule_start, capsule_end);
        if (pointLineSegmentDistanceSquared3 < capsuleRadiusSquared)
        {
            // lineSegment start in near capsule
            return true;
        }

        // check if lineSegment end is near capsule
        float pointLineSegmentDistanceSquared4 = Get2D_Point_LineSegment_DistanceSquared(lineSegment_end, capsule_start, capsule_end);
        return pointLineSegmentDistanceSquared4 < capsuleRadiusSquared;
    }

    inline bool Check2D_LineSegment_Triangle_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        return Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner1, triangle_corner2) ||
            Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner2, triangle_corner3) ||
            Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner3, triangle_corner1) ||
            Check2D_Point_Triangle_Overlap(lineSegment_start, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_LineSegment_Polygon_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount < 3)
        {
            return false;
        }

        size_t start_index = polygon_cornerCount - 1;
        for (size_t end_index = 0; end_index < polygon_cornerCount; ++end_index)
        {
            const glm::vec2& lineSegmentB_start = polygon_corners[start_index];
            const glm::vec2& lineSegmentB_end = polygon_corners[end_index];

            if (Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, lineSegmentB_start, lineSegmentB_end))
            {
                return true;
            }

            start_index = end_index;
        }

        return Check2D_Point_Polygon_Overlap(lineSegment_start, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_LineSegment_ConvexPolygon_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_LineSegment_Polygon_Overlap(lineSegment_start, lineSegment_end, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Overlap Functions (Rectangle) -------------------------------------------

    inline bool Check2D_Rectangle_Point_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& point_position)
    {
        return Check2D_Point_Rectangle_Overlap(point_position, rectangle_bottomLeft, rectangle_topRight);
    }

    inline bool Check2D_Rectangle_Line_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_Rectangle_Overlap(line_position, line_slope, rectangle_bottomLeft, rectangle_topRight);
    }

    inline bool Check2D_Rectangle_Ray_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        return Check2D_Ray_Rectangle_Overlap(ray_position, ray_direction, rectangle_bottomLeft, rectangle_topRight);
    }

    inline bool Check2D_Rectangle_LineSegment_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        return Check2D_LineSegment_Rectangle_Overlap(lineSegment_start, lineSegment_end, rectangle_bottomLeft, rectangle_topRight);
    }

    inline bool Check2D_Rectangle_Rectangle_Overlap(
        const glm::vec2& rectangleA_bottomLeft,
        const glm::vec2& rectangleA_topRight,
        const glm::vec2& rectangleB_bottomLeft,
        const glm::vec2& rectangleB_topRight)
    {
        // Axis-Aligned Bounding Box (AABB)
        return rectangleA_bottomLeft.x < rectangleB_topRight.x &&
            rectangleA_topRight.x > rectangleB_bottomLeft.x &&
            rectangleA_bottomLeft.y < rectangleB_topRight.y &&
            rectangleA_topRight.y> rectangleB_bottomLeft.y;
    }

    inline bool Check2D_Rectangle_OrientedRectangle_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        glm::vec2 rectangle_corner_array[4] = {
            rectangle_bottomLeft,
            glm::vec2(rectangle_topRight.x, rectangle_bottomLeft.y), // bottomRight
            rectangle_topRight,
            glm::vec2(rectangle_bottomLeft.x, rectangle_topRight.y) // topLeft
        };

        glm::vec2 orientedRectangle_corner_array[4] = {
            glm::vec2(-1.0f * orientedRectangle_halfSize.x, -1.0f * orientedRectangle_halfSize.y), // bottomLeft
            glm::vec2(orientedRectangle_halfSize.x, -1.0f * orientedRectangle_halfSize.y), // bottomRight
            glm::vec2(orientedRectangle_halfSize.x, orientedRectangle_halfSize.y), // topRight
            glm::vec2(-1.0f * orientedRectangle_halfSize.x, orientedRectangle_halfSize.y) // topLeft
        };

        for (size_t i = 0; i < 4; ++i)
        {
            glm::vec2& current_orientedRectangle_corner = orientedRectangle_corner_array[i];
            current_orientedRectangle_corner = Rotate2DVector(current_orientedRectangle_corner, orientedRectangle_rotation);
            current_orientedRectangle_corner += orientedRectangle_position;
        }

        const glm::vec2 axes[4] = {
            glm::vec2(1, 0), // axis aligned rectangle x-axis
            glm::vec2(0, 1), // axis aligned rectangle y-axis
            orientedRectangle_corner_array[1] - orientedRectangle_corner_array[0], // Oriented rectangle edge 1
            orientedRectangle_corner_array[3] - orientedRectangle_corner_array[0]  // Oriented rectangle edge 2
        };

        for (size_t i = 0; i < 4; ++i)
        {
            const glm::vec2& axisOfSeparation = axes[i];

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            ProjectPolygonOntoAxis(axisOfSeparation, &rectangle_corner_array[0], 4, projectionA_max, projectionA_min);

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            ProjectPolygonOntoAxis(axisOfSeparation, &orientedRectangle_corner_array[0], 4, projectionB_max, projectionB_min);

            if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
            {
                // projections don't overlap on axis of separation
                return false;
            }
        }

        return true;
    }

    inline bool Check2D_Rectangle_Circle_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        float testX = circle_position.x;
        float testY = circle_position.y;

        // find closest edge to circle
        if (circle_position.x < rectangle_bottomLeft.x)
        {
            // left edge
            testX = rectangle_bottomLeft.x;
        }
        else if (circle_position.x > rectangle_topRight.x)
        {
            // right edge
            testX = rectangle_topRight.x;
        }
        if (circle_position.y < rectangle_bottomLeft.y)
        {
            // bottom edge
            testY = rectangle_bottomLeft.y;
        }
        else if (circle_position.y > rectangle_topRight.y)
        {
            // top edge
            testY = rectangle_topRight.y;
        }

        float distanceX = circle_position.x - testX;
        float distanceY = circle_position.y - testY;
        float distanceSquared = distanceX * distanceX + distanceY * distanceY;
        float circleRadiusSquared = circle_radius * circle_radius;
        return distanceSquared < circleRadiusSquared;
    }

    inline bool Check2D_Rectangle_Capsule_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        // check if rectangle overlaps with one of the ends
        if (Check2D_Rectangle_Circle_Overlap(rectangle_bottomLeft, rectangle_topRight, capsule_start, capsule_radius) ||
            Check2D_Rectangle_Circle_Overlap(rectangle_bottomLeft, rectangle_topRight, capsule_end, capsule_radius))
        {
            return true;
        }

        glm::vec2 capsuleLine = capsule_end - capsule_start;
        glm::vec2 perpenduclarLine(-1.0f * capsuleLine.y, capsuleLine.x);
        perpenduclarLine *= 1.0f / glm::sqrt(glm::dot(perpenduclarLine, perpenduclarLine));
        perpenduclarLine *= capsule_radius;

        glm::vec2 rightCapsuleLine_start = capsule_start + perpenduclarLine;
        glm::vec2 rightCapsuleLine_end = capsule_end + perpenduclarLine;

        // check if one of the sides of the capsule cross the rectangle frame
        if (Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, rightCapsuleLine_start, rightCapsuleLine_end))
        {
            return true;
        }

        glm::vec2 leftCapsuleLine_start = capsule_start - perpenduclarLine;
        glm::vec2 leftCapsuleLine_end = capsule_end - perpenduclarLine;

        // check if the other side of the capsule cross the rectangle frame
        if (Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, leftCapsuleLine_start, leftCapsuleLine_end))
        {
            return true;
        }

        // check if a corner of the rectangle is inside the capsule rectangle
        float distanceSquared = Get2D_Point_LineSegment_DistanceSquared(rectangle_bottomLeft, capsule_start, capsule_end);
        float capsuleRadiusSquared = capsule_radius * capsule_radius;
        return distanceSquared < capsuleRadiusSquared;
    }

    inline bool Check2D_Rectangle_Triangle_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        glm::vec2 rectangle_corner_array[4] = {
            rectangle_bottomLeft,
            glm::vec2(rectangle_topRight.x, rectangle_bottomLeft.y), // bottomRight
            rectangle_topRight,
            glm::vec2(rectangle_bottomLeft.x, rectangle_topRight.y) // topLeft
        };

        glm::vec2 triangle_corner_array[3] = {
            triangle_corner1,
            triangle_corner2,
            triangle_corner3
        };

        const glm::vec2 axes[5] = {
            glm::vec2(1, 0), // axis aligned rectangle x-axis
            glm::vec2(0, 1), // axis aligned rectangle y-axis
            glm::vec2(-(triangle_corner2.y - triangle_corner1.y), triangle_corner2.x - triangle_corner1.x), // side 1,2 normal
            glm::vec2(-(triangle_corner3.y - triangle_corner2.y), triangle_corner3.x - triangle_corner2.x), // side 2,3 normal
            glm::vec2(-(triangle_corner1.y - triangle_corner3.y), triangle_corner1.x - triangle_corner3.x) // side 3,1 normal
        };

        for (size_t i = 0; i < 5; ++i)
        {
            const glm::vec2& axisOfSeparation = axes[i];

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            ProjectPolygonOntoAxis(axisOfSeparation, &rectangle_corner_array[0], 4, projectionA_max, projectionA_min);

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            ProjectPolygonOntoAxis(axisOfSeparation, &triangle_corner_array[0], 3, projectionB_max, projectionB_min);

            if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
            {
                // projections don't overlap on axis of separation
                return false;
            }
        }

        return true;
    }

    inline bool Check2D_Rectangle_Polygon_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount < 3)
        {
            return false;
        }

        size_t start_index = polygon_cornerCount - 1;
        for (size_t end_index = 0; end_index < polygon_cornerCount; ++end_index)
        {
            const glm::vec2& lineSegment_start = polygon_corners[start_index];
            const glm::vec2& lineSegment_end = polygon_corners[end_index];

            if (Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, lineSegment_start, lineSegment_end))
            {
                return true;
            }

            start_index = end_index;
        }

        return Check2D_Point_Polygon_Overlap(rectangle_bottomLeft, polygon_corners, polygon_cornerCount) ||
            Check2D_Point_Rectangle_Overlap(polygon_corners[0], rectangle_bottomLeft, rectangle_topRight);
    }

    inline bool Check2D_Rectangle_ConvexPolygon_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        // Note this can be further optimized, we only need to check 2 axis for
        // a rectangle when doing the SAT.

        glm::vec2 convexPolygonA_array[4] = {
            rectangle_bottomLeft,
            glm::vec2(rectangle_topRight.x, rectangle_bottomLeft.y), // bottomRight
            rectangle_topRight,
            glm::vec2(rectangle_bottomLeft.x, rectangle_topRight.y)}; // topLeft
        return Check2D_ConvexPolygon_ConvexPolygon_Overlap(&convexPolygonA_array[0], 4, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Overlap Functions (OrientedRectangle) -----------------------------------

    inline bool Check2D_OrientedRectangle_Point_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& point_position)
    {
        return Check2D_Point_OrientedRectangle_Overlap(point_position, orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation);
    }

    inline bool Check2D_OrientedRectangle_Line_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_OrientedRectangle_Overlap(line_position, line_slope, orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation);
    }

    inline bool Check2D_OrientedRectangle_Ray_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        return Check2D_Ray_OrientedRectangle_Overlap(ray_position, ray_direction, orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation);
    }

    inline bool Check2D_OrientedRectangle_LineSegment_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        return Check2D_LineSegment_OrientedRectangle_Overlap(lineSegment_start, lineSegment_end, orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation);
    }

    inline bool Check2D_OrientedRectangle_Rectangle_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return Check2D_Rectangle_OrientedRectangle_Overlap(rectangle_bottomLeft, rectangle_topRight, orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation);
    }

    inline bool Check2D_OrientedRectangle_OrientedRectangle_Overlap(
        const glm::vec2& orientedRectangleA_halfSize,
        const glm::vec2& orientedRectangleA_position,
        const float& orientedRectangleA_rotation,
        const glm::vec2& orientedRectangleB_halfSize,
        const glm::vec2& orientedRectangleB_position,
        const float& orientedRectangleB_rotation)
    {
        // Combine the transformations applying them to only orientedRectangleB,
        // making orientedRectangleA nolonger orinented

        // Cancel orientedRectangleA translation by applying the reverse to
        // orientedRectangleB
        glm::vec2 combinedRectangleB_position = orientedRectangleB_position - orientedRectangleA_position;

        // Cancel orientedRectangleA rotation by applying the reverse to
        // orientedRectangleB
        combinedRectangleB_position = Rotate2DVector(combinedRectangleB_position, -1.0f * orientedRectangleA_rotation);
        float combinedRectangleB_rotation = orientedRectangleB_rotation - orientedRectangleA_rotation;

        return Check2D_Rectangle_OrientedRectangle_Overlap(-1.0f * orientedRectangleA_halfSize, orientedRectangleA_halfSize, orientedRectangleB_halfSize, combinedRectangleB_position, combinedRectangleB_rotation);
    }

    inline bool Check2D_OrientedRectangle_Circle_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        // rotate and translate the circle rather then the orientedRectangle
        glm::vec2 transformedCirclePosition = circle_position - orientedRectangle_position;
        transformedCirclePosition = Rotate2DVector(transformedCirclePosition, -1.0f * orientedRectangle_rotation);

        return Check2D_Rectangle_Circle_Overlap(-1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize, transformedCirclePosition, circle_radius);
    }

    inline bool Check2D_OrientedRectangle_Capsule_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        // rotate and translate the capsule rather then the orientedRectangle
        glm::vec2 transformedCapsuleStart = capsule_start - orientedRectangle_position;
        transformedCapsuleStart = Rotate2DVector(transformedCapsuleStart, -1.0f * orientedRectangle_rotation);
        glm::vec2 transformedCapsuleEnd = capsule_end - orientedRectangle_position;
        transformedCapsuleEnd = Rotate2DVector(transformedCapsuleEnd, -1.0f * orientedRectangle_rotation);

        return Check2D_Rectangle_Capsule_Overlap(-1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize, transformedCapsuleStart, transformedCapsuleEnd, capsule_radius);
    }

    inline bool Check2D_OrientedRectangle_Triangle_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        // rotate and translate the triangle rather then the orientedRectangle
        glm::vec2 transformedTriangleCorner1 = triangle_corner1 - orientedRectangle_position;
        transformedTriangleCorner1 = Rotate2DVector(transformedTriangleCorner1, -1.0f * orientedRectangle_rotation);
        glm::vec2 transformedTriangleCorner2 = triangle_corner2 - orientedRectangle_position;
        transformedTriangleCorner2 = Rotate2DVector(transformedTriangleCorner2, -1.0f * orientedRectangle_rotation);
        glm::vec2 transformedTriangleCorner3 = triangle_corner3 - orientedRectangle_position;
        transformedTriangleCorner3 = Rotate2DVector(transformedTriangleCorner3, -1.0f * orientedRectangle_rotation);

        return Check2D_Rectangle_Triangle_Overlap(-1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize, transformedTriangleCorner1, transformedTriangleCorner2, transformedTriangleCorner3);
    }

    inline bool Check2D_OrientedRectangle_Polygon_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount < 3)
        {
            return false;
        }

        glm::vec2 rectangle_bottomLeft = -1.0f * orientedRectangle_halfSize;
        const glm::vec2& rectangle_topRight = orientedRectangle_halfSize;

        size_t start_index = polygon_cornerCount - 1;
        for (size_t end_index = 0; end_index < polygon_cornerCount; ++end_index)
        {
            const glm::vec2& lineSegment_start = polygon_corners[start_index];
            const glm::vec2& lineSegment_end = polygon_corners[end_index];

            // rotate and translate the lineSegment rather then the orientedRectangle
            glm::vec2 transformedLineSegment_start = lineSegment_start - orientedRectangle_position;
            transformedLineSegment_start = Rotate2DVector(transformedLineSegment_start, -1.0f * orientedRectangle_rotation);
            glm::vec2 transformedLineSegment_end = lineSegment_end - orientedRectangle_position;
            transformedLineSegment_end = Rotate2DVector(transformedLineSegment_end, -1.0f * orientedRectangle_rotation);

            if (Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, transformedLineSegment_start, transformedLineSegment_end))
            {
                return true;
            }

            start_index = end_index;
        }

        return Check2D_Point_Polygon_Overlap(orientedRectangle_position, polygon_corners, polygon_cornerCount) ||
            Check2D_Point_OrientedRectangle_Overlap(polygon_corners[0], orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation);
    }

    inline bool Check2D_OrientedRectangle_ConvexPolygon_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        // Note this can be further optimized, we only need to check 2 axis for
        // a rectangle when doing the SAT.

        glm::vec2 convexPolygonA_array[4];
        convexPolygonA_array[0] = Rotate2DVector(-orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        convexPolygonA_array[1].x = orientedRectangle_halfSize.x;
        convexPolygonA_array[1].y = -orientedRectangle_halfSize.y;
        convexPolygonA_array[1] = Rotate2DVector(convexPolygonA_array[1], orientedRectangle_rotation) + orientedRectangle_position;
        convexPolygonA_array[2] = Rotate2DVector(orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        convexPolygonA_array[3].x = -orientedRectangle_halfSize.x;
        convexPolygonA_array[3].y = orientedRectangle_halfSize.y;
        convexPolygonA_array[3] = Rotate2DVector(convexPolygonA_array[3], orientedRectangle_rotation) + orientedRectangle_position;
        return Check2D_ConvexPolygon_ConvexPolygon_Overlap(&convexPolygonA_array[0], 4, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Overlap Functions (Circle) ----------------------------------------------

    inline bool Check2D_Circle_Point_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& point_position)
    {
        return Check2D_Point_Circle_Overlap(point_position, circle_position, circle_radius);
    }

    inline bool Check2D_Circle_Line_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_Circle_Overlap(line_position, line_slope, circle_position, circle_radius);
    }

    inline bool Check2D_Circle_Ray_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        return Check2D_Ray_Circle_Overlap(ray_position, ray_direction, circle_position, circle_radius);
    }

    inline bool Check2D_Circle_LineSegment_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        return Check2D_LineSegment_Circle_Overlap(lineSegment_start, lineSegment_end, circle_position, circle_radius);
    }

    inline bool Check2D_Circle_Rectangle_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return Check2D_Rectangle_Circle_Overlap(rectangle_bottomLeft, rectangle_topRight, circle_position, circle_radius);
    }

    inline bool Check2D_Circle_OrientedRectangle_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        return Check2D_OrientedRectangle_Circle_Overlap(orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, circle_position, circle_radius);
    }

    inline bool Check2D_Circle_Circle_Overlap(
        const glm::vec2& circleA_position,
        const float& circleA_radius,
        const glm::vec2& circleB_position,
        const float& circleB_radius)
    {
        return Check2D_Point_Circle_Overlap(circleA_position, circleB_position, circleA_radius + circleB_radius);
    }

    inline bool Check2D_Circle_Capsule_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        return Check2D_Point_Capsule_Overlap(circle_position, capsule_start, capsule_end, circle_radius + capsule_radius);
    }

    inline bool Check2D_Circle_Triangle_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        return Check2D_Point_Capsule_Overlap(circle_position, triangle_corner1, triangle_corner2, circle_radius) ||
            Check2D_Point_Capsule_Overlap(circle_position, triangle_corner2, triangle_corner3, circle_radius) ||
            Check2D_Point_Capsule_Overlap(circle_position, triangle_corner3, triangle_corner1, circle_radius) ||
            Check2D_Point_Triangle_Overlap(circle_position, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Circle_Polygon_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount < 3)
        {
            return false;
        }

        size_t start_index = polygon_cornerCount - 1;
        for (size_t end_index = 0; end_index < polygon_cornerCount; ++end_index)
        {
            const glm::vec2& lineSegment_start = polygon_corners[start_index];
            const glm::vec2& lineSegment_end = polygon_corners[end_index];

            if (Check2D_Point_Capsule_Overlap(circle_position, lineSegment_start, lineSegment_end, circle_radius))
            {
                return true;
            }

            start_index = end_index;
        }

        return Check2D_Point_Polygon_Overlap(circle_position, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Circle_ConvexPolygon_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        if (convexPolygon_cornerCount < 3)
        {
            return false;
        }

        size_t start_indexA = convexPolygon_cornerCount - 1;
        for (size_t end_indexA = 0; end_indexA < convexPolygon_cornerCount; ++end_indexA)
        {
            const glm::vec2& lineSegmentA_start = convexPolygon_corners[start_indexA];
            const glm::vec2& lineSegmentA_end = convexPolygon_corners[end_indexA];
            glm::vec2 axisOfSeparation = lineSegmentA_end - lineSegmentA_start;
            axisOfSeparation = glm::vec2(axisOfSeparation.y, -1.0f * axisOfSeparation.x);
            axisOfSeparation = glm::normalize(axisOfSeparation);

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            ProjectCircleOntoAxis(axisOfSeparation, circle_position, circle_radius, projectionA_max, projectionA_min);

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            ProjectPolygonOntoAxis(axisOfSeparation, convexPolygon_corners, convexPolygon_cornerCount, projectionB_max, projectionB_min);

            if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
            {
                // projections don't overlap on axis of separation
                return false;
            }

            start_indexA = end_indexA;
        }

        glm::vec2 axisOfSeparation = glm::normalize(circle_position - convexPolygon_corners[0]);
        axisOfSeparation = glm::normalize(axisOfSeparation);

        float projectionA_max = -std::numeric_limits<float>::infinity();
        float projectionA_min = std::numeric_limits<float>::infinity();
        ProjectCircleOntoAxis(axisOfSeparation, circle_position, circle_radius, projectionA_max, projectionA_min);

        float projectionB_max = -std::numeric_limits<float>::infinity();
        float projectionB_min = std::numeric_limits<float>::infinity();
        ProjectPolygonOntoAxis(axisOfSeparation, convexPolygon_corners, convexPolygon_cornerCount, projectionB_max, projectionB_min);

        if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
        {
            // projections don't overlap on axis of separation
            return false;
        }

        return true;
    }

    // Overlap Functions (Capsule) ---------------------------------------------

    inline bool Check2D_Capsule_Point_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& point_position)
    {
        return Check2D_Point_Capsule_Overlap(point_position, capsule_start, capsule_end, capsule_radius);
    }

    inline bool Check2D_Capsule_Line_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_Capsule_Overlap(line_position, line_slope, capsule_start, capsule_end, capsule_radius);
    }

    inline bool Check2D_Capsule_Ray_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        return Check2D_Ray_Capsule_Overlap(ray_position, ray_direction, capsule_start, capsule_end, capsule_radius);
    }

    inline bool Check2D_Capsule_LineSegment_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        return Check2D_LineSegment_Capsule_Overlap(lineSegment_start, lineSegment_end, capsule_start, capsule_end, capsule_radius);
    }

    inline bool Check2D_Capsule_Rectangle_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return Check2D_Rectangle_Capsule_Overlap(rectangle_bottomLeft, rectangle_topRight, capsule_start, capsule_end, capsule_radius);
    }

    inline bool Check2D_Capsule_OrientedRectangle_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        return Check2D_OrientedRectangle_Capsule_Overlap(orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, capsule_start, capsule_end, capsule_radius);
    }

    inline bool Check2D_Capsule_Circle_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        return Check2D_Point_Capsule_Overlap(circle_position, capsule_start, capsule_end, circle_radius + capsule_radius);
    }

    inline bool Check2D_Capsule_Capsule_Overlap(
        const glm::vec2& capsuleA_start,
        const glm::vec2& capsuleA_end,
        const float& capsuleA_radius,
        const glm::vec2& capsuleB_start,
        const glm::vec2& capsuleB_end,
        const float& capsuleB_radius)
    {
        return Check2D_LineSegment_Capsule_Overlap(capsuleA_start, capsuleA_end, capsuleB_start, capsuleB_end, capsuleA_radius + capsuleB_radius);
    }

    inline bool Check2D_Capsule_Triangle_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        return Check2D_LineSegment_Capsule_Overlap(capsule_start, capsule_end, triangle_corner1, triangle_corner2, capsule_radius) ||
            Check2D_LineSegment_Capsule_Overlap(capsule_start, capsule_end, triangle_corner2, triangle_corner3, capsule_radius) ||
            Check2D_LineSegment_Capsule_Overlap(capsule_start, capsule_end, triangle_corner3, triangle_corner1, capsule_radius) ||
            Check2D_Point_Triangle_Overlap(capsule_start, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Capsule_Polygon_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount < 3)
        {
            return false;
        }

        size_t start_index = polygon_cornerCount - 1;
        for (size_t end_index = 0; end_index < polygon_cornerCount; ++end_index)
        {
            const glm::vec2& lineSegment_start = polygon_corners[start_index];
            const glm::vec2& lineSegment_end = polygon_corners[end_index];

            if (Check2D_LineSegment_Capsule_Overlap(lineSegment_start, lineSegment_end, capsule_start, capsule_end, capsule_radius))
            {
                return true;
            }

            start_index = end_index;
        }

        return Check2D_Point_Polygon_Overlap(capsule_start, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Capsule_ConvexPolygon_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        if (convexPolygon_cornerCount < 3)
        {
            return false;
        }

        size_t start_indexA = convexPolygon_cornerCount - 1;
        for (size_t end_indexA = 0; end_indexA < convexPolygon_cornerCount; ++end_indexA)
        {
            const glm::vec2& lineSegmentA_start = convexPolygon_corners[start_indexA];
            const glm::vec2& lineSegmentA_end = convexPolygon_corners[end_indexA];
            glm::vec2 axisOfSeparation = lineSegmentA_end - lineSegmentA_start;
            axisOfSeparation = glm::vec2(axisOfSeparation.y, -1.0f * axisOfSeparation.x);
            axisOfSeparation = glm::normalize(axisOfSeparation);

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            ProjectCapsuleOntoAxis(axisOfSeparation, capsule_start, capsule_end, capsule_radius, projectionA_max, projectionA_min);

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            ProjectPolygonOntoAxis(axisOfSeparation, convexPolygon_corners, convexPolygon_cornerCount, projectionB_max, projectionB_min);

            if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
            {
                // projections don't overlap on axis of separation
                return false;
            }

            start_indexA = end_indexA;
        }

        glm::vec2 axisOfSeparation = capsule_end - capsule_start;
        axisOfSeparation = glm::vec2(axisOfSeparation.y, -1.0f * axisOfSeparation.x);
        axisOfSeparation = glm::normalize(axisOfSeparation);

        float projectionA_max = -std::numeric_limits<float>::infinity();
        float projectionA_min = std::numeric_limits<float>::infinity();
        ProjectCapsuleOntoAxis(axisOfSeparation, capsule_start, capsule_end, capsule_radius, projectionA_max, projectionA_min);

        float projectionB_max = -std::numeric_limits<float>::infinity();
        float projectionB_min = std::numeric_limits<float>::infinity();
        ProjectPolygonOntoAxis(axisOfSeparation, convexPolygon_corners, convexPolygon_cornerCount, projectionB_max, projectionB_min);

        if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
        {
            // projections don't overlap on axis of separation
            return false;
        }

        return true;
    }

    // Overlap Functions (Triangle) --------------------------------------------

    inline bool Check2D_Triangle_Point_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& point_position)
    {
        return Check2D_Point_Triangle_Overlap(point_position, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Triangle_Line_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_Triangle_Overlap(line_position, line_slope, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Triangle_Ray_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        return Check2D_Ray_Triangle_Overlap(ray_position, ray_direction, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Triangle_LineSegment_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        return Check2D_LineSegment_Triangle_Overlap(lineSegment_start, lineSegment_end, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Triangle_Rectangle_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return Check2D_Rectangle_Triangle_Overlap(rectangle_bottomLeft, rectangle_topRight, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Triangle_OrientedRectangle_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        return Check2D_OrientedRectangle_Triangle_Overlap(orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Triangle_Circle_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        return Check2D_Circle_Triangle_Overlap(circle_position, circle_radius, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Triangle_Capsule_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        return Check2D_Capsule_Triangle_Overlap(capsule_start, capsule_end, capsule_radius, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Triangle_Triangle_Overlap(
        const glm::vec2& triangleA_corner1,
        const glm::vec2& triangleA_corner2,
        const glm::vec2& triangleA_corner3,
        const glm::vec2& triangleB_corner1,
        const glm::vec2& triangleB_corner2,
        const glm::vec2& triangleB_corner3)
    {
        glm::vec2 convexPolygonA_array[3] = { triangleA_corner1, triangleA_corner2, triangleA_corner3 };
        glm::vec2 convexPolygonB_array[3] = { triangleB_corner1, triangleB_corner2, triangleB_corner3 };
        return Check2D_ConvexPolygon_ConvexPolygon_Overlap(&convexPolygonA_array[0], 3, &convexPolygonB_array[0], 3);
    }

    inline bool Check2D_Triangle_Polygon_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount < 3)
        {
            return false;
        }

        size_t start_index = polygon_cornerCount - 1;
        for (size_t end_index = 0; end_index < polygon_cornerCount; ++end_index)
        {
            const glm::vec2& lineSegment_start = polygon_corners[start_index];
            const glm::vec2& lineSegment_end = polygon_corners[end_index];

            if (Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner1, triangle_corner2) ||
                Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner2, triangle_corner3) ||
                Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, triangle_corner3, triangle_corner1))
            {
                return true;
            }

            start_index = end_index;
        }

        return Check2D_Point_Polygon_Overlap(triangle_corner1, polygon_corners, polygon_cornerCount) ||
            Check2D_Point_Triangle_Overlap(polygon_corners[0], triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Triangle_ConvexPolygon_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        glm::vec2 convexPolygonA_array[3] = { triangle_corner1, triangle_corner2, triangle_corner3 };
        return Check2D_ConvexPolygon_ConvexPolygon_Overlap(&convexPolygonA_array[0], 3, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Overlap Functions (Polygon) ---------------------------------------------

    inline bool Check2D_Polygon_Point_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& point_position)
    {
        return Check2D_Point_Polygon_Overlap(point_position, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Line_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_Polygon_Overlap(line_position, line_slope, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Ray_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        return Check2D_Ray_Polygon_Overlap(ray_position, ray_direction, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_LineSegment_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        return Check2D_LineSegment_Polygon_Overlap(lineSegment_start, lineSegment_end, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Rectangle_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return Check2D_Rectangle_Polygon_Overlap(rectangle_bottomLeft, rectangle_topRight, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_OrientedRectangle_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        return Check2D_OrientedRectangle_Polygon_Overlap(orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Circle_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        return Check2D_Circle_Polygon_Overlap(circle_position, circle_radius, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Capsule_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        return Check2D_Capsule_Polygon_Overlap(capsule_start, capsule_end, capsule_radius, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Triangle_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        return Check2D_Triangle_Polygon_Overlap(triangle_corner1, triangle_corner2, triangle_corner3, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Polygon_Overlap(
        const glm::vec2* const& polygonA_corners,
        const size_t& polygonA_cornerCount,
        const glm::vec2* const& polygonB_corners,
        const size_t& polygonB_cornerCount)
    {
        if (polygonA_cornerCount < 3 || polygonB_cornerCount < 3)
        {
            return false;
        }

        size_t start_indexA = polygonA_cornerCount - 1;
        for (size_t end_indexA = 0; end_indexA < polygonA_cornerCount; ++end_indexA)
        {
            const glm::vec2& lineSegmentA_start = polygonA_corners[start_indexA];
            const glm::vec2& lineSegmentA_end = polygonA_corners[end_indexA];

            size_t start_indexB = polygonB_cornerCount - 1;
            for (size_t end_indexB = 0; end_indexB < polygonB_cornerCount; ++end_indexB)
            {
                const glm::vec2& lineSegmentB_start = polygonB_corners[start_indexB];
                const glm::vec2& lineSegmentB_end = polygonB_corners[end_indexB];

                if (Check2D_LineSegment_LineSegment_Overlap(lineSegmentA_start, lineSegmentA_end, lineSegmentB_start, lineSegmentB_end))
                {
                    return true;
                }

                start_indexB = end_indexB;
            }

            start_indexA = end_indexA;
        }

        return Check2D_Point_Polygon_Overlap(polygonA_corners[0], polygonB_corners, polygonB_cornerCount) ||
            Check2D_Point_Polygon_Overlap(polygonB_corners[0], polygonA_corners, polygonA_cornerCount);
    }

    inline bool Check2D_Polygon_ConvexPolygon_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Polygon_Polygon_Overlap(polygon_corners, polygon_cornerCount, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Overlap Functions (ConvexPolygon) ---------------------------------------

    inline bool Check2D_ConvexPolygon_Point_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& point_position)
    {
        return Check2D_Point_ConvexPolygon_Overlap(point_position, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Line_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_ConvexPolygon_Overlap(line_position, line_slope, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Ray_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        return Check2D_Ray_ConvexPolygon_Overlap(ray_position, ray_direction, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_LineSegment_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        return Check2D_LineSegment_ConvexPolygon_Overlap(lineSegment_start, lineSegment_end, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Rectangle_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return Check2D_Rectangle_ConvexPolygon_Overlap(rectangle_bottomLeft, rectangle_topRight, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_OrientedRectangle_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        return Check2D_OrientedRectangle_ConvexPolygon_Overlap(orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Circle_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        return Check2D_Circle_ConvexPolygon_Overlap(circle_position, circle_radius, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Capsule_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        return Check2D_Capsule_ConvexPolygon_Overlap(capsule_start, capsule_end, capsule_radius, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Triangle_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        return Check2D_Triangle_ConvexPolygon_Overlap(triangle_corner1, triangle_corner2, triangle_corner3, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Polygon_Overlap(
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        return Check2D_Polygon_ConvexPolygon_Overlap(polygon_corners, polygon_cornerCount, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_ConvexPolygon_Overlap(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount)
    {
        if (convexPolygonA_cornerCount < 3 || convexPolygonB_cornerCount < 3)
        {
            return false;
        }

        return Check2D_ConvexPolygon_ConvexPolygon_HalfSeparatedAxisTheorem(
            convexPolygonA_corners,
            convexPolygonA_cornerCount,
            convexPolygonB_corners,
            convexPolygonB_cornerCount) &&
            Check2D_ConvexPolygon_ConvexPolygon_HalfSeparatedAxisTheorem(
                convexPolygonB_corners,
                convexPolygonB_cornerCount,
                convexPolygonA_corners,
                convexPolygonA_cornerCount);
    }

    // Closest Point Functions -------------------------------------------------

    inline void Get2D_Point_Line_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& closestPoint_position)
    {
        if (std::isinf(line_slope))
        {
            // Nearest point on the Ray is the start
            closestPoint_position = glm::vec2(line_position.x, point_position.y);
            return;
        }

        float dx = line_position.x - point_position.x;
        float dy = line_position.y - point_position.y;
        float projection = dy - line_slope * dx;
        float directionLengthSquared = (1.0f + line_slope * line_slope);
        float normalizedProjection = projection / directionLengthSquared;
        closestPoint_position = glm::vec2(point_position.x - normalizedProjection * line_slope, point_position.y + normalizedProjection);
    }

    inline void Get2D_Point_Ray_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        glm::vec2& closestPoint_position)
    {
        glm::vec2 rayToPoint = point_position - ray_position;
        float dotProduct = glm::dot(ray_direction, rayToPoint);
        if (dotProduct < 0.0f)
        {
            // Nearest point on the Ray is the start
            closestPoint_position = ray_position;
            return;
        }

        float projection = dotProduct / glm::dot(ray_direction, ray_direction);
        closestPoint_position = ray_position + projection * ray_direction;
    }

    inline void Get2D_Point_LineSegment_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        glm::vec2& closestPoint_position)
    {
        glm::vec2 lineSegment = lineSegment_end - lineSegment_start;
        glm::vec2 startToPoint = point_position - lineSegment_start;
        float dotProduct1 = glm::dot(lineSegment, startToPoint);
        if (dotProduct1 < 0.0f)
        {
            // Nearest point on the line segment is the start point
            closestPoint_position = lineSegment_start;
            return;
        }

        glm::vec2 endToPoint = point_position - lineSegment_end;
        float dotProduct2 = glm::dot(lineSegment, endToPoint);
        if (dotProduct2 > 0.0f)
        {
            // Nearest point on the line segment is the end point
            closestPoint_position = lineSegment_end;
            return;
        }

        float projection = dotProduct1 / glm::dot(lineSegment, lineSegment);
        closestPoint_position = lineSegment_start + projection * lineSegment;
    }

    inline void Get2D_Point_Rectangle_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& closestPoint_position)
    {
        // Calculate the closest point on the rectangle to the point
        float closestX = std::max(rectangle_bottomLeft.x, std::min(point_position.x, rectangle_topRight.x));
        float closestY = std::max(rectangle_bottomLeft.y, std::min(point_position.y, rectangle_topRight.y));

        // Set the closest point
        closestPoint_position = glm::vec2(closestX, closestY);
    }

    inline void Get2D_Point_OrientedRectangle_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& closestPoint_position)
    {
        // rotate and translate the point rather then the orientedRectangle
        glm::vec2 translatedPointPosition = point_position - orientedRectangle_position;
        glm::vec2 rotatedPointPosition = Rotate2DVector(translatedPointPosition, -1.0f * orientedRectangle_rotation);

        Get2D_Point_Rectangle_ClosestPoint(
            rotatedPointPosition,
            -1.0f * orientedRectangle_halfSize,
            orientedRectangle_halfSize,
            closestPoint_position);

        // rotate and translate back
        closestPoint_position = Rotate2DVector(closestPoint_position, orientedRectangle_rotation);
        closestPoint_position += orientedRectangle_position;
    }

    inline void Get2D_Point_Circle_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& closestPoint_position)
    {
        glm::vec2 circleCenterToPoint = point_position - circle_position;
        float circleCenterToPointDistanceSquared = glm::dot(circleCenterToPoint, circleCenterToPoint);

        if (circleCenterToPointDistanceSquared <= circle_radius * circle_radius)
        {
            closestPoint_position = point_position;
        }
        else
        {
            float inverseCircleCenterToPointDistance = 1.0f / glm::sqrt(circleCenterToPointDistanceSquared);
            closestPoint_position = circle_position + circleCenterToPoint * circle_radius * inverseCircleCenterToPointDistance;
        }
    }

    inline void Get2D_Point_Capsule_ClosestPoint(
        const glm::vec2& point_position,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& closestPoint_position)
    {
        Get2D_Point_LineSegment_ClosestPoint(
            point_position,
            capsule_start,
            capsule_end,
            closestPoint_position);
        Get2D_Point_Circle_ClosestPoint(
            point_position,
            closestPoint_position,
            capsule_radius,
            closestPoint_position);
    }

    // Distance Squared Functions ----------------------------------------------

    inline float Get2D_Point_Point_DistanceSquared(
        const glm::vec2& pointA_position,
        const glm::vec2& pointB_position)
    {
        glm::vec2 pointA_to_pointB = pointB_position - pointA_position;
        return glm::dot(pointA_to_pointB, pointA_to_pointB);
    }

    inline float Get2D_Point_Line_DistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        if (std::isinf(line_slope))
        {
            float distance = point_position.x - line_position.x;
            return distance * distance;
        }

        float dx = point_position.x - line_position.x;
        float dy = point_position.y - line_position.y;
        float projection = dy - line_slope * dx;
        float directionLengthSquared = (1.0f + line_slope * line_slope);
        return projection * projection / directionLengthSquared;
    }

    inline float Get2D_Point_Ray_DistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        glm::vec2 rayToPoint = point_position - ray_position;
        float dotProduct = glm::dot(ray_direction, rayToPoint);
        if (dotProduct < 0.0f)
        {
            // Nearest point on the Ray is the start
            return glm::dot(rayToPoint, rayToPoint);
        }

        float projection = dotProduct / glm::dot(ray_direction, ray_direction);
        return glm::dot(rayToPoint, rayToPoint) - dotProduct * projection;
    }

    inline float Get2D_Point_LineSegment_DistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        glm::vec2 lineSegment = lineSegment_end - lineSegment_start;
        glm::vec2 startToPoint = point_position - lineSegment_start;
        float dotProduct1 = glm::dot(lineSegment, startToPoint);
        if (dotProduct1 < 0.0f)
        {
            // Nearest Point on the LineSegment is the start
            return glm::dot(startToPoint, startToPoint);
        }

        glm::vec2 endToPoint = point_position - lineSegment_end;
        float dotProduct2 = glm::dot(lineSegment, endToPoint);
        if (dotProduct2 > 0.0f)
        {
            // Nearest Point on the LineSegment is the end
            return glm::dot(endToPoint, endToPoint);
        }

        float projection = dotProduct1 / glm::dot(lineSegment, lineSegment);
        return glm::dot(startToPoint, startToPoint) - dotProduct1 * projection;
    }

    // Distance Functions ------------------------------------------------------

    inline float Get2D_Point_Circle_Distance(
        const glm::vec2& point_position,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        float centerDistanceSquared = Get2D_Point_Point_DistanceSquared(point_position, circle_position);

        float centerDistance = glm::sqrt(centerDistanceSquared);
        if (centerDistance > circle_radius)
        {
            return centerDistance - circle_radius;
        }
        return 0.0f;
    }

    inline float Get2D_Point_Capsule_Distance(
        const glm::vec2& point_position,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        float centerDistanceSquared = Get2D_Point_LineSegment_DistanceSquared(point_position, capsule_start, capsule_end);

        float centerDistance = glm::sqrt(centerDistanceSquared);
        if (centerDistance > capsule_radius)
        {
            return centerDistance - capsule_radius;
        }
        return 0.0f;
    }

    // Closest Point And Distance Squared Functions ----------------------------

    inline float Get2D_Point_Line_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& closestPoint_position)
    {
        if (std::isinf(line_slope))
        {
            closestPoint_position = glm::vec2(line_position.x, point_position.y);
            float distance = point_position.x - line_position.x;
            return distance * distance;
        }

        float dx = line_position.x - point_position.x;
        float dy = line_position.y - point_position.y;
        float projection = dy - line_slope * dx;
        float directionLengthSquared = (1.0f + line_slope * line_slope);
        float normalizedProjection = projection / directionLengthSquared;
        closestPoint_position = glm::vec2(point_position.x - normalizedProjection * line_slope, point_position.y + normalizedProjection);
        return projection * projection / directionLengthSquared;
    }

    inline float Get2D_Point_Ray_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        glm::vec2& closestPoint_position)
    {
        glm::vec2 rayToPoint = point_position - ray_position;
        float dotProduct = glm::dot(ray_direction, rayToPoint);
        if (dotProduct < 0.0f)
        {
            // Nearest point on the Ray is the start
            closestPoint_position = ray_position;
            return glm::dot(rayToPoint, rayToPoint);
        }

        float projection = dotProduct / glm::dot(ray_direction, ray_direction);
        closestPoint_position = ray_position + projection * ray_direction;
        return glm::dot(rayToPoint, rayToPoint) - dotProduct * projection;
    }

    inline float Get2D_Point_LineSegment_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        glm::vec2& closestPoint_position)
    {
        glm::vec2 lineSegment = lineSegment_end - lineSegment_start;
        glm::vec2 startToPoint = point_position - lineSegment_start;
        float dotProduct1 = glm::dot(lineSegment, startToPoint);
        if (dotProduct1 < 0.0f)
        {
            // Nearest Point on the LineSegment is the start
            closestPoint_position = lineSegment_start;
            return glm::dot(startToPoint, startToPoint);
        }

        glm::vec2 endToPoint = point_position - lineSegment_end;
        float dotProduct2 = glm::dot(lineSegment, endToPoint);
        if (dotProduct2 > 0.0f)
        {
            // Nearest Point on the LineSegment is the end
            closestPoint_position = lineSegment_end;
            return glm::dot(endToPoint, endToPoint);
        }

        float projection = dotProduct1 / glm::dot(lineSegment, lineSegment);
        closestPoint_position = lineSegment_start + projection * lineSegment;
        return glm::dot(startToPoint, startToPoint) - dotProduct1 * projection;
    }

    inline float Get2D_Point_Rectangle_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& closestPoint_position)
    {
        Get2D_Point_Rectangle_ClosestPoint(
            point_position,
            rectangle_bottomLeft,
            rectangle_topRight,
            closestPoint_position);

        glm::vec2 pointToClosestPoint = closestPoint_position - point_position;
        return glm::dot(pointToClosestPoint, pointToClosestPoint);
    }

    inline float Get2D_Point_OrientedRectangle_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& closestPoint_position)
    {
        // rotate and translate the point rather then the orientedRectangle
        glm::vec2 translatedPointPosition = point_position - orientedRectangle_position;
        glm::vec2 rotatedPointPosition = Rotate2DVector(translatedPointPosition, -1.0f * orientedRectangle_rotation);

        float distanceSquared = Get2D_Point_Rectangle_ClosestPointAndDistanceSquared(
            rotatedPointPosition,
            -1.0f * orientedRectangle_halfSize,
            orientedRectangle_halfSize,
            closestPoint_position);

        // rotate and translate back
        closestPoint_position = Rotate2DVector(closestPoint_position, orientedRectangle_rotation);
        closestPoint_position += orientedRectangle_position;

        return distanceSquared;
    }

    inline float Get2D_Point_Triangle_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        glm::vec2& closestPoint_position)
    {
        // Barycentric Technique

        glm::vec2 AB = triangle_corner2 - triangle_corner1;
        glm::vec2 AC = triangle_corner3 - triangle_corner1;
        glm::vec2 AP = point_position - triangle_corner1;

        float dot_AC_AC = glm::dot(AC, AC); // corner1 to corner3 distance squared
        float dot_AB_AC = glm::dot(AB, AC);
        float dot_AC_AP = glm::dot(AC, AP);
        float dot_AB_AB = glm::dot(AB, AB); // corner1 to corner2 distance squared
        float dot_AB_AP = glm::dot(AB, AP);

        float denominator = (dot_AC_AC * dot_AB_AB - dot_AB_AC * dot_AB_AC);
        float u = (dot_AB_AB * dot_AC_AP - dot_AB_AC * dot_AB_AP) / denominator;
        float v = (dot_AC_AC * dot_AB_AP - dot_AB_AC * dot_AC_AP) / denominator;

        if (u > 0.0f && v > 0.0f && (u + v) < 1.0f)
        {
            // Point is inside the triangle, return 0 distance
            closestPoint_position = point_position;
            return 0.0f;
        }

        glm::vec2 BC = triangle_corner3 - triangle_corner2;
        glm::vec2 BP = point_position - triangle_corner2;

        float dot_BC_BC = glm::dot(BC, BC); // corner2 to corner3 distance squared
        float dot_BC_BP = glm::dot(BC, BP);

        // Calculate distance to each edge of the triangle
        // Edge 1 (from corner1 to corner2)
        float t1 = glm::clamp(dot_AB_AP / dot_AB_AB, 0.0f, 1.0f);
        glm::vec2 closest_point_on_edge1 = triangle_corner1 + t1 * AB;
        glm::vec2 edge1_to_point = point_position - closest_point_on_edge1;
        float distanceSquared_to_edge1 = glm::dot(edge1_to_point, edge1_to_point);

        closestPoint_position = closest_point_on_edge1;
        float distanceSquared = distanceSquared_to_edge1;

        // Edge 2 (from corner2 to corner3)
        float t2 = glm::clamp(dot_BC_BP / dot_BC_BC, 0.0f, 1.0f);
        glm::vec2 closest_point_on_edge2 = triangle_corner2 + t2 * BC;
        glm::vec2 edge2_to_point = point_position - closest_point_on_edge2;
        float distanceSquared_to_edge2 = glm::dot(edge2_to_point, edge2_to_point);

        if (distanceSquared_to_edge2 < distanceSquared)
        {
            closestPoint_position = closest_point_on_edge2;
            distanceSquared = distanceSquared_to_edge2;
        }

        // Edge 3 (from corner1 to corner3)
        float t3 = glm::clamp(dot_AC_AP / dot_AC_AC, 0.0f, 1.0f);
        glm::vec2 closest_point_on_edge3 = triangle_corner1 + t3 * AC;
        glm::vec2 edge3_to_point = point_position - closest_point_on_edge3;
        float distanceSquared_to_edge3 = glm::dot(edge3_to_point, edge3_to_point);

        if (distanceSquared_to_edge3 < distanceSquared)
        {
            closestPoint_position = closest_point_on_edge3;
            distanceSquared = distanceSquared_to_edge3;
        }

        return distanceSquared;
    }

    inline float Get2D_Point_Polygon_ClosestPointAndDistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        glm::vec2& closestPoint_position)
    {
        if (polygon_cornerCount > 2)
        {
            if (Check2D_Point_Polygon_Overlap(point_position, polygon_corners, polygon_cornerCount))
            {
                closestPoint_position = point_position;
                return 0.0f;
            }
            else
            {
                float minDistanceSquared = std::numeric_limits<float>::infinity();

                size_t start_index = polygon_cornerCount - 1;
                for (size_t end_index = 0; end_index < polygon_cornerCount; ++end_index)
                {
                    const glm::vec2& lineSegment_start = polygon_corners[start_index];
                    const glm::vec2& lineSegment_end = polygon_corners[end_index];

                    glm::vec2 AB = lineSegment_end - lineSegment_start;
                    glm::vec2 AP = point_position - lineSegment_start;

                    float dot_AB_AB = glm::dot(AB, AB); // length squared
                    float dot_AB_AP = glm::dot(AB, AP);

                    float t1 = glm::clamp(dot_AB_AP / dot_AB_AB, 0.0f, 1.0f);
                    glm::vec2 closest_point_on_edge = lineSegment_start + t1 * AB;
                    glm::vec2 edge_to_point = point_position - closest_point_on_edge;
                    float distanceSquared_to_edge = glm::dot(edge_to_point, edge_to_point);

                    if (distanceSquared_to_edge < minDistanceSquared)
                    {
                        closestPoint_position = closest_point_on_edge;
                        minDistanceSquared = distanceSquared_to_edge;
                    }

                    start_index = end_index;
                }

                return minDistanceSquared;
            }
        }
        else if (polygon_cornerCount == 2)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            const glm::vec2& corner2_position = polygon_corners[1];
            return Get2D_Point_LineSegment_ClosestPointAndDistanceSquared(
                point_position,
                corner1_position,
                corner2_position,
                closestPoint_position);
        }
        else if (polygon_cornerCount == 1)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            closestPoint_position = corner1_position;
            return Get2D_Point_Point_DistanceSquared(
                point_position,
                corner1_position);
        }
        else
        {
            return std::numeric_limits<float>::infinity();
        }
    }

    // Rectangle Collision Point And Normal And Depth Functions ---------------

    inline bool Get2D_Rectangle_Rectangle_CollisionPointNormalDepth(
        const glm::vec2& rectangleA_bottomLeft,
        const glm::vec2& rectangleA_topRight,
        const glm::vec2& rectangleB_bottomLeft,
        const glm::vec2& rectangleB_topRight,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        // Check if there is a collision
        if (!Check2D_Rectangle_Rectangle_Overlap(rectangleA_bottomLeft, rectangleA_topRight, rectangleB_bottomLeft, rectangleB_topRight))
        {
            return false;
        }

        // Collision Point will be the center of the overlap rectangle
        glm::vec2 overlapRectangle_bottomLeft;
        glm::vec2 overlapRectangle_topRight;

        overlapRectangle_bottomLeft.x = std::max(rectangleA_bottomLeft.x, rectangleB_bottomLeft.x);
        overlapRectangle_bottomLeft.y = std::max(rectangleA_bottomLeft.y, rectangleB_bottomLeft.y);
        overlapRectangle_topRight.x = std::min(rectangleA_topRight.x, rectangleB_topRight.x);
        overlapRectangle_topRight.y = std::min(rectangleA_topRight.y, rectangleB_topRight.y);

        collisionPoint = (overlapRectangle_bottomLeft + overlapRectangle_topRight) * 0.5f;

        glm::vec2 overlapRectangle_dimensions = overlapRectangle_topRight - overlapRectangle_bottomLeft;
        glm::vec2 rectangleA_center = (rectangleA_bottomLeft + rectangleA_topRight) * 0.5f;
        glm::vec2 rectangleB_center = (rectangleB_bottomLeft + rectangleB_topRight) * 0.5f;
        glm::vec2 a_to_b = rectangleB_center - rectangleA_center;

        if (overlapRectangle_dimensions.y <= overlapRectangle_dimensions.x)
        {
            collisionNormal.x = 0.0f;
            collisionDepth = overlapRectangle_dimensions.y;

            if (a_to_b.y >= 0)
            {
                collisionNormal.y = 1.0f;
            }
            else
            {
                collisionNormal.y = -1.0f;
            }
        }
        else
        {
            collisionNormal.y = 0.0f;
            collisionDepth = overlapRectangle_dimensions.x;

            if (a_to_b.x >= 0)
            {
                collisionNormal.x = 1.0f;
            }
            else
            {
                collisionNormal.x = -1.0f;
            }
        }

        // If one rectangle is completely inside another, depth cannot simply
        // be based on the overlap rectangle formed. It needs to be further
        // adjusted.

        // Check if A is completely inside B
        bool a_inside_b = rectangleA_bottomLeft.x > rectangleB_bottomLeft.x &&
            rectangleA_topRight.x < rectangleB_topRight.x &&
            rectangleA_bottomLeft.y > rectangleB_bottomLeft.y &&
            rectangleA_topRight.y < rectangleB_topRight.y;

        // Check if B is completely inside A
        bool b_inside_a = rectangleB_bottomLeft.x > rectangleA_bottomLeft.x &&
            rectangleB_topRight.x < rectangleA_topRight.x &&
            rectangleB_bottomLeft.y > rectangleA_bottomLeft.y &&
            rectangleB_topRight.y < rectangleA_topRight.y;

        if (b_inside_a)
        {
            const glm::vec2* outsideRectangle_bottomLeft_ptr = &rectangleA_bottomLeft;
            const glm::vec2* outsideRectangle_topRight_ptr = &rectangleA_topRight;

            if (overlapRectangle_dimensions.y <= overlapRectangle_dimensions.x)
            {
                if (a_to_b.y >= 0)
                {
                    collisionDepth += outsideRectangle_topRight_ptr->y - overlapRectangle_topRight.y;
                }
                else
                {
                    collisionDepth += overlapRectangle_bottomLeft.y - outsideRectangle_bottomLeft_ptr->y;
                }
            }
            else
            {
                if (a_to_b.x >= 0)
                {
                    collisionDepth += outsideRectangle_topRight_ptr->x - overlapRectangle_topRight.x;
                }
                else
                {
                    collisionDepth += overlapRectangle_bottomLeft.x - outsideRectangle_bottomLeft_ptr->x;
                }
            }
        }
        else if (a_inside_b)
        {
            const glm::vec2* outsideRectangle_bottomLeft_ptr = &rectangleB_bottomLeft;
            const glm::vec2* outsideRectangle_topRight_ptr = &rectangleB_topRight;

            if (overlapRectangle_dimensions.y <= overlapRectangle_dimensions.x)
            {
                if (a_to_b.y >= 0)
                {
                    collisionDepth += overlapRectangle_bottomLeft.y - outsideRectangle_bottomLeft_ptr->y;
                }
                else
                {
                    collisionDepth += outsideRectangle_topRight_ptr->y - overlapRectangle_topRight.y;
                }
            }
            else
            {
                if (a_to_b.x >= 0)
                {
                    collisionDepth += overlapRectangle_bottomLeft.x - outsideRectangle_bottomLeft_ptr->x;
                }
                else
                {
                    collisionDepth += outsideRectangle_topRight_ptr->x - overlapRectangle_topRight.x;
                }
            }
        }

        return true;
    }

    // Circle Collision Point And Normal And Depth Functions -------------------

    inline bool Get2D_Circle_Circle_CollisionPointNormalDepth(
        const glm::vec2& circleA_position,
        const float& circleA_radius,
        const glm::vec2& circleB_position,
        const float& circleB_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 a_to_b = circleB_position - circleA_position;

        float a_to_b_distanceSquared = glm::dot(a_to_b, a_to_b);

        // float combinedCircleRadiusSquared = (circleA_radius + circleB_radius) * (circleA_radius + circleB_radius);
        // if (a_to_b_distanceSquared > combinedCircleRadiusSquared))
        // {
        //     return false;
        // }

        float a_to_b_distance = glm::sqrt(a_to_b_distanceSquared);

        if (a_to_b_distance > (circleA_radius + circleB_radius))
        {
            return false;
        }

        collisionNormal = a_to_b / a_to_b_distance; // normalize

        float collisionPointDistance = (a_to_b_distance - circleB_radius + circleA_radius) * 0.5f;

        collisionPoint = circleA_position + collisionNormal * collisionPointDistance;

        collisionDepth = (circleA_radius - collisionPointDistance) * 2.0f;

        return true;
    }

    // Triangle Collision Point And Normal And Depth Functions -----------------

    inline bool Get2D_Triangle_Triangle_CollisionPointNormalDepth(
        const glm::vec2& triangleA_corner1,
        const glm::vec2& triangleA_corner2,
        const glm::vec2& triangleA_corner3,
        const glm::vec2& triangleB_corner1,
        const glm::vec2& triangleB_corner2,
        const glm::vec2& triangleB_corner3,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 convexPolygonA_array[3] = { triangleA_corner1, triangleA_corner2, triangleA_corner3 };
        glm::vec2 convexPolygonB_array[3] = { triangleB_corner1, triangleB_corner2, triangleB_corner3 };
        return Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth(convexPolygonA_array, 3, convexPolygonB_array, 3, collisionPoint, collisionNormal, collisionDepth);
    }

    // ConvexPolygon Collision Point And Normal And Depth Functions ------------

    inline bool Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        if (convexPolygonA_cornerCount < 3 || convexPolygonB_cornerCount < 3)
        {
            return false;
        }

        glm::vec2 final_collisionNormal;
        float final_collisionDepth = std::numeric_limits<float>::infinity();

        bool projection_max_is_from_polygonA;
        bool projection_min_is_from_polygonA;
        size_t projection_max_index;
        size_t projection_min_index;

        size_t start_indexA = convexPolygonA_cornerCount - 1;
        for (size_t end_indexA = 0; end_indexA < convexPolygonA_cornerCount; ++end_indexA)
        {
            const glm::vec2& lineSegmentA_start = convexPolygonA_corners[start_indexA];
            const glm::vec2& lineSegmentA_end = convexPolygonA_corners[end_indexA];
            glm::vec2 axisOfSeparation = lineSegmentA_end - lineSegmentA_start;
            axisOfSeparation = glm::vec2(axisOfSeparation.y, -1.0f * axisOfSeparation.x);
            axisOfSeparation = glm::normalize(axisOfSeparation);

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            size_t projectionA_max_index;
            size_t projectionA_min_index;
            ProjectPolygonOntoAxis_2(axisOfSeparation, convexPolygonA_corners, convexPolygonA_cornerCount, projectionA_max, projectionA_min, projectionA_max_index, projectionA_min_index);

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            size_t projectionB_max_index;
            size_t projectionB_min_index;
            ProjectPolygonOntoAxis_2(axisOfSeparation, convexPolygonB_corners, convexPolygonB_cornerCount, projectionB_max, projectionB_min, projectionB_max_index, projectionB_min_index);

            if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
            {
                // projections don't overlap on axis of separation
                return false;
            }

            float aMin_to_bMax = projectionB_max - projectionA_min;
            float bMin_to_aMax = projectionA_max - projectionB_min;

            float axisDepth;
            if (bMin_to_aMax < aMin_to_bMax)
            {
                axisDepth = bMin_to_aMax;
            }
            else
            {
                axisDepth = aMin_to_bMax;
            }

            if (axisDepth < final_collisionDepth)
            {
                final_collisionDepth = axisDepth;
                final_collisionNormal = axisOfSeparation;

                if (projectionA_max > projectionB_max)
                {
                    projection_max_is_from_polygonA = false;
                    projection_max_index = projectionB_max_index;
                }
                else
                {
                    projection_max_is_from_polygonA = true;
                    projection_max_index = projectionA_max_index;
                }

                if (projectionA_min > projectionB_min)
                {
                    projection_min_is_from_polygonA = true;
                    projection_min_index = projectionA_min_index;
                }
                else
                {
                    projection_min_is_from_polygonA = false;
                    projection_min_index = projectionB_min_index;
                }
            }

            start_indexA = end_indexA;
        }

        size_t start_indexB = convexPolygonB_cornerCount - 1;
        for (size_t end_indexB = 0; end_indexB < convexPolygonB_cornerCount; ++end_indexB)
        {
            const glm::vec2& lineSegmentB_start = convexPolygonB_corners[start_indexB];
            const glm::vec2& lineSegmentB_end = convexPolygonB_corners[end_indexB];
            glm::vec2 axisOfSeparation = lineSegmentB_end - lineSegmentB_start;
            axisOfSeparation = glm::vec2(axisOfSeparation.y, -1.0f * axisOfSeparation.x);
            axisOfSeparation = glm::normalize(axisOfSeparation);

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            size_t projectionA_max_index;
            size_t projectionA_min_index;
            ProjectPolygonOntoAxis_2(axisOfSeparation, convexPolygonA_corners, convexPolygonA_cornerCount, projectionA_max, projectionA_min, projectionA_max_index, projectionA_min_index);

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            size_t projectionB_max_index;
            size_t projectionB_min_index;
            ProjectPolygonOntoAxis_2(axisOfSeparation, convexPolygonB_corners, convexPolygonB_cornerCount, projectionB_max, projectionB_min, projectionB_max_index, projectionB_min_index);

            if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
            {
                // projections don't overlap on axis of separation
                return false;
            }

            float aMin_to_bMax = projectionB_max - projectionA_min;
            float bMin_to_aMax = projectionA_max - projectionB_min;

            float axisDepth;
            if (bMin_to_aMax < aMin_to_bMax)
            {
                axisDepth = bMin_to_aMax;
            }
            else
            {
                axisDepth = aMin_to_bMax;
            }

            if (axisDepth < final_collisionDepth)
            {
                final_collisionDepth = axisDepth;
                final_collisionNormal = axisOfSeparation;

                if (projectionA_max > projectionB_max)
                {
                    projection_max_is_from_polygonA = false;
                    projection_max_index = projectionB_max_index;
                }
                else
                {
                    projection_max_is_from_polygonA = true;
                    projection_max_index = projectionA_max_index;
                }

                if (projectionA_min > projectionB_min)
                {
                    projection_min_is_from_polygonA = true;
                    projection_min_index = projectionA_min_index;
                }
                else
                {
                    projection_min_is_from_polygonA = false;
                    projection_min_index = projectionB_min_index;
                }
            }

            start_indexB = end_indexB;
        }

        collisionNormal = glm::normalize(final_collisionNormal);
        collisionDepth = final_collisionDepth;

        glm::vec2 final_projection_max_point;
        glm::vec2 final_projection_max_neighbor_point;
        if (projection_max_is_from_polygonA)
        {
            final_projection_max_point = convexPolygonA_corners[projection_max_index];
            final_projection_max_neighbor_point = convexPolygonA_corners[(projection_max_index + 1) % convexPolygonA_cornerCount];
        }
        else
        {
            final_projection_max_point = convexPolygonB_corners[projection_max_index];
            final_projection_max_neighbor_point = convexPolygonB_corners[(projection_max_index + 1) % convexPolygonB_cornerCount];
        }

        glm::vec2 final_projection_min_point;
        glm::vec2 final_projection_min_neighbor_point;
        if (projection_min_is_from_polygonA)
        {
            final_projection_min_point = convexPolygonA_corners[projection_min_index];
            final_projection_min_neighbor_point = convexPolygonA_corners[(projection_min_index + 1) % convexPolygonA_cornerCount];
        }
        else
        {
            final_projection_min_point = convexPolygonB_corners[projection_min_index];
            final_projection_min_neighbor_point = convexPolygonB_corners[(projection_min_index + 1) % convexPolygonB_cornerCount];
        }

        bool final_projection_max_point_inside;
        if (projection_max_is_from_polygonA)
        {
            final_projection_max_point_inside =
                Check2D_Point_ConvexPolygon_Overlap(final_projection_max_point, convexPolygonB_corners, convexPolygonB_cornerCount);
        }
        else
        {
            final_projection_max_point_inside =
                Check2D_Point_ConvexPolygon_Overlap(final_projection_max_point, convexPolygonA_corners, convexPolygonA_cornerCount);
        }

        if (final_projection_max_point_inside)
        {
            collisionPoint = final_projection_max_point;

            glm::vec2 vectorToNeighbor = final_projection_max_neighbor_point - final_projection_max_point;
            glm::vec2 surfaceNormal(vectorToNeighbor.y, -vectorToNeighbor.x);

            if (projection_max_is_from_polygonA && glm::dot(collisionNormal, surfaceNormal) < 0)
            {
                collisionNormal *= -1.0f;
            }
            else if (!projection_max_is_from_polygonA && glm::dot(collisionNormal, surfaceNormal) > 0)
            {
                collisionNormal *= -1.0f;
            }

            return true;
        }

        bool final_projection_min_point_inside;
        if (projection_min_is_from_polygonA)
        {
            final_projection_min_point_inside =
                Check2D_Point_ConvexPolygon_Overlap(final_projection_min_point, convexPolygonB_corners, convexPolygonB_cornerCount);
        }
        else
        {
            final_projection_min_point_inside =
                Check2D_Point_ConvexPolygon_Overlap(final_projection_min_point, convexPolygonA_corners, convexPolygonA_cornerCount);
        }

        collisionPoint = final_projection_min_point;

        glm::vec2 vectorToNeighbor = final_projection_min_neighbor_point - final_projection_min_point;
        glm::vec2 surfaceNormal(vectorToNeighbor.y, -vectorToNeighbor.x);

        if (projection_min_is_from_polygonA && glm::dot(collisionNormal, surfaceNormal) < 0)
        {
            collisionNormal *= -1.0f;
        }
        else if (!projection_min_is_from_polygonA && glm::dot(collisionNormal, surfaceNormal) > 0)
        {
            collisionNormal *= -1.0f;
        }

        return true;
    }

    // Intersection Functions --------------------------------------------------

    inline bool Get2D_Line_Line_Intersection(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope,
        glm::vec2& intersection_position)
    {
        if (lineA_slope == lineB_slope)
        {
            return false;
        }
        else if (std::isinf(lineA_slope))
        {
            if (std::isinf(lineB_slope))
            {
                return false;
            }
            else
            {
                intersection_position.x = lineA_position.x;
                intersection_position.y = lineB_slope * intersection_position.x +
                    (lineB_position.y - lineB_slope * lineB_position.x);
            }
        }
        else if (std::isinf(lineB_slope))
        {
            intersection_position.x = lineB_position.x;
            intersection_position.y = lineA_slope * intersection_position.x +
                (lineA_position.y - lineA_slope * lineA_position.x);
        }
        else
        {
            float yInterceptA = lineA_position.y - lineA_slope * lineA_position.x;
            float yInterceptB = lineB_position.y - lineB_slope * lineB_position.x;
            intersection_position.x = (yInterceptB - yInterceptA) / (lineA_slope - lineB_slope);
            intersection_position.y = lineA_slope * intersection_position.x + yInterceptA;
        }
        return true;
    }

    // Helper Functions --------------------------------------------------------

    inline bool Check2D_Point_Rectangle_Overlap_H(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_oppositeCorner1,
        const glm::vec2& rectangle_oppositeCorner2)
    {
        float maxX, minX, maxY, minY;
        GetMinMax(rectangle_oppositeCorner1.x, rectangle_oppositeCorner2.x, minX, maxX);
        GetMinMax(rectangle_oppositeCorner1.y, rectangle_oppositeCorner2.y, minY, maxY);

        return (point_position.x < maxX) && (point_position.x > minX) &&
            (point_position.y < maxY) && (point_position.y > minY);
    }

    inline bool Check2D_Point_Triangle_Overlap_Alt(
        const glm::vec2& point_position,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        float triangleArea = Get2D_Triangle_Area(triangle_corner1, triangle_corner2, triangle_corner3);
        float area1 = Get2D_Triangle_Area(point_position, triangle_corner1, triangle_corner2);
        float area2 = Get2D_Triangle_Area(point_position, triangle_corner1, triangle_corner3);
        float area3 = Get2D_Triangle_Area(point_position, triangle_corner2, triangle_corner3);
        float areaSum = area1 + area2 + area3;

        return FloatEqualToFloat(triangleArea, areaSum);
    }

    inline bool Check2D_Rectangle_OrientedRectangle_Overlap_Alt(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        // apply transformations to the orientedRectangle corners
        glm::vec2 transformedRectangle_bottomLeft(-1.0f * orientedRectangle_halfSize.x, -1.0f * orientedRectangle_halfSize.y);
        transformedRectangle_bottomLeft = Rotate2DVector(transformedRectangle_bottomLeft, orientedRectangle_rotation);
        transformedRectangle_bottomLeft += orientedRectangle_position;

        glm::vec2 transformedRectangle_topRight(orientedRectangle_halfSize.x, orientedRectangle_halfSize.y);
        transformedRectangle_topRight = Rotate2DVector(transformedRectangle_topRight, orientedRectangle_rotation);
        transformedRectangle_topRight += orientedRectangle_position;

        glm::vec2 transformedRectangle_bottomRight(orientedRectangle_halfSize.x, -1.0f * orientedRectangle_halfSize.y);
        transformedRectangle_bottomRight = Rotate2DVector(transformedRectangle_bottomRight, orientedRectangle_rotation);
        transformedRectangle_bottomRight += orientedRectangle_position;

        // check if the bottom or the right of the orientedRectangle crosses any sides of the rectangle
        if (Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, transformedRectangle_bottomLeft, transformedRectangle_bottomRight) ||
            Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, transformedRectangle_bottomRight, transformedRectangle_topRight))
        {
            return true;
        }

        glm::vec2 transformedRectangle_topLeft(-1.0f * orientedRectangle_halfSize.x, orientedRectangle_halfSize.y);
        transformedRectangle_topLeft = Rotate2DVector(transformedRectangle_topLeft, orientedRectangle_rotation);
        transformedRectangle_topLeft += orientedRectangle_position;

        // check if the top or the left of the orientedRectangle crosses any sides of the rectangle
        if (Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, transformedRectangle_topRight, transformedRectangle_topLeft) ||
            Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, transformedRectangle_topLeft, transformedRectangle_bottomLeft))
        {
            return true;
        }

        // check if a corner of the orientedRectangle is inside the rectangle
        if (Check2D_Point_Rectangle_Overlap(transformedRectangle_bottomLeft, rectangle_bottomLeft, rectangle_topRight))
        {
            return true;
        }

        glm::vec2 transformedRectangle2_bottomLeft = rectangle_bottomLeft - orientedRectangle_position;
        transformedRectangle2_bottomLeft = Rotate2DVector(transformedRectangle2_bottomLeft, -1.0f * orientedRectangle_rotation);

        // check if the corner of the rectangle is inside the orientedRectangle
        return Check2D_Point_Rectangle_Overlap(transformedRectangle2_bottomLeft, -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize);
    }

    inline bool Check2D_Rectangle_Triangle_Overlap_Alt(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        return Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, triangle_corner1, triangle_corner2) ||
            Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, triangle_corner2, triangle_corner3) ||
            Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, triangle_corner3, triangle_corner1) ||
            Check2D_Point_Rectangle_Overlap(triangle_corner1, rectangle_bottomLeft, rectangle_topRight) ||
            Check2D_Point_Triangle_Overlap(rectangle_bottomLeft, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Circle_Triangle_Overlap_Alt(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        glm::vec2 triangle_corner_array[3] = {
            triangle_corner1,
            triangle_corner2,
            triangle_corner3
        };

        const glm::vec2 axes[4] = {
            glm::normalize(circle_position - triangle_corner1), // Additional axis for circle center
            glm::normalize(glm::vec2(-(triangle_corner2.y - triangle_corner1.y), triangle_corner2.x - triangle_corner1.x)), // side 1,2 normal
            glm::normalize(glm::vec2(-(triangle_corner3.y - triangle_corner2.y), triangle_corner3.x - triangle_corner2.x)), // side 2,3 normal
            glm::normalize(glm::vec2(-(triangle_corner1.y - triangle_corner3.y), triangle_corner1.x - triangle_corner3.x)) // side 3,1 normal
        };

        for (size_t i = 0; i < 4; ++i)
        {
            const glm::vec2& axisOfSeparation = axes[i];

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            ProjectCircleOntoAxis(axisOfSeparation, circle_position, circle_radius, projectionA_max, projectionA_min);

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            ProjectPolygonOntoAxis(axisOfSeparation, &triangle_corner_array[0], 3, projectionB_max, projectionB_min);

            if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
            {
                // projections don't overlap on axis of separation
                return false;
            }
        }

        return true;
    }

    inline bool Check2D_Triangle_Triangle_Overlap_Alt(
        const glm::vec2& triangleA_corner1,
        const glm::vec2& triangleA_corner2,
        const glm::vec2& triangleA_corner3,
        const glm::vec2& triangleB_corner1,
        const glm::vec2& triangleB_corner2,
        const glm::vec2& triangleB_corner3)
    {
        return Check2D_LineSegment_LineSegment_Overlap(triangleA_corner1, triangleA_corner2, triangleB_corner1, triangleB_corner2) ||
            Check2D_LineSegment_LineSegment_Overlap(triangleA_corner1, triangleA_corner2, triangleB_corner2, triangleB_corner3) ||
            Check2D_LineSegment_LineSegment_Overlap(triangleA_corner1, triangleA_corner2, triangleB_corner3, triangleB_corner1) ||
            Check2D_LineSegment_LineSegment_Overlap(triangleA_corner2, triangleA_corner3, triangleB_corner1, triangleB_corner2) ||
            Check2D_LineSegment_LineSegment_Overlap(triangleA_corner2, triangleA_corner3, triangleB_corner2, triangleB_corner3) ||
            Check2D_LineSegment_LineSegment_Overlap(triangleA_corner2, triangleA_corner3, triangleB_corner3, triangleB_corner1) ||
            Check2D_LineSegment_LineSegment_Overlap(triangleA_corner3, triangleA_corner1, triangleB_corner1, triangleB_corner2) ||
            Check2D_LineSegment_LineSegment_Overlap(triangleA_corner3, triangleA_corner1, triangleB_corner2, triangleB_corner3) ||
            Check2D_Point_Triangle_Overlap(triangleA_corner1, triangleB_corner1, triangleB_corner2, triangleB_corner3) ||
            Check2D_Point_Triangle_Overlap(triangleB_corner1, triangleA_corner1, triangleA_corner2, triangleA_corner3);
    }

    inline unsigned int Get2D_Line_Circle_IntersectionDirectionScalars(
        const glm::vec2& line_position,
        const glm::vec2& line_direction,
        const glm::vec2& circle_position,
        const float& circle_radius,
        float& intersection_directionScalar1,
        float& intersection_directionScalar2)
    {
        // Calculate coefficients for the quadratic equation of the line and circle intersection
        // a * x^2 + b * x + c = 0
        float a = glm::dot(line_direction, line_direction);
        float b = 2 * glm::dot(line_direction, line_position - circle_position);
        float c = glm::dot(line_position - circle_position, line_position - circle_position) - circle_radius * circle_radius;

        // Calculate the discriminant to determine the number of intersections
        float discriminant = b * b - 4 * a * c;

        if (discriminant > 0) {
            // Two distinct intersection points
            intersection_directionScalar1 = (-b + glm::sqrt(discriminant)) / (2 * a);
            intersection_directionScalar2 = (-b - glm::sqrt(discriminant)) / (2 * a);

            return 2;
        }
        else if (discriminant == 0) {
            // One intersection point (tangent)
            intersection_directionScalar1 = -b / (2 * a);

            return 1;
        }

        return 0;
    }

    inline unsigned int Get2D_Line_Circle_Intersections_Alt(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& intersection_position1,
        glm::vec2& intersection_position2)
    {
        // Calculate coefficients for the quadratic equation of the line and circle intersection
        // a * x^2 + b * x + c = 0
        float a = 1 + line_slope * line_slope;
        float b = -2 * (line_position.x - circle_position.x + line_slope * (line_position.y - circle_position.y));
        float c = (line_position.x - circle_position.x) * (line_position.x - circle_position.x) +
            (line_position.y - circle_position.y) * (line_position.y - circle_position.y) -
            circle_radius * circle_radius;

        // Calculate the discriminant to determine the number of intersections
        float discriminant = b * b - 4 * a * c;

        if (discriminant > 0) {
            // Two distinct intersection points
            float t1 = (-b + glm::sqrt(discriminant)) / (2 * a);
            float t2 = (-b - glm::sqrt(discriminant)) / (2 * a);

            // Calculate the intersection points using t
            intersection_position1.x = line_position.x + t1;
            intersection_position1.y = line_slope * t1 + line_position.y;
            intersection_position2.x = line_position.x + t2;
            intersection_position2.y = line_slope * t2 + line_position.y;

            return 2;
        }
        else if (discriminant == 0) {
            // One intersection point (tangent)
            float t = -b / (2 * a);

            // Calculate the tangent intersection point
            intersection_position1.x = line_position.x + t;
            intersection_position1.y = line_slope * t + line_position.y;

            return 1;
        }

        return 0;
    }

    inline bool Check2D_RectangleFrame_LineSegment_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        // Check if the lineSegment crosses the bottom or the right of the rectangle
        glm::vec2 rectangle_bottomRight(rectangle_topRight.x, rectangle_bottomLeft.y);
        if (Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, rectangle_bottomLeft, rectangle_bottomRight) ||
            Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, rectangle_bottomRight, rectangle_topRight))
        {
            return true;
        }

        // Check if the lineSegment crosses the top or the left of the rectangle
        glm::vec2 rectangle_topLeft(rectangle_bottomLeft.x, rectangle_topRight.y);
        return Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, rectangle_topRight, rectangle_topLeft) ||
            Check2D_LineSegment_LineSegment_Overlap(lineSegment_start, lineSegment_end, rectangle_topLeft, rectangle_bottomLeft);
    }

    inline int Get2D_Triangle_ApparentFacingDirection(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        float val = (triangle_corner2.y - triangle_corner1.y) * (triangle_corner3.x - triangle_corner2.x) -
            (triangle_corner2.x - triangle_corner1.x) * (triangle_corner3.y - triangle_corner2.y);
        if (val < 0.0f)
        {
            return 2; // counter-clockwise
        }
        else if (val > 0.0f)
        {
            return 1; // clockwise
        }
        else
        {
            return 0; // colinear
        }
    }

    inline float Get2D_Triangle_Area(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        // Heron’s formula
        return std::abs((triangle_corner1.x * (triangle_corner2.y - triangle_corner3.y) +
            triangle_corner2.x * (triangle_corner3.y - triangle_corner1.y) +
            triangle_corner3.x * (triangle_corner1.y - triangle_corner2.y)) / 2.0f);
    }

    inline bool Check2D_ConvexPolygon_ConvexPolygon_HalfSeparatedAxisTheorem(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount)
    {
        size_t start_indexA = convexPolygonA_cornerCount - 1;
        for (size_t end_indexA = 0; end_indexA < convexPolygonA_cornerCount; ++end_indexA)
        {
            const glm::vec2& lineSegmentA_start = convexPolygonA_corners[start_indexA];
            const glm::vec2& lineSegmentA_end = convexPolygonA_corners[end_indexA];
            glm::vec2 axisOfSeparation = lineSegmentA_end - lineSegmentA_start;
            axisOfSeparation = glm::vec2(axisOfSeparation.y, -1.0f * axisOfSeparation.x);

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            ProjectPolygonOntoAxis(axisOfSeparation, convexPolygonA_corners, convexPolygonA_cornerCount, projectionA_max, projectionA_min);

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            ProjectPolygonOntoAxis(axisOfSeparation, convexPolygonB_corners, convexPolygonB_cornerCount, projectionB_max, projectionB_min);

            if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
            {
                // projections don't overlap on axis of separation
                return false;
            }

            start_indexA = end_indexA;
        }

        return true;
    }

    inline void ProjectPolygonOntoAxis(
        const glm::vec2& axis,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        float& max,
        float& min)
    {
        for (size_t i = 0; i < polygon_cornerCount; ++i)
        {
            float currentProjection = glm::dot(polygon_corners[i], axis);

            if (currentProjection > max)
            {
                max = currentProjection;
            }

            if (currentProjection < min)
            {
                min = currentProjection;
            }
        }
    }

    inline void ProjectPolygonOntoAxis_2(
        const glm::vec2& axis,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        float& max,
        float& min,
        size_t& maxIndex,
        size_t& minIndex)
    {
        for (size_t i = 0; i < polygon_cornerCount; ++i)
        {
            float currentProjection = glm::dot(polygon_corners[i], axis);

            if (currentProjection > max)
            {
                max = currentProjection;
                maxIndex = i;
            }

            if (currentProjection < min)
            {
                min = currentProjection;
                minIndex = i;
            }
        }
    }

    inline void ProjectCircleOntoAxis(
        const glm::vec2& axis,
        const glm::vec2& circle_position,
        const float& circle_radius,
        float& max,
        float& min)
    {
        // Project the circle's center onto the axis
        float centerProjection = glm::dot(circle_position, axis);

        // The min and max projections are the center projection +/- the radius
        max = centerProjection + circle_radius;
        min = centerProjection - circle_radius;
    }

    inline void ProjectCapsuleOntoAxis(
        const glm::vec2& axis,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        float& max,
        float& min)
    {
        float currentProjection = glm::dot(capsule_start, axis);
        if (currentProjection > max)
        {
            max = currentProjection;
        }
        if (currentProjection < min)
        {
            min = currentProjection;
        }

        currentProjection = glm::dot(capsule_end, axis);
        if (currentProjection > max)
        {
            max = currentProjection;
        }
        if (currentProjection < min)
        {
            min = currentProjection;
        }

        max += capsule_radius;
        min -= capsule_radius;
    }

    inline float RotateSlope(float slope, float rotationInRadians)
    {
        glm::vec2 normalVector;
        if (std::isinf(slope))
        {
            normalVector = Rotate2DVector(glm::vec2(0.0f, 1.0f), rotationInRadians);
        }
        else
        {
            normalVector = Rotate2DVector(glm::normalize(glm::vec2(1.0f, slope)), rotationInRadians);
        }
        return normalVector.y / normalVector.x;

        // float cosAngle = glm::cos(rotationInRadians);
        // float sinAngle = glm::sin(rotationInRadians);
        // return (sinAngle + cosAngle * slope) / (cosAngle - sinAngle * slope);
    }

    inline glm::vec2 Get2D_DirectionFromSlope(float slope)
    {
        if (slope == std::numeric_limits<float>::infinity())
        {
            return glm::vec2(0.0f, 1.0f);
        }
        else if (slope == -std::numeric_limits<float>::infinity())
        {
            return glm::vec2(0.0f, -1.0f);
        }
        else
        {
            return glm::normalize(glm::vec2(1.0f, slope));
        }
    }

    inline float Get2D_SlopeFromDirection(const glm::vec2& direction)
    {
        if (direction.x == 0.0f)
        {
            return std::numeric_limits<float>::infinity();
        }
        else
        {
            return direction.y / direction.x;
        }
    }
}
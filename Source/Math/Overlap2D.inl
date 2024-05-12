


namespace Project001
{
    // Checking Point overlap --------------------------------------------------

    inline bool Check2D_Point_Point_Overlap(
        const glm::vec2& pointA_position,
        const glm::vec2& pointB_position)
    {
        return FloatEqualToFloat(pointA_position.x, pointB_position.x) &&
            FloatEqualToFloat(pointA_position.y, pointB_position.y);
    }

    inline bool Check2D_Point_Line_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        if (std::isinf(line_slope))
        {
            // vertical slope, so just compare x
            return FloatEqualToFloat(point_position.x, line_position.x);
        }
        else
        {
            // b = y1 - m * x1
            float yIntercept = line_position.y - line_slope * line_position.x;

            // y2 = m * x2 + b
            float expectedY = line_slope * point_position.x + yIntercept;

            return FloatEqualToFloat(point_position.y, expectedY);
        }
    }

    inline bool Check2D_Point_Ray_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        if (Check2D_Point_Point_Overlap(point_position, ray_position))
        {
            return true;
        }

        glm::vec2 ray_to_point = glm::normalize(point_position - ray_position);

        return FloatEqualToFloat(ray_direction.x, ray_to_point.x) &&
            FloatEqualToFloat(ray_direction.y, ray_to_point.y);
    }

    inline bool Check2D_Point_LineSegment_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        if (Check2D_Point_Rectangle_NoOverlap_H(point_position, lineSegment_start, lineSegment_end))
        {
            // the point is outside of the recangular range of the lineSegment
            return false;
        }

        float slopeDemoninator = lineSegment_end.x - lineSegment_start.x;
        if (slopeDemoninator == 0.0f)
        {
            // vertical slope, so just compare x
            return FloatEqualToFloat(point_position.x, lineSegment_start.x);
        }

        float slope = (lineSegment_end.y - lineSegment_start.y) / slopeDemoninator;

        // b = y1 - m * x1
        float yIntercept = lineSegment_start.y - slope * lineSegment_start.x;

        // y2 = m * x2 + b
        float expectedY = slope * point_position.x + yIntercept;

        return FloatEqualToFloat(point_position.y, expectedY);
    }

    inline bool Check2D_Point_Rectangle_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return FloatLessThanOrEqualToFloat(point_position.x, rectangle_topRight.x) &&
            FloatGreaterThanOrEqualToFloat(point_position.x, rectangle_bottomLeft.x) &&
            FloatLessThanOrEqualToFloat(point_position.y, rectangle_topRight.y) &&
            FloatGreaterThanOrEqualToFloat(point_position.y, rectangle_bottomLeft.y);
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
        return FloatLessThanOrEqualToFloat(pointToCircleCenterDistanceSquared, circleRadiusSquared);
    }

    inline bool Check2D_Point_Capsule_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        float pointLineSegmentDistanceSquared = Get2D_Point_LineSegment_DistanceSquared(point_position, capsule_start, capsule_end);
        float capsuleRadiusSquared = capsule_radius * capsule_radius;
        return FloatLessThanOrEqualToFloat(pointLineSegmentDistanceSquared, capsuleRadiusSquared);
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

        return FloatGreaterThanOrEqualToFloat(u, 0.0f) &&
            FloatGreaterThanOrEqualToFloat(v, 0.0f) &&
            !FloatGreaterThanOrEqualToFloat(u + v, 1.0f);
    }

    inline bool Check2D_Point_Polygon_Overlap(
        const glm::vec2& point_position,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount > 2)
        {
            // I'm going to check how many times a ray from the point intersects
            // the polygon's line segments.
            unsigned int intersectionCount = 0;

            // I don't use axis alinged rays. If they are aligned to
            // the x or y axis, they tend to collide with corners. I tend to
            // place things along the same horizontal axis or verticle axis.
            glm::vec2 ray_direction(0.78965f, 0.613558);
            glm::vec2 ray_perpendicular(-0.613558, 0.78965f);
            const glm::vec2& ray_position = point_position;

            // Remebering the previous intersection point so ray collisions with
            // corners only count as one intersction. This can cause a false
            // overlap to be detected if the point is outide and it's ray only
            // collides with an odd number of corners. Oh well... :(
            glm::vec2 previousIntersect_position(NAN, NAN);

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

                // the current intersection position to compared with the
                // previous to prevent double counting corners
                currentIntersect_position = ray_direction * t2;
                if (!Check2D_Point_Point_Overlap(previousIntersect_position, currentIntersect_position) &&
                    t2 >= 0.0f && t2 < 1.0f && t1 >= 0.0f)
                {
                    intersectionCount++;
                    previousIntersect_position = currentIntersect_position;
                }
                else
                {
                    previousIntersect_position = ray_position;
                }

                start_index = end_index;
            }

            if (intersectionCount % 2 == 1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (polygon_cornerCount == 2)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            const glm::vec2& corner2_position = polygon_corners[1];
            return Check2D_Point_LineSegment_Overlap(point_position, corner1_position, corner2_position);
        }
        else if (polygon_cornerCount == 1)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            return Check2D_Point_Point_Overlap(point_position, corner1_position);
        }
        else
        {
            return false;
        }
    }

    inline bool Check2D_Point_ConvexPolygon_Overlap(
        const glm::vec2& point_position,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Point_Polygon_Overlap(point_position, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking Line overlap ---------------------------------------------------

    inline bool Check2D_Line_Point_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& point_position)
    {
        return Check2D_Point_Line_Overlap(point_position, line_position, line_slope);
    }

    inline bool Check2D_Line_Line_Overlap(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope)
    {
        if ((std::isinf(lineA_slope) && std::isinf(lineB_slope)) || FloatEqualToFloat(lineA_slope, lineB_slope))
        {
            return Check2D_Point_Line_Overlap(lineA_position, lineB_position, lineB_slope);
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
                (ray_direction.x < 0.0f && line_position.x < ray_position.x) ||
                (ray_direction.x == 0.0f && Check2D_Point_Line_Overlap(ray_position, line_position, line_slope));
        }

        glm::vec2 perpendicular(-ray_direction.y, ray_direction.x);
        glm::vec2 lineToRay = ray_position - line_position;

        glm::vec2 line_direction = Get2D_DirectionFromSlope(line_slope);
        float denominator = glm::dot(line_direction, perpendicular);
        if (denominator == 0.0f)
        {
            if (Check2D_Point_Line_Overlap(ray_position, line_position, line_slope))
            {
                // lineSegment and ray are collinear
                return true;
            }

            // lineSegment and ray are parallel
            return false;
        }

        float t = (line_direction.x * lineToRay.y - lineToRay.x * line_direction.y) / denominator;

        return FloatGreaterThanOrEqualToFloat(t, 0.0f);
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
                (lineSegment_start.x >= line_position.x && lineSegment_end.x <= line_position.x);
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

        if (FloatEqualToFloat(lineSegment_start_yInterceptOffset, 0.0f) ||
            FloatEqualToFloat(lineSegment_end_yInterceptOffset, 0.0f))
        {
            // start or end of lineSegment on line
            return true;
        }

        // start and end of lineSegment are on opposite sides of the line
        return std::signbit(lineSegment_start_yInterceptOffset) !=
            std::signbit(lineSegment_end_yInterceptOffset);
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
        float rectangle_topRight_yInterceptOffset =
            rectangle_topRight.y - line_slope * rectangle_topRight.x - line_yIntercept;
        float rectangle_topLeft_yInterceptOffset =
            rectangle_topRight.y - line_slope * rectangle_bottomLeft.x - line_yIntercept;

        if (FloatEqualToFloat(rectangle_bottomLeft_yInterceptOffset, 0.0f) ||
            FloatEqualToFloat(rectangle_bottomRight_yInterceptOffset, 0.0f) ||
            FloatEqualToFloat(rectangle_topRight_yInterceptOffset, 0.0f) ||
            FloatEqualToFloat(rectangle_topLeft_yInterceptOffset, 0.0f))
        {
            // a rectangle corner is on line
            return true;
        }

        // at least one of the corners is on the opposite side of the line as
        // another corner
        return std::signbit(rectangle_bottomLeft_yInterceptOffset) != std::signbit(rectangle_bottomRight_yInterceptOffset) ||
            std::signbit(rectangle_bottomLeft_yInterceptOffset) != std::signbit(rectangle_topRight_yInterceptOffset) ||
            std::signbit(rectangle_bottomLeft_yInterceptOffset) != std::signbit(rectangle_topLeft_yInterceptOffset);
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
        return FloatLessThanOrEqualToFloat(pointLineDistanceSquared, circleRadiusSquared);
    }

    inline bool Check2D_Line_Capsule_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        if (std::isinf(line_slope))
        {
            if ((capsule_start.x < line_position.x && capsule_end.x > line_position.x) ||
                (capsule_start.x >= line_position.x && capsule_end.x <= line_position.x))
            {
                return true;
            }
        }
        else
        {
            // y = mx + b
            // b = y - m * x
            float yIntercept1 = line_position.y - line_slope * line_position.x;

            // y = mx + b
            // y - mx - b = 0 = yInterceptOffset
            // If yInterceptOffset < 0.0, the point is below the line
            // If yInterceptOffset > 0.0, the point is above the line
            float capsule_start_yInterceptOffset =
                capsule_start.y - line_slope * capsule_start.x - yIntercept1;
            float capsule_end_yInterceptOffset =
                capsule_end.y - line_slope * capsule_end.x - yIntercept1;

            if (FloatEqualToFloat(capsule_start_yInterceptOffset, 0.0f) ||
                FloatEqualToFloat(capsule_end_yInterceptOffset, 0.0f))
            {
                // start or end of capsule on line
                return true;
            }

            if (std::signbit(capsule_start_yInterceptOffset) !=
                std::signbit(capsule_end_yInterceptOffset))
            {
                // start and end of capsule are on opposite sides of the line
                return true;
            }
        }

        float capsuleRadiusSquared = capsule_radius * capsule_radius;

        // check if line intersects capsule start
        float capsuleStartDistanceSquared = Get2D_Point_Line_DistanceSquared(capsule_start, line_position, line_slope);
        if (FloatLessThanOrEqualToFloat(capsuleStartDistanceSquared, capsuleRadiusSquared))
        {
            return true;
        }

        // check if line intersects capsule end
        float capsuleEndDistanceSquared = Get2D_Point_Line_DistanceSquared(capsule_end, line_position, line_slope);
        return FloatLessThanOrEqualToFloat(capsuleEndDistanceSquared, capsuleRadiusSquared);
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
            return (triangle_corner1.x < line_position.x && triangle_corner2.x > line_position.x) ||
                (triangle_corner1.x >= line_position.x && triangle_corner2.x <= line_position.x) ||
                (triangle_corner1.x < line_position.x && triangle_corner3.x > line_position.x) ||
                (triangle_corner1.x >= line_position.x && triangle_corner3.x <= line_position.x);
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
        float triangle_corner3_yInterceptOffset =
            triangle_corner3.y - line_slope * triangle_corner3.x - line_yIntercept;

        if (FloatEqualToFloat(triangle_corner1_yInterceptOffset, 0.0f) ||
            FloatEqualToFloat(triangle_corner2_yInterceptOffset, 0.0f) ||
            FloatEqualToFloat(triangle_corner3_yInterceptOffset, 0.0f))
        {
            // a triangle corner is on line
            return true;
        }

        // at least one of the corners is on the opposite side of the line as
        // another corner
        return std::signbit(triangle_corner1_yInterceptOffset) != std::signbit(triangle_corner2_yInterceptOffset) ||
            std::signbit(triangle_corner1_yInterceptOffset) != std::signbit(triangle_corner3_yInterceptOffset);
    }

    inline bool Check2D_Line_Polygon_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount > 1)
        {
            if (std::isinf(line_slope))
            {
                const glm::vec2& firstCorner = polygon_corners[0];

                for (size_t index = 1; index < polygon_cornerCount; ++index)
                {
                    const glm::vec2& currentCorner = polygon_corners[index];

                    if ((firstCorner.x < line_position.x && currentCorner.x > line_position.x) ||
                        (firstCorner.x >= line_position.x && currentCorner.x <= line_position.x))
                    {
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
            // If yInterceptOffset < 0.0, the point is below the line
            // If yInterceptOffset > 0.0, the point is above the line
            float corner0_yInterceptOffset =
                polygon_corners[0].y - line_slope * polygon_corners[0].x - line_yIntercept;

            if (FloatEqualToFloat(corner0_yInterceptOffset, 0.0f))
            {
                // corner on line
                return true;
            }

            bool corner0_signbit = std::signbit(corner0_yInterceptOffset);

            for (size_t index = 0; index < polygon_cornerCount; ++index)
            {
                const glm::vec2& currentCorner = polygon_corners[index];

                float currentCorner_yInterceptOffset =
                    currentCorner.y - line_slope * currentCorner.x - line_yIntercept;

                if (FloatEqualToFloat(currentCorner_yInterceptOffset, 0.0f))
                {
                    // corner on line
                    return true;
                }

                if (corner0_signbit != std::signbit(currentCorner_yInterceptOffset))
                {
                    // current corner on opposite side of line from first corner
                    return true;
                }
            }

            return false;
        }
        else if (polygon_cornerCount == 1)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            return Check2D_Point_Line_Overlap(corner1_position, line_position, line_slope);
        }
        else
        {
            return false;
        }
    }

    inline bool Check2D_Line_ConvexPolygon_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Line_Polygon_Overlap(line_position, line_slope, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking Ray overlap ----------------------------------------------------

    inline bool Check2D_Ray_Point_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& point_position)
    {
        return Check2D_Point_Ray_Overlap(point_position, ray_position, ray_direction);
    }

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
            float rayA_slope = Get2D_SlopeFromDirection(rayA_direction);
            float rayB_slope = Get2D_SlopeFromDirection(rayB_direction);
            if (Check2D_Point_Ray_Overlap(rayA_position, rayB_position, rayB_direction) ||
                Check2D_Point_Ray_Overlap(rayB_position, rayA_position, rayA_direction))
            {
                // rays are collinear
                return true;
            }

            // rays are parallel
            return false;
        }

        float t1 = (rayB_direction.x * b_to_a.y - b_to_a.x * rayB_direction.y) / denominator;
        float t2 = glm::dot(b_to_a, perpendicular) / denominator;

        return FloatGreaterThanOrEqualToFloat(t2, 0.0f) &&
            FloatGreaterThanOrEqualToFloat(t1, 0.0f);
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
            float lineSegment_slope = Get2D_SlopeFromDirection(startToEnd);
            if (Check2D_Point_Ray_Overlap(lineSegment_start, ray_position, ray_direction))
            {
                // lineSegment and ray are collinear
                return true;
            }

            // lineSegment and ray are parallel
            return false;
        }

        float t1 = (startToEnd.x * startToRay.y - startToRay.x * startToEnd.y) / denominator;
        float t2 = glm::dot(startToRay, perpendicular) / denominator;

        return FloatGreaterThanOrEqualToFloat(t2, 0.0f) &&
            FloatLessThanOrEqualToFloat(t2, 1.0f) &&
            FloatGreaterThanOrEqualToFloat(t1, 0.0f);
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
        return FloatLessThanOrEqualToFloat(distanceSquared, circle_radius * circle_radius);
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
        if (FloatLessThanOrEqualToFloat(pointRayDistanceSquared1, capsuleRadiusSquared))
        {
            // capsule start is near the ray
            return true;
        }

        float pointRayDistanceSquared2 = Get2D_Point_Ray_DistanceSquared(capsule_end, ray_position, ray_direction);
        if (FloatLessThanOrEqualToFloat(pointRayDistanceSquared2, capsuleRadiusSquared))
        {
            // capsule end is near the ray
            return true;
        }

        // check if ray start is near capsule
        float pointLineSegmentDistanceSquared = Get2D_Point_LineSegment_DistanceSquared(ray_position, capsule_start, capsule_end);
        return FloatLessThanOrEqualToFloat(pointLineSegmentDistanceSquared, capsuleRadiusSquared);
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
        if (polygon_cornerCount > 1)
        {
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
        else if (polygon_cornerCount == 1)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            return Check2D_Point_Ray_Overlap(corner1_position, ray_position, ray_direction);
        }
        else
        {
            return false;
        }
    }

    inline bool Check2D_Ray_ConvexPolygon_Overlap(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Ray_Polygon_Overlap(ray_position, ray_direction, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking LineSegment overlap --------------------------------------------

    inline bool Check2D_LineSegment_Point_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& point_position)
    {
        return Check2D_Point_LineSegment_Overlap(point_position, lineSegment_start, lineSegment_end);
    }

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
        if (FloatLessThanOrEqualToFloat(pointLineSegmentDistanceSquared1, capsuleRadiusSquared))
        {
            // capsule start is near lineSegment
            return true;
        }

        float pointLineSegmentDistanceSquared2 = Get2D_Point_LineSegment_DistanceSquared(capsule_end, lineSegment_start, lineSegment_end);
        if (FloatLessThanOrEqualToFloat(pointLineSegmentDistanceSquared2, capsuleRadiusSquared))
        {
            // capsule end is near lineSegment
            return true;
        }

        float pointLineSegmentDistanceSquared3 = Get2D_Point_LineSegment_DistanceSquared(lineSegment_start, capsule_start, capsule_end);
        if (FloatLessThanOrEqualToFloat(pointLineSegmentDistanceSquared3, capsuleRadiusSquared))
        {
            // lineSegment start in near capsule
            return true;
        }

        // check if lineSegment end is near capsule
        float pointLineSegmentDistanceSquared4 = Get2D_Point_LineSegment_DistanceSquared(lineSegment_end, capsule_start, capsule_end);
        return FloatLessThanOrEqualToFloat(pointLineSegmentDistanceSquared4, capsuleRadiusSquared);
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
        if (polygon_cornerCount > 1)
        {
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
        else if (polygon_cornerCount == 1)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            return Check2D_Point_LineSegment_Overlap(corner1_position, lineSegment_start, lineSegment_end);
        }
        else
        {
            return false;
        }
    }

    inline bool Check2D_LineSegment_ConvexPolygon_Overlap(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        glm::vec2 convexPolygonA_array[2] = { lineSegment_start, lineSegment_end };
        const glm::vec2* convexPolygonA_corners = &convexPolygonA_array[0];
        return Check2D_ConvexPolygon_ConvexPolygon_Overlap(convexPolygonA_corners, 2, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking Rectangle overlap ----------------------------------------------

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
        return FloatLessThanOrEqualToFloat(rectangleA_bottomLeft.x, rectangleB_topRight.x) &&
            FloatGreaterThanOrEqualToFloat(rectangleA_topRight.x, rectangleB_bottomLeft.x) &&
            FloatLessThanOrEqualToFloat(rectangleA_bottomLeft.y, rectangleB_topRight.y) &&
            FloatGreaterThanOrEqualToFloat(rectangleA_topRight.y, rectangleB_bottomLeft.y);
    }

    inline bool Check2D_Rectangle_OrientedRectangle_Overlap(
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
        return FloatLessThanOrEqualToFloat(distanceSquared, circleRadiusSquared);
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
        perpenduclarLine /= glm::length(perpenduclarLine);
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

        // check if the other sidee of the capsule cross the rectangle frame
        if (Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, leftCapsuleLine_start, leftCapsuleLine_end))
        {
            return true;
        }

        // check if a corner of the rectangle is inside the capsule rectangle
        float distanceSquared = Get2D_Point_LineSegment_DistanceSquared(rectangle_bottomLeft, capsule_start, capsule_end);
        float capsuleRadiusSquared = capsule_radius * capsule_radius;
        return FloatLessThanOrEqualToFloat(distanceSquared, capsuleRadiusSquared);
    }

    inline bool Check2D_Rectangle_Triangle_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        return Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, triangle_corner1, triangle_corner2) ||
            Check2D_RectangleFrame_LineSegment_Overlap(rectangle_bottomLeft, rectangle_topRight, triangle_corner2, triangle_corner3) ||
            Check2D_LineSegment_Rectangle_Overlap(triangle_corner3, triangle_corner1, rectangle_bottomLeft, rectangle_topRight) ||
            Check2D_Point_Triangle_Overlap(rectangle_bottomLeft, triangle_corner1, triangle_corner2, triangle_corner3);
    }

    inline bool Check2D_Rectangle_Polygon_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount > 1)
        {
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
        else if (polygon_cornerCount == 1)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            return Check2D_Point_Rectangle_Overlap(corner1_position, rectangle_bottomLeft, rectangle_topRight);
        }
        else
        {
            return false;
        }
    }

    inline bool Check2D_Rectangle_ConvexPolygon_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        glm::vec2 convexPolygonA_array[4] = {
            rectangle_bottomLeft,
            glm::vec2(rectangle_topRight.x, rectangle_bottomLeft.y),
            rectangle_topRight,
            glm::vec2(rectangle_bottomLeft.x, rectangle_topRight.y)};
        const glm::vec2* convexPolygonA_corners = &convexPolygonA_array[0];
        return Check2D_ConvexPolygon_ConvexPolygon_Overlap(convexPolygonA_corners, 4, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking OrientedRectangle overlap --------------------------------------

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
        glm::vec2 translatedCirclePosition = circle_position - orientedRectangle_position;
        glm::vec2 rotatedCirclePosition = Rotate2DVector(translatedCirclePosition, -1.0f * orientedRectangle_rotation);

        return Check2D_Rectangle_Circle_Overlap(-1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize, rotatedCirclePosition, circle_radius);
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
        glm::vec2 translatedCapsuleStart = capsule_start - orientedRectangle_position;
        glm::vec2 rotatedCapsuleStart = Rotate2DVector(translatedCapsuleStart, -1.0f * orientedRectangle_rotation);
        glm::vec2 translatedCapsuleEnd = capsule_end - orientedRectangle_position;
        glm::vec2 rotatedCapsuleEnd = Rotate2DVector(translatedCapsuleEnd, -1.0f * orientedRectangle_rotation);

        return Check2D_Rectangle_Capsule_Overlap(-1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize, rotatedCapsuleStart, rotatedCapsuleEnd, capsule_radius);
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
        glm::vec2 translatedTriangleCorner1 = triangle_corner1 - orientedRectangle_position;
        glm::vec2 rotatedTriangleCorner1 = Rotate2DVector(translatedTriangleCorner1, -1.0f * orientedRectangle_rotation);
        glm::vec2 translatedTriangleCorner2 = triangle_corner2 - orientedRectangle_position;
        glm::vec2 rotatedTriangleCorner2 = Rotate2DVector(translatedTriangleCorner2, -1.0f * orientedRectangle_rotation);
        glm::vec2 translatedTriangleCorner3 = triangle_corner3 - orientedRectangle_position;
        glm::vec2 rotatedTriangleCorner3 = Rotate2DVector(translatedTriangleCorner3, -1.0f * orientedRectangle_rotation);

        return Check2D_Rectangle_Triangle_Overlap(-1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize, rotatedTriangleCorner1, rotatedTriangleCorner2, rotatedTriangleCorner3);
    }

    inline bool Check2D_OrientedRectangle_Polygon_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount > 1)
        {
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
        else if (polygon_cornerCount == 1)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            return Check2D_Point_OrientedRectangle_Overlap(corner1_position, orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation);
        }
        else
        {
            return false;
        }
    }

    inline bool Check2D_OrientedRectangle_ConvexPolygon_Overlap(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        glm::vec2 convexPolygonA_array[4];
        convexPolygonA_array[0] = Rotate2DVector(-orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        convexPolygonA_array[1].x = orientedRectangle_halfSize.x;
        convexPolygonA_array[1].y = -orientedRectangle_halfSize.y;
        convexPolygonA_array[1] = Rotate2DVector(convexPolygonA_array[1], orientedRectangle_rotation) + orientedRectangle_position;
        convexPolygonA_array[2] = Rotate2DVector(orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        convexPolygonA_array[3].x = -orientedRectangle_halfSize.x;
        convexPolygonA_array[3].y = orientedRectangle_halfSize.y;
        convexPolygonA_array[3] = Rotate2DVector(convexPolygonA_array[3], orientedRectangle_rotation) + orientedRectangle_position;
        const glm::vec2* convexPolygonA_corners = &convexPolygonA_array[0];
        return Check2D_ConvexPolygon_ConvexPolygon_Overlap(convexPolygonA_corners, 4, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking Circle overlap -------------------------------------------------

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
        if (polygon_cornerCount > 1)
        {
            size_t start_index = polygon_cornerCount - 1;
            for (size_t end_index = 0; end_index < polygon_cornerCount; ++end_index)
            {
                const glm::vec2& lineSegment_start = polygon_corners[start_index];
                const glm::vec2& lineSegment_end = polygon_corners[end_index];

                if (Check2D_LineSegment_Circle_Overlap(lineSegment_start, lineSegment_end, circle_position, circle_radius))
                {
                    return true;
                }

                start_index = end_index;
            }

            return Check2D_Point_Polygon_Overlap(circle_position, polygon_corners, polygon_cornerCount);
        }
        else if (polygon_cornerCount == 1)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            return Check2D_Point_Circle_Overlap(corner1_position, circle_position, circle_radius);
        }
        else
        {
            return false;
        }
    }

    inline bool Check2D_Circle_ConvexPolygon_Overlap(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Circle_Polygon_Overlap(circle_position, circle_radius, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking Capsule overlap ------------------------------------------------

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
        if (polygon_cornerCount > 1)
        {
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
        else if (polygon_cornerCount == 1)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            return Check2D_Point_Capsule_Overlap(corner1_position, capsule_start, capsule_end, capsule_radius);
        }
        else
        {
            return false;
        }
    }

    inline bool Check2D_Capsule_ConvexPolygon_Overlap(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Capsule_Polygon_Overlap(capsule_start, capsule_end, capsule_radius, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking Triangle overlap -----------------------------------------------

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

    inline bool Check2D_Triangle_Polygon_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount > 1)
        {
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
        else if (polygon_cornerCount == 1)
        {
            const glm::vec2& corner1_position = polygon_corners[0];
            return Check2D_Point_Triangle_Overlap(corner1_position, triangle_corner1, triangle_corner2, triangle_corner3);
        }
        else
        {
            return false;
        }
    }

    inline bool Check2D_Triangle_ConvexPolygon_Overlap(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        glm::vec2 convexPolygonA_array[3] = { triangle_corner1, triangle_corner2, triangle_corner3 };
        const glm::vec2* convexPolygonA_corners = &convexPolygonA_array[0];
        return Check2D_ConvexPolygon_ConvexPolygon_Overlap(convexPolygonA_corners, 3, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking Polygon overlap ------------------------------------------------

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
        if (polygonA_cornerCount > 1 && polygonB_cornerCount > 1)
        {
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

            if (polygonA_cornerCount > 2 || polygonB_cornerCount > 2)
            {
                return Check2D_Point_Polygon_Overlap(polygonA_corners[0], polygonB_corners, polygonB_cornerCount) ||
                    Check2D_Point_Polygon_Overlap(polygonB_corners[0], polygonA_corners, polygonA_cornerCount);
            }
            else
            {
                return false;
            }
        }
        else if (polygonA_cornerCount == 1)
        {
            return Check2D_Point_Polygon_Overlap(polygonA_corners[0], polygonB_corners, polygonB_cornerCount);
        }
        else if (polygonB_cornerCount == 1)
        {
            return Check2D_Point_Polygon_Overlap(polygonB_corners[0], polygonA_corners, polygonA_cornerCount);
        }
        else
        {
            return false;
        }
    }

    inline bool Check2D_Polygon_ConvexPolygon_Overlap(
        const glm::vec2* const& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Polygon_Polygon_Overlap(polygon_corners, polygon_cornerCount, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking Convex Polygon overlap -----------------------------------------

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
        if (convexPolygonA_cornerCount > 1)
        {
            if (convexPolygonB_cornerCount > 1)
            {
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
            else if (convexPolygonB_cornerCount == 1)
            {
                return Check2D_Point_Polygon_Overlap(convexPolygonB_corners[0], convexPolygonA_corners, convexPolygonA_cornerCount);
            }
            else
            {
                return false;
            }

        }
        else if (convexPolygonB_cornerCount > 1)
        {
            if (convexPolygonA_cornerCount == 1)
            {
                return Check2D_Point_Polygon_Overlap(convexPolygonA_corners[0], convexPolygonB_corners, convexPolygonB_cornerCount);
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
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
            float inverseCircleCenterToPointDistance = FastInverseSquareRoot(circleCenterToPointDistanceSquared); // can be zero
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

        float denominator = FastInverseSquareRoot(centerDistanceSquared); // can be zero
        float centerDistance = 1.0f / denominator;
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

        float denominator = FastInverseSquareRoot(centerDistanceSquared); // can be zero
        float centerDistance = 1.0f / denominator;
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

        if (FloatGreaterThanOrEqualToFloat(u, 0.0f) &&
            FloatGreaterThanOrEqualToFloat(v, 0.0f) &&
            !FloatGreaterThanOrEqualToFloat(u + v, 1.0f))
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
            // I'm going to check how many times a ray from the point intersects
            // the polygon's line segments.
            unsigned int intersectionCount = 0;

            // I don't use axis alinged rays. If they are aligned to
            // the x or y axis, they tend to collide with corners. I tend to
            // place things along the same horizontal axis or verticle axis.
            glm::vec2 ray_direction(0.78965f, 0.613558);
            glm::vec2 ray_perpendicular(-0.613558, 0.78965f);
            const glm::vec2& ray_position = point_position;

            // Remebering the previous intersection point so ray collisions with
            // corners only count as one intersction. This can cause a false
            // overlap to be detected if the point is outide and it's ray only
            // collides with an odd number of corners. Oh well... :(
            glm::vec2 previousIntersect_position(NAN, NAN);

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

                // the current intersection position to compared with the
                // previous to prevent double counting corners
                currentIntersect_position = ray_direction * t2;
                if (!Check2D_Point_Point_Overlap(previousIntersect_position, currentIntersect_position) &&
                    t2 >= 0.0f && t2 < 1.0f && t1 >= 0.0f)
                {
                    intersectionCount++;
                    previousIntersect_position = currentIntersect_position;
                }
                else
                {
                    previousIntersect_position = ray_position;
                }

                start_index = end_index;
            }

            if (intersectionCount % 2 == 1)
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

    // Get Line Collision Point And Normal Functions ---------------------------

    inline void Get2D_Line_Rectangle_CollisionPointAndNormal(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        glm::vec2 rectangle_center = (rectangle_bottomLeft + rectangle_topRight) * 0.5f;

        if (std::isinf(line_slope))
        {
            collisionPoint_position.x = line_position.x;
            collisionPoint_position.y = rectangle_center.y;

            if (line_position.x <= rectangle_center.x)
            {
                collisionNormal.x = 1.0f;
                collisionNormal.y = 0.0f;
            }
            else
            {
                collisionNormal.x = -1.0f;
                collisionNormal.y = 0.0f;
            }
        }
        else if (FloatEqualToFloat(line_slope, 0.0f))
        {
            collisionPoint_position.x = rectangle_center.x;
            collisionPoint_position.y = line_position.y;

            if (line_position.y <= rectangle_center.y)
            {
                collisionNormal.x = 0.0f;
                collisionNormal.y = 1.0f;
            }
            else
            {
                collisionNormal.x = 0.0f;
                collisionNormal.y = -1.0f;
            }
        }
        else
        {
            // y = m * x + b
            // b = y - m * x
            float line_yIntercept = line_position.y - line_slope * line_position.x;

            // y = m * x + b
            float y_intersect_left = line_slope * rectangle_bottomLeft.x + line_yIntercept;

            // y = m * x + b
            float y_intersect_right = line_slope * rectangle_topRight.x + line_yIntercept;

            // x = (y - b) / m
            float x_intersect_bottom = (rectangle_bottomLeft.y - line_yIntercept) / line_slope;

            // x = (y - b) / m
            float x_intersect_top = (rectangle_topRight.y - line_yIntercept) / line_slope;

            glm::vec2 intersections[2]; // must be 2 intersections
            size_t currentIntersection = 0;

            if (y_intersect_left > rectangle_bottomLeft.y && y_intersect_left < rectangle_topRight.y)
            {
                intersections[currentIntersection].x = rectangle_bottomLeft.x;
                intersections[currentIntersection].y = y_intersect_left;
                currentIntersection++;
            }

            if (y_intersect_right > rectangle_bottomLeft.y && y_intersect_right < rectangle_topRight.y)
            {
                intersections[currentIntersection].x = rectangle_topRight.x;
                intersections[currentIntersection].y = y_intersect_right;
                currentIntersection++;
            }

            if (x_intersect_bottom > rectangle_bottomLeft.x && x_intersect_bottom < rectangle_topRight.x)
            {
                intersections[currentIntersection].x = x_intersect_bottom;
                intersections[currentIntersection].y = rectangle_bottomLeft.y;
                currentIntersection++;
            }

            if (x_intersect_top > rectangle_bottomLeft.x && x_intersect_top < rectangle_topRight.x)
            {
                intersections[currentIntersection].x = x_intersect_top;
                intersections[currentIntersection].y = rectangle_topRight.y;
                currentIntersection++;
            }

            collisionPoint_position = (intersections[0] + intersections[1]) * 0.5f;

            glm::vec2 line_direction = glm::normalize(glm::vec2(1, line_slope));
            glm::vec2 line_perpendicular(-line_direction.y, line_direction.x);
            glm::vec2 collision_to_center = rectangle_center - collisionPoint_position;
            float scalar = glm::dot(collision_to_center, line_perpendicular);

            if (scalar >= 0)
            {
                collisionNormal = line_perpendicular;
            }
            else
            {
                collisionNormal = -line_perpendicular;
            }
        }
    }

    inline void Get2D_Line_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        // rotate and translate the line rather then the orientedRectangle
        glm::vec2 transformed_line_position = line_position;
        transformed_line_position -= orientedRectangle_position;
        transformed_line_position = Rotate2DVector(transformed_line_position, -1.0f * orientedRectangle_rotation);
        float transformed_line_slope = RotateSlope(line_slope, -1.0f * orientedRectangle_rotation);

        Get2D_Line_Rectangle_CollisionPointAndNormal(transformed_line_position, transformed_line_slope, -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize, collisionPoint_position, collisionNormal);
        collisionPoint_position = Rotate2DVector(collisionPoint_position, orientedRectangle_rotation);
        collisionPoint_position += orientedRectangle_position;
        collisionNormal = Rotate2DVector(collisionNormal, orientedRectangle_rotation);
    }

    inline void Get2D_Line_Circle_CollisionPointAndNormal(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        glm::vec2 line_direction = Get2D_DirectionFromSlope(line_slope);

        float intersection_directionScalar1;
        float intersection_directionScalar2;
        unsigned int intersection_count = Get2D_Line_Circle_IntersectionDirectionScalars(
            line_position,
            line_direction,
            circle_position,
            circle_radius,
            intersection_directionScalar1,
            intersection_directionScalar2
        );

        if (intersection_count == 2)
        {
            glm::vec2 collisionPoint_position1 = line_position + intersection_directionScalar1 * line_direction;
            glm::vec2 collisionPoint_position2 = line_position + intersection_directionScalar2 * line_direction;
            collisionPoint_position = (collisionPoint_position1 + collisionPoint_position2) * 0.5f;
        }
        else if (intersection_count == 1)
        {
            collisionPoint_position = line_position + intersection_directionScalar1 * line_direction;
        }

        glm::vec2 line_perpendicular(-line_direction.y, line_direction.x);
        glm::vec2 collision_to_center = circle_position - collisionPoint_position;
        float scalar = glm::dot(collision_to_center, line_perpendicular);

        if (scalar >= 0)
        {
            collisionNormal = line_perpendicular;
        }
        else
        {
            collisionNormal = -line_perpendicular;
        }
    }

    // Get Ray Collision Point And Normal Functions ----------------------------

    inline void Get2D_Ray_Rectangle_CollisionPointAndNormal(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        float ray_slope = ray_direction.y / ray_direction.x;
        if (!Check2D_Point_Rectangle_Overlap(ray_position, rectangle_bottomLeft, rectangle_topRight))
        {
            Get2D_Line_Rectangle_CollisionPointAndNormal(ray_position, ray_slope, rectangle_bottomLeft, rectangle_topRight, collisionPoint_position, collisionNormal);
        }
        else
        {
            Get2D_Ray_Rectangle_CollisionPointAndNormal_H(ray_position, ray_direction, rectangle_bottomLeft, rectangle_topRight, collisionPoint_position, collisionNormal);
        }
    }

    inline void Get2D_Ray_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        // rotate and translate the ray rather then the orientedRectangle
        glm::vec2 transformed_ray_position = ray_position;
        transformed_ray_position -= orientedRectangle_position;
        transformed_ray_position = Rotate2DVector(transformed_ray_position, -1.0f * orientedRectangle_rotation);
        glm::vec2 transformed_ray_direction = Rotate2DVector(ray_direction, -1.0f * orientedRectangle_rotation);

        Get2D_Ray_Rectangle_CollisionPointAndNormal(transformed_ray_position, transformed_ray_direction, -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize, collisionPoint_position, collisionNormal);
        collisionPoint_position = Rotate2DVector(collisionPoint_position, orientedRectangle_rotation);
        collisionPoint_position += orientedRectangle_position;
        collisionNormal = Rotate2DVector(collisionNormal, orientedRectangle_rotation);
    }

    inline void Get2D_Ray_Circle_CollisionPointAndNormal(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        size_t collisionPointCount = 0;

        if (Check2D_Point_Circle_Overlap(ray_position, circle_position, circle_radius))
        {
            collisionPoint_position = ray_position;
            collisionPointCount++;
        }
        else
        {
            collisionPoint_position = glm::vec2(0.0f, 0.0f);
        }

        float intersection_directionScalar1;
        float intersection_directionScalar2;
        unsigned int intersection_count = Get2D_Line_Circle_IntersectionDirectionScalars(
            ray_position,
            ray_direction,
            circle_position,
            circle_radius,
            intersection_directionScalar1,
            intersection_directionScalar2
        );

        if (intersection_count > 0 && intersection_directionScalar1 > 0.0f)
        {
            glm::vec2 currentIntersection_position = ray_position + intersection_directionScalar1 * ray_direction;
            collisionPointCount++;
            float collisionPointCount_float = (float)collisionPointCount;
            collisionPoint_position = collisionPoint_position * (collisionPointCount_float - 1.0f) / collisionPointCount_float + currentIntersection_position / collisionPointCount_float;

        }

        if (intersection_count > 1 && intersection_directionScalar2 > 0.0f && collisionPointCount < 2)
        {
            glm::vec2 currentIntersection_position = ray_position + intersection_directionScalar2 * ray_direction;
            collisionPointCount++;
            float collisionPointCount_float = (float)collisionPointCount;
            collisionPoint_position = collisionPoint_position * (collisionPointCount_float - 1.0f) / collisionPointCount_float + currentIntersection_position / collisionPointCount_float;
        }

        glm::vec2 ray_perpendicular(-ray_direction.y, ray_direction.x);
        glm::vec2 collision_to_center = circle_position - collisionPoint_position;
        float scalar = glm::dot(collision_to_center, ray_perpendicular);

        if (scalar >= 0.0f)
        {
            collisionNormal = ray_perpendicular;
        }
        else
        {
            collisionNormal = -ray_perpendicular;
        }
    }

    // Get LineSegment Collision Point And Normal Functions --------------------

    inline void Get2D_LineSegment_Rectangle_CollisionPointAndNormal(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        glm::vec2 lineSegment_direction = glm::normalize(lineSegment_end - lineSegment_start);
        float lineSegment_slope = lineSegment_direction.y / lineSegment_direction.x;
        if (Check2D_Point_Rectangle_Overlap(lineSegment_start, rectangle_bottomLeft, rectangle_topRight))
        {
            if (Check2D_Point_Rectangle_Overlap(lineSegment_end, rectangle_bottomLeft, rectangle_topRight))
            {
                glm::vec2 rectangle_center = (rectangle_bottomLeft + rectangle_topRight) * 0.5f;

                collisionPoint_position = (lineSegment_start + lineSegment_end) * 0.5f;

                glm::vec2 lineSegment_perpendicular(-lineSegment_direction.y, lineSegment_direction.x);

                glm::vec2 collision_to_center = rectangle_center - collisionPoint_position;
                float scalar = glm::dot(collision_to_center, lineSegment_perpendicular);

                if (scalar >= 0)
                {
                    collisionNormal = lineSegment_perpendicular;
                }
                else
                {
                    collisionNormal = -lineSegment_perpendicular;
                }
            }
            else
            {
                Get2D_Ray_Rectangle_CollisionPointAndNormal_H(lineSegment_start, lineSegment_direction, rectangle_bottomLeft, rectangle_topRight, collisionPoint_position, collisionNormal);
            }
        }
        else if (Check2D_Point_Rectangle_Overlap(lineSegment_end, rectangle_bottomLeft, rectangle_topRight))
        {
            Get2D_Ray_Rectangle_CollisionPointAndNormal_H(lineSegment_end, -lineSegment_direction, rectangle_bottomLeft, rectangle_topRight, collisionPoint_position, collisionNormal);
        }
        else
        {
            Get2D_Line_Rectangle_CollisionPointAndNormal(lineSegment_start, lineSegment_slope, rectangle_bottomLeft, rectangle_topRight, collisionPoint_position, collisionNormal);
        }
    }

    inline void Get2D_LineSegment_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        // rotate and translate the lineSegment rather then the orientedRectangle
        glm::vec2 transformed_lineSegment_start = lineSegment_start;
        transformed_lineSegment_start -= orientedRectangle_position;
        transformed_lineSegment_start = Rotate2DVector(transformed_lineSegment_start, -1.0f * orientedRectangle_rotation);
        glm::vec2 transformed_lineSegment_end = lineSegment_end;
        transformed_lineSegment_end -= orientedRectangle_position;
        transformed_lineSegment_end = Rotate2DVector(transformed_lineSegment_end, -1.0f * orientedRectangle_rotation);

        Get2D_LineSegment_Rectangle_CollisionPointAndNormal(transformed_lineSegment_start, transformed_lineSegment_end, -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize, collisionPoint_position, collisionNormal);
        collisionPoint_position = Rotate2DVector(collisionPoint_position, orientedRectangle_rotation);
        collisionPoint_position += orientedRectangle_position;
        collisionNormal = Rotate2DVector(collisionNormal, orientedRectangle_rotation);
    }

    inline void Get2D_LineSegment_Circle_CollisionPointAndNormal(
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        glm::vec2 lineSegment_startToEnd = lineSegment_end - lineSegment_start;
        size_t collisionPointCount = 0;

        if (Check2D_Point_Circle_Overlap(lineSegment_start, circle_position, circle_radius))
        {
            collisionPointCount++;
            collisionPoint_position = lineSegment_start;
        }
        else
        {
            collisionPoint_position = glm::vec2(0.0f, 0.0f);
        }

        if (Check2D_Point_Circle_Overlap(lineSegment_end, circle_position, circle_radius))
        {
            collisionPointCount++;
            float collisionPointCount_float = (float)collisionPointCount;
            collisionPoint_position = collisionPoint_position * (collisionPointCount_float - 1.0f) / collisionPointCount_float + lineSegment_end / collisionPointCount_float;
        }

        if (collisionPointCount < 2)
        {
            float intersection_directionScalar1;
            float intersection_directionScalar2;
            unsigned int intersection_count = Get2D_Line_Circle_IntersectionDirectionScalars(
                lineSegment_start,
                lineSegment_startToEnd,
                circle_position,
                circle_radius,
                intersection_directionScalar1,
                intersection_directionScalar2
            );

            if (intersection_count > 0 && intersection_directionScalar1 > 0.0f && intersection_directionScalar1 < 1.0f)
            {
                glm::vec2 currentIntersection_position = lineSegment_start + intersection_directionScalar1 * lineSegment_startToEnd;
                collisionPointCount++;
                float collisionPointCount_float = (float)collisionPointCount;
                collisionPoint_position = collisionPoint_position * (collisionPointCount_float - 1.0f) / collisionPointCount_float + currentIntersection_position / collisionPointCount_float;

            }

            if (intersection_count > 1 && intersection_directionScalar2 > 0.0f && intersection_directionScalar2 < 1.0f && collisionPointCount < 2)
            {
                glm::vec2 currentIntersection_position = lineSegment_start + intersection_directionScalar2 * lineSegment_startToEnd;
                collisionPointCount++;
                float collisionPointCount_float = (float)collisionPointCount;
                collisionPoint_position = collisionPoint_position * (collisionPointCount_float - 1.0f) / collisionPointCount_float + currentIntersection_position / collisionPointCount_float;
            }
        }

        glm::vec2 lineSegment_perpendicular(-lineSegment_startToEnd.y, lineSegment_startToEnd.x);
        lineSegment_perpendicular = glm::normalize(lineSegment_perpendicular);
        glm::vec2 collision_to_center = circle_position - collisionPoint_position;
        float scalar = glm::dot(collision_to_center, lineSegment_perpendicular);

        if (scalar >= 0)
        {
            collisionNormal = lineSegment_perpendicular;
        }
        else
        {
            collisionNormal = -lineSegment_perpendicular;
        }
    }

    // Get Rectangle Collision Point And Normal Functions ----------------------

    inline void Get2D_Rectangle_Line_CollisionPointAndNormal(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_Line_Rectangle_CollisionPointAndNormal(line_position, line_slope, rectangle_bottomLeft, rectangle_topRight, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_Rectangle_Ray_CollisionPointAndNormal(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_Ray_Rectangle_CollisionPointAndNormal(ray_position, ray_direction, rectangle_bottomLeft, rectangle_topRight, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_Rectangle_LineSegment_CollisionPointAndNormal(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_LineSegment_Rectangle_CollisionPointAndNormal(lineSegment_start, lineSegment_end, rectangle_bottomLeft, rectangle_topRight, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_Rectangle_Rectangle_CollisionPointAndNormal(
        const glm::vec2& rectangleA_bottomLeft,
        const glm::vec2& rectangleA_topRight,
        const glm::vec2& rectangleB_bottomLeft,
        const glm::vec2& rectangleB_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        glm::vec2 overlapRectangle_bottomLeft;
        glm::vec2 overlapRectangle_topRight;

        overlapRectangle_bottomLeft.x = std::max(rectangleA_bottomLeft.x, rectangleB_bottomLeft.x);
        overlapRectangle_bottomLeft .y= std::max(rectangleA_bottomLeft.y, rectangleB_bottomLeft.y);
        overlapRectangle_topRight.x = std::min(rectangleA_topRight.x, rectangleB_topRight.x);
        overlapRectangle_topRight.y = std::min(rectangleA_topRight.y, rectangleB_topRight.y);

        collisionPoint_position = (overlapRectangle_bottomLeft + overlapRectangle_topRight) * 0.5f;

        glm::vec2 overlapRectangle_dimensions = (overlapRectangle_topRight - overlapRectangle_bottomLeft);
        glm::vec2 rectangleA_center = (rectangleA_bottomLeft + rectangleA_topRight) * 0.5f;
        glm::vec2 rectangleB_center = (rectangleB_bottomLeft + rectangleB_topRight) * 0.5f;
        glm::vec2 a_to_b = rectangleB_center - rectangleA_center;

        if (std::abs(overlapRectangle_dimensions.y) <= std::abs(overlapRectangle_dimensions.x))
        {
            collisionNormal.x = 0.0f;

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

            if (a_to_b.x >= 0)
            {
                collisionNormal.x = 1.0f;
            }
            else
            {
                collisionNormal.x = -1.0f;
            }
        }
    }

    inline void Get2D_Rectangle_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        // The number the second rectangle's corners colliding with the first rectangle.
        glm::vec2 rectangleB_averageCornerCollisionPosition;
        size_t rectangleB_cornerCollisionCount = 0;

        { // rectangleB corner 1 check
            glm::vec2 orientedRectangle_bottomLeft(-1.0f * orientedRectangle_halfSize.x, -1.0f * orientedRectangle_halfSize.y);
            orientedRectangle_bottomLeft = Rotate2DVector(orientedRectangle_bottomLeft, orientedRectangle_rotation);
            orientedRectangle_bottomLeft += orientedRectangle_position;

            if (Check2D_Point_Rectangle_Overlap(orientedRectangle_bottomLeft, rectangle_bottomLeft, rectangle_topRight))
            {
                rectangleB_cornerCollisionCount++;
                float rectangleB_cornerCollisionCount_float = (float)rectangleB_cornerCollisionCount;
                rectangleB_averageCornerCollisionPosition = rectangleB_averageCornerCollisionPosition * (rectangleB_cornerCollisionCount_float - 1.0f) / rectangleB_cornerCollisionCount_float + orientedRectangle_bottomLeft / rectangleB_cornerCollisionCount_float;
            }
        }

        { // rectangleB corner 2 check
            glm::vec2 orientedRectangle_topRight(orientedRectangle_halfSize.x, orientedRectangle_halfSize.y);
            orientedRectangle_topRight = Rotate2DVector(orientedRectangle_topRight, orientedRectangle_rotation);
            orientedRectangle_topRight += orientedRectangle_position;

            if (Check2D_Point_Rectangle_Overlap(orientedRectangle_topRight, rectangle_bottomLeft, rectangle_topRight))
            {
                rectangleB_cornerCollisionCount++;
                float rectangleB_cornerCollisionCount_float = (float)rectangleB_cornerCollisionCount;
                rectangleB_averageCornerCollisionPosition = rectangleB_averageCornerCollisionPosition * (rectangleB_cornerCollisionCount_float - 1.0f) / rectangleB_cornerCollisionCount_float + orientedRectangle_topRight / rectangleB_cornerCollisionCount_float;
            }
        }

        { // rectangleB corner 3 check
            glm::vec2 orientedRectangle_bottomRight(orientedRectangle_halfSize.x, -1.0f * orientedRectangle_halfSize.y);
            orientedRectangle_bottomRight = Rotate2DVector(orientedRectangle_bottomRight, orientedRectangle_rotation);
            orientedRectangle_bottomRight += orientedRectangle_position;

            if (Check2D_Point_Rectangle_Overlap(orientedRectangle_bottomRight, rectangle_bottomLeft, rectangle_topRight))
            {
                rectangleB_cornerCollisionCount++;
                float rectangleB_cornerCollisionCount_float = (float)rectangleB_cornerCollisionCount;
                rectangleB_averageCornerCollisionPosition = rectangleB_averageCornerCollisionPosition * (rectangleB_cornerCollisionCount_float - 1.0f) / rectangleB_cornerCollisionCount_float + orientedRectangle_bottomRight / rectangleB_cornerCollisionCount_float;
            }
        }

        { // rectangleB corner 4 check
            glm::vec2 orientedRectangle_topLeft(-1.0f * orientedRectangle_halfSize.x, orientedRectangle_halfSize.y);
            orientedRectangle_topLeft = Rotate2DVector(orientedRectangle_topLeft, orientedRectangle_rotation);
            orientedRectangle_topLeft += orientedRectangle_position;

            if (Check2D_Point_Rectangle_Overlap(orientedRectangle_topLeft, rectangle_bottomLeft, rectangle_topRight))
            {
                rectangleB_cornerCollisionCount++;
                float rectangleB_cornerCollisionCount_float = (float)rectangleB_cornerCollisionCount;
                rectangleB_averageCornerCollisionPosition = rectangleB_averageCornerCollisionPosition * (rectangleB_cornerCollisionCount_float - 1.0f) / rectangleB_cornerCollisionCount_float + orientedRectangle_topLeft / rectangleB_cornerCollisionCount_float;
            }
        }

        bool foundMaxCollisionPoints = false;

        if (rectangleB_cornerCollisionCount > 2)
        {
            // If at least 3 corners of rectangleB are in rectangleA, then
            // there no corners of rectangleA in rectangleB.
            foundMaxCollisionPoints = true;
        }

        // The number the first rectangle's corners colliding with the second rectangle.
        glm::vec2 rectangleA_averageCornerCollisionPosition;
        size_t rectangleA_cornerCollisionCount = 0;

        if (!foundMaxCollisionPoints) // rectangleA corner 1 check
        {
            // rotate and translate the first rectangle so it's oriented rather then the orientedRectangle
            glm::vec2 transformed_rectangle_bottomLeft(rectangle_bottomLeft.x, rectangle_bottomLeft.y);
            transformed_rectangle_bottomLeft -= orientedRectangle_position;
            transformed_rectangle_bottomLeft = Rotate2DVector(transformed_rectangle_bottomLeft, -1.0f * orientedRectangle_rotation);

            if (Check2D_Point_Rectangle_Overlap(transformed_rectangle_bottomLeft, -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize))
            {
                rectangleA_cornerCollisionCount++;
                float rectangleA_cornerCollisionCount_float = (float)rectangleA_cornerCollisionCount;
                rectangleA_averageCornerCollisionPosition = rectangleA_averageCornerCollisionPosition * (rectangleA_cornerCollisionCount_float - 1.0f) / rectangleA_cornerCollisionCount_float + transformed_rectangle_bottomLeft / rectangleA_cornerCollisionCount_float;
            }

            if (rectangleB_cornerCollisionCount > 1 && rectangleA_cornerCollisionCount > 0)
            {
                // If at least 2 corners of rectangleB are in rectangleA and
                // 1 corner of rectangleA is in rectnalgeB, then there are no
                // more intersecting corners.
                foundMaxCollisionPoints = true;
            }
        }

        if (!foundMaxCollisionPoints) // rectangleA corner 2 check
        {
            glm::vec2 transformed_rectangle_topRight(rectangle_topRight.x, rectangle_topRight.y);
            transformed_rectangle_topRight -= orientedRectangle_position;
            transformed_rectangle_topRight = Rotate2DVector(transformed_rectangle_topRight, -1.0f * orientedRectangle_rotation);

            if (Check2D_Point_Rectangle_Overlap(transformed_rectangle_topRight, -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize))
            {
                rectangleA_cornerCollisionCount++;
                float rectangleA_cornerCollisionCount_float = (float)rectangleA_cornerCollisionCount;
                rectangleA_averageCornerCollisionPosition = rectangleA_averageCornerCollisionPosition * (rectangleA_cornerCollisionCount_float - 1.0f) / rectangleA_cornerCollisionCount_float + transformed_rectangle_topRight / rectangleA_cornerCollisionCount_float;
            }

            if (rectangleB_cornerCollisionCount > 1 && rectangleA_cornerCollisionCount > 0)
            {
                // If at least 2 corners of rectangleB are in rectangleA and
                // 1 corner of rectangleA is in rectnalgeB, then there are no
                // more intersecting corners.
                foundMaxCollisionPoints = true;
            }
        }

        if (!foundMaxCollisionPoints) // rectangleA corner 3 check
        {
            glm::vec2 transformed_rectangle_bottomRight(rectangle_topRight.x, rectangle_bottomLeft.y);
            transformed_rectangle_bottomRight -= orientedRectangle_position;
            transformed_rectangle_bottomRight = Rotate2DVector(transformed_rectangle_bottomRight, -1.0f * orientedRectangle_rotation);

            if (Check2D_Point_Rectangle_Overlap(transformed_rectangle_bottomRight, -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize))
            {
                rectangleA_cornerCollisionCount++;
                float rectangleA_cornerCollisionCount_float = (float)rectangleA_cornerCollisionCount;
                rectangleA_averageCornerCollisionPosition = rectangleA_averageCornerCollisionPosition * (rectangleA_cornerCollisionCount_float - 1.0f) / rectangleA_cornerCollisionCount_float + transformed_rectangle_bottomRight / rectangleA_cornerCollisionCount_float;
            }

            if (rectangleB_cornerCollisionCount > 1 && rectangleA_cornerCollisionCount > 0)
            {
                // If at least 2 corners of rectangleB are in rectangleA and
                // 1 corner of rectangleA is in rectnalgeB, then there are no
                // more intersecting corners.
                foundMaxCollisionPoints = true;
            }
        }

        if (!foundMaxCollisionPoints) // rectangleA corner 4 check
        {
            glm::vec2 transformed_rectangle_topLeft(rectangle_bottomLeft.x, rectangle_topRight.y);
            transformed_rectangle_topLeft -= orientedRectangle_position;
            transformed_rectangle_topLeft = Rotate2DVector(transformed_rectangle_topLeft, -1.0f * orientedRectangle_rotation);

            if (Check2D_Point_Rectangle_Overlap(transformed_rectangle_topLeft, -1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize))
            {
                rectangleA_cornerCollisionCount++;
                float rectangleA_cornerCollisionCount_float = (float)rectangleA_cornerCollisionCount;
                rectangleA_averageCornerCollisionPosition = rectangleA_averageCornerCollisionPosition * (rectangleA_cornerCollisionCount_float - 1.0f) / rectangleA_cornerCollisionCount_float + transformed_rectangle_topLeft / rectangleA_cornerCollisionCount_float;
            }
        }

        glm::vec2 rectangleA_position = (rectangle_topRight + rectangle_bottomLeft) * 0.5f;

        if (rectangleA_cornerCollisionCount != 0)
        {
            glm::vec2 transformedRectangleA_averageCornerCollisionPosition = Rotate2DVector(rectangleA_averageCornerCollisionPosition, orientedRectangle_rotation);
            transformedRectangleA_averageCornerCollisionPosition += orientedRectangle_position;

            if (rectangleB_cornerCollisionCount != 0) // Both rectangle corners are intersecting eachother
            {
                float rectangleB_cornerCollisionCount_float = (float)rectangleB_cornerCollisionCount;
                float rectangleA_cornerCollisionCount_float = (float)rectangleA_cornerCollisionCount;
                float combined_cornerCollisionCount_float = rectangleB_cornerCollisionCount_float + rectangleA_cornerCollisionCount_float;
                collisionPoint_position = (rectangleB_averageCornerCollisionPosition * rectangleB_cornerCollisionCount_float + transformedRectangleA_averageCornerCollisionPosition * rectangleA_cornerCollisionCount_float) / combined_cornerCollisionCount_float;
            }
            else // Only rectangleA corners are intersecting
            {
                collisionPoint_position = transformedRectangleA_averageCornerCollisionPosition;
            }
        }
        else if (rectangleB_cornerCollisionCount != 0) // Only rectangleA corners are intersecting
        {
            collisionPoint_position = rectangleB_averageCornerCollisionPosition;

        }
        else // No corners of either are intersecting the other
        {
            collisionPoint_position = (rectangleA_position + orientedRectangle_position) * 0.5f;
        }

        glm::vec2 a_to_b = orientedRectangle_position - rectangleA_position;

        if (rectangleB_cornerCollisionCount >= rectangleA_cornerCollisionCount)
        {
            glm::vec2 rectangle_dimensions = rectangle_topRight - rectangle_bottomLeft;
            glm::vec2 center_to_collision_ratio = (collisionPoint_position - rectangleA_position) / rectangle_dimensions;
            if (std::abs(center_to_collision_ratio.y) < std::abs(center_to_collision_ratio.x))
            {
                if (a_to_b.x < 0.0f)
                {
                    collisionNormal.x = -1.0f;
                }
                else
                {
                    collisionNormal.x = 1.0f;
                }
                collisionNormal.y = 0.0f;
            }
            else
            {
                collisionNormal.x = 0.0f;
                if (a_to_b.y < 0.0f)
                {
                    collisionNormal.y = -1.0f;
                }
                else
                {
                    collisionNormal.y = 1.0f;
                }
            }
        }
        else
        {
            glm::vec2 transformed_a_to_b = Rotate2DVector(a_to_b, -1.0f * orientedRectangle_rotation);

            glm::vec2 rectangle_dimensions = orientedRectangle_halfSize * 2.0f;
            glm::vec2 transformed_collision_position = collisionPoint_position;
            transformed_collision_position -= orientedRectangle_position;
            transformed_collision_position = Rotate2DVector(transformed_collision_position, -1.0f * orientedRectangle_rotation);
            glm::vec2 center_to_collision_ratio = transformed_collision_position / rectangle_dimensions;
            if (std::abs(center_to_collision_ratio.y) < std::abs(center_to_collision_ratio.x))
            {
                if (transformed_a_to_b.x < 0.0f)
                {
                    collisionNormal.x = -1.0f;
                }
                else
                {
                    collisionNormal.x = 1.0f;
                }
                collisionNormal.y = 0.0f;
            }
            else
            {
                collisionNormal.x = 0.0f;
                if (transformed_a_to_b.y < 0.0f)
                {
                    collisionNormal.y = -1.0f;
                }
                else
                {
                    collisionNormal.y = 1.0f;
                }
            }
            collisionNormal = Rotate2DVector(collisionNormal, orientedRectangle_rotation);
        }
    }

    inline void Get2D_Rectangle_Circle_CollisionPointAndNormal(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        bool circleInCornerZone = false;
        glm::vec2 collisionCorner;
        if (circle_position.y < rectangle_bottomLeft.y)
        {
            if (circle_position.x < rectangle_bottomLeft.x)
            {
                collisionCorner.x = rectangle_bottomLeft.x;
                collisionCorner.y = rectangle_bottomLeft.y;
                circleInCornerZone = true;
            }
            else if (circle_position.x > rectangle_topRight.x)
            {
                collisionCorner.x = rectangle_topRight.x;
                collisionCorner.y = rectangle_bottomLeft.y;
                circleInCornerZone = true;
            }
        }
        else if (circle_position.y > rectangle_topRight.y)
        {
            if (circle_position.x < rectangle_bottomLeft.x)
            {
                collisionCorner.x = rectangle_bottomLeft.x;
                collisionCorner.y = rectangle_topRight.y;
                circleInCornerZone = true;
            }
            else if (circle_position.x > rectangle_topRight.x)
            {
                collisionCorner.x = rectangle_topRight.x;
                collisionCorner.y = rectangle_topRight.y;
                circleInCornerZone = true;
            }
        }
        if (circleInCornerZone)
        {
            collisionPoint_position = collisionCorner;
            collisionNormal = glm::normalize(circle_position - collisionPoint_position);
        }
        else
        {
            glm::vec2 rectangle_position = (rectangle_topRight + rectangle_bottomLeft) * 0.5f;

            glm::vec2 rectangleCenter_circle_closestPoint;
            Get2D_Point_Circle_ClosestPoint(rectangle_position, circle_position, circle_radius, rectangleCenter_circle_closestPoint);

            glm::vec2 circleCenter_rectangle_closestPoint;
            Get2D_Point_Rectangle_ClosestPoint(circle_position, rectangle_bottomLeft, rectangle_topRight, circleCenter_rectangle_closestPoint);

            collisionPoint_position = (rectangleCenter_circle_closestPoint + circleCenter_rectangle_closestPoint) * 0.5f;

            glm::vec2 rectangle_dimensions = rectangle_topRight - rectangle_bottomLeft;
            glm::vec2 rectangleCenterToCircleCenter_ratio = (circle_position - rectangle_position) / rectangle_dimensions;
            if (std::abs(rectangleCenterToCircleCenter_ratio.y) < std::abs(rectangleCenterToCircleCenter_ratio.x))
            {
                if (rectangleCenterToCircleCenter_ratio.x < 0.0f)
                {
                    collisionNormal.x = -1.0f;
                }
                else
                {
                    collisionNormal.x = 1.0f;
                }
                collisionNormal.y = 0.0f;
            }
            else
            {
                collisionNormal.x = 0.0f;
                if (rectangleCenterToCircleCenter_ratio.y < 0.0f)
                {
                    collisionNormal.y = -1.0f;
                }
                else
                {
                    collisionNormal.y = 1.0f;
                }
            }
        }
    }

    // Get OrientedRectangle Collision Point And Normal Functions --------------

    inline void Get2D_OrientedRectangle_Line_CollisionPointAndNormal(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_Line_OrientedRectangle_CollisionPointAndNormal(line_position, line_slope, orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_OrientedRectangle_Ray_CollisionPointAndNormal(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_Ray_OrientedRectangle_CollisionPointAndNormal(ray_position, ray_direction, orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_OrientedRectangle_LineSegment_CollisionPointAndNormal(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_LineSegment_OrientedRectangle_CollisionPointAndNormal(lineSegment_start, lineSegment_end, orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_OrientedRectangle_Rectangle_CollisionPointAndNormal(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_Rectangle_OrientedRectangle_CollisionPointAndNormal(rectangle_bottomLeft, rectangle_topRight, orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_OrientedRectangle_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& orientedRectangleA_halfSize,
        const glm::vec2& orientedRectangleA_position,
        const float& orientedRectangleA_rotation,
        const glm::vec2& orientedRectangleB_halfSize,
        const glm::vec2& orientedRectangleB_position,
        const float& orientedRectangleB_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
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

        Get2D_Rectangle_OrientedRectangle_CollisionPointAndNormal(-1.0f * orientedRectangleA_halfSize, orientedRectangleA_halfSize, orientedRectangleB_halfSize, combinedRectangleB_position, combinedRectangleB_rotation, collisionPoint_position, collisionNormal);
        collisionPoint_position = Rotate2DVector(collisionPoint_position, orientedRectangleA_rotation);
        collisionPoint_position += orientedRectangleA_position;
        collisionNormal = Rotate2DVector(collisionNormal, orientedRectangleA_rotation);
    }

    inline void Get2D_OrientedRectangle_Circle_CollisionPointAndNormal(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& circle_position,
        const float& circle_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        // rotate and translate the circle rather then the orientedRectangle
        glm::vec2 translatedCirclePosition = circle_position - orientedRectangle_position;
        glm::vec2 rotatedCirclePosition = Rotate2DVector(translatedCirclePosition, -1.0f * orientedRectangle_rotation);

        Get2D_Rectangle_Circle_CollisionPointAndNormal(-1.0f * orientedRectangle_halfSize, orientedRectangle_halfSize, rotatedCirclePosition, circle_radius, collisionPoint_position, collisionNormal);
        collisionPoint_position = Rotate2DVector(collisionPoint_position, orientedRectangle_rotation);
        collisionPoint_position += orientedRectangle_position;
        collisionNormal = Rotate2DVector(collisionNormal, orientedRectangle_rotation);
    }

    // Get Circle Collision Point And Normal Functions -------------------------

    inline void Get2D_Circle_Line_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& line_position,
        const float& line_slope,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_Line_Circle_CollisionPointAndNormal(line_position, line_slope, circle_position, circle_radius, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_Circle_Ray_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_Ray_Circle_CollisionPointAndNormal(ray_position, ray_direction, circle_position, circle_radius, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_Circle_LineSegment_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_LineSegment_Circle_CollisionPointAndNormal(lineSegment_start, lineSegment_end, circle_position, circle_radius, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_Circle_Rectangle_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_Rectangle_Circle_CollisionPointAndNormal(rectangle_bottomLeft, rectangle_topRight, circle_position, circle_radius, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_Circle_OrientedRectangle_CollisionPointAndNormal(
        const glm::vec2& circle_position,
        const float& circle_radius,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        Get2D_OrientedRectangle_Circle_CollisionPointAndNormal(orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, circle_position, circle_radius, collisionPoint_position, collisionNormal);
        collisionNormal *= -1.0f;
    }

    inline void Get2D_Circle_Circle_CollisionPointAndNormal(
        const glm::vec2& circleA_position,
        const float& circleA_radius,
        const glm::vec2& circleB_position,
        const float& circleB_radius,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        glm::vec2 centerA_circleB_closestPoint;
        Get2D_Point_Circle_ClosestPoint(circleA_position, circleB_position, circleB_radius, centerA_circleB_closestPoint);
        glm::vec2 centerB_circleA_closestPoint;
        Get2D_Point_Circle_ClosestPoint(circleB_position, circleA_position, circleA_radius, centerB_circleA_closestPoint);
        collisionPoint_position = (centerA_circleB_closestPoint + centerB_circleA_closestPoint) * 0.5f;

        glm::vec2 a_to_b = circleB_position - circleA_position;
        collisionNormal = glm::normalize(a_to_b);

        // alternative aproximation:
        // collisionPoint_position = circleA_position + a_to_b * circleA_radius / (circleA_radius + circleB_radius);
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

    inline float Get2D_Point_Line_DistanceSquared_Alt(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        if (line_slope == std::numeric_limits<float>::infinity())
        {
            float distance = std::abs(point_position.x - line_position.x);
            return distance * distance;
        }

        float invertedSlope = -1.0f / line_slope;

        glm::vec2 perpendicularPoint;
        Get2D_Line_Line_Intersection_H(line_position, line_slope, point_position, invertedSlope, perpendicularPoint);

        return Get2D_Point_Point_DistanceSquared(point_position, perpendicularPoint);
    }

    inline bool Check2D_Point_Rectangle_NoOverlap_H(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_oppositeCorner1,
        const glm::vec2& rectangle_oppositeCorner2)
    {
        float maxX, minX, maxY, minY;
        GetMinMax(rectangle_oppositeCorner1.x, rectangle_oppositeCorner2.x, minX, maxX);
        GetMinMax(rectangle_oppositeCorner1.y, rectangle_oppositeCorner2.y, minY, maxY);

        return point_position.x > maxX || point_position.x < minX ||
            point_position.y > maxY || point_position.y < minY;
    }

    inline bool Check2D_Point_Rectangle_Overlap_H(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_oppositeCorner1,
        const glm::vec2& rectangle_oppositeCorner2)
    {
        float maxX, minX, maxY, minY;
        GetMinMax(rectangle_oppositeCorner1.x, rectangle_oppositeCorner2.x, minX, maxX);
        GetMinMax(rectangle_oppositeCorner1.y, rectangle_oppositeCorner2.y, minY, maxY);

        return FloatLessThanOrEqualToFloat(point_position.x, maxX) &&
            FloatGreaterThanOrEqualToFloat(point_position.x, minX) &&
            FloatLessThanOrEqualToFloat(point_position.y, maxY) &&
            FloatGreaterThanOrEqualToFloat(point_position.y, minY);
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

    inline void Get2D_Line_Line_Intersection_H(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope,
        glm::vec2& intersection_position)
    {
        // y = m * x + b
        // b = y - m * x
        float yInterceptA = lineA_position.y - lineA_slope * lineA_position.x;
        float yInterceptB = lineB_position.y - lineB_slope * lineB_position.x;

        // y = mA * x + bA
        // y = mB * x + bB
        // mA * x + bA = mB * x + bB
        // mA * x = mB * x + bB - bA
        // mA * x - mB * x = bB - bA
        // x * (mA - mB) = bB - bA
        // x = (bB - bA) / (mA - mB)
        intersection_position.x = (yInterceptB - yInterceptA) / (lineA_slope - lineB_slope);
        intersection_position.y = lineA_slope * intersection_position.x + yInterceptA;
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
            intersection_directionScalar1 = (-b + std::sqrt(discriminant)) / (2 * a);
            intersection_directionScalar2 = (-b - std::sqrt(discriminant)) / (2 * a);

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
            float t1 = (-b + std::sqrt(discriminant)) / (2 * a);
            float t2 = (-b - std::sqrt(discriminant)) / (2 * a);

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

    inline void Get2D_Ray_Rectangle_CollisionPointAndNormal_H(
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        glm::vec2& collisionPoint_position,
        glm::vec2& collisionNormal)
    {
        glm::vec2 rectangle_center = (rectangle_bottomLeft + rectangle_topRight) * 0.5f;

        if (FloatEqualToFloat(ray_direction.y, 0.0f))
        {
            if (ray_direction.x >= 0.0f)
            {
                collisionPoint_position.x = (ray_position.x + rectangle_topRight.x) * 0.5f;
                collisionPoint_position.y = ray_position.y;
            }
            else
            {
                collisionPoint_position.x = (ray_position.x + rectangle_bottomLeft.x) * 0.5f;
                collisionPoint_position.y = ray_position.y;
            }

            if (ray_position.y <= rectangle_center.y)
            {
                collisionNormal.x = 0.0f;
                collisionNormal.y = 1.0f;
            }
            else
            {
                collisionNormal.x = 0.0f;
                collisionNormal.y = -1.0f;
            }
        }
        else if (FloatEqualToFloat(ray_direction.x, 0.0f))
        {
            if (ray_direction.y >= 0.0f)
            {
                collisionPoint_position.x = ray_position.x;
                collisionPoint_position.y = (ray_position.y + rectangle_topRight.y) * 0.5f;
            }
            else
            {
                collisionPoint_position.x = ray_position.x;
                collisionPoint_position.y = (ray_position.y + rectangle_bottomLeft.y) * 0.5f;
            }

            if (ray_position.x <= rectangle_center.x)
            {
                collisionNormal.x = 1.0f;
                collisionNormal.y = 0.0f;
            }
            else
            {
                collisionNormal.x = -1.0f;
                collisionNormal.y = 0.0f;
            }
        }
        else
        {
            // x = ray_position.x + t_x * ray_direction.x
            // y = ray_position.y + t_y * ray_direction.y
            //
            // t_x = (x - ray_position.x) / ray_direction.x
            // t_y = (y - ray_position.y) / ray_direction.y

            float t_x;
            if (ray_direction.x >= 0.0f)
            {
                t_x = (rectangle_topRight.x - ray_position.x) / ray_direction.x;
            }
            else
            {
                t_x = (rectangle_bottomLeft.x - ray_position.x) / ray_direction.x;
            }

            collisionPoint_position.x = ray_position.x + t_x * ray_direction.x;
            collisionPoint_position.y = ray_position.y + t_x * ray_direction.y;

            if (collisionPoint_position.y > rectangle_topRight.y ||
                collisionPoint_position.y < rectangle_bottomLeft.y)
            {
                float t_y;
                if (ray_direction.y >= 0.0f)
                {
                    t_y = (rectangle_topRight.y - ray_position.y) / ray_direction.y;
                }
                else
                {
                    t_y = (rectangle_bottomLeft.y - ray_position.y) / ray_direction.y;
                }

                collisionPoint_position.x = ray_position.x + t_y * ray_direction.x;
                collisionPoint_position.y = ray_position.y + t_y * ray_direction.y;
            }

            collisionPoint_position = (collisionPoint_position + ray_position) * 0.5f;

            glm::vec2 ray_perpendicular(-ray_direction.y, ray_direction.x);

            glm::vec2 collision_to_center = rectangle_center - collisionPoint_position;
            float scalar = glm::dot(collision_to_center, ray_perpendicular);

            if (scalar >= 0)
            {
                collisionNormal = ray_perpendicular;
            }
            else
            {
                collisionNormal = -ray_perpendicular;
            }
        }
    }

    inline bool Check2D_Rectangle_Rectangle_Overlap_Alt(
        const glm::vec2& rectangleA_bottomLeft,
        const glm::vec2& rectangleA_topRight,
        const glm::vec2& rectangleB_bottomLeft,
        const glm::vec2& rectangleB_topRight)
    {
        // first check if one of rectangleA's corners is inside rectangleB

        if (Check2D_Point_Rectangle_Overlap(rectangleA_bottomLeft, rectangleB_bottomLeft, rectangleB_topRight) ||
            Check2D_Point_Rectangle_Overlap(rectangleA_topRight, rectangleB_bottomLeft, rectangleB_topRight))
        {
            return true;
        }

        glm::vec2 rectangleA_bottomRight(rectangleA_topRight.x, rectangleA_bottomLeft.y);
        if (Check2D_Point_Rectangle_Overlap(rectangleA_bottomRight, rectangleB_bottomLeft, rectangleB_topRight))
        {
            return true;
        }

        glm::vec2 rectangleA_topLeft(rectangleA_bottomLeft.x, rectangleA_topRight.y);
        if (Check2D_Point_Rectangle_Overlap(rectangleA_topLeft, rectangleB_bottomLeft, rectangleB_topRight))
        {
            return true;
        }

        // now check if one of rectangleB's corners is inside rectangleA

        if (Check2D_Point_Rectangle_Overlap(rectangleB_bottomLeft, rectangleA_bottomLeft, rectangleA_topRight) ||
            Check2D_Point_Rectangle_Overlap(rectangleB_topRight, rectangleA_bottomLeft, rectangleA_topRight))
        {
            return true;
        }

        glm::vec2 rectangleB_bottomRight(rectangleB_topRight.x, rectangleB_bottomLeft.y);
        if (Check2D_Point_Rectangle_Overlap(rectangleB_bottomRight, rectangleA_bottomLeft, rectangleA_topRight))
        {
            return true;
        }

        glm::vec2 rectangleB_topLeft(rectangleB_bottomLeft.x, rectangleB_topRight.y);
        return Check2D_Point_Rectangle_Overlap(rectangleB_topLeft, rectangleA_bottomLeft, rectangleA_topRight);
    }

    inline bool Check2D_Rectangle_Circle_Overlap_Alt(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        return Check2D_RectangleFrame_Circle_Overlap(rectangle_bottomLeft, rectangle_topRight, circle_position, circle_radius) ||
            Check2D_Point_Rectangle_Overlap(circle_position, rectangle_bottomLeft, rectangle_topRight);
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

    inline bool Check2D_RectangleFrame_Circle_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        glm::vec2 rectangle_bottomRight(rectangle_topRight.x, rectangle_bottomLeft.y);

        float circleRadiusSquared = circle_radius * circle_radius;

        // Check if the bottom of the rectangle crosses the circle
        float pointLineSegmentDistanceSquared1 = Get2D_Point_LineSegment_DistanceSquared(circle_position, rectangle_bottomLeft, rectangle_bottomRight);
        if (FloatLessThanOrEqualToFloat(pointLineSegmentDistanceSquared1, circleRadiusSquared))
        {
            return true;
        }

        // Check if the right of the rectangle crosses the circle
        float pointLineSegmentDistanceSquared2 = Get2D_Point_LineSegment_DistanceSquared(circle_position, rectangle_bottomRight, rectangle_topRight);
        if (FloatLessThanOrEqualToFloat(pointLineSegmentDistanceSquared2, circleRadiusSquared))
        {
            return true;
        }

        glm::vec2 rectangle_topLeft(rectangle_bottomLeft.x, rectangle_topRight.y);

        // Check if the top of the rectangle crosses the circle
        float pointLineSegmentDistanceSquared3 = Get2D_Point_LineSegment_DistanceSquared(circle_position, rectangle_topRight, rectangle_topLeft);
        if (FloatLessThanOrEqualToFloat(pointLineSegmentDistanceSquared3, circleRadiusSquared))
        {
            return true;
        }

        // Check if the left of the rectangle crosses the circle
        float pointLineSegmentDistanceSquared4 = Get2D_Point_LineSegment_DistanceSquared(circle_position, rectangle_topLeft, rectangle_bottomLeft);
        return FloatLessThanOrEqualToFloat(pointLineSegmentDistanceSquared4, circleRadiusSquared);
    }

    inline float Get2D_Slope(const glm::vec2& start, const glm::vec2& end)
    {
        float denominator = end.x - start.x;
        if (denominator == 0.0f)
        {
            return std::numeric_limits<float>::infinity();
        }
        else
        {
            return (end.y - start.y) / denominator;
        }
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
            glm::vec2 axisOfSeparation(-(lineSegmentA_end.y - lineSegmentA_start.y), lineSegmentA_end.x - lineSegmentA_start.x);

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            for (size_t i = 0; i < convexPolygonA_cornerCount; ++i)
            {
                float currentProjection = glm::dot(convexPolygonA_corners[i], axisOfSeparation);

                if (currentProjection > projectionA_max)
                {
                    projectionA_max = currentProjection;
                }

                if (currentProjection < projectionA_min)
                {
                    projectionA_min = currentProjection;
                }
            }

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            for (size_t i = 0; i < convexPolygonB_cornerCount; ++i)
            {
                float currentProjection = glm::dot(convexPolygonB_corners[i], axisOfSeparation);

                if (currentProjection > projectionB_max)
                {
                    projectionB_max = currentProjection;
                }

                if (currentProjection < projectionB_min)
                {
                    projectionB_min = currentProjection;
                }
            }

            if (!(FloatLessThanOrEqualToFloat(projectionA_min, projectionB_max) &&
                FloatGreaterThanOrEqualToFloat(projectionA_max, projectionB_min)))
            {
                // projections don't overlap on axis of separation
                return false;
            }

            start_indexA = end_indexA;
        }

        return true;
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

        // float cosAngle = std::cosf(rotationInRadians);
        // float sinAngle = std::sinf(rotationInRadians);
        // 
        // // slope = rise / run = y / x
        // // y = slope
        // // x = 1.0
        // // newY = sinAngle * x + cosAngle * y = sinAngle + cosAngle * slope
        // // newX = cosAngle * x - sinAngle * y = cosAngle - sinAngle * slope
        // // newSlope = newY / newX
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

    // From Quake III Arena
    inline float FastInverseSquareRoot(float number)
    {
        long i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y = number;
        i = *(long*)&y; // evil floating point bit level hacking
        i = 0x5f3759df - (i >> 1); // what?
        y = *(float*)&i;
        y = y * (threehalfs - (x2 * y * y)); // 1st iteration
        // y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration

        return y;
    }
}
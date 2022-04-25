


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
        if (line_slope == INFINITY)
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
        glm::vec2 pointToCircle = point_position - circle_position;
        float pointToCircleDistanceSquared = glm::dot(pointToCircle, pointToCircle);
        float circleRadiusSquared = circle_radius * circle_radius;
        return FloatLessThanOrEqualToFloat(pointToCircleDistanceSquared, circleRadiusSquared);
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

        float dot00 = glm::dot(AC, AC);
        float dot01 = glm::dot(AC, AB);
        float dot02 = glm::dot(AC, AP);
        float dot11 = glm::dot(AB, AB);
        float dot12 = glm::dot(AB, AP);

        float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

        return FloatGreaterThanOrEqualToFloat(u, 0.0f) &&
            FloatGreaterThanOrEqualToFloat(v, 0.0f) &&
            !FloatGreaterThanOrEqualToFloat(u + v, 1.0f);
    }

    inline bool Check2D_Point_Polygon_Overlap(
        const glm::vec2& point_position,
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount > 2)
        {
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

                float denominator = glm::dot(startToEnd, ray_perpendicular);
                if (denominator == 0.0f)
                {
                    // add a bit to avoid divide-by-zero
                    denominator += FLT_MIN;
                }

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
        const glm::vec2*& convexPolygon_corners,
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
        return !FloatEqualToFloat(lineA_slope,lineB_slope) || Check2D_Point_Point_Overlap(lineA_position, lineB_position);
    }

    inline bool Check2D_Line_Ray_Overlap(
        const glm::vec2& line_position,
        const float& line_slope,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
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
        glm::vec2 transformed_line_start = line_position;
        transformed_line_start -= orientedRectangle_position;
        transformed_line_start = Rotate2DVector(transformed_line_start, -1.0f * orientedRectangle_rotation);
        glm::vec2 transformed_line_end(line_position.x + 1.0f, line_position.y + line_slope);
        transformed_line_end -= orientedRectangle_position;
        transformed_line_end = Rotate2DVector(transformed_line_end, -1.0f * orientedRectangle_rotation);
        float transformed_line_slope = Get2D_Slope(transformed_line_start, transformed_line_end);

        // y = mx + b
        // b = y - mx
        float line_yIntercept =
            transformed_line_start.y - transformed_line_slope * transformed_line_start.x;

        // y = mx + b
        // y - mx - b = 0 = yInterceptOffset
        // If yInterceptOffset < 0.0, the point is below the line
        // If yInterceptOffset > 0.0, the point is above the line
        float rectangle_bottomLeft_yInterceptOffset =
            -1.0f * orientedRectangle_halfSize.y - transformed_line_slope * -1.0f * orientedRectangle_halfSize.x - line_yIntercept;
        float rectangle_bottomRight_yInterceptOffset =
            -1.0f * orientedRectangle_halfSize.y - transformed_line_slope * orientedRectangle_halfSize.x - line_yIntercept;
        float rectangle_topRight_yInterceptOffset =
            orientedRectangle_halfSize.y - transformed_line_slope * orientedRectangle_halfSize.x - line_yIntercept;
        float rectangle_topLeft_yInterceptOffset =
            orientedRectangle_halfSize.y - transformed_line_slope * -1.0f * orientedRectangle_halfSize.x - line_yIntercept;

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
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        if (polygon_cornerCount > 1)
        {
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
        const glm::vec2*& convexPolygon_corners,
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
        const glm::vec2*& polygon_corners,
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
        const glm::vec2*& convexPolygon_corners,
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
        const glm::vec2*& polygon_corners,
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
        const glm::vec2*& convexPolygon_corners,
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
        const glm::vec2*& polygon_corners,
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
        const glm::vec2*& convexPolygon_corners,
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
        const glm::vec2*& polygon_corners,
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
        const glm::vec2*& convexPolygon_corners,
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
        const glm::vec2*& polygon_corners,
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
        const glm::vec2*& convexPolygon_corners,
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
        const glm::vec2*& polygon_corners,
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
        const glm::vec2*& convexPolygon_corners,
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
        const glm::vec2*& polygon_corners,
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
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        glm::vec2 convexPolygonA_array[3] = { triangle_corner1, triangle_corner2, triangle_corner3 };
        const glm::vec2* convexPolygonA_corners = &convexPolygonA_array[0];
        return Check2D_ConvexPolygon_ConvexPolygon_Overlap(convexPolygonA_corners, 3, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking Polygon overlap ------------------------------------------------

    inline bool Check2D_Polygon_Point_Overlap(
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& point_position)
    {
        return Check2D_Point_Polygon_Overlap(point_position, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Line_Overlap(
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_Polygon_Overlap(line_position, line_slope, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Ray_Overlap(
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        return Check2D_Ray_Polygon_Overlap(ray_position, ray_direction, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_LineSegment_Overlap(
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        return Check2D_LineSegment_Polygon_Overlap(lineSegment_start, lineSegment_end, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Rectangle_Overlap(
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return Check2D_Rectangle_Polygon_Overlap(rectangle_bottomLeft, rectangle_topRight, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_OrientedRectangle_Overlap(
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        return Check2D_OrientedRectangle_Polygon_Overlap(orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Circle_Overlap(
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        return Check2D_Circle_Polygon_Overlap(circle_position, circle_radius, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Capsule_Overlap(
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        return Check2D_Capsule_Polygon_Overlap(capsule_start, capsule_end, capsule_radius, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Triangle_Overlap(
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        return Check2D_Triangle_Polygon_Overlap(triangle_corner1, triangle_corner2, triangle_corner3, polygon_corners, polygon_cornerCount);
    }

    inline bool Check2D_Polygon_Polygon_Overlap(
        const glm::vec2*& polygonA_corners,
        const size_t& polygonA_cornerCount,
        const glm::vec2*& polygonB_corners,
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
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount,
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount)
    {
        return Check2D_Polygon_Polygon_Overlap(polygon_corners, polygon_cornerCount, convexPolygon_corners, convexPolygon_cornerCount);
    }

    // Checking Convex Polygon overlap -----------------------------------------

    inline bool Check2D_ConvexPolygon_Point_Overlap(
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& point_position)
    {
        return Check2D_Point_ConvexPolygon_Overlap(point_position, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Line_Overlap(
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        return Check2D_Line_ConvexPolygon_Overlap(line_position, line_slope, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Ray_Overlap(
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& ray_position,
        const glm::vec2& ray_direction)
    {
        return Check2D_Ray_ConvexPolygon_Overlap(ray_position, ray_direction, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_LineSegment_Overlap(
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        return Check2D_LineSegment_ConvexPolygon_Overlap(lineSegment_start, lineSegment_end, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Rectangle_Overlap(
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return Check2D_Rectangle_ConvexPolygon_Overlap(rectangle_bottomLeft, rectangle_topRight, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_OrientedRectangle_Overlap(
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation)
    {
        return Check2D_OrientedRectangle_ConvexPolygon_Overlap(orientedRectangle_halfSize, orientedRectangle_position, orientedRectangle_rotation, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Circle_Overlap(
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& circle_position,
        const float& circle_radius)
    {
        return Check2D_Circle_ConvexPolygon_Overlap(circle_position, circle_radius, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Capsule_Overlap(
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        return Check2D_Capsule_ConvexPolygon_Overlap(capsule_start, capsule_end, capsule_radius, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Triangle_Overlap(
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3)
    {
        return Check2D_Triangle_ConvexPolygon_Overlap(triangle_corner1, triangle_corner2, triangle_corner3, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_Polygon_Overlap(
        const glm::vec2*& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        const glm::vec2*& polygon_corners,
        const size_t& polygon_cornerCount)
    {
        return Check2D_Polygon_ConvexPolygon_Overlap(polygon_corners, polygon_cornerCount, convexPolygon_corners, convexPolygon_cornerCount);
    }

    inline bool Check2D_ConvexPolygon_ConvexPolygon_Overlap(
        const glm::vec2*& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2*& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount)
    {
        if (convexPolygonA_cornerCount > 1)
        {
            if (convexPolygonB_cornerCount > 1)
            {
                return Check2D_ConvexPolygon_ConvexPolygon_SeparatedAxisTheorem(
                    convexPolygonA_corners,
                    convexPolygonA_cornerCount,
                    convexPolygonB_corners,
                    convexPolygonB_cornerCount) &&
                    Check2D_ConvexPolygon_ConvexPolygon_SeparatedAxisTheorem(
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

    // Helper Functions --------------------------------------------------------

    inline float Get2D_Point_Line_DistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        if (line_slope == INFINITY)
        {
            float distance = std::abs(point_position.x - line_position.x);
            return distance * distance;
        }

        const float& y1 = line_slope;
        float x2 = point_position.x - line_position.x;
        float y2 = point_position.y - line_position.y;
        float temp = y2 - y1 * x2;
        return temp * temp / (1.0f + y1 * y1);
    }

    inline float Get2D_Point_Line_DistanceSquared_Alt(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        if (line_slope == INFINITY)
        {
            float distance = std::abs(point_position.x - line_position.x);
            return distance * distance;
        }

        float invertedSlope = -1.0f / line_slope;

        glm::vec2 perpendicularPoint;
        Get2D_Line_Line_Intersection_H(line_position, line_slope, point_position, invertedSlope, perpendicularPoint);

        glm::vec2 pointToPerpendicularPoint = perpendicularPoint - point_position;

        return glm::dot(pointToPerpendicularPoint, pointToPerpendicularPoint);
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
            // Nearest point is the ray's start position
            return glm::dot(rayToPoint, rayToPoint);
        }
        else
        {
            // Nearest point on the ray is perpendicular
            const float& x1 = ray_direction.x;
            const float& y1 = ray_direction.y;
            const float& x2 = rayToPoint.x;
            const float& y2 = rayToPoint.y;
            float temp = x1 * y2 - y1 * x2;
            return temp * temp / (x1 * x1 + y1 * y1);
        }
    }

    inline float Get2D_Point_LineSegment_DistanceSquared(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        glm::vec2 lineSegment = lineSegment_end - lineSegment_start;
        glm::vec2 endToPoint = point_position - lineSegment_end;
        glm::vec2 startToPoint = point_position - lineSegment_start;

        float dotProduct1 = glm::dot(lineSegment, endToPoint);
        float dotProduct2 = glm::dot(lineSegment, startToPoint);

        float minimumDistanceSquared = 0.0f;

        if (dotProduct1 > 0.0f)
        {
            // Nearest Point on the LineSegment is the end
            minimumDistanceSquared = glm::dot(endToPoint, endToPoint);
        }
        else if (dotProduct2 < 0.0f)
        {
            // Nearest Point on the LineSegment is the start
            minimumDistanceSquared = glm::dot(startToPoint, startToPoint);
        }
        else
        {
            // Nearest Point on the LineSegment is perpendicular
            const float& x1 = lineSegment.x;
            const float& y1 = lineSegment.y;
            const float& x2 = startToPoint.x;
            const float& y2 = startToPoint.y;
            float temp = x1 * y2 - y1 * x2;
            minimumDistanceSquared = temp * temp / (x1 * x1 + y1 * y1);
        }

        return minimumDistanceSquared;
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
            return INFINITY;
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

    inline bool Check2D_ConvexPolygon_ConvexPolygon_SeparatedAxisTheorem(
        const glm::vec2*& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2*& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount)
    {
        size_t start_indexA = convexPolygonA_cornerCount - 1;
        for (size_t end_indexA = 0; end_indexA < convexPolygonA_cornerCount; ++end_indexA)
        {
            const glm::vec2& lineSegmentA_start = convexPolygonA_corners[start_indexA];
            const glm::vec2& lineSegmentA_end = convexPolygonA_corners[end_indexA];

            glm::vec2 axisOfSeparation(-(lineSegmentA_end.y - lineSegmentA_start.y), lineSegmentA_end.x - lineSegmentA_start.x);

            float projectionA_max = -INFINITY;
            float projectionA_min = INFINITY;
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

            float projectionB_max = -INFINITY;
            float projectionB_min = INFINITY;
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
        }

        return true;
    }

    inline float RotateSlope(float slope, float rotationInRadians)
    {
        float cosAngle = std::cosf(rotationInRadians);
        float sinAngle = std::sinf(rotationInRadians);

        // slope = rise / run = y / x
        // y = slope
        // x = 1.0
        // newY = sinAngle * x + cosAngle * y = sinAngle + cosAngle * slope
        // newX = cosAngle * x - sinAngle * y = cosAngle - sinAngle * slope
        // newSlope = newY / newX
        return (sinAngle + cosAngle * slope) / (cosAngle - sinAngle * slope);
    }

    inline glm::vec2 Get2D_DirectionFromSlope(float slope)
    {
        if (slope == INFINITY)
        {
            return glm::vec2(1.0f, 0.0f);
        }
        else if (slope == -INFINITY)
        {
            return glm::vec2(-1.0f, 0.0f);
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
            return INFINITY;
        }
        else
        {
            return direction.y / direction.x;
        }
    }
}
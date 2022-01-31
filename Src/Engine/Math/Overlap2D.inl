


namespace Project001
{
    // Checking Point overlap -----------------------------------------------

    inline bool Check2D_Point_Point_Overlap(
        const glm::vec2& pointA_position,
        const glm::vec2& pointB_position)
    {
        return FloatsEqual(pointA_position.x, pointB_position.x) &&
            FloatsEqual(pointA_position.y, pointB_position.y);
    }

    inline bool Check2D_Point_Line_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        if (line_slope == INFINITY)
        {
            // vertical slope, so just compare x
            return FloatsEqual(point_position.x, line_position.x);
        }
        else
        {
            // b = y1 - m * x1
            float yIntercept = line_position.y - line_slope * line_position.x;

            // y2 = m * x2 + b
            float expectedY = line_slope * point_position.x + yIntercept;

            return FloatsEqual(point_position.y, expectedY);
        }
    }

    inline bool Check2D_Point_LineSegment_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        if (point_position.x > GetMax(lineSegment_start.x, lineSegment_end.x) ||
            point_position.x < GetMin(lineSegment_start.x, lineSegment_end.x) ||
            point_position.y > GetMax(lineSegment_start.y, lineSegment_end.y) ||
            point_position.y < GetMin(lineSegment_start.y, lineSegment_end.y))
        {
            // the point is outside of the recangular range of the lineSegment
            return false;
        }

        float slopeDemoninator = lineSegment_end.x - lineSegment_start.x;
        if (slopeDemoninator == 0.0f)
        {
            // vertical slope, so just compare x
            return FloatsEqual(point_position.x, lineSegment_start.x);
        }

        float slope = (lineSegment_end.y - lineSegment_start.y) / slopeDemoninator;

        // b = y1 - m * x1
        float yIntercept = lineSegment_start.y - slope * lineSegment_start.x;

        // y2 = m * x2 + b
        float expectedY = slope * point_position.x + yIntercept;

        return FloatsEqual(point_position.y, expectedY);
    }

    inline bool Check2D_Point_Rectangle_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight)
    {
        return point_position.x <= rectangle_topRight.x &&
            point_position.x >= rectangle_bottomLeft.x &&
            point_position.y <= rectangle_topRight.y &&
            point_position.y >= rectangle_bottomLeft.y;
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
        return glm::length(pointToCircle) <= circle_radius;
    }

    inline bool Check2D_Point_Capsule_Overlap(
        const glm::vec2& point_position,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        float pointLineSegmentDistance = Get2D_Point_LineSegment_Distance(point_position, capsule_start, capsule_end);
        return pointLineSegmentDistance <= capsule_radius;
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

        return u >= 0.0f && v >= 0.0f && (u + v) < 1.0f;
    }

    // Checking Line overlap ------------------------------------------------

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
        return lineA_slope != lineB_slope || Check2D_Point_Point_Overlap(lineA_position, lineB_position);
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

        if (FloatsEqual(lineSegment_start_yInterceptOffset, 0.0f) ||
            FloatsEqual(lineSegment_end_yInterceptOffset, 0.0f))
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

        if (FloatsEqual(rectangle_bottomLeft_yInterceptOffset, 0.0f) ||
            FloatsEqual(rectangle_bottomRight_yInterceptOffset, 0.0f) ||
            FloatsEqual(rectangle_topRight_yInterceptOffset, 0.0f) ||
            FloatsEqual(rectangle_topLeft_yInterceptOffset, 0.0f))
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

        if (FloatsEqual(rectangle_bottomLeft_yInterceptOffset, 0.0f) ||
            FloatsEqual(rectangle_bottomRight_yInterceptOffset, 0.0f) ||
            FloatsEqual(rectangle_topRight_yInterceptOffset, 0.0f) ||
            FloatsEqual(rectangle_topLeft_yInterceptOffset, 0.0f))
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
        float pointLineDistance = Get2D_Point_Line_Distance(circle_position, line_position, line_slope);
        return pointLineDistance <= circle_radius;
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

        if (FloatsEqual(capsule_start_yInterceptOffset, 0.0f) ||
            FloatsEqual(capsule_end_yInterceptOffset, 0.0f))
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

        // check if line intersects capsule start
        float capsuleStartDistance = Get2D_Point_Line_Distance(capsule_start, line_position, line_slope);
        if (capsuleStartDistance <= capsule_radius)
        {
            return true;
        }

        // check if line intersects capsule end
        float capsuleEndDistance = Get2D_Point_Line_Distance(capsule_end, line_position, line_slope);
        return capsuleEndDistance <= capsule_radius;
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

        if (FloatsEqual(triangle_corner1_yInterceptOffset, 0.0f) ||
            FloatsEqual(triangle_corner2_yInterceptOffset, 0.0f) ||
            FloatsEqual(triangle_corner3_yInterceptOffset, 0.0f))
        {
            // a triangle corner is on line
            return true;
        }

        // at least one of the corners is on the opposite side of the line as
        // another corner
        return std::signbit(triangle_corner1_yInterceptOffset) != std::signbit(triangle_corner2_yInterceptOffset) ||
            std::signbit(triangle_corner1_yInterceptOffset) != std::signbit(triangle_corner3_yInterceptOffset);
    }

    // Checking LineSegment overlap -----------------------------------------

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

        float pointLineSegmentDistance1 = Get2D_Point_LineSegment_Distance(capsule_start, lineSegment_start, lineSegment_end);
        if (pointLineSegmentDistance1 <= capsule_radius)
        {
            // capsule start is near lineSegment
            return true;
        }

        float pointLineSegmentDistance2 = Get2D_Point_LineSegment_Distance(capsule_end, lineSegment_start, lineSegment_end);
        if (pointLineSegmentDistance2 <= capsule_radius)
        {
            // capsule end is near lineSegment
            return true;
        }

        float pointLineSegmentDistance3 = Get2D_Point_LineSegment_Distance(lineSegment_start, capsule_start, capsule_end);
        if (pointLineSegmentDistance3 <= capsule_radius)
        {
            // lineSegment start in near capsule
            return true;
        }

        // check if lineSegment end is near capsule
        float pointLineSegmentDistance4 = Get2D_Point_LineSegment_Distance(lineSegment_end, capsule_start, capsule_end);
        return pointLineSegmentDistance4 <= capsule_radius;
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

    // Checking Rectangle overlap -------------------------------------------

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
        return Check2D_RectangleFrame_Circle_Overlap(rectangle_bottomLeft, rectangle_topRight, circle_position, circle_radius) ||
            Check2D_Point_Rectangle_Overlap(circle_position, rectangle_bottomLeft, rectangle_topRight);
    }

    inline bool Check2D_Rectangle_Capsule_Overlap(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius)
    {
        return Check2D_RectangleFrame_Circle_Overlap(rectangle_bottomLeft, rectangle_topRight, capsule_start, capsule_radius) ||
            Check2D_RectangleFrame_Circle_Overlap(rectangle_bottomLeft, rectangle_topRight, capsule_end, capsule_radius) ||
            Check2D_LineSegment_Rectangle_Overlap(capsule_start, capsule_end, rectangle_bottomLeft, rectangle_topRight);
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

    // Checking OrientedRectangle overlap -----------------------------------

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

    // Checking Circle overlap ----------------------------------------------

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

    // Checking Capsule overlap ---------------------------------------------

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

    // Checking Triangle overlap --------------------------------------------

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

    // Helper Functions --------------------------------------------------------

    inline float Get2D_Point_Line_Distance(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        if (line_slope == INFINITY)
        {
            return std::abs(point_position.x - line_position.x);
        }

        const float& y1 = line_slope;
        float x2 = point_position.x - line_position.x;
        float y2 = point_position.y - line_position.y;
        float mod = std::sqrtf(1.0f + y1 * y1);
        return std::abs(y2 - y1 * x2) / mod;
    }

    inline float Get2D_Point_Line_Distance_Alt(
        const glm::vec2& point_position,
        const glm::vec2& line_position,
        const float& line_slope)
    {
        if (line_slope == INFINITY)
        {
            return std::abs(point_position.x - line_position.x);
        }

        float invertedSlope = -1.0f / line_slope;

        glm::vec2 perpendicularPoint =
            Get2D_Line_Line_Intersection_H(line_position, line_slope, point_position, invertedSlope);

        glm::vec2 pointToPerpendicularPoint = perpendicularPoint - point_position;

        return glm::length(pointToPerpendicularPoint);
    }

    inline float Get2D_Point_LineSegment_Distance(
        const glm::vec2& point_position,
        const glm::vec2& lineSegment_start,
        const glm::vec2& lineSegment_end)
    {
        glm::vec2 lineSegment = lineSegment_end - lineSegment_start;
        glm::vec2 endToPoint = point_position - lineSegment_end;
        glm::vec2 startToPoint = point_position - lineSegment_start;

        float dotProduct1 = glm::dot(lineSegment, endToPoint);
        float dotProduct2 = glm::dot(lineSegment, startToPoint);

        float minimumDistance = 0.0f;

        if (dotProduct1 > 0.0f)
        {
            // Nearest Point on the LineSegment is the end
            minimumDistance = glm::length(endToPoint);
        }
        else if (dotProduct2 < 0.0f)
        {
            // Nearest Point on the LineSegment is the start
            minimumDistance = glm::length(startToPoint);
        }
        else
        {
            // Nearest Point on hte LineSegment is perpendicular
            const float& x1 = lineSegment.x;
            const float& y1 = lineSegment.y;
            const float& x2 = startToPoint.x;
            const float& y2 = startToPoint.y;
            float mod = std::sqrtf(x1 * x1 + y1 * y1);
            minimumDistance = std::abs(x1 * y2 - y1 * x2) / mod;
        }

        return minimumDistance;
    }

    inline bool Check2D_Point_Rectangle_Overlap_H(
        const glm::vec2& point_position,
        const glm::vec2& rectangle_oppositeCorner1,
        const glm::vec2& rectangle_oppositeCorner2)
    {
        return point_position.x <= GetMax(rectangle_oppositeCorner1.x, rectangle_oppositeCorner2.x) &&
            point_position.x >= GetMin(rectangle_oppositeCorner1.x, rectangle_oppositeCorner2.x) &&
            point_position.y <= GetMax(rectangle_oppositeCorner1.y, rectangle_oppositeCorner2.y) &&
            point_position.y >= GetMin(rectangle_oppositeCorner1.y, rectangle_oppositeCorner2.y);
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

        return FloatsEqual(triangleArea, areaSum);
    }

    inline glm::vec2 Get2D_Line_Line_Intersection_H(
        const glm::vec2& lineA_position,
        const float& lineA_slope,
        const glm::vec2& lineB_position,
        const float& lineB_slope)
    {
        glm::vec2 result(NAN, NAN);

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
        result.x = (yInterceptB - yInterceptA) / (lineA_slope - lineB_slope);
        result.y = lineA_slope * result.x + yInterceptA;

        return result;
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

        // Check if the bottom of the rectangle crosses the circle
        float pointLineSegmentDistance1 = Get2D_Point_LineSegment_Distance(circle_position, rectangle_bottomLeft, rectangle_bottomRight);
        if (pointLineSegmentDistance1 <= circle_radius)
        {
            return true;
        }

        // Check if the right of the rectangle crosses the circle
        float pointLineSegmentDistance2 = Get2D_Point_LineSegment_Distance(circle_position, rectangle_bottomRight, rectangle_topRight);
        if (pointLineSegmentDistance2 <= circle_radius)
        {
            return true;
        }

        glm::vec2 rectangle_topLeft(rectangle_bottomLeft.x, rectangle_topRight.y);

        // Check if the top of the rectangle crosses the circle
        float pointLineSegmentDistance3 = Get2D_Point_LineSegment_Distance(circle_position, rectangle_topRight, rectangle_topLeft);
        if (pointLineSegmentDistance3 <= circle_radius)
        {
            return true;
        }

        // Check if the left of the rectangle crosses the circle
        float pointLineSegmentDistance4 = Get2D_Point_LineSegment_Distance(circle_position, rectangle_topLeft, rectangle_bottomLeft);
        return pointLineSegmentDistance4 <= circle_radius;
    }

    inline float GetMax(float a, float b)
    {
        if (a > b)
        {
            return a;
        }
        else
        {
            return b;
        }
    }

    inline float GetMin(float a, float b)
    {
        if (a < b)
        {
            return a;
        }
        else
        {
            return b;
        }
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
}
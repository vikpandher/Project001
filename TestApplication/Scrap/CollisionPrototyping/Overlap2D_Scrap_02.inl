


namespace Project001
{
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
        overlapRectangle_bottomLeft.y = std::max(rectangleA_bottomLeft.y, rectangleB_bottomLeft.y);
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
}
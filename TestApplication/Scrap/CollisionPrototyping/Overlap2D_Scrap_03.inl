


namespace Project001
{
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

    inline bool Get2D_Rectangle_OrientedRectangle_CollisionPointNormalDepth(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 rectangle_corners[4] = {
            rectangle_bottomLeft,
            glm::vec2(rectangle_topRight.x, rectangle_bottomLeft.y), // bottomRight
            rectangle_topRight,
            glm::vec2(rectangle_bottomLeft.x, rectangle_topRight.y) }; // topLeft

        glm::vec2 rectangle_axes[2];
        rectangle_axes[0].x = 1.0f;
        rectangle_axes[0].y = 0.0f;
        rectangle_axes[1].x = 0.0f;
        rectangle_axes[1].y = 1.0f;

        glm::vec2 orientedRectangle_corners[4];
        orientedRectangle_corners[0] = Rotate2DVector(-orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[1].x = orientedRectangle_halfSize.x;
        orientedRectangle_corners[1].y = -orientedRectangle_halfSize.y;
        orientedRectangle_corners[1] = Rotate2DVector(orientedRectangle_corners[1], orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[2] = Rotate2DVector(orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[3].x = -orientedRectangle_halfSize.x;
        orientedRectangle_corners[3].y = orientedRectangle_halfSize.y;
        orientedRectangle_corners[3] = Rotate2DVector(orientedRectangle_corners[3], orientedRectangle_rotation) + orientedRectangle_position;

        glm::vec2 orientedRectangle_axes[2];
        orientedRectangle_axes[0].x = 1.0f;
        orientedRectangle_axes[0].y = 0.0f;
        orientedRectangle_axes[0] = Rotate2DVector(orientedRectangle_axes[0], orientedRectangle_rotation);
        orientedRectangle_axes[1].x = 0.0f;
        orientedRectangle_axes[1].y = 1.0f;
        orientedRectangle_axes[1] = Rotate2DVector(orientedRectangle_axes[1], orientedRectangle_rotation);

        return Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H3(
            rectangle_corners, 4,
            rectangle_axes, 2,
            orientedRectangle_corners, 4,
            orientedRectangle_axes, 2,
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    inline bool Get2D_Rectangle_Capsule_CollisionPointNormalDepth(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 rectangle_corners[4] = {
            rectangle_bottomLeft,
            glm::vec2(rectangle_topRight.x, rectangle_bottomLeft.y), // bottomRight
            rectangle_topRight,
            glm::vec2(rectangle_bottomLeft.x, rectangle_topRight.y) }; // topLeft

        bool overlap = Get2D_Capsule_ConvexPolygon_CollisionPointNormalDepth(
            capsule_start,
            capsule_end,
            capsule_radius,
            rectangle_corners, 4,
            collisionPoint,
            collisionNormal,
            collisionDepth);
        if (!overlap)
        {
            return false;
        }

        collisionNormal *= -1.0f;

        return true;
    }

    inline bool Get2D_Rectangle_Triangle_CollisionPointNormalDepth(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 rectangle_corners[4] = {
            rectangle_bottomLeft,
            glm::vec2(rectangle_topRight.x, rectangle_bottomLeft.y), // bottomRight
            rectangle_topRight,
            glm::vec2(rectangle_bottomLeft.x, rectangle_topRight.y) }; // topLeft

        glm::vec2 rectangle_axes[2];
        rectangle_axes[0].x = 1.0f;
        rectangle_axes[0].y = 0.0f;
        rectangle_axes[1].x = 0.0f;
        rectangle_axes[1].y = 1.0f;

        glm::vec2 triangle_corners[3] = { triangle_corner1, triangle_corner2, triangle_corner3 };

        return Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H2(
            rectangle_corners, 4,
            rectangle_axes, 2,
            triangle_corners, 3,
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    inline bool Get2D_Rectangle_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2& rectangle_bottomLeft,
        const glm::vec2& rectangle_topRight,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        if (convexPolygon_cornerCount < 3)
        {
            return false;
        }

        glm::vec2 rectangle_corners[4] = {
            rectangle_bottomLeft,
            glm::vec2(rectangle_topRight.x, rectangle_bottomLeft.y), // bottomRight
            rectangle_topRight,
            glm::vec2(rectangle_bottomLeft.x, rectangle_topRight.y) }; // topLeft

        glm::vec2 rectangle_axes[2];
        rectangle_axes[0].x = 1.0f;
        rectangle_axes[0].y = 0.0f;
        rectangle_axes[1].x = 0.0f;
        rectangle_axes[1].y = 1.0f;

        return Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H2(
            rectangle_corners, 4,
            rectangle_axes, 2,
            convexPolygon_corners, convexPolygon_cornerCount,
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    // Oriented Rectangle Collision Point And Normal And Depth Functions -------

    inline bool Get2D_OrientedRectangle_OrientedRectangle_CollisionPointNormalDepth(
        const glm::vec2& orientedRectangleA_halfSize,
        const glm::vec2& orientedRectangleA_position,
        const float& orientedRectangleA_rotation,
        const glm::vec2& orientedRectangleB_halfSize,
        const glm::vec2& orientedRectangleB_position,
        const float& orientedRectangleB_rotation,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 orientedRectangleA_corners[4];
        orientedRectangleA_corners[0] = Rotate2DVector(-orientedRectangleA_halfSize, orientedRectangleA_rotation) + orientedRectangleA_position;
        orientedRectangleA_corners[1].x = orientedRectangleA_halfSize.x;
        orientedRectangleA_corners[1].y = -orientedRectangleA_halfSize.y;
        orientedRectangleA_corners[1] = Rotate2DVector(orientedRectangleA_corners[1], orientedRectangleA_rotation) + orientedRectangleA_position;
        orientedRectangleA_corners[2] = Rotate2DVector(orientedRectangleA_halfSize, orientedRectangleA_rotation) + orientedRectangleA_position;
        orientedRectangleA_corners[3].x = -orientedRectangleA_halfSize.x;
        orientedRectangleA_corners[3].y = orientedRectangleA_halfSize.y;
        orientedRectangleA_corners[3] = Rotate2DVector(orientedRectangleA_corners[3], orientedRectangleA_rotation) + orientedRectangleA_position;

        glm::vec2 orientedRectangleA_axes[2];
        orientedRectangleA_axes[0].x = 1.0f;
        orientedRectangleA_axes[0].y = 0.0f;
        orientedRectangleA_axes[0] = Rotate2DVector(orientedRectangleA_axes[0], orientedRectangleA_rotation);
        orientedRectangleA_axes[1].x = 0.0f;
        orientedRectangleA_axes[1].y = 1.0f;
        orientedRectangleA_axes[1] = Rotate2DVector(orientedRectangleA_axes[1], orientedRectangleA_rotation);

        glm::vec2 orientedRectangleB_corners[4];
        orientedRectangleB_corners[0] = Rotate2DVector(-orientedRectangleB_halfSize, orientedRectangleB_rotation) + orientedRectangleB_position;
        orientedRectangleB_corners[1].x = orientedRectangleB_halfSize.x;
        orientedRectangleB_corners[1].y = -orientedRectangleB_halfSize.y;
        orientedRectangleB_corners[1] = Rotate2DVector(orientedRectangleB_corners[1], orientedRectangleB_rotation) + orientedRectangleB_position;
        orientedRectangleB_corners[2] = Rotate2DVector(orientedRectangleB_halfSize, orientedRectangleB_rotation) + orientedRectangleB_position;
        orientedRectangleB_corners[3].x = -orientedRectangleB_halfSize.x;
        orientedRectangleB_corners[3].y = orientedRectangleB_halfSize.y;
        orientedRectangleB_corners[3] = Rotate2DVector(orientedRectangleB_corners[3], orientedRectangleB_rotation) + orientedRectangleB_position;

        glm::vec2 orientedRectangleB_axes[2];
        orientedRectangleB_axes[0].x = 1.0f;
        orientedRectangleB_axes[0].y = 0.0f;
        orientedRectangleB_axes[0] = Rotate2DVector(orientedRectangleB_axes[0], orientedRectangleB_rotation);
        orientedRectangleB_axes[1].x = 0.0f;
        orientedRectangleB_axes[1].y = 1.0f;
        orientedRectangleB_axes[1] = Rotate2DVector(orientedRectangleB_axes[1], orientedRectangleB_rotation);

        return Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H3(
            orientedRectangleA_corners, 4,
            orientedRectangleA_axes, 2,
            orientedRectangleB_corners, 4,
            orientedRectangleB_axes, 2,
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    inline bool Get2D_OrientedRectangle_Capsule_CollisionPointNormalDepth(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 orientedRectangle_corners[4];
        orientedRectangle_corners[0] = Rotate2DVector(-orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[1].x = orientedRectangle_halfSize.x;
        orientedRectangle_corners[1].y = -orientedRectangle_halfSize.y;
        orientedRectangle_corners[1] = Rotate2DVector(orientedRectangle_corners[1], orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[2] = Rotate2DVector(orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[3].x = -orientedRectangle_halfSize.x;
        orientedRectangle_corners[3].y = orientedRectangle_halfSize.y;
        orientedRectangle_corners[3] = Rotate2DVector(orientedRectangle_corners[3], orientedRectangle_rotation) + orientedRectangle_position;

        bool overlap = Get2D_Capsule_ConvexPolygon_CollisionPointNormalDepth(
            capsule_start,
            capsule_end,
            capsule_radius,
            orientedRectangle_corners, 4,
            collisionPoint,
            collisionNormal,
            collisionDepth);
        if (!overlap)
        {
            return false;
        }

        collisionNormal *= -1.0f;

        return true;
    }

    inline bool Get2D_OrientedRectangle_Triangle_CollisionPointNormalDepth(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 orientedRectangle_corners[4];
        orientedRectangle_corners[0] = Rotate2DVector(-orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[1].x = orientedRectangle_halfSize.x;
        orientedRectangle_corners[1].y = -orientedRectangle_halfSize.y;
        orientedRectangle_corners[1] = Rotate2DVector(orientedRectangle_corners[1], orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[2] = Rotate2DVector(orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[3].x = -orientedRectangle_halfSize.x;
        orientedRectangle_corners[3].y = orientedRectangle_halfSize.y;
        orientedRectangle_corners[3] = Rotate2DVector(orientedRectangle_corners[3], orientedRectangle_rotation) + orientedRectangle_position;

        glm::vec2 orientedRectangle_axes[2];
        orientedRectangle_axes[0].x = 1.0f;
        orientedRectangle_axes[0].y = 0.0f;
        orientedRectangle_axes[0] = Rotate2DVector(orientedRectangle_axes[0], orientedRectangle_rotation);
        orientedRectangle_axes[1].x = 0.0f;
        orientedRectangle_axes[1].y = 1.0f;
        orientedRectangle_axes[1] = Rotate2DVector(orientedRectangle_axes[1], orientedRectangle_rotation);

        glm::vec2 triangle_corners[3] = { triangle_corner1, triangle_corner2, triangle_corner3 };

        return Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H2(
            orientedRectangle_corners, 4,
            orientedRectangle_axes, 2,
            triangle_corners, 3,
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    inline bool Get2D_OrientedRectangle_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2& orientedRectangle_halfSize,
        const glm::vec2& orientedRectangle_position,
        const float& orientedRectangle_rotation,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        if (convexPolygon_cornerCount < 3)
        {
            return false;
        }

        glm::vec2 orientedRectangle_corners[4];
        orientedRectangle_corners[0] = Rotate2DVector(-orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[1].x = orientedRectangle_halfSize.x;
        orientedRectangle_corners[1].y = -orientedRectangle_halfSize.y;
        orientedRectangle_corners[1] = Rotate2DVector(orientedRectangle_corners[1], orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[2] = Rotate2DVector(orientedRectangle_halfSize, orientedRectangle_rotation) + orientedRectangle_position;
        orientedRectangle_corners[3].x = -orientedRectangle_halfSize.x;
        orientedRectangle_corners[3].y = orientedRectangle_halfSize.y;
        orientedRectangle_corners[3] = Rotate2DVector(orientedRectangle_corners[3], orientedRectangle_rotation) + orientedRectangle_position;

        glm::vec2 orientedRectangle_axes[2];
        orientedRectangle_axes[0].x = 1.0f;
        orientedRectangle_axes[0].y = 0.0f;
        orientedRectangle_axes[0] = Rotate2DVector(orientedRectangle_axes[0], orientedRectangle_rotation);
        orientedRectangle_axes[1].x = 0.0f;
        orientedRectangle_axes[1].y = 1.0f;
        orientedRectangle_axes[1] = Rotate2DVector(orientedRectangle_axes[1], orientedRectangle_rotation);

        return Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H2(
            orientedRectangle_corners, 4,
            orientedRectangle_axes, 2,
            convexPolygon_corners, convexPolygon_cornerCount,
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    // Capsule Collision Point And Normal And Depth Functions ------------------

    inline bool Get2D_Capsule_Capsule_CollisionPointNormalDepth(
        const glm::vec2& capsuleA_start,
        const glm::vec2& capsuleA_end,
        const float& capsuleA_radius,
        const glm::vec2& capsuleB_start,
        const glm::vec2& capsuleB_end,
        const float& capsuleB_radius,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 axes[3];

        axes[0] = capsuleA_end - capsuleA_start;
        axes[0] = glm::vec2(axes[0].y, -1.0f * axes[0].x);
        axes[0] = glm::normalize(axes[0]);

        axes[1] = capsuleB_end - capsuleB_start;
        axes[1] = glm::vec2(axes[1].y, -1.0f * axes[1].x);
        axes[1] = glm::normalize(axes[1]);

        axes[2] = capsuleB_start - capsuleA_start;
        float minEndpointDistanceSquared = glm::dot(axes[2], axes[2]);

        glm::vec2 startA_to_endB = capsuleB_end - capsuleA_start;
        float startA_endB_distanceSquared = glm::dot(startA_to_endB, startA_to_endB);
        if (startA_endB_distanceSquared < minEndpointDistanceSquared)
        {
            minEndpointDistanceSquared = startA_endB_distanceSquared;
            axes[2] = startA_to_endB;
        }

        glm::vec2 endA_to_startB = capsuleB_start - capsuleA_end;
        float endA_startB_distanceSquared = glm::dot(endA_to_startB, endA_to_startB);
        if (endA_startB_distanceSquared < minEndpointDistanceSquared)
        {
            minEndpointDistanceSquared = endA_startB_distanceSquared;
            axes[2] = endA_to_startB;
        }

        glm::vec2 endA_to_endB = capsuleB_end - capsuleA_end;
        float endA_endB_distanceSquared = glm::dot(endA_to_endB, endA_to_endB);
        if (endA_endB_distanceSquared < minEndpointDistanceSquared)
        {
            minEndpointDistanceSquared = endA_endB_distanceSquared;
            axes[2] = endA_to_endB;
        }

        axes[2] = glm::normalize(axes[2]);

        glm::vec2 final_collisionNormal;
        float final_collisionDepth = std::numeric_limits<float>::infinity();

        for (size_t i = 0; i < 3; ++i)
        {
            const glm::vec2& axisOfSeparation = axes[i];

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            ProjectCapsuleOntoAxis(axisOfSeparation, capsuleA_start, capsuleA_end, capsuleA_radius, projectionA_max, projectionA_min);

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            ProjectCapsuleOntoAxis(axisOfSeparation, capsuleB_start, capsuleB_end, capsuleB_radius, projectionB_max, projectionB_min);

            if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
            {
                // Projections don't overlap on axis of separation
                return false;
            }

            float aMin_to_bMax = projectionB_max - projectionA_min;
            float bMin_to_aMax = projectionA_max - projectionB_min;

            float axisDepth = std::min(bMin_to_aMax, aMin_to_bMax);

            if (axisDepth < final_collisionDepth)
            {
                final_collisionDepth = axisDepth;
                final_collisionNormal = axisOfSeparation;
            }
        }

        collisionNormal = glm::normalize(final_collisionNormal);
        collisionDepth = final_collisionDepth;

        glm::vec2 capsuleA_closestPoint = capsuleA_start;
        glm::vec2 capsuleB_closestPoint;
        float closestPoint_distanceSquared = Get2D_Point_LineSegment_ClosestPointAndDistanceSquared(capsuleA_start, capsuleB_start, capsuleB_end, capsuleB_closestPoint);

        glm::vec2 new_capsuleB_closestPoint;
        float endA_closestPoint_distanceSquared = Get2D_Point_LineSegment_ClosestPointAndDistanceSquared(capsuleA_end, capsuleB_start, capsuleB_end, new_capsuleB_closestPoint);
        if (endA_closestPoint_distanceSquared < closestPoint_distanceSquared)
        {
            closestPoint_distanceSquared = endA_closestPoint_distanceSquared;
            capsuleA_closestPoint = capsuleA_end;
            capsuleB_closestPoint = new_capsuleB_closestPoint;
        }

        glm::vec2 new_capsuleA_closestPoint;
        float startB_closestPoint_distanceSquared = Get2D_Point_LineSegment_ClosestPointAndDistanceSquared(capsuleB_start, capsuleA_start, capsuleA_end, new_capsuleA_closestPoint);
        if (startB_closestPoint_distanceSquared < closestPoint_distanceSquared)
        {
            closestPoint_distanceSquared = startB_closestPoint_distanceSquared;
            capsuleA_closestPoint = new_capsuleA_closestPoint;
            capsuleB_closestPoint = capsuleB_start;
        }

        float endB_closestPoint_distanceSquared = Get2D_Point_LineSegment_ClosestPointAndDistanceSquared(capsuleB_end, capsuleA_start, capsuleA_end, new_capsuleA_closestPoint);
        if (endB_closestPoint_distanceSquared < closestPoint_distanceSquared)
        {
            closestPoint_distanceSquared = endB_closestPoint_distanceSquared;
            capsuleA_closestPoint = new_capsuleA_closestPoint;
            capsuleB_closestPoint = capsuleB_end;
        }

        glm::vec2 a_to_b = capsuleB_closestPoint - capsuleA_closestPoint;
        if (glm::dot(collisionNormal, a_to_b) < 0)
        {
            collisionNormal *= -1.0f;
        }

        if (Check2D_LineSegment_LineSegment_Overlap(capsuleA_start, capsuleA_end, capsuleB_start, capsuleB_end))
        {
            collisionNormal *= -1.0f;
        }

        float a_to_b_distance = glm::length(a_to_b);
        float collisionPointDistance = (a_to_b_distance - capsuleB_radius + capsuleA_radius) * 0.5f;
        if (collisionPointDistance > capsuleA_radius)
        {
            collisionPointDistance = capsuleA_radius;
        }

        collisionPoint = capsuleA_closestPoint + collisionNormal * collisionPointDistance;

        return true;
    }

    inline bool Get2D_Capsule_Triangle_CollisionPointNormalDepth(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 convexPolygon_array[3] = { triangle_corner1, triangle_corner2, triangle_corner3 };
        return Get2D_Capsule_ConvexPolygon_CollisionPointNormalDepth(
            capsule_start,
            capsule_end,
            capsule_radius,
            convexPolygon_array, 3,
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    inline bool Get2D_Capsule_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2& capsule_start,
        const glm::vec2& capsule_end,
        const float& capsule_radius,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        if (convexPolygon_cornerCount < 3)
        {
            return false;
        }

        glm::vec2 axes[2];

        axes[0] = capsule_end - capsule_start;
        axes[0] = glm::vec2(axes[0].y, -1.0f * axes[0].x);
        axes[0] = glm::normalize(axes[0]);

        glm::vec2 closestPointToStart_position;
        float closestPointToStart_distance = Get2D_Point_PolygonEdge_ClosestPointAndDistanceSquared(capsule_start, convexPolygon_corners, convexPolygon_cornerCount, closestPointToStart_position);

        glm::vec2 closestPointToEnd_position;
        float closestPointToEnd_distance = Get2D_Point_PolygonEdge_ClosestPointAndDistanceSquared(capsule_end, convexPolygon_corners, convexPolygon_cornerCount, closestPointToEnd_position);

        glm::vec2 closestPoint_position;
        float closestPoint_distanceSquared;
        if (closestPointToEnd_distance < closestPointToStart_distance)
        {
            axes[1] = closestPointToEnd_position - capsule_end;
            axes[1] = glm::normalize(axes[1]);

            closestPoint_distanceSquared = closestPointToEnd_distance;
            closestPoint_position = capsule_end;
        }
        else
        {
            axes[1] = closestPointToStart_position - capsule_start;
            axes[1] = glm::normalize(axes[1]);

            closestPoint_distanceSquared = closestPointToStart_distance;
            closestPoint_position = capsule_start;
        }

        glm::vec2 final_collisionNormal;
        float final_collisionDepth = std::numeric_limits<float>::infinity();

        for (size_t i = 0; i < 2; ++i)
        {
            const glm::vec2& axisOfSeparation = axes[i];

            float projectionA_max = -std::numeric_limits<float>::infinity();
            float projectionA_min = std::numeric_limits<float>::infinity();
            ProjectPolygonOntoAxis(axisOfSeparation, convexPolygon_corners, convexPolygon_cornerCount, projectionA_max, projectionA_min);

            float projectionB_max = -std::numeric_limits<float>::infinity();
            float projectionB_min = std::numeric_limits<float>::infinity();
            ProjectCapsuleOntoAxis(axisOfSeparation, capsule_start, capsule_end, capsule_radius, projectionB_max, projectionB_min);

            if (!(projectionA_min < projectionB_max && projectionA_max > projectionB_min))
            {
                // Projections don't overlap on axis of separation
                return false;
            }

            float aMin_to_bMax = projectionB_max - projectionA_min;
            float bMin_to_aMax = projectionA_max - projectionB_min;

            float axisDepth = std::min(bMin_to_aMax, aMin_to_bMax);

            if (axisDepth < final_collisionDepth)
            {
                final_collisionDepth = axisDepth;
                final_collisionNormal = axisOfSeparation;
            }
        }

        collisionNormal = glm::normalize(final_collisionNormal);
        collisionDepth = final_collisionDepth;

        for (size_t i = 0; i < convexPolygon_cornerCount; ++i)
        {
            glm::vec2 currentClosestPoint_position;
            float currentClosestPoint_distanceSquared = Get2D_Point_LineSegment_ClosestPointAndDistanceSquared(convexPolygon_corners[i], capsule_start, capsule_end, currentClosestPoint_position);

            if (currentClosestPoint_distanceSquared < closestPoint_distanceSquared)
            {
                closestPoint_position = currentClosestPoint_position;
                closestPoint_distanceSquared = currentClosestPoint_distanceSquared;
            }
        }

        glm::vec2 polygon_centroid = GetPolygonCentroid(convexPolygon_corners, convexPolygon_cornerCount);

        glm::vec2 capsuleCenter_to_polygonCenter = polygon_centroid - closestPoint_position;

        if (glm::dot(collisionNormal, capsuleCenter_to_polygonCenter) < 0)
        {
            collisionNormal *= -1.0f;
        }

        float collisionOffset = capsule_radius - collisionDepth * 0.5f;
        if (collisionOffset < 0.0f)
        {
            collisionOffset = 0.0f;
        }

        collisionPoint = closestPoint_position + collisionNormal * collisionOffset;

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
        glm::vec2 triangleA_corners[3] = { triangleA_corner1, triangleA_corner2, triangleA_corner3 };
        glm::vec2 triangleB_corners[3] = { triangleB_corner1, triangleB_corner2, triangleB_corner3 };

        return Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H1(
            triangleA_corners, 3,
            triangleB_corners, 3,
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    inline bool Get2D_Triangle_ConvexPolygon_CollisionPointNormalDepth(
        const glm::vec2& triangle_corner1,
        const glm::vec2& triangle_corner2,
        const glm::vec2& triangle_corner3,
        const glm::vec2* const& convexPolygon_corners,
        const size_t& convexPolygon_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        if (convexPolygon_cornerCount < 3)
        {
            return false;
        }

        glm::vec2 triangle_corners[3] = { triangle_corner1, triangle_corner2, triangle_corner3 };

        return Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H1(
            triangle_corners, 3,
            convexPolygon_corners, convexPolygon_cornerCount,
            collisionPoint,
            collisionNormal,
            collisionDepth);
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

        return Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H1(
            convexPolygonA_corners, convexPolygonA_cornerCount,
            convexPolygonB_corners, convexPolygonB_cornerCount,
            collisionPoint,
            collisionNormal,
            collisionDepth);
    }

    // -------------------------------------------------------------------------

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

    inline bool Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H1(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
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

        ConvexPolygon_ConvexPolygon_FixNormalAndSelectCollisionPoint_H(
            convexPolygonA_corners,
            convexPolygonA_cornerCount,
            convexPolygonB_corners,
            convexPolygonB_cornerCount,
            projection_max_is_from_polygonA,
            projection_min_is_from_polygonA,
            projection_max_index,
            projection_min_index,
            collisionPoint,
            collisionNormal);

        return true;
    }

    inline bool Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H2(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolyhonA_axes,
        const size_t& convexPolygonA_axisCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 final_collisionNormal;
        float final_collisionDepth = std::numeric_limits<float>::infinity();

        bool projection_max_is_from_polygonA;
        bool projection_min_is_from_polygonA;
        size_t projection_max_index;
        size_t projection_min_index;

        for (size_t i = 0; i < convexPolygonA_axisCount; ++i)
        {
            const glm::vec2& axisOfSeparation = convexPolyhonA_axes[i];

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

        ConvexPolygon_ConvexPolygon_FixNormalAndSelectCollisionPoint_H(
            convexPolygonA_corners,
            convexPolygonA_cornerCount,
            convexPolygonB_corners,
            convexPolygonB_cornerCount,
            projection_max_is_from_polygonA,
            projection_min_is_from_polygonA,
            projection_max_index,
            projection_min_index,
            collisionPoint,
            collisionNormal);

        return true;
    }

    inline bool Get2D_ConvexPolygon_ConvexPolygon_CollisionPointNormalDepth_H3(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolyhonA_axes,
        const size_t& convexPolygonA_axisCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        const glm::vec2* const& convexPolyhonB_axes,
        const size_t& convexPolygonB_axisCount,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal,
        float& collisionDepth)
    {
        glm::vec2 final_collisionNormal;
        float final_collisionDepth = std::numeric_limits<float>::infinity();

        bool projection_max_is_from_polygonA;
        bool projection_min_is_from_polygonA;
        size_t projection_max_index;
        size_t projection_min_index;

        for (size_t i = 0; i < convexPolygonA_axisCount; ++i)
        {
            const glm::vec2& axisOfSeparation = convexPolyhonA_axes[i];

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
        }

        for (size_t i = 0; i < convexPolygonB_axisCount; ++i)
        {
            const glm::vec2& axisOfSeparation = convexPolyhonB_axes[i];

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
        }

        collisionNormal = glm::normalize(final_collisionNormal);
        collisionDepth = final_collisionDepth;

        ConvexPolygon_ConvexPolygon_FixNormalAndSelectCollisionPoint_H(
            convexPolygonA_corners,
            convexPolygonA_cornerCount,
            convexPolygonB_corners,
            convexPolygonB_cornerCount,
            projection_max_is_from_polygonA,
            projection_min_is_from_polygonA,
            projection_max_index,
            projection_min_index,
            collisionPoint,
            collisionNormal);

        return true;
    }

    inline void ConvexPolygon_ConvexPolygon_FixNormalAndSelectCollisionPoint_H(
        const glm::vec2* const& convexPolygonA_corners,
        const size_t& convexPolygonA_cornerCount,
        const glm::vec2* const& convexPolygonB_corners,
        const size_t& convexPolygonB_cornerCount,
        const bool& projection_max_is_from_polygonA,
        const bool& projection_min_is_from_polygonA,
        const size_t& projection_max_index,
        const size_t& projection_min_index,
        glm::vec2& collisionPoint,
        glm::vec2& collisionNormal)
    {
        glm::vec2 final_projection_max_point;
        if (projection_max_is_from_polygonA)
        {
            final_projection_max_point = convexPolygonA_corners[projection_max_index];
        }
        else
        {
            final_projection_max_point = convexPolygonB_corners[projection_max_index];
        }

        glm::vec2 final_projection_min_point;
        if (projection_min_is_from_polygonA)
        {
            final_projection_min_point = convexPolygonA_corners[projection_min_index];
        }
        else
        {
            final_projection_min_point = convexPolygonB_corners[projection_min_index];
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

            glm::vec2 polygon_centeroid;
            if (projection_max_is_from_polygonA)
            {
                polygon_centeroid = GetPolygonCentroid(convexPolygonA_corners, convexPolygonA_cornerCount);
            }
            else
            {
                polygon_centeroid = GetPolygonCentroid(convexPolygonB_corners, convexPolygonB_cornerCount);
            }

            glm::vec2 collision_to_centeroid = polygon_centeroid - collisionPoint;

            if (projection_max_is_from_polygonA && glm::dot(collisionNormal, collision_to_centeroid) > 0)
            {
                collisionNormal *= -1.0f;
            }
            else if (!projection_max_is_from_polygonA && glm::dot(collisionNormal, collision_to_centeroid) < 0)
            {
                collisionNormal *= -1.0f;
            }

            return;
        }

        // bool final_projection_min_point_inside;
        // if (projection_min_is_from_polygonA)
        // {
        //     final_projection_min_point_inside =
        //         Check2D_Point_ConvexPolygon_Overlap(final_projection_min_point, convexPolygonB_corners, convexPolygonB_cornerCount);
        // }
        // else
        // {
        //     final_projection_min_point_inside =
        //         Check2D_Point_ConvexPolygon_Overlap(final_projection_min_point, convexPolygonA_corners, convexPolygonA_cornerCount);
        // }

        collisionPoint = final_projection_min_point;

        glm::vec2 polygon_centeroid;
        if (projection_min_is_from_polygonA)
        {
            polygon_centeroid = GetPolygonCentroid(convexPolygonA_corners, convexPolygonA_cornerCount);
        }
        else
        {
            polygon_centeroid = GetPolygonCentroid(convexPolygonB_corners, convexPolygonB_cornerCount);
        }

        glm::vec2 collision_to_centeroid = polygon_centeroid - collisionPoint;

        if (projection_min_is_from_polygonA && glm::dot(collisionNormal, collision_to_centeroid) > 0)
        {
            collisionNormal *= -1.0f;
        }
        else if (!projection_min_is_from_polygonA && glm::dot(collisionNormal, collision_to_centeroid) < 0)
        {
            collisionNormal *= -1.0f;
        }
    }
}
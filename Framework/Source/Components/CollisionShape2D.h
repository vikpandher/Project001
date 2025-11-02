// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-12-24

#pragma once

#include "glm/glm.hpp"

#include <vector>



namespace Project001
{
    enum class CollisionShape2D_Type
    {
        COLLISION_SHAPE_2D_TYPE_NONE = 0,

        COLLISION_SHAPE_2D_TYPE_POINT,
        COLLISION_SHAPE_2D_TYPE_LINE,
        COLLISION_SHAPE_2D_TYPE_RAY,
        COLLISION_SHAPE_2D_TYPE_LINE_SEGMENT,
        COLLISION_SHAPE_2D_TYPE_RECTANGLE,
        COLLISION_SHAPE_2D_TYPE_ORIENTED_RECTANGLE,
        COLLISION_SHAPE_2D_TYPE_CIRCLE,
        COLLISION_SHAPE_2D_TYPE_CAPSULE,
        COLLISION_SHAPE_2D_TYPE_TRIANGLE,
        COLLISION_SHAPE_2D_TYPE_POLYGON,
        COLLISION_SHAPE_2D_TYPE_CONVEX_POLYGON
    };

    // -------------------------------------------------------------------------

    namespace CollisionShape2D
    {
        static const glm::vec2 s_defaultPosition(0.0f, 0.0f);
        static const glm::vec2 s_defaultDirection(1.0f, 0.0f);
    };

    struct CollisionPoint2D
    {
        CollisionPoint2D(const glm::vec2& position = CollisionShape2D::s_defaultPosition, unsigned int tag = 0, bool enabled = true);

        glm::vec2 position;

        unsigned int tag;
        bool enabled;
    };

    inline CollisionPoint2D::CollisionPoint2D(const glm::vec2& position, unsigned int tag, bool enabled)
        : position(position)
        , tag(tag)
        , enabled(enabled)
    {}

    // -------------------------------------------------------------------------

    struct CollisionLine2D
    {
        CollisionLine2D(const glm::vec2& position = CollisionShape2D::s_defaultPosition, const float& slope = 0.0f, unsigned int tag = 0, bool enabled = true);

        glm::vec2 position;
        float slope;

        unsigned int tag;
        bool enabled;
    };

    inline CollisionLine2D::CollisionLine2D(const glm::vec2& position, const float& slope, unsigned int tag, bool enabled)
        : position(position)
        , slope(slope)
        , tag(tag)
        , enabled(enabled)
    {}

    // -------------------------------------------------------------------------

    struct CollisionRay2D
    {
        CollisionRay2D(const glm::vec2& position = CollisionShape2D::s_defaultPosition, const glm::vec2& direction = CollisionShape2D::s_defaultDirection, unsigned int tag = 0, bool enabled = true);

        glm::vec2 position;
        glm::vec2 direction; // keep this normalized (a unit vector)

        unsigned int tag;
        bool enabled;
    };

    inline CollisionRay2D::CollisionRay2D(const glm::vec2& position, const glm::vec2& direction, unsigned int tag, bool enabled)
        : position(position)
        , direction(direction)
        , tag(tag)
        , enabled(enabled)
    {}

    // -------------------------------------------------------------------------

    struct CollisionLineSegment2D
    {
        CollisionLineSegment2D(const glm::vec2& start = CollisionShape2D::s_defaultPosition, const glm::vec2& end = CollisionShape2D::s_defaultPosition, unsigned int tag = 0, bool enabled = true);

        glm::vec2 start;
        glm::vec2 end;

        unsigned int tag;
        bool enabled;
    };

    inline CollisionLineSegment2D::CollisionLineSegment2D(const glm::vec2& start, const glm::vec2& end, unsigned int tag, bool enabled)
        : start(start)
        , end(end)
        , tag(tag)
        , enabled(enabled)
    {}

    // -------------------------------------------------------------------------

    struct CollisionRectangle2D
    {
        CollisionRectangle2D(const glm::vec2& bottomLeft = CollisionShape2D::s_defaultPosition, const glm::vec2& topRight = CollisionShape2D::s_defaultPosition, unsigned int tag = 0, bool enabled = true);

        glm::vec2 bottomLeft;
        glm::vec2 topRight;

        unsigned int tag;
        bool enabled;
    };

    inline CollisionRectangle2D::CollisionRectangle2D(const glm::vec2& bottomLeft, const glm::vec2& topRight, unsigned int tag, bool enabled)
        : bottomLeft(bottomLeft)
        , topRight(topRight)
        , tag(tag)
        , enabled(enabled)
    {}

    // -------------------------------------------------------------------------

    struct CollisionOrientedRectangle2D
    {
        CollisionOrientedRectangle2D(const glm::vec2& halfSize = CollisionShape2D::s_defaultPosition, const glm::vec2& position = CollisionShape2D::s_defaultPosition, const float& rotation = 0.0f, unsigned int tag = 0, bool enabled = true);

        glm::vec2 halfSize;
        glm::vec2 position;
        float rotation;

        unsigned int tag;
        bool enabled;
    };

    inline CollisionOrientedRectangle2D::CollisionOrientedRectangle2D(const glm::vec2& halfSize, const glm::vec2& position, const float& rotation, unsigned int tag, bool enabled)
        : halfSize(halfSize)
        , position(position)
        , rotation(rotation)
        , tag(tag)
        , enabled(enabled)
    {}

    // -------------------------------------------------------------------------

    struct CollisionCircle2D
    {
        CollisionCircle2D(const glm::vec2& position = CollisionShape2D::s_defaultPosition, const float& radius = 0.0f, unsigned int tag = 0, bool enabled = true);

        glm::vec2 position;
        float radius;

        unsigned int tag;
        bool enabled;
    };

    inline CollisionCircle2D::CollisionCircle2D(const glm::vec2& position, const float& radius, unsigned int tag, bool enabled)
        : position(position)
        , radius(radius)
        , tag(tag)
        , enabled(enabled)
    {}

    // -------------------------------------------------------------------------

    struct CollisionCapsule2D
    {
        CollisionCapsule2D(const glm::vec2& start = CollisionShape2D::s_defaultPosition, const glm::vec2& end = CollisionShape2D::s_defaultPosition, const float& radius = 0.0f, unsigned int tag = 0, bool enabled = true);

        glm::vec2 start;
        glm::vec2 end;
        float radius;

        unsigned int tag;
        bool enabled;
    };

    inline CollisionCapsule2D::CollisionCapsule2D(const glm::vec2& start, const glm::vec2& end, const float& radius, unsigned int tag, bool enabled)
        : start(start)
        , end(end)
        , radius(radius)
        , tag(tag)
        , enabled(enabled)
    {}

    // -------------------------------------------------------------------------

    struct CollisionTriangle2D
    {
        CollisionTriangle2D(const glm::vec2& corner1 = CollisionShape2D::s_defaultPosition, const glm::vec2& corner2 = CollisionShape2D::s_defaultPosition, const glm::vec2& corner3 = CollisionShape2D::s_defaultPosition, unsigned int tag = 0, bool enabled = true);

        glm::vec2 corner1;
        glm::vec2 corner2;
        glm::vec2 corner3;

        unsigned int tag;
        bool enabled;
    };

    inline CollisionTriangle2D::CollisionTriangle2D(const glm::vec2& corner1, const glm::vec2& corner2, const glm::vec2& corner3, unsigned int tag, bool enabled)
        : corner1(corner1)
        , corner2(corner2)
        , corner3(corner3)
        , tag(tag)
        , enabled(enabled)
    {}

    // -------------------------------------------------------------------------

    struct CollisionPolygon2D
    {
        CollisionPolygon2D(const std::vector<glm::vec2>& corners = std::vector<glm::vec2>(), unsigned int tag = 0, bool enabled = true);

        std::vector<glm::vec2> corners; // corners go around counterclockwise

        unsigned int tag;
        bool enabled;
    };

    inline CollisionPolygon2D::CollisionPolygon2D(const std::vector<glm::vec2>& corners, unsigned int tag, bool enabled)
        : corners(corners)
        , tag(tag)
        , enabled(enabled)
    {}

    // -------------------------------------------------------------------------

    struct CollisionConvexPolygon2D
    {
        CollisionConvexPolygon2D(const std::vector<glm::vec2>& corners = std::vector<glm::vec2>(), unsigned int tag = 0, bool enabled = true);

        std::vector<glm::vec2> corners; // corners go around counterclockwise

        unsigned int tag;
        bool enabled;
    };

    inline CollisionConvexPolygon2D::CollisionConvexPolygon2D(const std::vector<glm::vec2>& corners, unsigned int tag, bool enabled)
        : corners(corners)
        , tag(tag)
        , enabled(enabled)
    {}
}

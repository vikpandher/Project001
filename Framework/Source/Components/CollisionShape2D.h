// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-03

#pragma once

#include "glm/glm.hpp"

#include <vector>



namespace Project001
{
    namespace CollisionShape2D
    {
        // PHYSICS_TYPE_NOT_SET
        // ---------------------------------------------------------------------
        // * will use physics type of the collision body containing this
        //   collision shape
        //
        // PHYSICS_TYPE_RIGID_BODY
        // ---------------------------------------------------------------------
        // * overlap is detected and resolved
        // * the collision point, normal, and depth are collected
        // * the collision is resolved
        //   (position, veloicty, and angular velocity are updated)
        // 
        // PHYSICS_TYPE_SENSOR
        // ---------------------------------------------------------------------
        // * overlap is detected but NOT resovled
        // * the collision point, normal, and depth are collected
        // * the collision is NOT resolved
        //   (position, veloicty, and angular velocity are not updated)
        // 
        // PHYSICS_TYPE_SIMPLE_SENSOR
        // ---------------------------------------------------------------------
        // * overlap is detected but NOT resolved
        // * the collision point, normal, and depth are NOT collected
        //   (they remain NAN, only collecting entityIds and tags)
        // 
        // * CollisionPoint2D, CollisionLine2D, CollisionRay2D,
        //   CollisionLineSegment2D, and CollisionPolygon2D can only be this
        //   physics type
        enum class PhysicsType
        {
            PHYSICS_TYPE_NONE = 0,

            PHYSICS_TYPE_RIGID_BODY,
            PHYSICS_TYPE_SENSOR,
            PHYSICS_TYPE_SIMPLE_SENSOR
        };

        enum class ShapeType
        {
            SHAPE_TYPE_NONE = 0,

            SHAPE_TYPE_POINT,
            SHAPE_TYPE_LINE,
            SHAPE_TYPE_RAY,
            SHAPE_TYPE_LINE_SEGMENT,
            SHAPE_TYPE_RECTANGLE,
            SHAPE_TYPE_ORIENTED_RECTANGLE,
            SHAPE_TYPE_CIRCLE,
            SHAPE_TYPE_CAPSULE,
            SHAPE_TYPE_TRIANGLE,
            SHAPE_TYPE_POLYGON,
            SHAPE_TYPE_CONVEX_POLYGON
        };

        static constexpr glm::vec2 s_origin(0.0f, 0.0f);
        static constexpr glm::vec2 s_defaultDirection(1.0f, 0.0f);
    };

    struct CollisionPoint2D
    {
        CollisionPoint2D(const glm::vec2& position = CollisionShape2D::s_origin, unsigned int tag = 0, bool enabled = true);

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
        CollisionLine2D(const glm::vec2& position = CollisionShape2D::s_origin, const float& slope = 0.0f, unsigned int tag = 0, bool enabled = true);

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
        CollisionRay2D(const glm::vec2& position = CollisionShape2D::s_origin, const glm::vec2& direction = CollisionShape2D::s_defaultDirection, unsigned int tag = 0, bool enabled = true);

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
        CollisionLineSegment2D(const glm::vec2& start = CollisionShape2D::s_origin, const glm::vec2& end = CollisionShape2D::s_origin, unsigned int tag = 0, bool enabled = true);

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
        CollisionRectangle2D(const glm::vec2& bottomLeft = CollisionShape2D::s_origin, const glm::vec2& topRight = CollisionShape2D::s_origin, unsigned int tag = 0, bool enabled = true, CollisionShape2D::PhysicsType physicsType = CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE);

        glm::vec2 bottomLeft;
        glm::vec2 topRight;

        unsigned int tag;
        bool enabled;

        CollisionShape2D::PhysicsType physicsType;
    };

    inline CollisionRectangle2D::CollisionRectangle2D(const glm::vec2& bottomLeft, const glm::vec2& topRight, unsigned int tag, bool enabled, CollisionShape2D::PhysicsType physicsType)
        : bottomLeft(bottomLeft)
        , topRight(topRight)
        , tag(tag)
        , enabled(enabled)
        , physicsType(physicsType)
    {}

    // -------------------------------------------------------------------------

    struct CollisionOrientedRectangle2D
    {
        CollisionOrientedRectangle2D(const glm::vec2& halfSize = CollisionShape2D::s_origin, const glm::vec2& position = CollisionShape2D::s_origin, const float& rotation = 0.0f, unsigned int tag = 0, bool enabled = true, CollisionShape2D::PhysicsType physicsType = CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE);

        glm::vec2 halfSize;
        glm::vec2 position;
        float rotation;

        unsigned int tag;
        bool enabled;

        CollisionShape2D::PhysicsType physicsType;
    };

    inline CollisionOrientedRectangle2D::CollisionOrientedRectangle2D(const glm::vec2& halfSize, const glm::vec2& position, const float& rotation, unsigned int tag, bool enabled, CollisionShape2D::PhysicsType physicsType)
        : halfSize(halfSize)
        , position(position)
        , rotation(rotation)
        , tag(tag)
        , enabled(enabled)
        , physicsType(physicsType)
    {}

    // -------------------------------------------------------------------------

    struct CollisionCircle2D
    {
        CollisionCircle2D(const glm::vec2& position = CollisionShape2D::s_origin, const float& radius = 0.0f, unsigned int tag = 0, bool enabled = true, CollisionShape2D::PhysicsType physicsType = CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE);

        glm::vec2 position;
        float radius;

        unsigned int tag;
        bool enabled;

        CollisionShape2D::PhysicsType physicsType;
    };

    inline CollisionCircle2D::CollisionCircle2D(const glm::vec2& position, const float& radius, unsigned int tag, bool enabled, CollisionShape2D::PhysicsType physicsType)
        : position(position)
        , radius(radius)
        , tag(tag)
        , enabled(enabled)
        , physicsType(physicsType)
    {}

    // -------------------------------------------------------------------------

    struct CollisionCapsule2D
    {
        CollisionCapsule2D(const glm::vec2& start = CollisionShape2D::s_origin, const glm::vec2& end = CollisionShape2D::s_origin, const float& radius = 0.0f, unsigned int tag = 0, bool enabled = true, CollisionShape2D::PhysicsType physicsType = CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE);

        glm::vec2 start;
        glm::vec2 end;
        float radius;

        unsigned int tag;
        bool enabled;

        CollisionShape2D::PhysicsType physicsType;
    };

    inline CollisionCapsule2D::CollisionCapsule2D(const glm::vec2& start, const glm::vec2& end, const float& radius, unsigned int tag, bool enabled, CollisionShape2D::PhysicsType physicsType)
        : start(start)
        , end(end)
        , radius(radius)
        , tag(tag)
        , enabled(enabled)
        , physicsType(physicsType)
    {}

    // -------------------------------------------------------------------------

    struct CollisionTriangle2D
    {
        CollisionTriangle2D(const glm::vec2& corner1 = CollisionShape2D::s_origin, const glm::vec2& corner2 = CollisionShape2D::s_origin, const glm::vec2& corner3 = CollisionShape2D::s_origin, unsigned int tag = 0, bool enabled = true, CollisionShape2D::PhysicsType physicsType = CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE);

        glm::vec2 corner1;
        glm::vec2 corner2;
        glm::vec2 corner3;

        unsigned int tag;
        bool enabled;

        CollisionShape2D::PhysicsType physicsType;
    };

    inline CollisionTriangle2D::CollisionTriangle2D(const glm::vec2& corner1, const glm::vec2& corner2, const glm::vec2& corner3, unsigned int tag, bool enabled, CollisionShape2D::PhysicsType physicsType)
        : corner1(corner1)
        , corner2(corner2)
        , corner3(corner3)
        , tag(tag)
        , enabled(enabled)
        , physicsType(physicsType)
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
        CollisionConvexPolygon2D(const std::vector<glm::vec2>& corners = std::vector<glm::vec2>(), unsigned int tag = 0, bool enabled = true, CollisionShape2D::PhysicsType physicsType = CollisionShape2D::PhysicsType::PHYSICS_TYPE_NONE);

        std::vector<glm::vec2> corners; // corners go around counterclockwise

        unsigned int tag;
        bool enabled;

        CollisionShape2D::PhysicsType physicsType;
    };

    inline CollisionConvexPolygon2D::CollisionConvexPolygon2D(const std::vector<glm::vec2>& corners, unsigned int tag, bool enabled, CollisionShape2D::PhysicsType physicsType)
        : corners(corners)
        , tag(tag)
        , enabled(enabled)
        , physicsType(physicsType)
    {}
}

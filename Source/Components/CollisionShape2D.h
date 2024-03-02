#pragma once

#include "glm/glm.hpp"

#include <vector>



namespace Project001
{
    struct CollisionShape2D
    {
        unsigned int tag;
        bool tangible;

    protected:
        CollisionShape2D();
        CollisionShape2D(unsigned int tag, bool tangible);
    };

    inline CollisionShape2D::CollisionShape2D()
        : tag(0)
        , tangible(true)
    {}

    inline CollisionShape2D::CollisionShape2D(unsigned int tag, bool tangible)
        : tag(tag)
        , tangible(tangible)
    {}

    // -------------------------------------------------------------------------

    struct CollisionPoint2D : CollisionShape2D
    {
        CollisionPoint2D();
        CollisionPoint2D(const glm::vec2& position);
        CollisionPoint2D(const glm::vec2& position, unsigned int tag, bool tangible);

        glm::vec2 position;
    };

    inline CollisionPoint2D::CollisionPoint2D()
        : position(0.0f, 0.0f)
    {}

    inline CollisionPoint2D::CollisionPoint2D(const glm::vec2& position)
        : position(position)
    {}

    inline CollisionPoint2D::CollisionPoint2D(const glm::vec2& position, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , position(position)
    {}

    // -------------------------------------------------------------------------

    struct CollisionLine2D : CollisionShape2D
    {
        CollisionLine2D();
        CollisionLine2D(const glm::vec2& position, const float& slope);
        CollisionLine2D(const glm::vec2& position, const float& slope, unsigned int tag, bool tangible);

        glm::vec2 position;
        float slope;
    };

    inline CollisionLine2D::CollisionLine2D()
        : position(0.0f, 0.0f)
        , slope(0.0f)
    {}

    inline CollisionLine2D::CollisionLine2D(const glm::vec2& position, const float& slope)
        : position(position)
        , slope(slope)
    {}

    inline CollisionLine2D::CollisionLine2D(const glm::vec2& position, const float& slope, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , position(position)
        , slope(slope)
    {}

    // -------------------------------------------------------------------------

    struct CollisionRay2D : CollisionShape2D
    {
        CollisionRay2D();
        CollisionRay2D(const glm::vec2& position, const glm::vec2& direction);
        CollisionRay2D(const glm::vec2& position, const glm::vec2& direction, unsigned int tag, bool tangible);

        glm::vec2 position;
        glm::vec2 direction; // keep this normalized (a unit vector)
    };

    inline CollisionRay2D::CollisionRay2D()
        : position(0.0f, 0.0f)
        , direction(1.0f, 0.0f)
    {}

    inline CollisionRay2D::CollisionRay2D(const glm::vec2& position, const glm::vec2& direction)
        : position(position)
        , direction(direction)
    {}

    inline CollisionRay2D::CollisionRay2D(const glm::vec2& position, const glm::vec2& direction, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , position(position)
        , direction(direction)
    {}

    // -------------------------------------------------------------------------

    struct CollisionLineSegment2D : CollisionShape2D
    {
        CollisionLineSegment2D();
        CollisionLineSegment2D(const glm::vec2& start, const glm::vec2& end);
        CollisionLineSegment2D(const glm::vec2& start, const glm::vec2& end, unsigned int tag, bool tangible);

        glm::vec2 start;
        glm::vec2 end;
    };

    inline CollisionLineSegment2D::CollisionLineSegment2D()
        : start(0.0f, 0.0f)
        , end(0.0f, 0.0f)
    {}

    inline CollisionLineSegment2D::CollisionLineSegment2D(const glm::vec2& start, const glm::vec2& end)
        : start(start)
        , end(end)
    {}

    inline CollisionLineSegment2D::CollisionLineSegment2D(const glm::vec2& start, const glm::vec2& end, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , start(start)
        , end(end)
    {}

    // -------------------------------------------------------------------------

    struct CollisionRectangle2D : CollisionShape2D
    {
        CollisionRectangle2D();
        CollisionRectangle2D(const glm::vec2& bottomLeft, const glm::vec2& topRight);
        CollisionRectangle2D(const glm::vec2& bottomLeft, const glm::vec2& topRight, unsigned int tag, bool tangible);

        glm::vec2 bottomLeft;
        glm::vec2 topRight;
    };

    inline CollisionRectangle2D::CollisionRectangle2D()
        : bottomLeft(0.0f, 0.0f)
        , topRight(0.0f, 0.0f)
    {}

    inline CollisionRectangle2D::CollisionRectangle2D(const glm::vec2& bottomLeft, const glm::vec2& topRight)
        : bottomLeft(bottomLeft)
        , topRight(topRight)
    {}

    inline CollisionRectangle2D::CollisionRectangle2D(const glm::vec2& bottomLeft, const glm::vec2& topRight, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , bottomLeft(bottomLeft)
        , topRight(topRight)
    {}

    // -------------------------------------------------------------------------

    struct CollisionOrientedRectangle2D : CollisionShape2D
    {
        CollisionOrientedRectangle2D();
        CollisionOrientedRectangle2D(const glm::vec2& halfSize, const glm::vec2& position, const float& rotation);
        CollisionOrientedRectangle2D(const glm::vec2& halfSize, const glm::vec2& position, const float& rotation, unsigned int tag, bool tangible);

        glm::vec2 halfSize;
        glm::vec2 position;
        float rotation;
    };

    inline CollisionOrientedRectangle2D::CollisionOrientedRectangle2D()
        : halfSize(0.0f, 0.0f)
        , position(0.0f, 0.0f)
        , rotation(0.0f)
    {}

    inline CollisionOrientedRectangle2D::CollisionOrientedRectangle2D(const glm::vec2& halfSize, const glm::vec2& position, const float& rotation)
        : halfSize(halfSize)
        , position(position)
        , rotation(rotation)
    {}

    inline CollisionOrientedRectangle2D::CollisionOrientedRectangle2D(const glm::vec2& halfSize, const glm::vec2& position, const float& rotation, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , halfSize(halfSize)
        , position(position)
        , rotation(rotation)
    {}

    // -------------------------------------------------------------------------

    struct CollisionCircle2D : CollisionShape2D
    {
        CollisionCircle2D();
        CollisionCircle2D(const glm::vec2& position, const float& radius);
        CollisionCircle2D(const glm::vec2& position, const float& radius, unsigned int tag, bool tangible);

        glm::vec2 position;
        float radius;
    };

    inline CollisionCircle2D::CollisionCircle2D()
        : position(0.0f, 0.0f)
        , radius(0.0f)
    {}

    inline CollisionCircle2D::CollisionCircle2D(const glm::vec2& position, const float& radius)
        : position(position)
        , radius(radius)
    {}

    inline CollisionCircle2D::CollisionCircle2D(const glm::vec2& position, const float& radius, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , position(position)
        , radius(radius)
    {}

    // -------------------------------------------------------------------------

    struct CollisionCapsule2D : CollisionShape2D
    {
        CollisionCapsule2D();
        CollisionCapsule2D(const glm::vec2& start, const glm::vec2& end, const float& radius);
        CollisionCapsule2D(const glm::vec2& start, const glm::vec2& end, const float& radius, unsigned int tag, bool tangible);

        glm::vec2 start;
        glm::vec2 end;
        float radius;
    };

    inline CollisionCapsule2D::CollisionCapsule2D()
        : start(0.0f, 0.0f)
        , end(0.0f, 0.0f)
        , radius(0.0f)
    {}

    inline CollisionCapsule2D::CollisionCapsule2D(const glm::vec2& start, const glm::vec2& end, const float& radius)
        : start(start)
        , end(end)
        , radius(radius)
    {}

    inline CollisionCapsule2D::CollisionCapsule2D(const glm::vec2& start, const glm::vec2& end, const float& radius, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , start(start)
        , end(end)
        , radius(radius)
    {}

    // -------------------------------------------------------------------------

    struct CollisionTriangle2D : CollisionShape2D
    {
        CollisionTriangle2D();
        CollisionTriangle2D(const glm::vec2& corner1, const glm::vec2& corner2, const glm::vec2& corner3);
        CollisionTriangle2D(const glm::vec2& corner1, const glm::vec2& corner2, const glm::vec2& corner3, unsigned int tag, bool tangible);

        glm::vec2 corner1;
        glm::vec2 corner2;
        glm::vec2 corner3;
    };

    inline CollisionTriangle2D::CollisionTriangle2D()
        : corner1(0.0f, 0.0f)
        , corner2(0.0f, 0.0f)
        , corner3(0.0f, 0.0f)
    {}

    inline CollisionTriangle2D::CollisionTriangle2D(const glm::vec2& corner1, const glm::vec2& corner2, const glm::vec2& corner3)
        : corner1(corner1)
        , corner2(corner2)
        , corner3(corner3)
    {}

    inline CollisionTriangle2D::CollisionTriangle2D(const glm::vec2& corner1, const glm::vec2& corner2, const glm::vec2& corner3, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , corner1(corner1)
        , corner2(corner2)
        , corner3(corner3)
    {}

    // -------------------------------------------------------------------------

    struct CollisionPolygon2D : CollisionShape2D
    {
        CollisionPolygon2D();
        CollisionPolygon2D(const std::vector<glm::vec2>& positions);
        CollisionPolygon2D(const std::vector<glm::vec2>& positions, unsigned int tag, bool tangible);

        std::vector<glm::vec2> positions;
    };

    inline CollisionPolygon2D::CollisionPolygon2D()
    {}

    inline CollisionPolygon2D::CollisionPolygon2D(const std::vector<glm::vec2>& positions)
        : positions(positions)
    {}

    inline CollisionPolygon2D::CollisionPolygon2D(const std::vector<glm::vec2>& positions, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , positions(positions)
    {}

    // -------------------------------------------------------------------------

    struct CollisionConvexPolygon2D : CollisionShape2D
    {
        CollisionConvexPolygon2D();
        CollisionConvexPolygon2D(const std::vector<glm::vec2>& positions);
        CollisionConvexPolygon2D(const std::vector<glm::vec2>& positions, unsigned int tag, bool tangible);

        std::vector<glm::vec2> positions;
    };

    inline CollisionConvexPolygon2D::CollisionConvexPolygon2D()
    {}

    inline CollisionConvexPolygon2D::CollisionConvexPolygon2D(const std::vector<glm::vec2>& positions)
        : positions(positions)
    {}

    inline CollisionConvexPolygon2D::CollisionConvexPolygon2D(const std::vector<glm::vec2>& positions, unsigned int tag, bool tangible)
        : CollisionShape2D(tag, tangible)
        , positions(positions)
    {}
}

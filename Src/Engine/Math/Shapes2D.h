#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    struct Point2D
    {
        Point2D();
        Point2D(const glm::vec2& position);

        glm::vec2 position;
    };

    inline Point2D::Point2D()
        : position(0.0f, 0.0f)
    {}

    inline Point2D::Point2D(const glm::vec2& position)
        : position(position)
    {}

    struct Line2D
    {
        Line2D();
        Line2D(const glm::vec2& position, const float& slope);

        glm::vec2 position;
        float slope;
    };

    inline Line2D::Line2D()
        : position(0.0f, 0.0f)
        , slope(0.0f)
    {}

    inline Line2D::Line2D(const glm::vec2& position, const float& slope)
        : position(position)
        , slope(slope)
    {}

    struct Ray2D
    {
        Ray2D();
        Ray2D(const glm::vec2& position, const glm::vec2& direction);

        glm::vec2 position;
        glm::vec2 direction; // keep this normalized (a unit vector)
    };

    inline Ray2D::Ray2D()
        : position(0.0f, 0.0f)
        , direction(1.0f, 0.0f)
    {}

    inline Ray2D::Ray2D(const glm::vec2& position, const glm::vec2& direction)
        : position(position)
        , direction(direction)
    {}

    struct LineSegment2D
    {
        LineSegment2D();
        LineSegment2D(const glm::vec2& start, const glm::vec2& end);

        glm::vec2 start;
        glm::vec2 end;
    };

    inline LineSegment2D::LineSegment2D()
        : start(0.0f, 0.0f)
        , end(0.0f, 0.0f)
    {}

    inline LineSegment2D::LineSegment2D(const glm::vec2& start, const glm::vec2& end)
        : start(start)
        , end(end)
    {}

    struct Rectangle2D
    {
        Rectangle2D();
        Rectangle2D(const glm::vec2& bottomLeft, const glm::vec2& topRight);

        glm::vec2 bottomLeft;
        glm::vec2 topRight;
    };

    inline Rectangle2D::Rectangle2D()
        : bottomLeft(0.0f, 0.0f)
        , topRight(0.0f, 0.0f)
    {}

    inline Rectangle2D::Rectangle2D(const glm::vec2& bottomLeft, const glm::vec2& topRight)
        : bottomLeft(bottomLeft)
        , topRight(topRight)
    {}

    struct OrientedRectangle2D
    {
        OrientedRectangle2D();
        OrientedRectangle2D(const glm::vec2& halfSize, const glm::vec2& position, const float& rotation);

        glm::vec2 halfSize;
        glm::vec2 position;
        float rotation;
    };

    inline OrientedRectangle2D::OrientedRectangle2D()
        : halfSize(0.0f, 0.0f)
        , position(0.0f, 0.0f)
        , rotation(0.0f)
    {}

    inline OrientedRectangle2D::OrientedRectangle2D(const glm::vec2& halfSize, const glm::vec2& position, const float& rotation)
        : halfSize(halfSize)
        , position(position)
        , rotation(rotation)
    {}

    struct Circle2D
    {
        Circle2D();
        Circle2D(const glm::vec2& position, const float& radius);

        glm::vec2 position;
        float radius;
    };

    inline Circle2D::Circle2D()
        : position(0.0f, 0.0f)
        , radius(0.0f)
    {}

    inline Circle2D::Circle2D(const glm::vec2& position, const float& radius)
        : position(position)
        , radius(radius)
    {}

    struct Capsule2D
    {
        Capsule2D();
        Capsule2D(const glm::vec2& start, const glm::vec2& end, const float& radius);

        glm::vec2 start;
        glm::vec2 end;
        float radius;
    };

    inline Capsule2D::Capsule2D()
        : start(0.0f, 0.0f)
        , end(0.0f, 0.0f)
        , radius(0.0f)
    {}

    inline Capsule2D::Capsule2D(const glm::vec2& start, const glm::vec2& end, const float& radius)
        : start(start)
        , end(end)
        , radius(radius)
    {}

    struct Triangle2D
    {
        Triangle2D();
        Triangle2D(const glm::vec2& corner1, const glm::vec2& corner2, const glm::vec2& corner3);

        glm::vec2 corner1;
        glm::vec2 corner2;
        glm::vec2 corner3;
    };

    inline Triangle2D::Triangle2D()
        : corner1(0.0f, 0.0f)
        , corner2(0.0f, 0.0f)
        , corner3(0.0f, 0.0f)
    {}

    inline Triangle2D::Triangle2D(const glm::vec2& corner1, const glm::vec2& corner2, const glm::vec2& corner3)
        : corner1(corner1)
        , corner2(corner2)
        , corner3(corner3)
    {}
}

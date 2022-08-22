#pragma once

#include "glm/gtc/quaternion.hpp"



namespace Project001
{
    struct Point
    {
        Point();
        Point(const glm::vec3& position);

        glm::vec3 position;
    };

    inline Point::Point()
        : position(0.0f, 0.0f, 0.0f)
    {}

    inline Point::Point(const glm::vec3& position)
        : position(position)
    {}

    struct Line
    {
        Line();
        Line(const glm::vec3& position, const glm::vec3& direction);

        glm::vec3 position;
        glm::vec3 direction; // keep this normalized (a unit vector)

        // NOTE:
        // Should I replace direction with a slopeXY and slopeXZ for Line?
    };

    inline Line::Line()
        : position(0.0f, 0.0f, 0.0f)
        , direction(1.0f, 0.0f, 0.0f)
    {}

    inline Line::Line(const glm::vec3& position, const glm::vec3& direction)
        : position(position)
        , direction(direction)
    {}

    // Rays are like Lines, but they only go to infinity in one direction
    typedef Line Ray;

    struct LineSegment
    {
        LineSegment();
        LineSegment(const glm::vec3& start, const glm::vec3& end);

        glm::vec3 start;
        glm::vec3 end;
    };

    inline LineSegment::LineSegment()
        : start(0.0f, 0.0f, 0.0f)
        , end(0.0f, 0.0f, 0.0f)
    {}

    inline LineSegment::LineSegment(const glm::vec3& start, const glm::vec3& end)
        : start(start)
        , end(end)
    {}

    struct Plane
    {
        Plane();
        Plane(const glm::vec3& normal, const glm::vec3& center);

        float GetDistanceFromOrigin();

        glm::vec3 normal; // keep this normalized (a unit vector)
        // float distance; // distance from origin
        glm::vec3 center; // Using this instead of distance
    };

    inline Plane::Plane()
        : normal(0.0f, 1.0f, 0.0f)
        , center(0.0f, 0.0f, 0.0f)
    {}

    inline Plane::Plane(const glm::vec3& normal, const glm::vec3& center)
        : normal(normal)
        , center(center)
    {}

    inline float Plane::GetDistanceFromOrigin()
    {
        // point on plane = (x0, y0, z0)
        // point = (x1, y1, z1)
        // plane normal vector = (A, B, C)
        // D = -A*x0 - B*y0 - C*z0
        // d = distance from plane to point
        // d = (A*x1 + B*y1 + C*z1 + D) / sqrt(A^2 + B^2 +C^2)
        //
        // Substitude (0, 0, 0) for point:
        // d = (D) / sqrt(A^2 + B^2 +C^2)
        // d = (-A*x0 - B*y0 - C*z0) / sqrt(A^2 + B^2 +C^2)
        // 
        // if the distance is negative, the point is in the opposite direction
        // to that of the plane normal

        float& A = normal.x;
        float& B = normal.y;
        float& C = normal.z;
        float& x0 = center.x;
        float& y0 = center.y;
        float& z0 = center.z;

        float numerator = -A * x0 - B * y0 - C * z0;
        float denominator = glm::sqrt(A * A + B * B + C * C);

        return numerator / denominator;
    }

    struct Triangle
    {
        Triangle();
        Triangle(const glm::vec3& corner1, const glm::vec3& corner2, const glm::vec3& corner3);

        glm::vec3 corner1;
        glm::vec3 corner2;
        glm::vec3 corner3;
    };

    inline Triangle::Triangle()
        : corner1(0.0f, 0.0f, 0.0f)
        , corner2(0.0f, 0.0f, 0.0f)
        , corner3(0.0f, 0.0f, 0.0f)
    {}

    inline Triangle::Triangle(const glm::vec3& corner1, const glm::vec3& corner2, const glm::vec3& corner3)
        : corner1(corner1)
        , corner2(corner2)
        , corner3(corner3)
    {}

    struct AxisAlignedBoundingBox
    {
        AxisAlignedBoundingBox();
        AxisAlignedBoundingBox(const glm::vec3& min, const glm::vec3& max);

        glm::vec3 min;
        glm::vec3 max;
    };

    inline AxisAlignedBoundingBox::AxisAlignedBoundingBox()
        : min(0.0f, 0.0f, 0.0f)
        , max(0.0f, 0.0f, 0.0f)
    {}

    inline AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3& min, const glm::vec3& max)
        : min(min)
        , max(max)
    {}

    struct OrientedBoundingBox
    {
        OrientedBoundingBox();
        OrientedBoundingBox(const glm::vec3& halfSize, const glm::vec3& position, const glm::quat& orientation);

        glm::vec3 halfSize;
        glm::vec3 position;
        glm::quat orientation;
    };

    inline OrientedBoundingBox::OrientedBoundingBox()
        : halfSize(0.0f, 0.0f, 0.0f)
        , position(0.0f, 0.0f, 0.0f)
        , orientation(1.0f, 0.0f, 0.0f, 0.0f)
    {}

    inline OrientedBoundingBox::OrientedBoundingBox(const glm::vec3& halfSize, const glm::vec3& position, const glm::quat& orientation)
        : halfSize(halfSize)
        , position(position)
        , orientation(orientation)
    {}

    struct Sphere
    {
        Sphere();
        Sphere(const glm::vec3& position, const float& radius);

        glm::vec3 position;
        float radius;
    };

    inline Sphere::Sphere()
        : position(0.0f, 0.0f, 0.0f)
        , radius(0.0f)
    {}

    inline Sphere::Sphere(const glm::vec3& position, const float& radius)
        : position(position)
        , radius(radius)
    {}

    struct Capsule
    {
        Capsule();
        Capsule(const glm::vec3& start, const glm::vec3 end, const float& radius);

        glm::vec3 start;
        glm::vec3 end;
        float radius;
    };

    inline Capsule::Capsule()
        : start(0.0f, 0.0f, 0.0f)
        , end(0.0f, 0.0f, 0.0f)
        , radius(0.0f)
    {}

    inline Capsule::Capsule(const glm::vec3& start, const glm::vec3 end, const float& radius)
        : start(start)
        , end(end)
        , radius(radius)
    {}
}

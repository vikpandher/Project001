#include "glm/glm.hpp"



namespace Project001
{
    static const float s_defaultMarginOfError = 0.000001f;

    // Point Interactions ------------------------------------------------------

    bool PointPointIntersection2d(
        const glm::vec2& pointA,
        const glm::vec2& pointB,
        float marginOfError = s_defaultMarginOfError);

    bool PointLineIntersection2d(
        const glm::vec2& point,
        const glm::vec2& linePoint,
        float slope,
        float marginOfError = s_defaultMarginOfError);

    bool PointLineIntersection2d(
        const glm::vec2& point,
        const glm::vec2& lineStart,
        const glm::vec2& lineEnd,
        float marginOfError = s_defaultMarginOfError);

    bool PointLineSegmentIntersection2d(
        const glm::vec2& point,
        const glm::vec2& lineStart,
        const glm::vec2& lineEnd,
        float marginOfError = s_defaultMarginOfError);

    bool PointFixedRectangleIntersection2d(
        const glm::vec2& point,
        const glm::vec2& rectangleCornerA,
        const glm::vec2& rectangleCornerB,
        float marginOfError = s_defaultMarginOfError);

    // TODO
    bool PointRectangleIntersection2d(
        const glm::vec2& point,
        const glm::vec2& rectangleCenter,
        const glm::vec2& rectangleDimensions,
        float rectangleRotation,
        float marginOfError = s_defaultMarginOfError);

    // Line Interactions -------------------------------------------------------

    glm::vec2 GetLineLineIntersection2d(
        const glm::vec2& point1,
        const float& slope1,
        const glm::vec2& point2,
        const float& slope2);

    // Tests -------------------------------------------------------------------

    void TestPointLineIntersection2d();

    void TestPointFixedRectangleIntersection2d();

    void TestPointRectangleIntersection2d();
}
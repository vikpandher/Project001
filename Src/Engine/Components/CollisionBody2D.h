#pragma once

#include <vector>

#include "Engine/Components/Placement2D.h"
#include "Engine/Math/Shapes2D.h"



namespace Project001
{
    class CollisionBody2D : public Placement2D
    {
    public:
        CollisionBody2D();

        size_t AddPoint(const Point2D& point);
        Point2D& GetPoint(size_t index);
        std::vector<Point2D>& GetPoints();

        size_t AddLine(const Line2D& line);
        Line2D& GetLine(size_t index);
        std::vector<Line2D>& GetLines();

        size_t AddLineSegment(const LineSegment2D& lineSegment);
        LineSegment2D& GetLineSegment(size_t index);
        std::vector<LineSegment2D>& GetLineSegments();

        size_t AddRectangle(const Rectangle2D& rectangle);
        Rectangle2D& GetRectangle(size_t index);
        std::vector<Rectangle2D>& GetRectangles();

        size_t AddOrientedRectangle(const OrientedRectangle2D& orientedRectangle);
        OrientedRectangle2D& GetOrientedRectangle(size_t index);
        std::vector<OrientedRectangle2D>& GetOrientedRectangles();

        size_t AddCircle(const Circle2D& circle);
        Circle2D& GetCircle(size_t index);
        std::vector<Circle2D>& GetCircles();

        size_t AddCapsule(const Capsule2D& capsule);
        Capsule2D& GetCapsule(size_t index);
        std::vector<Capsule2D>& GetCapsules();

        size_t AddTriangle(const Triangle2D& triangle);
        Triangle2D& GetTriangle(size_t index);
        std::vector<Triangle2D>& GetTriangles();

        void CalculateBoundingRadius();
        float GetBoundingRadius() const;
        void SetBoundingRadius(float boundingRadius);

        void CalculateTransforedShapes();

        bool GetColliding() const;
        void SetColliding(bool colliding);

        void CalculateCollision(CollisionBody2D& other);

        bool GetCollision(const Point2D& point);

    protected:
        // Inherited:
        // glm::vec2 position_;
        // float rotation_;

        std::vector<Point2D> points_;
        std::vector<Line2D> lines_;
        std::vector<LineSegment2D> lineSegments_;
        std::vector<Rectangle2D> rectangles_;
        std::vector<OrientedRectangle2D> orientedRectangles_;
        std::vector<Circle2D> circles_;
        std::vector<Capsule2D> capsules_;
        std::vector<Triangle2D> triangles_;

        std::vector<Point2D> transformedPoints_;
        std::vector<Line2D> transformedLines_;
        std::vector<LineSegment2D> transformedLineSegments_;
        std::vector<Rectangle2D> transformedRectangles_;
        std::vector<OrientedRectangle2D> transformedOrientedRectangles_;
        std::vector<Circle2D> transformedCircles_;
        std::vector<Capsule2D> transformedCapsules_;
        std::vector<Triangle2D> transformedTriangles_;

        float boundingRadius_;

        bool colliding_;
    };

    inline CollisionBody2D::CollisionBody2D()
        : boundingRadius_(0.0f)
        , colliding_(false)
    {}

    inline size_t CollisionBody2D::AddPoint(const Point2D& point)
    {
        points_.push_back(point);
        return points_.size() - 1;
    }

    inline Point2D& CollisionBody2D::GetPoint(size_t index)
    {
        return points_[index];
    }

    inline std::vector<Point2D>& CollisionBody2D::GetPoints()
    {
        return points_;
    }

    inline size_t CollisionBody2D::AddLine(const Line2D& line)
    {
        lines_.push_back(line);
        return lines_.size() - 1;
    }

    inline Line2D& CollisionBody2D::GetLine(size_t index)
    {
        return lines_[index];
    }

    inline std::vector<Line2D>& CollisionBody2D::GetLines()
    {
        return lines_;
    }

    inline size_t CollisionBody2D::AddLineSegment(const LineSegment2D& lineSegment)
    {
        lineSegments_.push_back(lineSegment);
        return lineSegments_.size() - 1;
    }

    inline LineSegment2D& CollisionBody2D::GetLineSegment(size_t index)
    {
        return lineSegments_[index];
    }

    inline std::vector<LineSegment2D>& CollisionBody2D::GetLineSegments()
    {
        return lineSegments_;
    }

    inline size_t CollisionBody2D::AddRectangle(const Rectangle2D& rectangle)
    {
        rectangles_.push_back(rectangle);
        return rectangles_.size() - 1;
    }

    inline Rectangle2D& CollisionBody2D::GetRectangle(size_t index)
    {
        return rectangles_[index];
    }

    inline std::vector<Rectangle2D>& CollisionBody2D::GetRectangles()
    {
        return rectangles_;
    }

    inline size_t CollisionBody2D::AddOrientedRectangle(const OrientedRectangle2D& orientedRectangle)
    {
        orientedRectangles_.push_back(orientedRectangle);
        return orientedRectangles_.size() - 1;
    }

    inline OrientedRectangle2D& CollisionBody2D::GetOrientedRectangle(size_t index)
    {
        return orientedRectangles_[index];
    }

    inline std::vector<OrientedRectangle2D>& CollisionBody2D::GetOrientedRectangles()
    {
        return orientedRectangles_;
    }

    inline size_t CollisionBody2D::AddCircle(const Circle2D& circle)
    {
        circles_.push_back(circle);
        return circles_.size() - 1;
    }

    inline Circle2D& CollisionBody2D::GetCircle(size_t index)
    {
        return circles_[index];
    }

    inline std::vector<Circle2D>& CollisionBody2D::GetCircles()
    {
        return circles_;
    }

    inline size_t CollisionBody2D::AddCapsule(const Capsule2D& capsule)
    {
        capsules_.push_back(capsule);
        return capsules_.size() - 1;
    }

    inline Capsule2D& CollisionBody2D::GetCapsule(size_t index)
    {
        return capsules_[index];
    }

    inline std::vector<Capsule2D>& CollisionBody2D::GetCapsules()
    {
        return capsules_;
    }

    inline size_t CollisionBody2D::AddTriangle(const Triangle2D& triangle)
    {
        triangles_.push_back(triangle);
        return triangles_.size() - 1;
    }

    inline Triangle2D& CollisionBody2D::GetTriangle(size_t index)
    {
        return triangles_[index];
    }

    inline std::vector<Triangle2D>& CollisionBody2D::GetTriangles()
    {
        return triangles_;
    }

    inline float CollisionBody2D::GetBoundingRadius() const
    {
        return boundingRadius_;
    }

    inline void CollisionBody2D::SetBoundingRadius(float boundingRadius)
    {
        boundingRadius_ = boundingRadius;
    }

    inline bool CollisionBody2D::GetColliding() const
    {
        return colliding_;
    }

    inline void CollisionBody2D::SetColliding(bool colliding)
    {
        colliding_ = colliding;
    }
}
#pragma once

#include <vector>

#include "Engine/Components/Placement.h"
#include "Engine/Math/Shapes3D.h"



namespace Project001
{
    class CollisionBody : public Placement
    {
    public:
        CollisionBody();

        size_t AddPoint(const Point& point);
        Point& GetPoint(size_t index);
        std::vector<Point>& GetPoints();

        size_t AddLine(const Line& line);
        Line& GetLine(size_t index);
        std::vector<Line>& GetLines();

        size_t AddLineSegment(const LineSegment& lineSegment);
        LineSegment& GetLineSegment(size_t index);
        std::vector<LineSegment>& GetLineSegments();

        size_t AddPlane(const Plane& plane);
        Plane& GetPlane(size_t index);
        std::vector<Plane>& GetPlanes();

        size_t AddTriangle(const Triangle& triangle);
        Triangle& GetTriangle(size_t index);
        std::vector<Triangle>& GetTriangles();

        size_t AddAxisAlignedBoundingBox(const AxisAlignedBoundingBox& axisAlignedBoundingBox);
        AxisAlignedBoundingBox& GetAxisAlignedBoundingBox(size_t index);
        std::vector<AxisAlignedBoundingBox>& GetAxisAlignedBoundingBoxes();

        size_t AddOrientedBoundingBox(const OrientedBoundingBox& orientedBoundingBox);
        OrientedBoundingBox& GetOrientedBoundingBox(size_t index);
        std::vector<OrientedBoundingBox>& GetOrientedBoundingBoxes();

        size_t AddSphere(const Sphere& sphere);
        Sphere& GetSphere(size_t index);
        std::vector<Sphere>& GetSpheres();

        void CalculateBoundingRadius(); // TODO
        float GetBoundingRadius() const;
        void SetBoundingRadius(float boundingRadius);

        void CalculateTransforedShapes(); // TODO

        bool GetColliding() const;
        void SetColliding(bool colliding);

        void CalculateCollision(CollisionBody& other); // TODO

    protected:
        // Inherited:
        // glm::vec3 position_;
        // glm::quat orientation_;

        std::vector<Point> points_;
        std::vector<Line> lines_;
        std::vector<LineSegment> lineSegments_;
        std::vector<Plane> planes_;
        std::vector<Triangle> triangles_;
        std::vector<AxisAlignedBoundingBox> axisAlignedBoundingBoxes_;
        std::vector<OrientedBoundingBox> orientedBoundingBoxes_;
        std::vector<Sphere> spheres_;

        std::vector<Point> transformedPoints_;
        std::vector<Line> transformedLines_;
        std::vector<LineSegment> transformedLineSegments_;
        std::vector<Plane> transformedPlanes_;
        std::vector<Triangle> transformedTriangles_;
        std::vector<AxisAlignedBoundingBox> transformedAxisAlignedBoundingBoxes_;
        std::vector<OrientedBoundingBox> transformedOrientedBoundingBoxes_;
        std::vector<Sphere> transformedSpheres_;

        float boundingRadius_;

        bool colliding_;
    };

    inline CollisionBody::CollisionBody()
        : boundingRadius_(0.0f)
        , colliding_(false)
    {}

    inline size_t CollisionBody::AddPoint(const Point& point)
    {
        points_.push_back(point);
        return points_.size() - 1;
    }

    inline Point& CollisionBody::GetPoint(size_t index)
    {
        return points_[index];
    }

    inline std::vector<Point>& CollisionBody::GetPoints()
    {
        return points_;
    }

    inline size_t CollisionBody::AddLine(const Line& line)
    {
        lines_.push_back(line);
        return lines_.size() - 1;
    }

    inline Line& CollisionBody::GetLine(size_t index)
    {
        return lines_[index];
    }

    inline std::vector<Line>& CollisionBody::GetLines()
    {
        return lines_;
    }

    inline size_t CollisionBody::AddLineSegment(const LineSegment& lineSegment)
    {
        lineSegments_.push_back(lineSegment);
        return lineSegments_.size() - 1;
    }

    inline LineSegment& CollisionBody::GetLineSegment(size_t index)
    {
        return lineSegments_[index];
    }

    inline std::vector<LineSegment>& CollisionBody::GetLineSegments()
    {
        return lineSegments_;
    }

    inline size_t CollisionBody::AddPlane(const Plane& plane)
    {
        planes_.push_back(plane);
        return planes_.size() - 1;
    }

    inline Plane& CollisionBody::GetPlane(size_t index)
    {
        return planes_[index];
    }

    inline std::vector<Plane>& CollisionBody::GetPlanes()
    {
        return planes_;
    }

    inline size_t CollisionBody::AddTriangle(const Triangle& triangle)
    {
        triangles_.push_back(triangle);
        return triangles_.size() - 1;
    }

    inline Triangle& CollisionBody::GetTriangle(size_t index)
    {
        return triangles_[index];
    }

    inline std::vector<Triangle>& CollisionBody::GetTriangles()
    {
        return triangles_;
    }

    inline size_t CollisionBody::AddAxisAlignedBoundingBox(const AxisAlignedBoundingBox& axisAlignedBoundingBox)
    {
        axisAlignedBoundingBoxes_.push_back(axisAlignedBoundingBox);
        return axisAlignedBoundingBoxes_.size() - 1;
    }

    inline AxisAlignedBoundingBox& CollisionBody::GetAxisAlignedBoundingBox(size_t index)
    {
        return axisAlignedBoundingBoxes_[index];
    }

    inline std::vector<AxisAlignedBoundingBox>& CollisionBody::GetAxisAlignedBoundingBoxes()
    {
        return axisAlignedBoundingBoxes_;
    }

    inline size_t CollisionBody::AddOrientedBoundingBox(const OrientedBoundingBox& orientedBoundingBox)
    {
        orientedBoundingBoxes_.push_back(orientedBoundingBox);
        return orientedBoundingBoxes_.size() - 1;
    }

    inline OrientedBoundingBox& CollisionBody::GetOrientedBoundingBox(size_t index)
    {
        return orientedBoundingBoxes_[index];
    }

    inline std::vector<OrientedBoundingBox>& CollisionBody::GetOrientedBoundingBoxes()
    {
        return orientedBoundingBoxes_;
    }

    inline size_t CollisionBody::AddSphere(const Sphere& sphere)
    {
        spheres_.push_back(sphere);
        return spheres_.size() - 1;
    }

    inline Sphere& CollisionBody::GetSphere(size_t index)
    {
        return spheres_[index];
    }

    inline std::vector<Sphere>& CollisionBody::GetSpheres()
    {
        return spheres_;
    }

    inline float CollisionBody::GetBoundingRadius() const
    {
        return boundingRadius_;
    }

    inline void CollisionBody::SetBoundingRadius(float boundingRadius)
    {
        boundingRadius_ = boundingRadius;
    }

    inline bool CollisionBody::GetColliding() const
    {
        return colliding_;
    }

    inline void CollisionBody::SetColliding(bool colliding)
    {
        colliding_ = colliding;
    }
}
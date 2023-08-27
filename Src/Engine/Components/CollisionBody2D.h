#pragma once

#include "Engine/Components/Placement2D.h"
#include "Engine/Math/Shapes2D.h"
#include "Engine/Components/CollisionData.h"



namespace Project001
{
    class CollisionBody2D : public Placement2D
    {
    public:
        CollisionBody2D();

        void SetTangible(bool tangible);
        const bool GetTangible() const;

        void SetCollisionGroupMask(uint32_t collisionGroupMask);
        uint32_t GetCollisionGroupMask() const;

        size_t AddPoint(const Point2D& point, unsigned int id = 0);
        const std::vector<Point2D>& GetPoints() const;
        const std::vector<unsigned int>& GetPointIds() const;
        const std::vector<Point2D>& GetTransformedPoints() const;
        const std::vector<unsigned int>& GetTransformedPointIds() const;

        size_t AddLine(const Line2D& line, unsigned int id = 0);
        const std::vector<Line2D>& GetLines() const;
        const std::vector<unsigned int>& GetLineIds() const;
        const std::vector<Line2D>& GetTransformedLines() const;
        const std::vector<unsigned int>& GetTransformedLineIds() const;

        size_t AddRay(const Ray2D& ray, unsigned int id = 0);
        const std::vector<Ray2D>& GetRays() const;
        const std::vector<unsigned int>& GetRayIds() const;
        const std::vector<Ray2D>& GetTransformedRays() const;
        const std::vector<unsigned int>& GetTransformedRayIds() const;

        size_t AddLineSegment(const LineSegment2D& lineSegment, unsigned int id = 0);
        const std::vector<LineSegment2D>& GetLineSegments() const;
        const std::vector<unsigned int>& GetLineSegmentIds() const;
        const std::vector<LineSegment2D>& GetTransformedLineSegments() const;
        const std::vector<unsigned int>& GetTransformedLineSegmentIds() const;

        size_t AddRectangle(const Rectangle2D& rectangle, unsigned int id = 0);
        const std::vector<Rectangle2D>& GetRectangles() const;
        const std::vector<unsigned int>& GetRectangleIds() const;
        const std::vector<Rectangle2D>& GetTransformedRectangles() const;
        const std::vector<unsigned int>& GetTransformedRectangleIds() const;

        size_t AddOrientedRectangle(const OrientedRectangle2D& orientedRectangle, unsigned int id = 0);
        const std::vector<OrientedRectangle2D>& GetOrientedRectangles() const;
        const std::vector<unsigned int>& GetOrientedRectangleIds() const;
        const std::vector<OrientedRectangle2D>& GetTransformedOrientedRectangles() const;
        const std::vector<unsigned int>& GetTransformedOrientedRectangleIds() const;

        size_t AddCircle(const Circle2D& circle, unsigned int id = 0);
        const std::vector<Circle2D>& GetCircles() const;
        const std::vector<unsigned int>& GetCircleIds() const;
        const std::vector<Circle2D>& GetTransformedCircles() const;
        const std::vector<unsigned int>& GetTransformedCircleIds() const;

        size_t AddCapsule(const Capsule2D& capsule, unsigned int id = 0);
        const std::vector<Capsule2D>& GetCapsules() const;
        const std::vector<unsigned int>& GetCapsuleIds() const;
        const std::vector<Capsule2D>& GetTransformedCapsules() const;
        const std::vector<unsigned int>& GetTransformedCapsuleIds() const;

        size_t AddTriangle(const Triangle2D& triangle, unsigned int id = 0);
        const std::vector<Triangle2D>& GetTriangles() const;
        const std::vector<unsigned int>& GetTriangleIds() const;
        const std::vector<Triangle2D>& GetTransformedTriangles() const;
        const std::vector<unsigned int>& GetTransformedTriangleIds() const;

        size_t AddPolygon(const Polygon2D& polygon, unsigned int id = 0);
        const std::vector<Polygon2D>& GetPolygons() const;
        const std::vector<unsigned int>& GetPolygonIds() const;
        const std::vector<Polygon2D>& GetTransformedPolygons() const;
        const std::vector<unsigned int>& GetTransformedPolygonIds() const;

        size_t AddConvexPolygon(const ConvexPolygon2D& convexPolygon, unsigned int id = 0);
        const std::vector<ConvexPolygon2D>& GetConvexPolygons() const;
        const std::vector<unsigned int>& GetConvexPolygonIds() const;
        const std::vector<ConvexPolygon2D>& GetTransformedConvexPolygons() const;
        const std::vector<unsigned int>& GetTransformedConvexPolygonIds() const;

        bool BoundingRadiusUpToDate() const;
        bool TransformedShapesUpToDate() const;

        float GetBoundingRadius() const;
        void CalculateBoundingRadius();

        void CalculateTransformedShapes();

        void AddCollision(const CollisionData& collision);
        void ClearCollisions();
        const std::vector<CollisionData>& GetCollisions() const;

        // Overwriten:

        void SetPosition(const glm::vec2& position);
        void SetPosition(float x, float y);
        void SetPositionX(float x);
        void SetPositionY(float y);

        void AddTranslation(const glm::vec2& translation);
        void AddTranslation(float x, float y);
        void AddTranslationX(float x);
        void AddTranslationY(float y);

        void RevolveAround(const glm::vec2& focalPoint, float angleInRadians);

        void TranslateUp(float d);
        void TranslateRight(float d);

        void AddRotation(float angleInRadians);
        void SetRotation(float angleInRadians);

    protected:
        // Inherited:
        // glm::vec2 position_;
        // float rotation_;

        std::vector<Point2D> points_;
        std::vector<Line2D> lines_;
        std::vector<Ray2D> rays_;
        std::vector<LineSegment2D> lineSegments_;
        std::vector<Rectangle2D> rectangles_;
        std::vector<OrientedRectangle2D> orientedRectangles_;
        std::vector<Circle2D> circles_;
        std::vector<Capsule2D> capsules_;
        std::vector<Triangle2D> triangles_;
        std::vector<Polygon2D> polygons_;
        std::vector<ConvexPolygon2D> convexPolygons_;

        std::vector<unsigned int> pointIds_;
        std::vector<unsigned int> lineIds_;
        std::vector<unsigned int> rayIds_;
        std::vector<unsigned int> lineSegmentIds_;
        std::vector<unsigned int> rectangleIds_;
        std::vector<unsigned int> orientedRectangleIds_;
        std::vector<unsigned int> circleIds_;
        std::vector<unsigned int> capsuleIds_;
        std::vector<unsigned int> triangleIds_;
        std::vector<unsigned int> polygonIds_;
        std::vector<unsigned int> convexPolygonIds_;

        std::vector<Point2D> transformedPoints_;
        std::vector<Line2D> transformedLines_;
        std::vector<Ray2D> transformedRays_;
        std::vector<LineSegment2D> transformedLineSegments_;
        std::vector<Rectangle2D> transformedRectangles_;
        std::vector<OrientedRectangle2D> transformedOrientedRectangles_;
        std::vector<Circle2D> transformedCircles_;
        std::vector<Capsule2D> transformedCapsules_;
        std::vector<Triangle2D> transformedTriangles_;
        std::vector<Polygon2D> transformedPolygons_;
        std::vector<ConvexPolygon2D> transformedConvexPolygons_;

        // When being transformed a shape can become a different shape.
        // For example, with a rotation rectangle becomes orientedRectangle.
        // So there are transformed id vectors.
        std::vector<unsigned int> transformedPointIds_;
        std::vector<unsigned int> transformedLineIds_;
        std::vector<unsigned int> transformedRayIds_;
        std::vector<unsigned int> transformedLineSegmentIds_;
        std::vector<unsigned int> transformedRectangleIds_;
        std::vector<unsigned int> transformedOrientedRectangleIds_;
        std::vector<unsigned int> transformedCircleIds_;
        std::vector<unsigned int> transformedCapsuleIds_;
        std::vector<unsigned int> transformedTriangleIds_;
        std::vector<unsigned int> transformedPolygonIds_;
        std::vector<unsigned int> transformedConvexPolygonIds_;

        bool tangible_;

        uint32_t collisionGroupMask_;

        float boundingRadius_;

        bool boundingRadiusUpToDate_;
        bool transformedShapesUpToDate_;

        std::vector<CollisionData> collisions_;
    };

    inline CollisionBody2D::CollisionBody2D()
        : tangible_(true)
        , collisionGroupMask_(0b00000000000000000000000000000001)
        , boundingRadius_(0.0f)
        , boundingRadiusUpToDate_(false)
        , transformedShapesUpToDate_(false)
    {}

    inline void CollisionBody2D::SetTangible(bool tangible)
    {
        tangible_ = tangible;
    }

    inline const bool CollisionBody2D::GetTangible() const
    {
        return tangible_;
    }

    inline void CollisionBody2D::SetCollisionGroupMask(uint32_t collisionGroupMask)
    {
        collisionGroupMask_ = collisionGroupMask;
    }

    inline uint32_t CollisionBody2D::GetCollisionGroupMask() const
    {
        return collisionGroupMask_;
    }

    inline size_t CollisionBody2D::AddPoint(const Point2D& point, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        points_.push_back(point);
        pointIds_.push_back(id);
        return points_.size() - 1;
    }

    inline const std::vector<Point2D>& CollisionBody2D::GetPoints() const
    {
        return points_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetPointIds() const
    {
        return pointIds_;
    }

    inline const std::vector<Point2D>& CollisionBody2D::GetTransformedPoints() const
    {
        return transformedPoints_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedPointIds() const
    {
        return transformedPointIds_;
    }

    inline size_t CollisionBody2D::AddLine(const Line2D& line, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        lines_.push_back(line);
        lineIds_.push_back(id);
        return lines_.size() - 1;
    }

    inline const std::vector<Line2D>& CollisionBody2D::GetLines() const
    {
        return lines_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetLineIds() const
    {
        return lineIds_;
    }

    inline const std::vector<Line2D>& CollisionBody2D::GetTransformedLines() const
    {
        return transformedLines_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedLineIds() const
    {
        return transformedLineIds_;
    }

    inline size_t CollisionBody2D::AddRay(const Ray2D& ray, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        rays_.push_back(ray);
        rayIds_.push_back(id);
        return rays_.size() - 1;
    }

    inline const std::vector<Ray2D>& CollisionBody2D::GetRays() const
    {
        return rays_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetRayIds() const
    {
        return rayIds_;
    }

    inline const std::vector<Ray2D>& CollisionBody2D::GetTransformedRays() const
    {
        return transformedRays_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedRayIds() const
    {
        return transformedRayIds_;
    }

    inline size_t CollisionBody2D::AddLineSegment(const LineSegment2D& lineSegment, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        lineSegments_.push_back(lineSegment);
        lineSegmentIds_.push_back(id);
        return lineSegments_.size() - 1;
    }

    inline const std::vector<LineSegment2D>& CollisionBody2D::GetLineSegments() const
    {
        return lineSegments_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetLineSegmentIds() const
    {
        return lineSegmentIds_;
    }

    inline const std::vector<LineSegment2D>& CollisionBody2D::GetTransformedLineSegments() const
    {
        return transformedLineSegments_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedLineSegmentIds() const
    {
        return transformedLineSegmentIds_;
    }

    inline size_t CollisionBody2D::AddRectangle(const Rectangle2D& rectangle, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        rectangles_.push_back(rectangle);
        rectangleIds_.push_back(id);
        return rectangles_.size() - 1;
    }

    inline const std::vector<Rectangle2D>& CollisionBody2D::GetRectangles() const
    {
        return rectangles_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetRectangleIds() const
    {
        return rectangleIds_;
    }

    inline const std::vector<Rectangle2D>& CollisionBody2D::GetTransformedRectangles() const
    {
        return transformedRectangles_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedRectangleIds() const
    {
        return transformedRectangleIds_;
    }

    inline size_t CollisionBody2D::AddOrientedRectangle(const OrientedRectangle2D& orientedRectangle, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        orientedRectangles_.push_back(orientedRectangle);
        orientedRectangleIds_.push_back(id);
        return orientedRectangles_.size() - 1;
    }

    inline const std::vector<OrientedRectangle2D>& CollisionBody2D::GetOrientedRectangles() const
    {
        return orientedRectangles_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetOrientedRectangleIds() const
    {
        return orientedRectangleIds_;
    }

    inline const std::vector<OrientedRectangle2D>& CollisionBody2D::GetTransformedOrientedRectangles() const
    {
        return transformedOrientedRectangles_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedOrientedRectangleIds() const
    {
        return transformedOrientedRectangleIds_;
    }

    inline size_t CollisionBody2D::AddCircle(const Circle2D& circle, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        circles_.push_back(circle);
        circleIds_.push_back(id);
        return circles_.size() - 1;
    }

    inline const std::vector<Circle2D>& CollisionBody2D::GetCircles() const
    {
        return circles_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetCircleIds() const
    {
        return circleIds_;
    }

    inline const std::vector<Circle2D>& CollisionBody2D::GetTransformedCircles() const
    {
        return transformedCircles_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedCircleIds() const
    {
        return transformedCircleIds_;
    }

    inline size_t CollisionBody2D::AddCapsule(const Capsule2D& capsule, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        capsules_.push_back(capsule);
        capsuleIds_.push_back(id);
        return capsules_.size() - 1;
    }

    inline const std::vector<Capsule2D>& CollisionBody2D::GetCapsules() const
    {
        return capsules_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetCapsuleIds() const
    {
        return capsuleIds_;
    }

    inline const std::vector<Capsule2D>& CollisionBody2D::GetTransformedCapsules() const
    {
        return transformedCapsules_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedCapsuleIds() const
    {
        return transformedCapsuleIds_;
    }

    inline size_t CollisionBody2D::AddTriangle(const Triangle2D& triangle, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        triangles_.push_back(triangle);
        triangleIds_.push_back(id);
        return triangles_.size() - 1;
    }

    inline const std::vector<Triangle2D>& CollisionBody2D::GetTriangles() const
    {
        return triangles_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTriangleIds() const
    {
        return triangleIds_;
    }

    inline const std::vector<Triangle2D>& CollisionBody2D::GetTransformedTriangles() const
    {
        return transformedTriangles_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedTriangleIds() const
    {
        return transformedTriangleIds_;
    }

    inline size_t CollisionBody2D::AddPolygon(const Polygon2D& polygon, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        polygons_.push_back(polygon);
        polygonIds_.push_back(id);
        return polygons_.size() - 1;
    }

    inline const std::vector<Polygon2D>& CollisionBody2D::GetPolygons() const
    {
        return polygons_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetPolygonIds() const
    {
        return polygonIds_;
    }

    inline const std::vector<Polygon2D>& CollisionBody2D::GetTransformedPolygons() const
    {
        return transformedPolygons_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedPolygonIds() const
    {
        return transformedPolygonIds_;
    }

    inline size_t CollisionBody2D::AddConvexPolygon(const ConvexPolygon2D& convexPolygon, unsigned int id)
    {
        boundingRadiusUpToDate_ = false;
        transformedShapesUpToDate_ = false;
        convexPolygons_.push_back(convexPolygon);
        convexPolygonIds_.push_back(id);
        return polygons_.size() - 1;
    }

    inline const std::vector<ConvexPolygon2D>& CollisionBody2D::GetConvexPolygons() const
    {
        return convexPolygons_;
    }

    inline const std::vector<ConvexPolygon2D>& CollisionBody2D::GetTransformedConvexPolygons() const
    {
        return transformedConvexPolygons_;
    }

    inline const std::vector<unsigned int>& CollisionBody2D::GetTransformedConvexPolygonIds() const
    {
        return transformedConvexPolygonIds_;
    }

    inline bool CollisionBody2D::BoundingRadiusUpToDate() const
    {
        return boundingRadiusUpToDate_;
    }

    inline bool CollisionBody2D::TransformedShapesUpToDate() const
    {
        return transformedShapesUpToDate_;
    }

    inline float CollisionBody2D::GetBoundingRadius() const
    {
        return boundingRadius_;
    }

    inline void CollisionBody2D::AddCollision(const CollisionData& collision)
    {
        collisions_.push_back(collision);
    }

    inline void CollisionBody2D::ClearCollisions()
    {
        collisions_.clear();
    }

    inline const std::vector<CollisionData>& CollisionBody2D::GetCollisions() const
    {
        return collisions_;
    }

    inline void CollisionBody2D::SetPosition(const glm::vec2& position)
    {
        transformedShapesUpToDate_ = false;
        Position2D::SetPosition(position);
    }

    inline void CollisionBody2D::SetPosition(float x, float y)
    {
        transformedShapesUpToDate_ = false;
        Position2D::SetPosition(x, y);
    }

    inline void CollisionBody2D::SetPositionX(float x)
    {
        transformedShapesUpToDate_ = false;
        Position2D::SetPositionX(x);
    }

    inline void CollisionBody2D::SetPositionY(float y)
    {
        transformedShapesUpToDate_ = false;
        Position2D::SetPositionY(y);
    }

    inline void CollisionBody2D::AddTranslation(const glm::vec2& translation)
    {
        transformedShapesUpToDate_ = false;
        Position2D::AddTranslation(translation);
    }

    inline void CollisionBody2D::AddTranslation(float x, float y)
    {
        transformedShapesUpToDate_ = false;
        Position2D::AddTranslation(x, y);
    }

    inline void CollisionBody2D::AddTranslationX(float x)
    {
        transformedShapesUpToDate_ = false;
        Position2D::AddTranslationX(x);
    }

    inline void CollisionBody2D::AddTranslationY(float y)
    {
        transformedShapesUpToDate_ = false;
        Position2D::AddTranslationY(y);
    }

    inline void CollisionBody2D::RevolveAround(const glm::vec2& focalPoint, float angleInRadians)
    {
        transformedShapesUpToDate_ = false;
        Position2D::RevolveAround(focalPoint, angleInRadians);
    }

    inline void CollisionBody2D::TranslateUp(float d)
    {
        transformedShapesUpToDate_ = false;
        Placement2D::TranslateUp(d);
    }

    inline void CollisionBody2D::TranslateRight(float d)
    {
        transformedShapesUpToDate_ = false;
        Placement2D::TranslateRight(d);
    }

    inline void CollisionBody2D::AddRotation(float angleInRadians)
    {
        transformedShapesUpToDate_ = false;
        Placement2D::AddRotation(angleInRadians);
    }

    inline void CollisionBody2D::SetRotation(float angleInRadians)
    {
        transformedShapesUpToDate_ = false;
        Placement2D::SetRotation(angleInRadians);
    }
}
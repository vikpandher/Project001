#include "CollisionBody.h"

#include "Engine/Math/Overlap2D.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    void CollisionBody::CalculateBoundingRadius()
    {
        float largestRadius = 0.0f;

        boundingRadius_ = largestRadius;
    }

    void CollisionBody::CalculateTransforedShapes()
    {
        transformedPoints_.clear();
        transformedLines_.clear();
        transformedLineSegments_.clear();
        transformedPlanes_.clear();
        transformedTriangles_.clear();
        transformedAxisAlignedBoundingBoxes_.clear();
        transformedOrientedBoundingBoxes_.clear();
        transformedSpheres_.clear();
    }

    void CollisionBody::CalculateCollision(CollisionBody& other)
    {
  
    }
}
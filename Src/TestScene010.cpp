#include "TestScene010.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/CollisionBody2D.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/Overlap2D.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Math/VectorUtilities.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"

#define _LOG_TEST(x) if ((x)) {_LOG_MESSAGE("%s %d --TEST-PASSED--", __FILENAME__, __LINE__);}\
else {_LOG_MESSAGE("%s %d --TEST-FAILED--*", __FILENAME__, __LINE__);} static_assert(true, "")

#define _LOG_TEST_QUIET(x) if (!(x))\
{_LOG_MESSAGE("%s %d --TEST-FAILED--*", __FILENAME__, __LINE__);} static_assert(true, "")



// public: ---------------------------------------------------------------------

TestScene010::TestScene010()
{
    Test_GetCameraRollPitchYaw();

    Test_CartesianToPolar();
    Test_PolarToCartesian();
    Test_Get2DVectorAngle();

    TestCollision2D();

    Test_Misc();
}

TestScene010::~TestScene010()
{}

const char* TestScene010::Name()
{
    return "TestScene010";
}

void TestScene010::Initialize()
{
    TestSceneBase002::Initialize();

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    for (int i = 1; i >= -1; --i)
    {
        for (int j = -4; j <= 3; ++j)
        {
            modelEntityPositions.emplace_back((float)j + 0.5f, (float)i, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // -------------------------------------------------------------------------

    // point 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.01f, 10));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddPoint(Project001::Point2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // line 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-100.0f, 0.0f);
        shapePoints.emplace_back(100.0f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLine(Project001::Line2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // lineSegment 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, -0.8f);
        shapePoints.emplace_back(0.48f, 0.8f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLineSegment(Project001::LineSegment2D(
            glm::vec2(-0.48f, -0.8f),
            glm::vec2(0.48f, 0.8f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // rectangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, 0.24f);
        shapePoints.emplace_back(-0.32f, -0.24f);
        shapePoints.emplace_back(0.32f, -0.24f);
        shapePoints.emplace_back(0.32f, 0.24f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddRectangle(Project001::Rectangle2D(
            glm::vec2(-0.32f, -0.24f),
            glm::vec2(0.32f, 0.24f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // orientedRectangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, 0.32f);
        shapePoints.emplace_back(-0.48f, -0.32f);
        shapePoints.emplace_back(0.48f, -0.32f);
        shapePoints.emplace_back(0.48f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::quarter_pi<float>());

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddOrientedRectangle(Project001::OrientedRectangle2D(
            glm::vec2(0.48f, 0.32f),
            glm::vec2(),
            glm::quarter_pi<float>()
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // circle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddCircle(Project001::Circle2D(
            glm::vec2(),
            0.32f
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // capsule 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.80f, 0.24f, 8));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddCapsule(Project001::Capsule2D(
            glm::vec2(0.0f, -0.40f),
            glm::vec2(0.0f, 0.40f),
            0.12f
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // triangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, -0.48f);
        shapePoints.emplace_back(0.48f, 0.0f);
        shapePoints.emplace_back(0.0f, 0.48f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddTriangle(Project001::Triangle2D(
            glm::vec2(-0.48f, -0.48f),
            glm::vec2(0.48f, 0.0f),
            glm::vec2(0.0f, 0.48f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // point 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.01f, 10));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddPoint(Project001::Point2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // line 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-100.0f, 0.0f);
        shapePoints.emplace_back(100.0f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLine(Project001::Line2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // lineSegment 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, 0.32f);
        shapePoints.emplace_back(0.32f, -0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLineSegment(Project001::LineSegment2D(
            glm::vec2(-0.32f, 0.32f),
            glm::vec2(0.32f, -0.32f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // rectangle 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.24f, 0.32f);
        shapePoints.emplace_back(-0.24f, -0.32f);
        shapePoints.emplace_back(0.24f, -0.32f);
        shapePoints.emplace_back(0.24f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddRectangle(Project001::Rectangle2D(
            glm::vec2(-0.24f, -0.32f),
            glm::vec2(0.24f, 0.32f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // orientedRectangle 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, 0.16f);
        shapePoints.emplace_back(-0.32f, -0.16f);
        shapePoints.emplace_back(0.32f, -0.16f);
        shapePoints.emplace_back(0.32f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, -1.0f * glm::quarter_pi<float>());

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddOrientedRectangle(Project001::OrientedRectangle2D(
            glm::vec2(0.32f, 0.16f),
            glm::vec2(),
            -1.0f * glm::quarter_pi<float>()
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // circle 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.24f, 24));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddCircle(Project001::Circle2D(
            glm::vec2(),
            0.24f
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // capsule 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.64f, 0.24f, 8));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::half_pi<float>());

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddCapsule(Project001::Capsule2D(
            glm::vec2(-0.32f, 0.0f),
            glm::vec2(0.32f, 0.0f),
            0.12f
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // triangle 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, 0.32f);
        shapePoints.emplace_back(0.32f, -0.32f);
        shapePoints.emplace_back(0.32f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddTriangle(Project001::Triangle2D(
            glm::vec2(-0.32f, 0.32f),
            glm::vec2(0.32f, -0.32f),
            glm::vec2(0.32f, 0.16f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // point 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.01f, 10));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddPoint(Project001::Point2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // line 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-100.0f, 0.0f);
        shapePoints.emplace_back(100.0f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLine(Project001::Line2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // lineSegment 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.16f, -0.16f);
        shapePoints.emplace_back(0.16f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLineSegment(Project001::LineSegment2D(
            glm::vec2(-0.16f, -0.16f),
            glm::vec2(0.16f, 0.16f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // rectangle 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.24f, 0.16f);
        shapePoints.emplace_back(-0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddRectangle(Project001::Rectangle2D(
            glm::vec2(-0.24f, -0.16f),
            glm::vec2(0.24f, 0.16f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // orientedRectangle 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.24f, 0.16f);
        shapePoints.emplace_back(-0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, 0.16f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::quarter_pi<float>());

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddOrientedRectangle(Project001::OrientedRectangle2D(
            glm::vec2(0.24f, 0.16f),
            glm::vec2(),
            glm::quarter_pi<float>()
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // circle 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.16f, 24));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddCircle(Project001::Circle2D(
            glm::vec2(),
            0.16f
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // capsule 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.48f, 0.16f, 8));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddCapsule(Project001::Capsule2D(
            glm::vec2(0.0f, -0.24f),
            glm::vec2(0.0f, 0.24f),
            0.08f
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // triangle 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, -0.32f);
        shapePoints.emplace_back(0.32f, 0.0f);
        shapePoints.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddTriangle(Project001::Triangle2D(
            glm::vec2(-0.32f, -0.32f),
            glm::vec2(0.32f, 0.0f),
            glm::vec2(0.0f, 0.32f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }
}

void TestScene010::Deinitialize()
{
    TestSceneBase002::Deinitialize();
}

void TestScene010::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene010::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase002::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene010::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene011"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene011"));
            }
        }
    }
}

// private: --------------------------------------------------------------------

void TestScene010::Test_GetCameraRollPitchYaw() const
{
    Project001::Camera testCamera;

    float _00_pit = testCamera.GetPitch();
    float _00_yaw = testCamera.GetYaw();
    float _00_rol = testCamera.GetRoll();

    // just pitch
    testCamera.AddPitch(glm::half_pi<float>());

    float _01_pit = testCamera.GetPitch();
    float _01_yaw = testCamera.GetYaw();
    float _01_rol = testCamera.GetRoll();

    testCamera.ResetOrientation();
    testCamera.AddPitch(glm::pi<float>());

    float _02_pit = testCamera.GetPitch();
    float _02_yaw = testCamera.GetYaw();
    float _02_rol = testCamera.GetRoll();

    testCamera.ResetOrientation();
    testCamera.AddPitch(glm::three_over_two_pi<float>());

    float _03_pit = testCamera.GetPitch();
    float _03_yaw = testCamera.GetYaw();
    float _03_rol = testCamera.GetRoll();

    testCamera.ResetOrientation();
    testCamera.AddPitch(glm::two_pi<float>());

    float _04_pit = testCamera.GetPitch();
    float _04_yaw = testCamera.GetYaw();
    float _04_rol = testCamera.GetRoll();

    // just yaw
    testCamera.ResetOrientation();
    testCamera.AddYaw(glm::half_pi<float>());

    float _05_pit = testCamera.GetPitch();
    float _05_yaw = testCamera.GetYaw();
    float _05_rol = testCamera.GetRoll();

    testCamera.ResetOrientation();
    testCamera.AddYaw(glm::pi<float>());

    float _06_pit = testCamera.GetPitch();
    float _06_yaw = testCamera.GetYaw();
    float _06_rol = testCamera.GetRoll();

    testCamera.ResetOrientation();
    testCamera.AddYaw(glm::three_over_two_pi<float>());

    float _07_pit = testCamera.GetPitch();
    float _07_yaw = testCamera.GetYaw();
    float _07_rol = testCamera.GetRoll();

    testCamera.ResetOrientation();
    testCamera.AddYaw(glm::two_pi<float>());

    float _08_pit = testCamera.GetPitch();
    float _08_yaw = testCamera.GetYaw();
    float _08_rol = testCamera.GetRoll();

    // just roll
    testCamera.ResetOrientation();
    testCamera.AddRoll(glm::half_pi<float>());

    float _09_pit = testCamera.GetPitch();
    float _09_yaw = testCamera.GetYaw();
    float _09_rol = testCamera.GetRoll();

    testCamera.ResetOrientation();
    testCamera.AddRoll(glm::pi<float>());

    float _10_pit = testCamera.GetPitch();
    float _10_yaw = testCamera.GetYaw();
    float _10_rol = testCamera.GetRoll();

    testCamera.ResetOrientation();
    testCamera.AddRoll(glm::three_over_two_pi<float>());

    float _11_pit = testCamera.GetPitch();
    float _11_yaw = testCamera.GetYaw();
    float _11_rol = testCamera.GetRoll();

    testCamera.ResetOrientation();
    testCamera.AddRoll(glm::two_pi<float>());

    float _12_pit = testCamera.GetPitch();
    float _12_yaw = testCamera.GetYaw();
    float _12_rol = testCamera.GetRoll();
}

void TestScene010::Test_CartesianToPolar() const
{
    glm::vec2 input00(0.0f, 0.0f);
    glm::vec2 output00 = Project001::CartesianToPolar(input00);

    glm::vec2 input01(1.0f, 0.0f);
    glm::vec2 output01 = Project001::CartesianToPolar(input01);

    glm::vec2 input02(1.0f, 1.0f);
    glm::vec2 output02 = Project001::CartesianToPolar(input02);

    glm::vec2 input03(0.0f, 1.0f);
    glm::vec2 output03 = Project001::CartesianToPolar(input03);

    glm::vec2 input04(-1.0f, 1.0f);
    glm::vec2 output04 = Project001::CartesianToPolar(input04);

    glm::vec2 input05(-1.0f, 0.0f);
    glm::vec2 output05 = Project001::CartesianToPolar(input05);

    glm::vec2 input06(-1.0f, -1.0f);
    glm::vec2 output06 = Project001::CartesianToPolar(input06);

    glm::vec2 input07(0.0f, -1.0f);
    glm::vec2 output07 = Project001::CartesianToPolar(input07);

    glm::vec2 input08(1.0f, -1.0f);
    glm::vec2 output08 = Project001::CartesianToPolar(input08);
}

void TestScene010::Test_PolarToCartesian() const
{
    glm::vec2 input00(0.0f, 0.0f);
    glm::vec2 output00 = Project001::PolarToCartesian(input00);

    glm::vec2 input01(1.0f, 0.0f);
    glm::vec2 output01 = Project001::PolarToCartesian(input01);

    glm::vec2 input02(1.0f, glm::quarter_pi<float>());
    glm::vec2 output02 = Project001::PolarToCartesian(input02);

    glm::vec2 input03(1.0f, glm::half_pi<float>());
    glm::vec2 output03 = Project001::PolarToCartesian(input03);

    glm::vec2 input04(1.0f, glm::half_pi<float>() + glm::quarter_pi<float>());
    glm::vec2 output04 = Project001::PolarToCartesian(input04);

    glm::vec2 input05(1.0f, glm::pi<float>());
    glm::vec2 output05 = Project001::PolarToCartesian(input05);

    glm::vec2 input06(1.0f, glm::pi<float>() + glm::quarter_pi<float>());
    glm::vec2 output06 = Project001::PolarToCartesian(input06);

    glm::vec2 input07(1.0f, glm::pi<float>() + glm::half_pi<float>());
    glm::vec2 output07 = Project001::PolarToCartesian(input07);

    glm::vec2 input08(1.0f, glm::pi<float>() + glm::half_pi<float>() + glm::quarter_pi<float>());
    glm::vec2 output08 = Project001::PolarToCartesian(input08);

    glm::vec2 input09(1.0f, glm::two_pi<float>() + glm::pi<float>());
    glm::vec2 output09 = Project001::PolarToCartesian(input09);
}

void TestScene010::Test_Get2DVectorAngle() const
{
    glm::vec2 vector00(0.0f, 0.0f);
    glm::vec2 vector01(1.0f, 0.0f);
    glm::vec2 vector02(1.0f, 1.0f);
    glm::vec2 vector03(0.0f, 1.0f);
    glm::vec2 vector04(-1.0f, 1.0f);
    glm::vec2 vector05(-1.0f, 0.0f);
    glm::vec2 vector06(-1.0f, -1.0f);
    glm::vec2 vector07(0.0f, -1.0f);
    glm::vec2 vector08(1.0f, -1.0f);
    float angle0000 = Project001::Get2DVectorAngle(vector00, vector00);
    float angle0001 = Project001::Get2DVectorAngle(vector00, vector01);
    float angle0003 = Project001::Get2DVectorAngle(vector00, vector03);
    float angle0101 = Project001::Get2DVectorAngle(vector01, vector01);
    float angle0102 = Project001::Get2DVectorAngle(vector01, vector02);
    float angle0201 = Project001::Get2DVectorAngle(vector02, vector01);
    float angle0103 = Project001::Get2DVectorAngle(vector01, vector03);
    float angle0104 = Project001::Get2DVectorAngle(vector01, vector04);
    float angle0105 = Project001::Get2DVectorAngle(vector01, vector05);
    float angle0106 = Project001::Get2DVectorAngle(vector01, vector06);
    float angle0107 = Project001::Get2DVectorAngle(vector01, vector07);
    float angle0108 = Project001::Get2DVectorAngle(vector01, vector08);
}

void TestScene010::TestCollision2D() const
{
    Test_Get2D_Point_Line_Distance();
    Test_Get2D_Point_LineSegment_Distance();
    Test_RotateSlope();
}

void TestScene010::Test_Get2D_Point_Line_Distance() const
{
    glm::vec2 point_p0_p0(0.0f, 0.0f);
    glm::vec2 point_p1_p0(1.0f, 0.0f);
    glm::vec2 point_p1_p1(1.0f, 1.0f);
    glm::vec2 point_p0_p1(0.0f, 1.0f);
    glm::vec2 point_n1_p1(-1.0f, 1.0f);
    glm::vec2 point_n1_p0(-1.0f, 0.0f);
    glm::vec2 point_n1_n1(-1.0f, -1.0f);
    glm::vec2 point_p0_n1(0.0f, -1.0f);
    glm::vec2 point_p1_n1(1.0f, -1.0f);
    glm::vec2 point_p2_p0(2.0f, 0.0f);
    glm::vec2 point_p2_p2(2.0f, 2.0f);
    glm::vec2 point_p0_p2(0.0f, 2.0f);
    glm::vec2 point_n2_p2(-2.0f, 2.0f);
    glm::vec2 point_n2_p0(-2.0f, 0.0f);
    glm::vec2 point_n2_n2(-2.0f, -2.0f);
    glm::vec2 point_p0_n2(0.0f, -2.0f);
    glm::vec2 point_p2_n2(2.0f, -2.0f);

    float distance01 = Project001::Get2D_Point_Line_DistanceSquared(point_p1_p0, point_n1_p0, INFINITY);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance01, 4.0f));

    float distance02 = Project001::Get2D_Point_Line_DistanceSquared(point_p1_p0, point_p1_p0, INFINITY);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance02, 0.0f));

    float distance03 = Project001::Get2D_Point_Line_DistanceSquared(point_p1_p0, point_n1_p1, 0.0f);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance03, 1.0f));

    float distance04 = Project001::Get2D_Point_Line_DistanceSquared(point_n1_n1, point_p0_n1, 0.0f);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance04, 0.0f));

    float distance05 = Project001::Get2D_Point_Line_DistanceSquared(point_n1_p1, point_p0_p0, 1.0f);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance05, 2.0f));

    float distance06 = Project001::Get2D_Point_Line_DistanceSquared(point_p0_p0, point_n1_p1, 1.0f);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance06, 2.0f));

    float distance07 = Project001::Get2D_Point_Line_DistanceSquared(point_n1_p1, point_p0_p0, -1.0f);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance07, 0.0f));

    float distance08 = Project001::Get2D_Point_Line_DistanceSquared(point_n1_n1, point_p0_p0, -1.0f);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance08, 2.0f));

    float distance09 = Project001::Get2D_Point_Line_DistanceSquared(point_p1_p0, point_p0_p0, 2.0f);
    float correctDistance09 = std::sinf(std::atanf(2.0f));
    correctDistance09 *= correctDistance09;
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance09, correctDistance09));
}

void TestScene010::Test_Get2D_Point_LineSegment_Distance() const
{
    glm::vec2 point_p0_p0(0.0f, 0.0f);
    glm::vec2 point_p1_p0(1.0f, 0.0f);
    glm::vec2 point_p1_p1(1.0f, 1.0f);
    glm::vec2 point_p0_p1(0.0f, 1.0f);
    glm::vec2 point_n1_p1(-1.0f, 1.0f);
    glm::vec2 point_n1_p0(-1.0f, 0.0f);
    glm::vec2 point_n1_n1(-1.0f, -1.0f);
    glm::vec2 point_p0_n1(0.0f, -1.0f);
    glm::vec2 point_p1_n1(1.0f, -1.0f);
    glm::vec2 point_p2_p0(2.0f, 0.0f);
    glm::vec2 point_p2_p2(2.0f, 2.0f);
    glm::vec2 point_p0_p2(0.0f, 2.0f);
    glm::vec2 point_n2_p2(-2.0f, 2.0f);
    glm::vec2 point_n2_p0(-2.0f, 0.0f);
    glm::vec2 point_n2_n2(-2.0f, -2.0f);
    glm::vec2 point_p0_n2(0.0f, -2.0f);
    glm::vec2 point_p2_n2(2.0f, -2.0f);

    float distance01 = Project001::Get2D_Point_LineSegment_DistanceSquared(point_p1_p0, point_n1_n1, point_n1_p0);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance01, 4.0f));

    float distance02 = Project001::Get2D_Point_LineSegment_DistanceSquared(point_p1_p1, point_p0_n1, point_p0_p0);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance02, 2.0f));

    float distance03 = Project001::Get2D_Point_LineSegment_DistanceSquared(point_n2_p2, point_p0_n1, point_p0_p0);
    _LOG_TEST_QUIET(Project001::FloatEqualToFloat(distance03, 8.0f));
}

void TestScene010::Test_RotateSlope() const
{
    float slope01 = 0.0f;
    float rotation01 = glm::pi<float>();
    float result01 = Project001::RotateSlope(slope01, rotation01);
}

void TestScene010::Test_Misc() const
{
    glm::vec2 testVec(3.0f, 4.0f);
    float length = glm::length(testVec);

    float infinity = INFINITY;
    infinity += 1;
    infinity *= 2;
    infinity /= 0;
    infinity *= 0;

    float infinity2 = 1.0f;
    infinity2 /= 0.0;

    std::vector<int> testVector;
    size_t testVectorSize = sizeof(testVector);
    size_t intSize = sizeof(int);
    size_t pointerSize = sizeof(int*);

    glm::mat3 mat000(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
    float det000 = glm::determinant(mat000);

    glm::vec2 test01;
    Project001::Get2D_Line_Line_Intersection_H(glm::vec2(0.0f, 0.0f), 1.0f, glm::vec2(1.0f, 1.0f), 1.0f, test01);

    glm::vec3 test02(0.0f, 0.0f, 0.0f);
    test02 = glm::normalize(test02);
}

#include "TestScene004.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/CollisionBody2D.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/Overlap2D.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Math/FloatsEqual.h"
#include "Engine/Math/VectorAngles.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"

#define _LOG_TEST(x) if ((x)) {_LOG_MESSAGE("%s %d --TEST-PASSED--", __FILENAME__, __LINE__);}\
else {_LOG_MESSAGE("%s %d --TEST-FAILED---", __FILENAME__, __LINE__);} static_assert(true, "")



// public: ---------------------------------------------------------------------

TestScene004::TestScene004()
{
    ClearIndiciesAndEntityIds();

    Test_CartesianToPolar();
    Test_PolarToCartesian();
    Test_Get2DVectorAngle();

    TestCollision2D();

    Test_Misc();
}

TestScene004::~TestScene004()
{}

const char* TestScene004::Name()
{
    return "TestScene004";
}

void TestScene004::Initialize()
{
    TestSceneFramework::Initialize();

    rendererPtr_->SetDepthTesting(false);

    // main camera entity
    // -------------------------------------------------------------------------
    {
        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        int framebufferWidth;
        int framebufferHeight;
        windowPtr_->GetFramebufferSize(framebufferWidth, framebufferHeight);
        float aspectRatio = (float)framebufferWidth / (float)framebufferHeight;
        cameraPtr->SetProjectionToOrthographic();
        cameraPtr->SetTopCutoff(3.5f);
        cameraPtr->SetBottomCutoff(-3.5f);
        cameraPtr->SetLeftCutoff(aspectRatio * -3.5f);
        cameraPtr->SetRightCutoff(aspectRatio * 3.5f);
        // cameraPtr->TurnOn();
    }

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
        unsigned int tempMeshIndex;
        _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(tempMeshIndex, 0.01f, 10));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddPoint(Project001::Point2D());
        collisionBody2DPtr->CalculateBoundingRadius();

        selectedEntityIdIndex_ = 0;
    }

    // line 1
    {
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-100.0f, 0.0f);
        shapePoints.emplace_back(100.0f, 0.0f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(tempMeshIndex, shapePoints, 0.01f));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLine(Project001::Line2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // lineSegment 1
    {
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, -0.8f);
        shapePoints.emplace_back(0.48f, 0.8f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(tempMeshIndex, shapePoints, 0.01f));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, 0.24f);
        shapePoints.emplace_back(-0.32f, -0.24f);
        shapePoints.emplace_back(0.32f, -0.24f);
        shapePoints.emplace_back(0.32f, 0.24f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleFan(tempMeshIndex, shapePoints, shapePoints));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, 0.32f);
        shapePoints.emplace_back(-0.48f, -0.32f);
        shapePoints.emplace_back(0.48f, -0.32f);
        shapePoints.emplace_back(0.48f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleFan(tempMeshIndex, shapePoints, shapePoints));
        _FAIL_CHECK(meshStoresPtr_->RotateMeshZ(tempMeshIndex, glm::quarter_pi<float>()));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(tempMeshIndex, 0.32f, 24));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        _FAIL_CHECK(meshStoresPtr_->Generate2DCapsule(tempMeshIndex, 0.80f, 0.24f, 8));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, -0.48f);
        shapePoints.emplace_back(0.48f, 0.0f);
        shapePoints.emplace_back(0.0f, 0.48f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleFan(tempMeshIndex, shapePoints, shapePoints));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(tempMeshIndex, 0.01f, 10));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddPoint(Project001::Point2D());
        collisionBody2DPtr->CalculateBoundingRadius();

        selectedEntityIdIndex_ = 0;
    }

    // line 2
    {
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-100.0f, 0.0f);
        shapePoints.emplace_back(100.0f, 0.0f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(tempMeshIndex, shapePoints, 0.01f));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLine(Project001::Line2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // lineSegment 2
    {
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, 0.32f);
        shapePoints.emplace_back(0.32f, -0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(tempMeshIndex, shapePoints, 0.01f));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.24f, 0.32f);
        shapePoints.emplace_back(-0.24f, -0.32f);
        shapePoints.emplace_back(0.24f, -0.32f);
        shapePoints.emplace_back(0.24f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleFan(tempMeshIndex, shapePoints, shapePoints));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, 0.16f);
        shapePoints.emplace_back(-0.32f, -0.16f);
        shapePoints.emplace_back(0.32f, -0.16f);
        shapePoints.emplace_back(0.32f, 0.16f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleFan(tempMeshIndex, shapePoints, shapePoints));
        _FAIL_CHECK(meshStoresPtr_->RotateMeshZ(tempMeshIndex, -1.0f * glm::quarter_pi<float>()));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(tempMeshIndex, 0.24f, 24));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        _FAIL_CHECK(meshStoresPtr_->Generate2DCapsule(tempMeshIndex, 0.64f, 0.24f, 8));
        _FAIL_CHECK(meshStoresPtr_->RotateMeshZ(tempMeshIndex, glm::half_pi<float>()));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, 0.32f);
        shapePoints.emplace_back(0.32f, -0.32f);
        shapePoints.emplace_back(0.32f, 0.16f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleFan(tempMeshIndex, shapePoints, shapePoints));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(tempMeshIndex, 0.01f, 10));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddPoint(Project001::Point2D());
        collisionBody2DPtr->CalculateBoundingRadius();

        selectedEntityIdIndex_ = 0;
    }

    // line 3
    {
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-100.0f, 0.0f);
        shapePoints.emplace_back(100.0f, 0.0f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(tempMeshIndex, shapePoints, 0.01f));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLine(Project001::Line2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // lineSegment 3
    {
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.16f, -0.16f);
        shapePoints.emplace_back(0.16f, 0.16f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DLine(tempMeshIndex, shapePoints, 0.01f));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.24f, 0.16f);
        shapePoints.emplace_back(-0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, 0.16f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleFan(tempMeshIndex, shapePoints, shapePoints));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.24f, 0.16f);
        shapePoints.emplace_back(-0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, 0.16f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleFan(tempMeshIndex, shapePoints, shapePoints));
        _FAIL_CHECK(meshStoresPtr_->RotateMeshZ(tempMeshIndex, glm::quarter_pi<float>()));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        _FAIL_CHECK(meshStoresPtr_->Generate2DRegularPolygon(tempMeshIndex, 0.16f, 24));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        _FAIL_CHECK(meshStoresPtr_->Generate2DCapsule(tempMeshIndex, 0.48f, 0.16f, 8));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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
        unsigned int tempMeshIndex;
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, -0.32f);
        shapePoints.emplace_back(0.32f, 0.0f);
        shapePoints.emplace_back(0.0f, 0.32f);
        _FAIL_CHECK(meshStoresPtr_->Generate2DTriangleFan(tempMeshIndex, shapePoints, shapePoints));
        meshIndicies_.push_back(tempMeshIndex);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshIndex(tempMeshIndex);

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

void TestScene004::Deinitialize()
{
    TestSceneFramework::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene004::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene004::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene004::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestScene004::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene004::ProcessUpdateEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::FrameBufferSizeEvent>(event, std::bind(&TestScene004::TestSceneFramework::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene004::TestSceneFramework::ProcessRenderEvent, this, std::placeholders::_1));
}

// protected: ------------------------------------------------------------------

void TestScene004::ClearIndiciesAndEntityIds()
{
    meshIndicies_.clear();
    entityIds_.clear();

    selectedEntityIdIndex_ = (unsigned int)-1;
}

void TestScene004::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene001"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene001"));
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_N)
        {
            selectedEntityIdIndex_++;
            if (selectedEntityIdIndex_ >= entityIds_.size())
            {
                selectedEntityIdIndex_ = 0;
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_B)
        {
            if (selectedEntityIdIndex_ == 0)
            {
                selectedEntityIdIndex_ = entityIds_.size() - 1;
            }
            else
            {
                selectedEntityIdIndex_--;
            }
        }
    }
}

void TestScene004::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1 &&
        buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
    {
        float xPosition, yPosition;
        windowPtr_->GetCursorPosition(xPosition, yPosition);

        int windowWidth, windowHeight;
        windowPtr_->GetWindowSize(windowWidth, windowHeight);

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        const float& topCutoff = cameraPtr->GetTopCutoff();
        const float& bottomCutoff = cameraPtr->GetBottomCutoff();
        const float& leftCutoff = cameraPtr->GetLeftCutoff();
        const float& rightCutoff = cameraPtr->GetRightCutoff();

        xPosition = (xPosition / (float)windowWidth - 0.5f) * (rightCutoff - leftCutoff);
        yPosition = (((float)windowHeight - yPosition) / (float)windowHeight - 0.5f) * (topCutoff - bottomCutoff);

        Project001::Point2D touchPoint(glm::vec2(xPosition, yPosition));

        Project001::CollisionBody2D* collisionBody2DArray = nullptr;
        size_t collisionBodyCount = 0;

        _FAIL_CHECK(componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount));

        for (unsigned int i = 0; i < collisionBodyCount; ++i)
        {
            Project001::CollisionBody2D& currentCollisionBody2D = collisionBody2DArray[i];
            if (currentCollisionBody2D.GetCollision(touchPoint))
            {
                unsigned int entityId;
                _FAIL_CHECK(componentStoresPtr_->GetComponentEntityId(&currentCollisionBody2D, entityId));

                for (size_t i = 0; i < entityIds_.size(); ++i)
                {
                    if (entityId == entityIds_[i])
                    {
                        selectedEntityIdIndex_ = i;
                    }
                }
            }
        }
    }
}

void TestScene004::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.1f;
    float cameraResize = 1.0f + speedConstant * yOffset;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    float newTopCutoff = cameraPtr->GetTopCutoff() * cameraResize;
    float newBottomCutoff = cameraPtr->GetBottomCutoff() * cameraResize;
    float newLeftCutoff = cameraPtr->GetLeftCutoff() * cameraResize;
    float newRightCutoff = cameraPtr->GetRightCutoff() * cameraResize;

    if (newTopCutoff > 0.1f && newTopCutoff < 10.0f)
    {
        cameraPtr->SetTopCutoff(newTopCutoff);
        cameraPtr->SetBottomCutoff(newBottomCutoff);
        cameraPtr->SetLeftCutoff(newLeftCutoff);
        cameraPtr->SetRightCutoff(newRightCutoff);
    }
}

void TestScene004::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long timestep_ns = updateEvent.timestep_ns;

    UpdatedSelectedEntityPosition(timestep_ns);
    SyncCollisionComponents();

    DetectCollisions();
}

void TestScene004::UpdatedSelectedEntityPosition(unsigned long timestep_ns)
{
    float timestep_s = (float)(timestep_ns / 1000000) / 1000;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    float speedConstant = cameraPtr->GetTopCutoff();

    float cameraTranslationSpeed = speedConstant * timestep_s;
    float cameraRotationSpeed = speedConstant * 2.0f * timestep_s;

    bool movingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
    bool movingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
    bool movingUp = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
    bool movingDown = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
    bool rollingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
    bool rollingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

    unsigned int selectedEntityId = entityIds_[selectedEntityIdIndex_];

    Project001::RenderedModel* renderedModelPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(selectedEntityId, renderedModelPtr));

    if (movingLeft)
    {
        renderedModelPtr->AddTranslationX(-1.0f * cameraTranslationSpeed);
    }

    if (movingRight)
    {
        renderedModelPtr->AddTranslationX(cameraTranslationSpeed);
    }

    if (movingUp)
    {
        renderedModelPtr->AddTranslationY(cameraTranslationSpeed);
    }

    if (movingDown)
    {
        renderedModelPtr->AddTranslationY(-1.0f * cameraTranslationSpeed);
    }

    // if (rollingLeft)
    // {
    //     renderedModelPtr->AddWorldRotationZ(cameraRotationSpeed);
    // }

    // if (rollingRight)
    // {
    //     renderedModelPtr->AddWorldRotationZ(-1.0f * cameraRotationSpeed);
    // }
}

void TestScene004::DetectCollisions()
{
    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    _FAIL_CHECK(componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount));

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& currentCollisionBody2D = collisionBody2DArray[i];
        currentCollisionBody2D.CalculateTransforedShapes();
        currentCollisionBody2D.SetColliding(false);
    }

    for (unsigned int i = 0; i < collisionBodyCount - 1; ++i)
    {
        Project001::CollisionBody2D& firstCollisionBody2D = collisionBody2DArray[i];
        for (unsigned int j = i + 1; j < collisionBodyCount; ++j)
        {
            Project001::CollisionBody2D& secondCollisionBody2D = collisionBody2DArray[j];
            firstCollisionBody2D.CalculateCollision(secondCollisionBody2D);
        }
    }

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        unsigned int entityId;
        _FAIL_CHECK(componentStoresPtr_->GetComponentEntityId(&collisionBody2D, entityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(entityId, renderedModelPtr));

        if (collisionBody2D.GetColliding())
        {
            if (entityId == entityIds_[selectedEntityIdIndex_])
            {
                renderedModelPtr->SetColorRGB(1.0f, 0.75f, 0.25f);
            }
            else
            {
                renderedModelPtr->SetColorRGB(1.0f, 0.25f, 0.25f);
            }
        }
        else
        {
            if (entityId == entityIds_[selectedEntityIdIndex_])
            {
                renderedModelPtr->SetColorRGB(0.25f, 0.25f, 1.0f);
            }
            else
            {
                renderedModelPtr->SetColorRGB(1.0f, 1.0f, 1.0f);
            }
        }
    }
}

void TestScene004::SyncCollisionComponents()
{
    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    _FAIL_CHECK(componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount));

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        unsigned int entityId;
        _FAIL_CHECK(componentStoresPtr_->GetComponentEntityId(&collisionBody2D, entityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(entityId, renderedModelPtr));

        collisionBody2D.SetPosition(renderedModelPtr->GetPosition());
    }
}

// private: --------------------------------------------------------------------

void TestScene004::Test_CartesianToPolar() const
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

void TestScene004::Test_PolarToCartesian() const
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

void TestScene004::Test_Get2DVectorAngle() const
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

void TestScene004::TestCollision2D() const
{
    Test_Get2D_Point_Line_Distance();
    Test_Get2D_Point_LineSegment_Distance();
    Test_RotateSlope();
}

void TestScene004::Test_Get2D_Point_Line_Distance() const
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

    float distance01 = Project001::Get2D_Point_Line_Distance(point_p1_p0, point_n1_p0, INFINITY);
    _LOG_TEST(Project001::FloatsEqual(distance01, 2.0f));

    float distance02 = Project001::Get2D_Point_Line_Distance(point_p1_p0, point_p1_p0, INFINITY);
    _LOG_TEST(Project001::FloatsEqual(distance02, 0.0f));

    float distance03 = Project001::Get2D_Point_Line_Distance(point_p1_p0, point_n1_p1, 0.0f);
    _LOG_TEST(Project001::FloatsEqual(distance03, 1.0f));

    float distance04 = Project001::Get2D_Point_Line_Distance(point_n1_n1, point_p0_n1, 0.0f);
    _LOG_TEST(Project001::FloatsEqual(distance04, 0.0f));

    float distance05 = Project001::Get2D_Point_Line_Distance(point_n1_p1, point_p0_p0, 1.0f);
    _LOG_TEST(Project001::FloatsEqual(distance05, std::sqrtf(2.0f)));

    float distance06 = Project001::Get2D_Point_Line_Distance(point_p0_p0, point_n1_p1, 1.0f);
    _LOG_TEST(Project001::FloatsEqual(distance06, std::sqrtf(2.0f)));

    float distance07 = Project001::Get2D_Point_Line_Distance(point_n1_p1, point_p0_p0, -1.0f);
    _LOG_TEST(Project001::FloatsEqual(distance07, 0.0f));

    float distance08 = Project001::Get2D_Point_Line_Distance(point_n1_n1, point_p0_p0, -1.0f);
    _LOG_TEST(Project001::FloatsEqual(distance08, std::sqrtf(2.0f)));

    float distance09 = Project001::Get2D_Point_Line_Distance(point_p1_p0, point_p0_p0, 2.0f);
    _LOG_TEST(Project001::FloatsEqual(distance09, std::sinf(std::atanf(2.0f))));
}

void TestScene004::Test_Get2D_Point_LineSegment_Distance() const
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

    float distance01 = Project001::Get2D_Point_LineSegment_Distance(point_p1_p0, point_n1_n1, point_n1_p0);
    _LOG_TEST(Project001::FloatsEqual(distance01, 2.0f));

    float distance02 = Project001::Get2D_Point_LineSegment_Distance(point_p1_p1, point_p0_n1, point_p0_p0);
    _LOG_TEST(Project001::FloatsEqual(distance02, std::sqrt(2.0f)));

    float distance03 = Project001::Get2D_Point_LineSegment_Distance(point_n2_p2, point_p0_n1, point_p0_p0);
    _LOG_TEST(Project001::FloatsEqual(distance03, std::sqrtf(8.0f)));
}

void TestScene004::Test_RotateSlope() const
{
    float slope01 = 0.0f;
    float rotation01 = glm::pi<float>();
    float result01 = Project001::RotateSlope(slope01, rotation01);
}

void TestScene004::Test_Misc() const
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
}

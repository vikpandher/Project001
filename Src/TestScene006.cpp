#include "TestScene006.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/CollisionBody2D.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/Overlap2D.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Math/VectorAngles.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene006::TestScene006()
    : cursorGrabbingEntity_(false)
    , previousCursorDownPosition_(0.0f, 0.0f)
{
    ClearIndiciesAndEntityIds();
}

TestScene006::~TestScene006()
{}

const char* TestScene006::Name()
{
    return "TestScene006";
}

void TestScene006::Initialize()
{
    windowPtr_ = GetApplicationWindowPtr();

    componentStoresPtr_ = GetApplicationComponentStoresPtr();
    meshStoresPtr_ = GetApplicationMeshStoresPtr();

    rendererPtr_ = GetApplicationRendererPtr();
    rendererPtr_->SetDepthTesting(false);

    int windowWidth, windowHeight;
    windowPtr_->GetWindowSize(windowWidth, windowHeight);
    windowPtr_->SetAspectRatio(windowWidth, windowHeight);

    // main camera entity
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(mainCameraEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(mainCameraEntityId_));

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        int framebufferWidth;
        int framebufferHeight;
        windowPtr_->GetFramebufferSize(framebufferWidth, framebufferHeight);
        float aspectRatio = (float)framebufferWidth / (float)framebufferHeight;
        cameraPtr->SetAspectRatio(aspectRatio);
        cameraPtr->SetPosition(0.0f, 0.0f, 5.0f);
        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->SetProjectionToOrthographic();
        cameraPtr->SetTopCutoff(3.5f);
        cameraPtr->SetBottomCutoff(-3.5f);
        cameraPtr->SetLeftCutoff(aspectRatio * -3.5f);
        cameraPtr->SetRightCutoff(aspectRatio * 3.5f);
        cameraPtr->TurnOn();
    }

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    for (int i = 1; i >= -1; --i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            modelEntityPositions.emplace_back((float)j, (float)i, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // -------------------------------------------------------------------------

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

void TestScene006::Deinitialize()
{
    componentStoresPtr_->DeleteAllEntities();
    meshStoresPtr_->ClearMeshes();
    rendererPtr_->ClearTextures();

    ClearIndiciesAndEntityIds();
}

void TestScene006::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene006::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::FrameBufferSizeEvent>(event, std::bind(&TestScene006::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene006::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene006::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene006::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestScene006::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene006::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected: ------------------------------------------------------------------

void TestScene006::ClearIndiciesAndEntityIds()
{
    selectedEntityIdIndex_ = (unsigned int)-1;
    meshIndicies_.clear();

    mainCameraEntityId_ = (unsigned int)-1;
    entityIds_.clear();
}

void TestScene006::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorButtonEvent)
{
    if (cursorGrabbingEntity_)
    {
        glm::vec2 currentPosition(cursorButtonEvent.xPosition, cursorButtonEvent.yPosition);

        int windowWidth, windowHeight;
        windowPtr_->GetWindowSize(windowWidth, windowHeight);

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        currentPosition = cameraPtr->ConvertPointFromWindowToOrthoWorld(windowWidth, windowHeight, currentPosition);

        float xOffset = currentPosition.x - previousCursorDownPosition_.x;
        float yOffset = currentPosition.y - previousCursorDownPosition_.y;

        if (selectedEntityIdIndex_ < entityIds_.size())
        {
            unsigned int selectedEntityId = entityIds_[selectedEntityIdIndex_];

            Project001::CollisionBody2D* collisionBody2DPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(selectedEntityId, collisionBody2DPtr));
            collisionBody2DPtr->AddTranslationX(xOffset);
            collisionBody2DPtr->AddTranslationY(yOffset);
        }

        previousCursorDownPosition_.x = currentPosition.x;
        previousCursorDownPosition_.y = currentPosition.y;
    }

    cursorButtonEvent.handled = true;
}

void TestScene006::ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent)
{
    const int& height = frameBufferSizeEvent.height;
    const int& width = frameBufferSizeEvent.width;

    int aspectRatioNumerator, aspectRatioDenominator;
    windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);

    if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
    {
        float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;

        int adjustedHeight = (int)(width / aspectRatio);
        int adjustedWidth = (int)(height * aspectRatio);

        if (adjustedWidth > width)
        {
            adjustedWidth = width;
        }

        if (adjustedHeight > height)
        {
            adjustedHeight = height;
        }

        int lowerLeftX = (width - adjustedWidth) / 2;
        int lowerLeftY = (height - adjustedHeight) / 2;

        rendererPtr_->SetFramebufferSize(adjustedWidth, adjustedHeight);
        rendererPtr_->SetViewportSize(lowerLeftX, lowerLeftY, adjustedWidth, adjustedHeight);
    }
    else
    {
        rendererPtr_->SetFramebufferSize(width, height);
        rendererPtr_->SetViewportSize(0, 0, width, height);
    }

    frameBufferSizeEvent.handled = true;
}

void TestScene006::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene007"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene007"));
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
                selectedEntityIdIndex_ = (unsigned int)entityIds_.size() - 1;
            }
            else if (selectedEntityIdIndex_ >= entityIds_.size())
            {
                selectedEntityIdIndex_ = (unsigned int)entityIds_.size() - 1;
            }
            else
            {
                selectedEntityIdIndex_--;
            }
        }
    }
}

void TestScene006::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    cursorGrabbingEntity_ = false;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1 &&
        buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
    {
        selectedEntityIdIndex_ = (unsigned int)-1;

        windowPtr_->GetCursorPosition(previousCursorDownPosition_.x, previousCursorDownPosition_.y);

        int windowWidth, windowHeight;
        windowPtr_->GetWindowSize(windowWidth, windowHeight);

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        previousCursorDownPosition_ = cameraPtr->ConvertPointFromWindowToOrthoWorld(windowWidth, windowHeight, previousCursorDownPosition_);

        Project001::CollisionBody2D* collisionBody2DArray = nullptr;
        size_t collisionBodyCount = 0;

        _FAIL_CHECK(componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount));

        // This loop goes backwards so I grab the component drawn last. This
        // only works that way because I added the components and render bodies
        // in the same order and haven't removed any.
        for (int i = (int)collisionBodyCount - 1; i >= 0; --i)
        {
            Project001::CollisionBody2D& currentCollisionBody2D = collisionBody2DArray[i];
            if (currentCollisionBody2D.GetCollision(previousCursorDownPosition_))
            {
                unsigned int entityId;
                _FAIL_CHECK(componentStoresPtr_->GetComponentEntityId(&currentCollisionBody2D, entityId));

                for (unsigned int i = 0; i < entityIds_.size(); ++i)
                {
                    if (entityId == entityIds_[i])
                    {
                        selectedEntityIdIndex_ = i;
                    }
                }

                cursorGrabbingEntity_ = true;
                break;
            }
        }
    }
}

void TestScene006::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    rendererPtr_->ClearDirectionalLight();
    rendererPtr_->ClearPointLights();
    rendererPtr_->ClearSpotLights();

    rendererPtr_->ClearLocalBuffers();

    Project001::RenderedModel* renderedModelArray = nullptr;
    size_t renderedModelCount = 0;

    componentStoresPtr_->GetAllComponents<Project001::RenderedModel>(renderedModelArray, renderedModelCount);

    for (unsigned int i = 0; i < renderedModelCount; ++i)
    {
        Project001::RenderedModel& currentRenderedModel = renderedModelArray[i];

        if (currentRenderedModel.IsVisible())
        {
            const Project001::MeshVertex* meshVerticies = nullptr;
            unsigned int meshVertexCount = 0;
            const unsigned int* meshIndicies = nullptr;
            unsigned int meshIndexCount = 0;

            _FAIL_CHECK(meshStoresPtr_->GetMesh(
                currentRenderedModel.GetMeshIndex(),
                meshVerticies,
                meshVertexCount,
                meshIndicies,
                meshIndexCount
            ));

            _FAIL_CHECK(rendererPtr_->AddMesh(
                meshVerticies,
                meshVertexCount,
                meshIndicies,
                meshIndexCount,
                currentRenderedModel.GetTextureIndex(),
                currentRenderedModel.GetSpecularIndex(),
                currentRenderedModel.GetPosition(),
                currentRenderedModel.GetOrientation(),
                currentRenderedModel.GetScale(),
                currentRenderedModel.GetColor(),
                currentRenderedModel.GetShininess(),
                currentRenderedModel.GetTranslucent(),
                currentRenderedModel.GetLit()
            ));
        }
    }

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    if (cameraPtr->IsTurnedOn())
    {
        rendererPtr_->SetViewMatrix(cameraPtr->GetViewMatrix());
        rendererPtr_->SetViewPosition(cameraPtr->GetPosition());
        rendererPtr_->SetProjectionMatrix(cameraPtr->GetProjectionMatrix());
        rendererPtr_->PrepareCapabilities();
        rendererPtr_->Render();
    }

    windowPtr_->SwapBuffers();

    renderEvent.handled = true;
}

void TestScene006::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
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

    scrollEvent.handled = true;
}

void TestScene006::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long timestep_ns = updateEvent.timestep_ns;

    UpdatedSelectedEntityPosition(timestep_ns);
    Sync_RenderedModel_CollisionBody_Components();

    DetectCollisions();
}

void TestScene006::UpdatedSelectedEntityPosition(unsigned long timestep_ns)
{
    float timestep_s = (float)(timestep_ns / 1000000) / 1000;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    float speedConstant = cameraPtr->GetTopCutoff();
    glm::vec2 cameraUp = cameraPtr->GetUpVector();
    glm::vec2 cameraLeft = cameraPtr->GetLeftVector();

    float translationSpeed = speedConstant * timestep_s;
    float rotationSpeed = speedConstant * 2.0f * timestep_s;

    bool movingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
    bool movingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
    bool movingUp = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
    bool movingDown = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
    bool rollingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
    bool rollingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

    if (selectedEntityIdIndex_ < entityIds_.size())
    {
        unsigned int selectedEntityId = entityIds_[selectedEntityIdIndex_];

        Project001::CollisionBody2D* collisionBodyPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(selectedEntityId, collisionBodyPtr));

        if (movingLeft)
        {
            // collisionBodyPtr->TranslateRight(-1.0f * translationSpeed);
            collisionBodyPtr->AddTranslation(cameraLeft * translationSpeed);
        }

        if (movingRight)
        {
            // collisionBodyPtr->TranslateRight(translationSpeed);
            collisionBodyPtr->AddTranslation(cameraLeft * -1.0f * translationSpeed);
        }

        if (movingUp)
        {
            // collisionBodyPtr->TranslateUp(translationSpeed);
            collisionBodyPtr->AddTranslation(cameraUp * translationSpeed);
        }

        if (movingDown)
        {
            // collisionBodyPtr->TranslateUp(-1.0f * translationSpeed);
            collisionBodyPtr->AddTranslation(cameraUp * -1.0f * translationSpeed);
        }

        if (rollingLeft)
        {
            collisionBodyPtr->AddRotation(rotationSpeed);
        }

        if (rollingRight)
        {
            collisionBodyPtr->AddRotation(-1.0f * rotationSpeed);
        }
    }
    else
    {
        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

        if (movingLeft)
        {
            cameraPtr->MoveLeft(translationSpeed);
        }

        if (movingRight)
        {
            cameraPtr->MoveRight(translationSpeed);
        }

        if (movingUp)
        {
            cameraPtr->MoveUp(translationSpeed);
        }

        if (movingDown)
        {
            cameraPtr->MoveDown(translationSpeed);
        }

        if (rollingLeft)
        {
            cameraPtr->AddRoll(-1.0f * rotationSpeed);
        }

        if (rollingRight)
        {
            cameraPtr->AddRoll(rotationSpeed);
        }
    }
}

void TestScene006::DetectCollisions()
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

    if (collisionBodyCount > 0)
    {
        for (unsigned int i = 0; i < collisionBodyCount - 1; ++i)
        {
            Project001::CollisionBody2D& firstCollisionBody2D = collisionBody2DArray[i];
            for (unsigned int j = i + 1; j < collisionBodyCount; ++j)
            {
                Project001::CollisionBody2D& secondCollisionBody2D = collisionBody2DArray[j];
                firstCollisionBody2D.CalculateCollision(secondCollisionBody2D);
            }
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
            if (selectedEntityIdIndex_ < entityIds_.size() && entityId == entityIds_[selectedEntityIdIndex_])
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
            if (selectedEntityIdIndex_ < entityIds_.size() && entityId == entityIds_[selectedEntityIdIndex_])
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

void TestScene006::Sync_RenderedModel_CollisionBody_Components()
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
        if (componentStoresPtr_->GetComponent<Project001::RenderedModel>(entityId, renderedModelPtr))
        {
            const float& positionX = collisionBody2D.GetPosition().x;
            const float& positionY = collisionBody2D.GetPosition().y;
            renderedModelPtr->SetPosition(positionX, positionY, 0.0f);
            const float& rotation = collisionBody2D.GetRotation();
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddWorldRotationZ(rotation);
        }
    }
}
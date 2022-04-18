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



// public: ---------------------------------------------------------------------

TestScene006::TestScene006()
{}

TestScene006::~TestScene006()
{}

const char* TestScene006::Name()
{
    return "TestScene006";
}

void TestScene006::Initialize()
{
    TestSceneBase002::Initialize();

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
    TestSceneBase002::Deinitialize();
}

void TestScene006::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene006::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase002::OnEvent(event);
}

// protected: ------------------------------------------------------------------

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
    }
}
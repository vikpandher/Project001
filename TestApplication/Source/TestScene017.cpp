// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-01

#include "TestScene017.h"

#include "TestApplicationData.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedMesh.h"
#include "Math/Overlap2D.h"
#include "Application.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"
#include "TextureLoader.h"

#include <random>



// public ----------------------------------------------------------------------

TestScene017::TestScene017(Project001::Application* applicationPtr)
    : TestSceneBase002(applicationPtr)
    , instructionScene_(applicationPtr)
{
    GetSharedDataPtr<TestApplicationData>()->testScene017Id = GetId();

    recolorOverlappingCollisionBodies_ = false;

    generateCollisionMarkerCollectionMesh_ = false;

    velocityBasedMovement_ = true;
}

TestScene017::~TestScene017()
{}

void TestScene017::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene017::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase002::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene017::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene017::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene017:            " << GetId());

    // Creating Entities
    // -------------------------------------------------------------------------

    applyGravity_ = true;

    CreateCollisionBodyEntities();

    // Member Scenes -----------------------------------------------------------

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests 2d Physics interactions.\n"
        "<Left-Click> on a shape to select it.\n"
        "<Left-Click> and drag to move it.\n"
        "<Left-Click> on the background to de-select it.\n"
        "When no shape is selected:\n"
        "   Use <WASD> to move the camera up, left, down, and right.\n"
        "   Use <Q> to roll camera left and <E> to roll camera right.\n"
        "When a shape is selected:\n"
        "   Use <WASD> to move the shape up, left, down, and right.\n"
        "   Use <Q> to roll shape left and <E> to roll shape right.\n"
        "   Press <R> to toggled fixed rotation for the shape.\n"
        "   Press <T> to toggled fixed translation for the shape.\n"
        "Press <B> and <N> to cycle between shapes.\n"
        "Press <G> to toggle gravity on and off.\n"
        "Use <Scroll> to zoom in and out.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId_;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene017::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene017:            " << GetId());
}

void TestScene017::CreateCollisionBodyEntities()
{
    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.friction = 1.0f;
    collisionBody2DCreationInfo.restitution = 0.4f;
    collisionBody2DCreationInfo.collisionGroupMask = mainCollisionGroupMask_;
    collisionBody2DCreationInfo.mass = 1.0f;

    { // Left Floor
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-4.0f, 0.25f);
        shapePoints.emplace_back(-4.0f, -0.25f);
        shapePoints.emplace_back(4.0f, -0.25f);
        shapePoints.emplace_back(4.0f, 0.25f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, -0.5f * glm::quarter_pi<float>());

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(-5.0f, 3.0f, 0.0f);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionOrientedRectangle2D>& collisionOrientedRectangles = collisionBody2DPtr->GetCollisionOrientedRectangles();
            collisionOrientedRectangles.emplace_back(
                glm::vec2(4.0f, 0.25f),
                glm::vec2(),
                -0.5f * glm::quarter_pi<float>()
            );

            collisionBody2DPtr->SetFixedTranslation(true);
            collisionBody2DPtr->SetFixedRotation(true);
        }
    }

    { // Right Floor
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-4.0f, 0.25f);
        shapePoints.emplace_back(-4.0f, -0.25f);
        shapePoints.emplace_back(4.0f, -0.25f);
        shapePoints.emplace_back(4.0f, 0.25f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, 0.5f * glm::quarter_pi<float>());

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(5.0f, 3.0f, 0.0f);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionOrientedRectangle2D>& collisionOrientedRectangles = collisionBody2DPtr->GetCollisionOrientedRectangles();
            collisionOrientedRectangles.emplace_back(
                glm::vec2(4.0f, 0.25f),
                glm::vec2(),
                0.5f * glm::quarter_pi<float>()
            );

            collisionBody2DPtr->SetFixedTranslation(true);
            collisionBody2DPtr->SetFixedRotation(true);
        }
    }

    // -------------------------------------------------------------------------

    std::mt19937 randomNumberEngine(777);
    std::uniform_real_distribution<float> distributionX(-8.0f, 8.0f);
    std::uniform_real_distribution<float> distributionY(-6.0f, 6.0f);

    unsigned int circleMeshId = (unsigned int)-1;

    float circleRadius = 0.08f;
    float circleBoundingRadius;
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, circleRadius, 12));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        GetRendererPtr()->CreateMesh(
            circleMeshId,
            newMeshDataPtr->meshVertexArray.data(),
            (unsigned int)newMeshDataPtr->meshVertexArray.size(),
            newMeshDataPtr->meshIndexArray.data(),
            (unsigned int)newMeshDataPtr->meshIndexArray.size()
        );
        circleBoundingRadius = newMeshDataPtr->maxBoundingRadius;
    }

    size_t count;
#ifndef NDEBUG
    count = 500;
#else
    count = 2000;
#endif
    for (size_t i = 0; i < count; ++i)
    {
        glm::vec3 currentPosition(distributionX(randomNumberEngine), distributionY(randomNumberEngine), 0.0f);

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshIdAndMaxBoundingRadius(circleMeshId, circleBoundingRadius);
            renderedMeshPtr->SetLit(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(),
                circleRadius
            );
        }
    }
}
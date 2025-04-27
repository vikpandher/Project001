// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-04-26

#include "TestScene016.h"

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



// public ----------------------------------------------------------------------

TestScene016::TestScene016(Project001::Application* applicationPtr)
    : TestSceneBase002(applicationPtr)
    , instructionScene_(applicationPtr)
{
    if (testApplicationDataPtr_ != nullptr)
    {
        testApplicationDataPtr_->testScene016Id = GetId();
    }

    recolorOverlappingCollisionBodies_ = false;

    physicsStepsPerUpdate_ = 8;

    useCollisionBodyQuadTree_ = false;

    velocityBasedMovement_ = true;
}

TestScene016::~TestScene016()
{}

void TestScene016::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene016::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase002::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene016::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene016::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene016:            " << GetId());

    // Creating Entities
    // -------------------------------------------------------------------------

    applyGravity_ = true;

    CreateEntitiesForGravityTest01();

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

void TestScene016::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene016:            " << GetId());
}

void TestScene016::CreateEntitiesForGravityTest01()
{
    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.friction = 1.0f;
    collisionBody2DCreationInfo.restitution = 0.4f;
    collisionBody2DCreationInfo.collisionGroupMask = mainCollisionGroupMask_;
    collisionBody2DCreationInfo.mass = 1.0f;

    { // Floor
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-5.0f, 1.0f);
        shapePoints.emplace_back(-5.0f, -1.0f);
        shapePoints.emplace_back(5.0f, -1.0f);
        shapePoints.emplace_back(5.0f, 1.0f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(0.0f, -4.0f, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-5.0f, -1.0f),
                glm::vec2(5.0f, 1.0f)
            );

            // std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
            // collisionConvexPolygons.emplace_back(shapePoints);

            collisionBody2DPtr->SetFixedTranslation(true);
            collisionBody2DPtr->SetFixedRotation(true);
        }
    }

    { // Circle 1
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(0.0f, -2.84f, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(),
                0.32f
            );
        }
    }

    { // Circle 2
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.24f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(0.0f, -2.36f, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(),
                0.24f
            );
        }
    }

    { // Circle 3
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.16f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(0.0f, -2.04f, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(),
                0.16f
            );
        }
    }

    for (size_t i = 0; i < 8; i++)
    { // Rectangle
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);

        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.5f, 0.25f);
        shapePoints.emplace_back(-0.5f, -0.25f);
        shapePoints.emplace_back(0.5f, -0.25f);
        shapePoints.emplace_back(0.5f, 0.25f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(-2.0f, -2.75f + 0.5f * (float)i, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-0.5f, -0.25f),
                glm::vec2(0.5f, 0.25f)
            );

            // std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
            // collisionConvexPolygons.emplace_back(shapePoints);

            collisionBody2DPtr->SetVelocity(glm::vec2(0.1f, 0.0f));
        }
    }

    for (size_t i = 0; i < 8; i++)
    { // Capsule
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);

        glm::vec2 capsuleStart(-0.5f, 0.0f);
        glm::vec2 capsuleEnd(0.5f, 0.0f);
        float capsuleRadius = 0.25f;
        FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, capsuleStart, capsuleEnd, capsuleRadius, 6));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(2.0f, -2.75f + 0.5f * (float)i, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionCapsule2D>& collisionCapsules = collisionBody2DPtr->GetCollisionCapsules();
            collisionCapsules.emplace_back(capsuleStart, capsuleEnd, capsuleRadius);

            collisionBody2DPtr->SetVelocity(glm::vec2(0.1f, 0.0f));
        }
    }

    { // Triangle 1
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, -0.48f);
        shapePoints.emplace_back(0.48f, 0.0f);
        shapePoints.emplace_back(0.0f, 0.48f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(-3.5f, -2.5f, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetTranslucent(true);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionTriangle2D>& collisionTriangles = collisionBody2DPtr->GetCollisionTriangles();
            collisionTriangles.emplace_back(
                glm::vec2(-0.48f, -0.48f),
                glm::vec2(0.48f, 0.0f),
                glm::vec2(0.0f, 0.48f)
            );
        }
    }

    { // Triangle 2
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, 0.32f);
        shapePoints.emplace_back(0.32f, -0.32f);
        shapePoints.emplace_back(0.32f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(-3.5f, -2.0f, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetTranslucent(true);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionTriangle2D>& collisionTriangles = collisionBody2DPtr->GetCollisionTriangles();
            collisionTriangles.emplace_back(
                glm::vec2(-0.32f, 0.32f),
                glm::vec2(0.32f, -0.32f),
                glm::vec2(0.32f, 0.16f)
            );
        }
    }

    { // Triangle 3
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, -0.32f);
        shapePoints.emplace_back(0.32f, 0.0f);
        shapePoints.emplace_back(0.0f, 0.32f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(-3.5f, -1.5f, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetTranslucent(true);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionTriangle2D>& collisionTriangles = collisionBody2DPtr->GetCollisionTriangles();
            collisionTriangles.emplace_back(
                glm::vec2(-0.32f, -0.32f),
                glm::vec2(0.32f, 0.0f),
                glm::vec2(0.0f, 0.32f)
            );
        }
    }

    // ConvexPolygon 1 (pentagon)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;

        int sides = 6;
        float radius = 0.36f;
        float sectionAngle = 2.0f * glm::pi<float>() / (float)sides;

        glm::vec2 radialVector(0.0f, radius);
        shapePoints.push_back(radialVector);

        for (size_t i = 0; i < sides - 1; ++i)
        {
            radialVector = Project001::Rotate2DVector(radialVector, sectionAngle);
            shapePoints.push_back(radialVector);
        }

        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(3.5f, -3.0f + 0.36f, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetTranslucent(true);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
            collisionConvexPolygons.emplace_back(shapePoints);
        }
    }

    // ConvexPolygon 2 (hexagon)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;

        int sides = 5;
        float radius = 0.36f;
        float sectionAngle = 2.0f * glm::pi<float>() / (float)sides;

        glm::vec2 radialVector(0.0f, radius);
        shapePoints.push_back(radialVector);

        for (size_t i = 0; i < sides - 1; ++i)
        {
            radialVector = Project001::Rotate2DVector(radialVector, sectionAngle);
            shapePoints.push_back(radialVector);
        }

        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(3.5f, -3.0f + 0.72f, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetTranslucent(true);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
            collisionConvexPolygons.emplace_back(shapePoints);
        }
    }

    GenerateCombinationShape(collisionBody2DCreationInfo, glm::vec3(0.0f, 0.0f, 0.0f));

    { // Circle 4
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.16f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(0.0f, 0.0f, 0.0f);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(),
                0.16f
            );
        }
    }
}

void TestScene016::GenerateCombinationShape(Project001::CollisionBody2DCreationInfo& collisionBody2DCreationInfo, glm::vec3& position)
{
    Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
    meshDataPtrArray_.push_back(newMeshDataPtr);

    Project001::MeshData tempMeshData;

    // Center Triangle
    const glm::vec2 centerTriangleCorner1(0.0f, 0.96f);
    const glm::vec2 centerTriangleCorner2(-0.32f, 0.32f);
    const glm::vec2 centerTriangleCorner3(0.32f, 0.32f);

    tempMeshData.Clear();
    std::vector<glm::vec2> centerTriangleCorners;
    centerTriangleCorners.push_back(centerTriangleCorner1);
    centerTriangleCorners.push_back(centerTriangleCorner2);
    centerTriangleCorners.push_back(centerTriangleCorner3);
    FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(tempMeshData, centerTriangleCorners));
    Project001::MeshLoader::CopyMesh(*newMeshDataPtr, tempMeshData);

    // Center Rectangle
    const glm::vec2 centerRectangleBottomLeft(-0.32f, -0.64f);
    const glm::vec2 centerRectangleTopRight(0.32f, -0.32f);

    tempMeshData.Clear();
    std::vector<glm::vec2> centerRectangleCorners;
    centerRectangleCorners.emplace_back(centerRectangleBottomLeft.x, centerRectangleTopRight.y);
    centerRectangleCorners.push_back(centerRectangleBottomLeft);
    centerRectangleCorners.emplace_back(centerRectangleTopRight.x, centerRectangleBottomLeft.y);
    centerRectangleCorners.push_back(centerRectangleTopRight);
    FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(tempMeshData, centerRectangleCorners));
    Project001::MeshLoader::CopyMesh(*newMeshDataPtr, tempMeshData);

    // Center Convex Polygon
    tempMeshData.Clear();
    std::vector<glm::vec2> centerConvexPolygonCorners;
    centerConvexPolygonCorners.emplace_back(0.0f, -0.64f);
    centerConvexPolygonCorners.emplace_back(-0.22f, -0.68f);
    centerConvexPolygonCorners.emplace_back(0.0f, -0.96f);
    centerConvexPolygonCorners.emplace_back(0.22f, -0.68f);
    FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(tempMeshData, centerConvexPolygonCorners));
    Project001::MeshLoader::CopyMesh(*newMeshDataPtr, tempMeshData);

    // Left Capsule
    const glm::vec2 leftCapsuleStart(-0.44f, 0.32f);
    const glm::vec2 leftCapsuleEnd(-0.44f, -0.32f);
    const float leftCapsuleRadius = 0.12f;

    tempMeshData.Clear();
    FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(
        tempMeshData,
        leftCapsuleStart,
        leftCapsuleEnd,
        leftCapsuleRadius,
        8));
    Project001::MeshLoader::CopyMesh(*newMeshDataPtr, tempMeshData);

    // Right Capsule
    const glm::vec2 rightCapsuleStart(0.44f, 0.32f);
    const glm::vec2 rightCapsuleEnd(0.44f, -0.32f);
    const float rightCapsuleRadius = 0.12f;

    tempMeshData.Clear();
    FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(
        tempMeshData,
        rightCapsuleStart,
        rightCapsuleEnd,
        rightCapsuleRadius,
        8));
    Project001::MeshLoader::CopyMesh(*newMeshDataPtr, tempMeshData);

    // Left Triangle
    const glm::vec2 leftTriangleCorner1(-0.56f, 0.32f);
    const glm::vec2 leftTriangleCorner2(-0.88f, -0.32f);
    const glm::vec2 leftTriangleCorner3(-0.56f, -0.32f);

    tempMeshData.Clear();
    std::vector<glm::vec2> leftTriangleCorners;
    leftTriangleCorners.push_back(leftTriangleCorner1);
    leftTriangleCorners.push_back(leftTriangleCorner2);
    leftTriangleCorners.push_back(leftTriangleCorner3);
    FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(tempMeshData, leftTriangleCorners));
    Project001::MeshLoader::CopyMesh(*newMeshDataPtr, tempMeshData);

    // Right Triangle
    const glm::vec2 rightTriangleCorner1(0.56f, 0.32f);
    const glm::vec2 rightTriangleCorner2(0.56f, -0.32f);
    const glm::vec2 rightTriangleCorner3(0.88f, -0.32f);

    tempMeshData.Clear();
    std::vector<glm::vec2> rightTriangleCorners;
    rightTriangleCorners.push_back(rightTriangleCorner1);
    rightTriangleCorners.push_back(rightTriangleCorner2);
    rightTriangleCorners.push_back(rightTriangleCorner3);
    FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(tempMeshData, rightTriangleCorners));
    Project001::MeshLoader::CopyMesh(*newMeshDataPtr, tempMeshData);

    Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
    Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

    unsigned int tempEntityId;
    componentStoresPtr_->CreateEntity(tempEntityId);
    entityIds_.push_back(tempEntityId);

    FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
    Project001::RenderedMesh* renderedMeshPtr = nullptr;
    FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
    if (renderedMeshPtr != nullptr)
    {
        renderedMeshPtr->SetPosition(position);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);
        renderedMeshPtr->SetTranslucent(true);
    }

    FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPosition(position);

        std::vector<Project001::CollisionTriangle2D>& collisionTrangles = collisionBody2DPtr->GetCollisionTriangles();
        collisionTrangles.emplace_back(centerTriangleCorner1, centerTriangleCorner2, centerTriangleCorner3);
        collisionTrangles.emplace_back(leftTriangleCorner1, leftTriangleCorner2, leftTriangleCorner3);
        collisionTrangles.emplace_back(rightTriangleCorner1, rightTriangleCorner2, rightTriangleCorner3);

        std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
        collisionRectangles.emplace_back(centerRectangleBottomLeft, centerRectangleTopRight);

        std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
        collisionConvexPolygons.emplace_back(centerConvexPolygonCorners);

        std::vector<Project001::CollisionCapsule2D>& collisionCapsules = collisionBody2DPtr->GetCollisionCapsules();
        collisionCapsules.emplace_back(leftCapsuleStart, leftCapsuleEnd, leftCapsuleRadius);
        collisionCapsules.emplace_back(rightCapsuleStart, rightCapsuleEnd, rightCapsuleRadius);
    }
}
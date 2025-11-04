// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-03

#include "TestScene025.h"

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

TestScene025::TestScene025(Project001::Application* applicationPtr)
    : TestSceneBase002(applicationPtr)
    , instructionScene_(applicationPtr)
{
    GetSharedDataPtr<TestApplicationData>()->testScene025Id = GetId();

    recolorOverlappingCollisionBodies_ = true;

    physicsStepsPerUpdate_ = 8;

    useCollisionBodyQuadTree_ = false;

    velocityBasedMovement_ = true;
}

TestScene025::~TestScene025()
{}

void TestScene025::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene025::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase002::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene025::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene025::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene025:            " << GetId());

    // Creating Entities
    // -------------------------------------------------------------------------

    CreateEntitiesForTest01();

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

void TestScene025::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene025:            " << GetId());
}

void TestScene025::CreateEntitiesForTest01()
{
    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.friction = 1.0f;
    collisionBody2DCreationInfo.restitution = 1.0f;
    collisionBody2DCreationInfo.collisionGroupMask = s_mainCollisionGroupMask_;

    float yPos01 = 4.0f;

    for (int i = 0; i < 7; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition((float)i, yPos01, 0.0f);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetUseLighting(false);
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
                0.32f
            );

            if (i == 0 || i == 1)
            {
                collisionBody2DPtr->SetVelocity(glm::vec2(1.0f, 0.0));
            }
        }
    }

    // -------------------------------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    for (int y = 0; y >= -3; --y)
    {
        for (int x = -2; x <= 2; ++x)
        {
            meshEntityPositions.emplace_back((float)x * 2.5f, (float)y * 1.5f + 0.5f, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // Rectangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, 0.24f);
        shapePoints.emplace_back(-0.48f, -0.24f);
        shapePoints.emplace_back(0.48f, -0.24f);
        shapePoints.emplace_back(0.48f, 0.24f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-0.48f, -0.24f),
                glm::vec2(0.48f, 0.24f)
            );
        }
    }

    // OrientedRectangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, 0.32f);
        shapePoints.emplace_back(-0.48f, -0.32f);
        shapePoints.emplace_back(0.48f, -0.32f);
        shapePoints.emplace_back(0.48f, 0.32f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::quarter_pi<float>());
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionOrientedRectangle2D>& collisionOrientedRectangles = collisionBody2DPtr->GetCollisionOrientedRectangles();
            collisionOrientedRectangles.emplace_back(
                glm::vec2(0.48f, 0.32f),
                glm::vec2(),
                glm::quarter_pi<float>()
            );
        }
    }

    // Circle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
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
                0.32f
            );
        }
    }

    // Capsule 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.80f, 0.24f, 8));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionCapsule2D>& collisionCapsules = collisionBody2DPtr->GetCollisionCapsules();
            collisionCapsules.emplace_back(
                glm::vec2(0.0f, -0.40f),
                glm::vec2(0.0f, 0.40f),
                0.12f
            );
        }
    }

    // Triangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.60f, -0.60f);
        shapePoints.emplace_back(0.48f, 0.0f);
        shapePoints.emplace_back(0.0f, 0.48f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionTriangle2D>& collisionTriangles = collisionBody2DPtr->GetCollisionTriangles();
            collisionTriangles.emplace_back(
                glm::vec2(-0.60f, -0.60f),
                glm::vec2(0.48f, 0.0f),
                glm::vec2(0.0f, 0.48f)
            );
        }
    }

    // Rectangle 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.96f, 0.24f);
        shapePoints.emplace_back(-0.96f, -0.24f);
        shapePoints.emplace_back(0.96f, -0.24f);
        shapePoints.emplace_back(0.96f, 0.24f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-0.96f, -0.24f),
                glm::vec2(0.96f, 0.24f)
            );
        }
    }

    // OrientedRectangle 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-1.92f, 0.08f);
        shapePoints.emplace_back(-1.92f, -0.08f);
        shapePoints.emplace_back(1.92f, -0.08f);
        shapePoints.emplace_back(1.92f, 0.08f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, -1.0f * glm::quarter_pi<float>());
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionOrientedRectangle2D>& collisionOrientedRectangles = collisionBody2DPtr->GetCollisionOrientedRectangles();
            collisionOrientedRectangles.emplace_back(
                glm::vec2(1.92f, 0.08f),
                glm::vec2(),
                -1.0f * glm::quarter_pi<float>()
            );
        }
    }

    // Circle 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.96f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
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
                0.96f
            );
        }
    }

    // Capsule 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.96f, 0.96f, 8));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::half_pi<float>());
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionCapsule2D>& collisionCapsules = collisionBody2DPtr->GetCollisionCapsules();
            collisionCapsules.emplace_back(
                glm::vec2(-0.48f, 0.0f),
                glm::vec2(0.48f, 0.0f),
                0.48f
            );
        }
    }

    // Triangle 2
    {
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
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
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

    // Rectangle 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.24f, 0.16f);
        shapePoints.emplace_back(-0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-0.24f, -0.16f),
                glm::vec2(0.24f, 0.16f)
            );
        }
    }

    // OrientedRectangle 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.24f, 0.16f);
        shapePoints.emplace_back(-0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::quarter_pi<float>());
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionOrientedRectangle2D>& collisionOrientedRectangles = collisionBody2DPtr->GetCollisionOrientedRectangles();
            collisionOrientedRectangles.emplace_back(
                glm::vec2(0.24f, 0.16f),
                glm::vec2(),
                glm::quarter_pi<float>()
            );
        }
    }

    // Circle 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.16f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
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
                0.16f
            );
        }
    }

    // Capsule 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.48f, 0.16f, 8));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionCapsule2D>& collisionCapsules = collisionBody2DPtr->GetCollisionCapsules();
            collisionCapsules.emplace_back(
                glm::vec2(0.0f, -0.24f),
                glm::vec2(0.0f, 0.24f),
                0.08f
            );
        }
    }

    // Triangle 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-1.28f, -1.28f);
        shapePoints.emplace_back(0.32f, 0.0f);
        shapePoints.emplace_back(0.0f, 0.32f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionTriangle2D>& collisionTriangles = collisionBody2DPtr->GetCollisionTriangles();
            collisionTriangles.emplace_back(
                glm::vec2(-1.28f, -1.28f),
                glm::vec2(0.32f, 0.0f),
                glm::vec2(0.0f, 0.32f)
            );
        }
    }

    // Rectangle 4
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.24f, 0.16f);
        shapePoints.emplace_back(-0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, -0.16f);
        shapePoints.emplace_back(0.24f, 0.16f);
        FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        FAIL_CHECK(Project001::MeshLoader::Generate2DRectangleFrame(*newMeshDataPtr, glm::vec2(-0.72f, -0.48f), glm::vec2(0.72f, 0.48f), 0.04f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-0.24f, -0.16f), glm::vec2(0.24f, 0.16f)
            );
            collisionRectangles.emplace_back(
                glm::vec2(-0.72f, -0.48f), glm::vec2(0.72f, 0.48f),
                1, true, Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR
            );
        }
    }

    // Circle 4
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        FAIL_CHECK(Project001::MeshLoader::Generate2DArc(*newMeshDataPtr, 0.92f, 0.96f, 24, 0.0f, 0.0f));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetUseLighting(false);
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(), 0.32f
            );
            collisionCircles.emplace_back(
                glm::vec2(), 0.96f,
                1, true, Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR
            );
        }
    }
}
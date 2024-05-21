#include "TestScene015.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedMesh.h"
#include "Math/Overlap2D.h"
#include "Application.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"



// public ----------------------------------------------------------------------

TestScene015::TestScene015(Project001::Application* applicationPtr)
    : TestSceneBase002(applicationPtr)
    , instructionScene_(applicationPtr)
{}

TestScene015::~TestScene015()
{}

void TestScene015::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene015::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase002::HandleEvent(event);

    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene015::ProcessUpdateEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene015::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene015::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    _LOG_MESSAGE("INITIALIZING:   TestScene015:            %u", GetId());

    // Creating Entities
    // -------------------------------------------------------------------------

    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    for (int i = -2; i <= 2; ++i)
    {
        for (int j = 2; j >= -2; --j)
        {
            meshEntityPositions.emplace_back((float)i, (float)j, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // -------------------------------------------------------------------------

    // Circle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(),
                0.32f
            );

            collisionBody2DPtr->SetVelocity(glm::vec2(1.0f, 0.0));
        }
    }

    // Circle 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.24f, 24));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(),
                0.24f
            );

            collisionBody2DPtr->SetAcceleration(glm::vec2(1.0f, 0.0));
        }
    }

    // Rectangle 1
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
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-0.32f, -0.24f),
                glm::vec2(0.32f, 0.24f)
            );

            collisionBody2DPtr->SetAngularVelocity(glm::quarter_pi<float>());
        }
    }

    // Rectangle 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.24f, 0.48f);
        shapePoints.emplace_back(-0.24f, -0.48f);
        shapePoints.emplace_back(0.24f, -0.48f);
        shapePoints.emplace_back(0.24f, 0.48f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-0.24f, -0.48f),
                glm::vec2(0.24f, 0.48f)
            );

            collisionBody2DPtr->SetAngularAcceleration(glm::quarter_pi<float>());
        }
    }

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
        "Press <B> and <N> to cycle between shapes.\n"
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

void TestScene015::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    _LOG_MESSAGE("DEINITIALIZING: TestScene015:            %u", GetId());
}

void TestScene015::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        const glm::vec2& collisionBody2D_position = collisionBody2D.GetPosition();

        if (collisionBody2D_position.x > 8.0f)
        {
            collisionBody2D.SetPositionX(-8.0f);
        }
        else if (collisionBody2D_position.x < -8.0f)
        {
            collisionBody2D.SetPositionX(8.0f);
        }

        if (collisionBody2D_position.y > 6.0f)
        {
            collisionBody2D.SetPositionY(-6.0f);
        }
        else if (collisionBody2D_position.y < -6.0f)
        {
            collisionBody2D.SetPositionY(6.0f);
        }

        const glm::vec2& collisionBody2D_velocity = collisionBody2D.GetVelocity();
        float velocityMagnitude = glm::length(collisionBody2D_velocity);

        if (velocityMagnitude > 10.1f)
        {
            glm::vec2 newVelocity = glm::normalize(collisionBody2D_velocity) * 10.0f;
            collisionBody2D.SetVelocity(newVelocity);
        }

        const float& collisionBody2D_angularVelocity = collisionBody2D.GetAngularVelocity();

        if (collisionBody2D_angularVelocity > 2.0f * glm::pi<float>())
        {
            constexpr float newAngularVelocity = 2.0f * glm::pi<float>();
            collisionBody2D.SetAngularVelocity(newAngularVelocity);
        }
    }
}
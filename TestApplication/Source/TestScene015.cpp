#include "TestScene015.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedMesh.h"
#include "Math/Overlap2D.h"
#include "Application.h"
#include "ComponentStores.h"
#include "FreetypeTextLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"
#include "TextureLoader.h"



// public ----------------------------------------------------------------------

TestScene015::TestScene015(Project001::Application* applicationPtr)
    : TestSceneBase002(applicationPtr)
    , instructionScene_(applicationPtr)
    , texture001_TextureId_((unsigned int)-1)
    , ui_energy_MeshDataPtr_(nullptr)
    , ui_energy_EntityId_((unsigned int)-1)
    , gravity_(0.0f, 0.0f)
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

    // Load Textures
    // -------------------------------------------------------------------------

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/100x100_2.png"));
        rendererPtr_->CreateTexture(texture001_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    // Generate Meshes ---------------------------------------------------------

    ui_energy_MeshDataPtr_ = new Project001::MeshData();
    meshDataPtrArray_.push_back(ui_energy_MeshDataPtr_);

    // Creating Entities
    // -------------------------------------------------------------------------

    CreateEntityForEnergyText();

    // CreateEntitiesForNewtonsCradle01(3.0f);
    // CreateEntitiesForNewtonsCradle02(2.0f);
    // CreateEntitiesForNewtonsCradle03(1.0f);
    // CreateEntitiesForNewtonsCradle04(0.0f); // has bodies with infinite mass
    // CreateEntitiesForNewtonsCradle05(-1.0f, 1.0f);
    // CreateEntitiesForNewtonsCradle06(-2.0f, 1.0f);

    gravity_.y = -1.0f;

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

    // Texture Data ------------------------------------------------------------

    texture001_TextureId_ = (unsigned int)-1;

    // Mesh Data ---------------------------------------------------------------

    // dont need to delete these here since they are added to meshDataPtrArray_

    ui_energy_MeshDataPtr_ = nullptr;

    // Entity Ids --------------------------------------------------------------

    ui_energy_EntityId_ = (unsigned int)-1;

    // -------------------------------------------------------------------------

    gravity_ = glm::vec2(0.0f, 0.0f);
}

void TestScene015::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    float timestep_s = (float)updateEvent.timestep_ns / 1e9f;
    ApplyGravity(timestep_s);

    CapVelocities();

    UpdateEnergyString();
}

void TestScene015::ApplyGravity(float timestep_s)
{
    unsigned int selectedEntityId = (unsigned int)-1;
    if (cursorGrabbingEntity_ && selectedEntityIdIndex_ < entityIds_.size())
    {
        selectedEntityId = entityIds_[selectedEntityIdIndex_];
    }

    Project001::CollisionBody2D* collisionBody2DArray = nullptr;
    size_t collisionBodyCount = 0;

    componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBody2DArray, collisionBodyCount);

    for (unsigned int i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& collisionBody2D = collisionBody2DArray[i];

        if (!(collisionBody2D.GetCollisionGroupMask() & collisionGroupMask_))
        {
            continue;
        }

        unsigned int currentEntityId = (unsigned int)-1;
        if (componentStoresPtr_->GetComponentEntityId(currentEntityId, &collisionBody2D) &&
            currentEntityId == selectedEntityId)
        {
            continue;
        }

        if (collisionBody2D.GetFixedTranslation())
        {
            continue;
        }

        collisionBody2D.SetVelocity(collisionBody2D.GetVelocity() + gravity_ * timestep_s);
    }
}

void TestScene015::CapVelocities()
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
            collisionBody2D.SetPositionX(-8.0f + std::fmod(collisionBody2D_position.x, 8.0f));
        }
        else if (collisionBody2D_position.x < -8.0f)
        {
            collisionBody2D.SetPositionX(8.0f + std::fmod(collisionBody2D_position.x, 8.0f));
        }
        // else if (std::isnan(collisionBody2D_position.x))
        // {
        //     collisionBody2D.SetPositionX(0.0f);
        // }

        if (collisionBody2D_position.y > 6.0f)
        {
            collisionBody2D.SetPositionY(-6.0f + std::fmod(collisionBody2D_position.y, 6.0f));
        }
        else if (collisionBody2D_position.y < -6.0f)
        {
            collisionBody2D.SetPositionY(6.0f + std::fmod(collisionBody2D_position.y, 6.0f));
        }
        // else if (std::isnan(collisionBody2D_position.y))
        // {
        //     collisionBody2D.SetPositionY(0.0f);
        // }

        // Cap Velocity

        const glm::vec2& collisionBody2D_velocity = collisionBody2D.GetVelocity();
        float velocityMagnitude = glm::length(collisionBody2D_velocity);

        if (velocityMagnitude > 10.0f)
        {
            glm::vec2 newVelocity = glm::normalize(collisionBody2D_velocity) * 10.0f;
            collisionBody2D.SetVelocity(newVelocity);
        }

        // Cap Angular Velocity

        const float& collisionBody2D_angularVelocity = collisionBody2D.GetAngularVelocity();

        if (collisionBody2D_angularVelocity > 4.0f * glm::pi<float>())
        {
            constexpr float newAngularVelocity = 4.0f * glm::pi<float>();
            collisionBody2D.SetAngularVelocity(newAngularVelocity);
        }
        else if (collisionBody2D_angularVelocity < -4.0f * glm::pi<float>())
        {
            constexpr float newAngularVelocity = -4.0f * glm::pi<float>();
            collisionBody2D.SetAngularVelocity(newAngularVelocity);
        }
    }
}

void TestScene015::CreateEntityForEnergyText()
{
    float uiCameraHalfHeight = 0.0f;
    float uiCameraHalfWidth = 0.0f;

    {
        Project001::Camera* cameraPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, uiCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            uiCameraHalfHeight = cameraPtr->GetTopCutoff();
            uiCameraHalfWidth = cameraPtr->GetRightCutoff();
        }
    }

    {
        componentStoresPtr_->CreateEntity(ui_energy_EntityId_);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(ui_energy_EntityId_));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, ui_energy_EntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_uiCameraMask_);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetMeshDataPtr(ui_energy_MeshDataPtr_);
            renderedMeshPtr->SetTextureId(font01_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetPositionX(uiCameraHalfWidth - 0.2f);
            renderedMeshPtr->SetPositionY(uiCameraHalfHeight - 0.2f);
        }
    }
}

void TestScene015::UpdateEnergyString()
{
    ui_energy_MeshDataPtr_->Clear();

    float totalLinearKeneticEnergy = 0.0f;
    float totalRotationalKeneticEnergy = 0.0f;

    Project001::CollisionBody2D* collisionBodyPtrs = nullptr;
    size_t collisionBodyCount = 0;
    bool componentFound = componentStoresPtr_->GetAllComponents<Project001::CollisionBody2D>(collisionBodyPtrs, collisionBodyCount);

    for (size_t i = 0; i < collisionBodyCount; ++i)
    {
        Project001::CollisionBody2D& currentCollisionBody = collisionBodyPtrs[i];

        totalLinearKeneticEnergy += currentCollisionBody.GetLinearKeneticEnergy();
        totalRotationalKeneticEnergy += currentCollisionBody.GetRotationalKeneticEnergy();
    }

    std::string energy_string = std::to_string(totalLinearKeneticEnergy) + " + " + std::to_string(totalRotationalKeneticEnergy) + " = " + std::to_string(totalLinearKeneticEnergy + totalRotationalKeneticEnergy);

    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadMeshData(*ui_energy_MeshDataPtr_, *font01_FontDataPtr_, energy_string, fontPixelSize_));
    Project001::MeshLoader::RecenterMesh(*ui_energy_MeshDataPtr_);
    Project001::MeshLoader::TranslateMesh(*ui_energy_MeshDataPtr_, -0.5f * ui_energy_MeshDataPtr_->GetSize());
}

void TestScene015::CreateEntitiesForNewtonsCradle01(float yPos)
{
    for (int i = -4; i < 5; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition((float)i, yPos, 0.0f);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(texture001_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
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

            if (i == -4)
            {
                collisionBody2DPtr->SetVelocity(glm::vec2(1.0f, 0.0));
            }
        }
    }
}

void TestScene015::CreateEntitiesForNewtonsCradle02(float yPos)
{
    for (int i = -4; i < 5; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition((float)i, yPos, 0.0f);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(texture001_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
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

            if (i == 4)
            {
                collisionBody2DPtr->SetVelocity(glm::vec2(-2.0f, 0.0));
            }
        }
    }
}

void TestScene015::CreateEntitiesForNewtonsCradle03(float yPos)
{
    for (int i = -4; i < 5; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition((float)i, yPos, 0.0f);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(texture001_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
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

            if (i == -4)
            {
                collisionBody2DPtr->SetVelocity(glm::vec2(1.0f, 0.0));
            }
            else if (i == 4)
            {
                collisionBody2DPtr->SetVelocity(glm::vec2(-1.0f, 0.0));
            }
        }
    }
}

void TestScene015::CreateEntitiesForNewtonsCradle04(float yPos)
{
    for (int i = -4; i < 5; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition((float)i, yPos, 0.0f);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(texture001_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
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

            if (i == -4)
            {
                collisionBody2DPtr->SetMass(std::numeric_limits<float>::infinity());
            }
            else if (i == -3)
            {
                // collisionBody2DPtr->SetPositionY(currentPosition.y + 0.01f);
                collisionBody2DPtr->SetVelocity(glm::vec2(1.0f, 0.0));
            }
            else if (i == 4)
            {
                collisionBody2DPtr->SetMass(std::numeric_limits<float>::infinity());
            }
        }
    }
}

void TestScene015::CreateEntitiesForNewtonsCradle05(float yPos, float friction)
{
    for (int i = -4; i < 5; ++i)
    {
        if (i == -2)
        {
            continue;
        }

        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition((float)i, yPos, 0.0f);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(texture001_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
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

            collisionBody2DPtr->SetFriction(friction);

            if (i == -4)
            {
                collisionBody2DPtr->SetFixedTranslation(true);
            }
            else if (i == -3)
            {
                collisionBody2DPtr->SetVelocity(glm::vec2(1.0f, 0.0));
                collisionBody2DPtr->SetAngularVelocity(2.0f * glm::pi<float>());
            }
            else if (i == 4)
            {
                collisionBody2DPtr->SetFixedTranslation(true);
            }
        }
    }
}

void TestScene015::CreateEntitiesForNewtonsCradle06(float yPos, float friction)
{
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(-2.0f, yPos, 0.0f);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh * renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(texture001_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
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

            collisionBody2DPtr->SetFriction(friction);

            collisionBody2DPtr->SetVelocity(glm::vec2(1.0f, 0.0));
            // collisionBody2DPtr->SetAngularVelocity(glm::pi<float>());
        }
    }

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.12f, 0.72f);
        shapePoints.emplace_back(-0.12f, -0.72f);
        shapePoints.emplace_back(0.12f, -0.72f);
        shapePoints.emplace_back(0.12f, 0.72f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(2.0f, yPos + 0.64f, 0.0f);

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
                glm::vec2(-0.12f, -0.72f),
                glm::vec2(0.12f, 0.72f)
            );

            collisionBody2DPtr->SetFriction(friction);
        }
    }
}

void TestScene015::CreateEntitiesForGravityTest01()
{
    { // Floor
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-4.0f, 1.0f);
        shapePoints.emplace_back(-4.0f, -1.0f);
        shapePoints.emplace_back(4.0f, -1.0f);
        shapePoints.emplace_back(4.0f, 1.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(0.0f, -4.0f, 0.0f);

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
            collisionBody2DPtr->SetCollisionGroupMask(collisionGroupMask_);
            collisionBody2DPtr->SetPosition(currentPosition);

            // std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            // collisionRectangles.emplace_back(
            //     glm::vec2(-4.0f, -1.0f),
            //     glm::vec2(4.0f, 1.0f)
            // );

            std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
            collisionConvexPolygons.emplace_back(shapePoints);

            collisionBody2DPtr->SetFriction(1.0f);

            collisionBody2DPtr->SetFixedTranslation(true);
            collisionBody2DPtr->SetFixedRotation(true);
        }
    }

    { // Circle
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        // glm::vec3 currentPosition(0.0f, 0.0f, 0.0f);
        glm::vec3 currentPosition(0.0f, -2.84f, 0.0f);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(texture001_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetCollisionGroupMask(collisionGroupMask_);
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(),
                0.32f
            );

            collisionBody2DPtr->SetFriction(1.0f);
            collisionBody2DPtr->SetRestitution(0.5f);

            collisionBody2DPtr->SetVelocity(glm::vec2(0.1f, 0.0f));
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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(-2.0f, -2.75f + 0.5f * (float)i, 0.0f);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(texture001_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetCollisionGroupMask(collisionGroupMask_);
            collisionBody2DPtr->SetPosition(currentPosition);

            // std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            // collisionRectangles.emplace_back(
            //     glm::vec2(-0.5f, -0.25f),
            //     glm::vec2(0.5f, 0.25f)
            // );

            std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
            collisionConvexPolygons.emplace_back(shapePoints);

            collisionBody2DPtr->SetFriction(1.0f);
            collisionBody2DPtr->SetRestitution(0.5f);

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
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, capsuleStart, capsuleEnd, capsuleRadius, 6));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition(2.0f, -2.75f + 0.5f * (float)i, 0.0f);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetPosition(currentPosition);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(texture001_TextureId_);
            renderedMeshPtr->SetTranslucent(true);
            renderedMeshPtr->SetLit(false);
        }

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetCollisionGroupMask(collisionGroupMask_);
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionCapsule2D>& collisionCapsules = collisionBody2DPtr->GetCollisionCapsules();
            collisionCapsules.emplace_back(capsuleStart, capsuleEnd, capsuleRadius);

            collisionBody2DPtr->SetFriction(1.0f);
            collisionBody2DPtr->SetRestitution(0.5f);

            collisionBody2DPtr->SetVelocity(glm::vec2(0.1f, 0.0f));
        }
    }
}
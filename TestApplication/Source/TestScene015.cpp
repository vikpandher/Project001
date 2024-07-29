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
{
    recolorOverlappingCollisionBodies_ = false;

    physicsStepsPerUpdate_ = 8;

    useCollisionBodyQuadTree_ = false;

    velocityBasedMovement_ = true;
}

TestScene015::~TestScene015()
{}

void TestScene015::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene015::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase002::HandleEvent(event);

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

    // Creating Entities
    // -------------------------------------------------------------------------

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
            collisionBody2DPtr->SetCollisionGroupMask(gravityCollisionGroupMask_);
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
            collisionBody2DPtr->SetCollisionGroupMask(gravityCollisionGroupMask_);
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
            collisionBody2DPtr->SetCollisionGroupMask(gravityCollisionGroupMask_);
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
            collisionBody2DPtr->SetCollisionGroupMask(gravityCollisionGroupMask_);
            collisionBody2DPtr->SetPosition(currentPosition);

            std::vector<Project001::CollisionCapsule2D>& collisionCapsules = collisionBody2DPtr->GetCollisionCapsules();
            collisionCapsules.emplace_back(capsuleStart, capsuleEnd, capsuleRadius);

            collisionBody2DPtr->SetFriction(1.0f);
            collisionBody2DPtr->SetRestitution(0.5f);

            collisionBody2DPtr->SetVelocity(glm::vec2(0.1f, 0.0f));
        }
    }
}
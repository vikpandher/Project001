// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-20

#include "TestScene103.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedModel.h"
#include "Math/MathUtilities.h"
#include "Resources/PixelFont5x6.h"
#include "CollisionSystem2D.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "RenderSystem.h"
#include "TextureLoader.h"
#include "Window.h"



struct CharacterInfo
{
    enum class State
    {
        STATE_STANDING,
        STATE_WALKING,
        STATE_RUNNING,
        STATE_STUNNED
    };

    State state = State::STATE_STANDING;
    State animationState = State::STATE_STANDING; // the change in state will be delayed
};

struct PersonInfo
{
    enum class State
    {
        STATE_STANDING,
        STATE_WALKING
    };

    State state = State::STATE_STANDING;
    float stateDuration_s = 0.0f;
};

// public ----------------------------------------------------------------------

TestScene103::TestScene103(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , instructionScene_(applicationPtr)
    , collisionBodyQuadTreeMeshDataPtr_(nullptr)
    , cursorCircle_MeshDataPtr_(nullptr)
    , pixelFont_FontDataPtr_(nullptr)
    , pixelFont_TextureDataPtr_(nullptr)
    , pixelFont_TextureId_((unsigned int)-1)
    , fog_MeshDataPtr_(nullptr)
    , groundDark_MeshDataPtr_(nullptr)
    , groundDark_TextureDataPtr_(nullptr)
    , groundDark_TextureId_((unsigned int)-1)
    , groundLit_MeshDataPtr_(nullptr)
    , groundLit_TextureDataPtr_(nullptr)
    , groundLit_TextureId_((unsigned int)-1)
    , groundGrid_MeshDataPtr_(nullptr)
    , groundGrid_MeshId_((unsigned int)-1)
    , groundGridLabels_MeshDataPtr_(nullptr)
    , groundGridLabels_MeshId_((unsigned int)-1)
    , groundCollision_MeshDataPtr_(nullptr)
    , playerDark_MeshDataPtr_(nullptr)
    , playerDark_TextureDataPtr_(nullptr)
    , playerDark_TextureId_((unsigned int)-1)
    , playerLit_MeshDataPtr_(nullptr)
    , playerLit_TextureDataPtr_(nullptr)
    , playerLit_TextureId_((unsigned int)-1)
    , playerCollision_MeshDataPtr_(nullptr)
    , playerLightBottom_MeshDataPtr_(nullptr)
    , playerLightTop_MeshDataPtr_(nullptr)
    , playerLightStrong_MeshDataPtr_(nullptr)
    , playerLightCollision_MeshDataPtr_(nullptr)
    , testUiBackground_MeshDataPtr_(nullptr)
    , testUiText01_MeshDataPtr_(nullptr)
    , testUiText02_MeshDataPtr_(nullptr)
    , testUiText03_MeshDataPtr_(nullptr)
    , testPersonLit_MeshDataPtr_(nullptr)
    , testPersonLit_TextureDataPtr_(nullptr)
    , testPersonLit_TextureId_((unsigned int)-1)
    , testPersonDark_MeshDataPtr_(nullptr)
    , testPersonDark_TextureDataPtr_(nullptr)
    , testPersonDark_TextureId_((unsigned int)-1)
    , testPersonCollision_MeshDataPtr_(nullptr)
    , testMonsterLit_MeshDataPtr_(nullptr)
    , testMonsterLit_TextureDataPtr_(nullptr)
    , testMonsterLit_TextureId_((unsigned int)-1)
    , testMonsterDark_MeshDataPtr_(nullptr)
    , testMonsterDark_TextureDataPtr_(nullptr)
    , testMonsterDark_TextureId_((unsigned int)-1)
    , testMonsterCollision_MeshDataPtr_(nullptr)
    , testMonsterVisionCollision_MeshDataPtr_(nullptr)
    , testLampLit_MeshDataPtr_(nullptr)
    , testLampLit_TextureDataPtr_(nullptr)
    , testLampLit_TextureId_((unsigned int)-1)
    , testLampDark_MeshDataPtr_(nullptr)
    , testLampDark_TextureDataPtr_(nullptr)
    , testLampDark_TextureId_((unsigned int)-1)
    , testLampCollision_MeshDataPtr_(nullptr)
    , testLampLightBottom_MeshDataPtr_(nullptr)
    , testLampLightTop_MeshDataPtr_(nullptr)
    , testLampLightCollision_MeshDataPtr_(nullptr)
    , testHouseLit_MeshDataPtr_(nullptr)
    , testHouseLit_TextureDataPtr_(nullptr)
    , testHouseLit_TextureId_((unsigned int)-1)
    , testHouseDark_MeshDataPtr_(nullptr)
    , testHouseDark_TextureDataPtr_(nullptr)
    , testHouseDark_TextureId_((unsigned int)-1)
    , testHouseCollision_MeshDataPtr_(nullptr)
    , testHouseText_MeshDataPtr_(nullptr)
    , testHouseLightBottom_MeshDataPtr_(nullptr)
    , testHouseLightTop_MeshDataPtr_(nullptr)
    , testHouseLightCollision_MeshDataPtr_(nullptr)
    , testHouseDoorCollision_MeshDataPtr_(nullptr)
    , mainCameraLight1_EntityId_((unsigned int)-1)
    , mainCameraLight2_EntityId_((unsigned int)-1)
    , mainCameraDark1_EntityId_((unsigned int)-1)
    , mainCameraDark2_EntityId_((unsigned int)-1)
    , mainCameraDebug_EntityId_((unsigned int)-1)
    , mainCamera_LookAtPoint_()
    , mainCamera_DistanceAway_(0.0f)
    , mainCamera_Locked_(true)
    , uiCamera_EntityId_((unsigned int)-1)
    , cursor_EntityId_((unsigned int)-1)
    , cursorPositionRenderedMeshIndex_((unsigned int)-1)
    , cursorPressRenderedMeshIndex_((unsigned int)-1)
    , cursorReleaseRenderedMeshIndex_((unsigned int)-1)
    , cursorPositionCollisionPointIndex_((unsigned int)-1)
    , cursorPressCollisionPointIndex_((unsigned int)-1)
    , cursorReleaseCollisionPointIndex_((unsigned int)-1)
    , fog_EntityId_((unsigned int)-1)
    , ground_EntityId_((unsigned int)-1)
    , player_EntityId_((unsigned int)-1)
    , playerLight_EntityId_((unsigned int)-1)
    , testUi_EntityId_((unsigned int)-1)
    , testPeople_EntityIds_{(unsigned int)-1}
    , testPerson_EntityId_((unsigned int)-1)
    , testMonster_EntityId_((unsigned int)-1)
    , testMonsterVision_EntityId_((unsigned int)-1)
    , testLamp_EntityId_((unsigned int)-1)
    , testLampLight_EntityId_((unsigned int)-1)
    , testHouse_EntityId_((unsigned int)-1)
    , testHouseLight_EntityId_((unsigned int)-1)
    , paused_(false)
    , randomNumberEngine_(777)
{
    GetSharedDataPtr<TestApplicationData>()->testScene103Id = GetId();
}

TestScene103::~TestScene103()
{}

void TestScene103::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene103::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene103::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene103::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene103::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene103::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene103::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestScene103::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene103::ProcessUpdateEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene103::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene103:            " << GetId());

    // Resources ---------------------------------------------------------------

    collisionBodyQuadTreeMeshDataPtr_ = new Project001::MeshData();
    Project001::CollisionSystem2D::ResetCollisionBodyQuadTree2D(
        glm::vec2(-1160.0f, -1160.0f),
        glm::vec2(1160.0f, 1160.0f),
        5,
        16
    );

    LoadCursorResources();
    LoadPixelFontResources();
    LoadFogResources();
    LoadGroundResources();
    LoadGroundGridResources();
    LoadPlayerResources();
    LoadPlayerLightResources();

    LoadTestUiResources();

    LoadTestPersonResources();
    LoadTestMonsterResources();
    LoadTestMonsterVisionResources();
    LoadTestLampResources();
    LoadTestLampLightResources();
    LoadTestHouseResources();
    LoadTestHouseLightResources();

    // Entities ----------------------------------------------------------------

    CreateMainCameraEntities();
    CreateUiCameraEntity();
    CreateCursorEntity();
    CreateFogEntity();
    CreateGroundEntity();
    CreatePlayerEntity();
    CreatePlayerLightEntity();

    CreateTestUiEntity();

    std::uniform_real_distribution<float> distributionX(-1024.0f, 1024.0f);
    std::uniform_real_distribution<float> distributionY(-1024.0f, 1024.0f);

    for (size_t i = 0; i < testPeople_Size_; ++i)
    {
        glm::vec2 currentPosition(distributionX(randomNumberEngine_), distributionY(randomNumberEngine_));
        CreateTestPersonEntity(testPeople_EntityIds_[i], currentPosition);
    }

    CreateTestPersonEntity(testPerson_EntityId_, glm::vec2(0.0f, 0.0f));
    CreateTestMonsterEntity();
    CreateTestMonsterVisionEntity();
    CreateTestLampEntity();
    CreateTestLampLightEntity();
    CreateTestHouseEntity();
    CreateTestHouseLightEntity();

    // Member Scenes -----------------------------------------------------------

    InitializeInstructionScene();
}

void TestScene103::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene103:            " << GetId());

    // -------------------------------------------------------------------------

    GetRendererPtr()->DeleteAllTextures();
    GetRendererPtr()->DeleteAllMeshes();
    GetComponentStoresPtr()->DeleteAllEntities();

    // Resources ---------------------------------------------------------------

    delete collisionBodyQuadTreeMeshDataPtr_;
    collisionBodyQuadTreeMeshDataPtr_ = nullptr;

    delete cursorCircle_MeshDataPtr_;
    cursorCircle_MeshDataPtr_ = nullptr;

    pixelFont_FontDataPtr_ = nullptr;
    pixelFont_TextureDataPtr_ = nullptr;
    pixelFont_TextureId_ = (unsigned int)-1;

    delete fog_MeshDataPtr_;
    fog_MeshDataPtr_ = nullptr;

    delete groundDark_MeshDataPtr_;
    groundDark_MeshDataPtr_ = nullptr;
    delete groundDark_TextureDataPtr_;
    groundDark_TextureDataPtr_ = nullptr;
    groundDark_TextureId_ = (unsigned int)-1;
    delete groundLit_MeshDataPtr_;
    groundLit_MeshDataPtr_ = nullptr;
    delete groundLit_TextureDataPtr_;
    groundLit_TextureDataPtr_ = nullptr;
    groundLit_TextureId_ = (unsigned int)-1;
    delete groundGrid_MeshDataPtr_;
    groundGrid_MeshDataPtr_ = nullptr;
    groundGrid_MeshId_ = (unsigned int)-1;
    delete groundGridLabels_MeshDataPtr_;
    groundGridLabels_MeshDataPtr_ = nullptr;
    groundGridLabels_MeshId_ = (unsigned int)-1;
    delete groundCollision_MeshDataPtr_;
    groundCollision_MeshDataPtr_ = nullptr;

    delete playerDark_MeshDataPtr_;
    playerDark_MeshDataPtr_ = nullptr;
    delete playerDark_TextureDataPtr_;
    playerDark_TextureDataPtr_ = nullptr;
    playerDark_TextureId_ = (unsigned int)-1;

    delete playerLit_MeshDataPtr_;
    playerLit_MeshDataPtr_ = nullptr;
    delete playerLit_TextureDataPtr_;
    playerLit_TextureDataPtr_ = nullptr;
    playerLit_TextureId_ = (unsigned int)-1;
    delete playerCollision_MeshDataPtr_;
    playerCollision_MeshDataPtr_ = nullptr;

    delete playerLightBottom_MeshDataPtr_;
    playerLightBottom_MeshDataPtr_ = nullptr;
    delete playerLightTop_MeshDataPtr_;
    playerLightTop_MeshDataPtr_ = nullptr;
    delete playerLightStrong_MeshDataPtr_;
    playerLightStrong_MeshDataPtr_ = nullptr;
    delete playerLightCollision_MeshDataPtr_;
    playerLightCollision_MeshDataPtr_ = nullptr;

    delete testUiBackground_MeshDataPtr_;
    testUiBackground_MeshDataPtr_ = nullptr;
    delete testUiText01_MeshDataPtr_;
    testUiText01_MeshDataPtr_ = nullptr;
    delete testUiText02_MeshDataPtr_;
    testUiText02_MeshDataPtr_ = nullptr;
    delete testUiText03_MeshDataPtr_;
    testUiText03_MeshDataPtr_ = nullptr;

    delete testPersonLit_MeshDataPtr_;
    testPersonLit_MeshDataPtr_ = nullptr;
    delete testPersonLit_TextureDataPtr_;
    testPersonLit_TextureDataPtr_ = nullptr;
    testPersonLit_TextureId_ = (unsigned int)-1;
    delete testPersonDark_MeshDataPtr_;
    testPersonDark_MeshDataPtr_ = nullptr;
    delete testPersonDark_TextureDataPtr_;
    testPersonDark_TextureDataPtr_ = nullptr;
    testPersonDark_TextureId_ = (unsigned int)-1;
    delete testPersonCollision_MeshDataPtr_;
    testPersonCollision_MeshDataPtr_ = nullptr;

    delete testMonsterLit_MeshDataPtr_;
    testMonsterLit_MeshDataPtr_ = nullptr;
    delete testMonsterLit_TextureDataPtr_;
    testMonsterLit_TextureDataPtr_ = nullptr;
    testMonsterLit_TextureId_ = (unsigned int)-1;
    delete testMonsterDark_MeshDataPtr_;
    testMonsterDark_MeshDataPtr_ = nullptr;
    delete testMonsterDark_TextureDataPtr_;
    testMonsterDark_TextureDataPtr_ = nullptr;
    testMonsterDark_TextureId_ = (unsigned int)-1;
    delete testMonsterCollision_MeshDataPtr_;
    testMonsterCollision_MeshDataPtr_ = nullptr;

    delete testMonsterVisionCollision_MeshDataPtr_;
    testMonsterVisionCollision_MeshDataPtr_ = nullptr;

    delete testLampLit_MeshDataPtr_;
    testLampLit_MeshDataPtr_ = nullptr;
    delete testLampLit_TextureDataPtr_;
    testLampLit_TextureDataPtr_ = nullptr;
    testLampLit_TextureId_ = (unsigned int)-1;
    delete testLampDark_MeshDataPtr_;
    testLampDark_MeshDataPtr_ = nullptr;
    delete testLampDark_TextureDataPtr_;
    testLampDark_TextureDataPtr_ = nullptr;
    testLampDark_TextureId_ = (unsigned int)-1;
    delete testLampCollision_MeshDataPtr_;
    testLampCollision_MeshDataPtr_ = nullptr;

    delete testLampLightBottom_MeshDataPtr_;
    testLampLightBottom_MeshDataPtr_ = nullptr;
    delete testLampLightTop_MeshDataPtr_;
    testLampLightTop_MeshDataPtr_ = nullptr;
    delete testLampLightCollision_MeshDataPtr_;
    testLampLightCollision_MeshDataPtr_ = nullptr;

    delete testHouseLit_MeshDataPtr_;
    testHouseLit_MeshDataPtr_ = nullptr;
    delete testHouseLit_TextureDataPtr_;
    testHouseLit_TextureDataPtr_ = nullptr;
    testHouseLit_TextureId_ = (unsigned int)-1;
    delete testHouseDark_MeshDataPtr_;
    testHouseDark_MeshDataPtr_ = nullptr;
    delete testHouseDark_TextureDataPtr_;
    testHouseDark_TextureDataPtr_ = nullptr;
    testHouseDark_TextureId_ = (unsigned int)-1;
    delete testHouseCollision_MeshDataPtr_;
    testHouseCollision_MeshDataPtr_ = nullptr;

    delete testHouseText_MeshDataPtr_;
    testHouseText_MeshDataPtr_ = nullptr;

    delete testHouseLightBottom_MeshDataPtr_;
    testHouseLightBottom_MeshDataPtr_ = nullptr;
    delete testHouseLightTop_MeshDataPtr_;
    testHouseLightTop_MeshDataPtr_ = nullptr;
    delete testHouseLightCollision_MeshDataPtr_;
    testHouseLightCollision_MeshDataPtr_ = nullptr;
    delete testHouseDoorCollision_MeshDataPtr_;
    testHouseDoorCollision_MeshDataPtr_ = nullptr;

    // Entities ----------------------------------------------------------------

    mainCameraLight1_EntityId_ = (unsigned int)-1;
    mainCameraLight2_EntityId_ = (unsigned int)-1;
    mainCameraDark1_EntityId_ = (unsigned int)-1;
    mainCameraDark2_EntityId_ = (unsigned int)-1;
    mainCameraDebug_EntityId_ = (unsigned int)-1;
    mainCamera_LookAtPoint_ = glm::vec3();
    mainCamera_DistanceAway_ = 0.0f;
    mainCamera_Locked_ = true;

    uiCamera_EntityId_ = (unsigned int)-1;

    cursor_EntityId_ = (unsigned int)-1;
    cursorPositionRenderedMeshIndex_ = (unsigned int)-1;
    cursorPressRenderedMeshIndex_ = (unsigned int)-1;
    cursorReleaseRenderedMeshIndex_ = (unsigned int)-1;
    cursorPositionCollisionPointIndex_ = (unsigned int)-1;
    cursorPressCollisionPointIndex_ = (unsigned int)-1;
    cursorReleaseCollisionPointIndex_ = (unsigned int)-1;

    fog_EntityId_ = (unsigned int)-1;

    ground_EntityId_ = (unsigned int)-1;

    player_EntityId_ = (unsigned int)-1;

    playerLight_EntityId_ = (unsigned int)-1;

    testUi_EntityId_ = (unsigned int)-1;
    testPerson_EntityId_ = (unsigned int)-1;
    testMonster_EntityId_ = (unsigned int)-1;
    testMonsterVision_EntityId_ = (unsigned int)-1;
    testLamp_EntityId_ = (unsigned int)-1;
    testLampLight_EntityId_ = (unsigned int)-1;
    testHouse_EntityId_ = (unsigned int)-1;
    testHouseLight_EntityId_ = (unsigned int)-1;

    paused_ = false;
}

void TestScene103::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    UpdateCursorPosition(cursorPositionEvent.xPosition, cursorPositionEvent.yPosition);

    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        collisionPoints[cursorPositionCollisionPointIndex_].enabled = true;
    }
}

void TestScene103::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_ESCAPE)
        {
            TestApplicationData* testApplicationDataPtr = GetSharedDataPtr<TestApplicationData>();
            SendEventToApplication(Project001::SwitchSceneEvent(testApplicationDataPtr->testScene001Id));
            if (GetActiveScene()->GetId() == testApplicationDataPtr->testScene001Id)
            {
                SendEventToScene(GetId(), Project001::DeinitializeEvent());
                SendEventToApplication(Project001::InitializeEvent());
            }
            return;
        }
    }
    else if (keyCode == Project001::KeyCode::KEY_CODE_1)
    {
        Project001::Camera* cameraPtr = nullptr;
        if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraDebug_EntityId_))
        {
            cameraPtr->SetTurnedOn(!cameraPtr->GetTurnedOn());
        }
    }
    else if (keyCode == Project001::KeyCode::KEY_CODE_2)
    {
        auto ToggleCameraProjection = [&](unsigned int& camera_EntityId)
            {
                Project001::Camera* cameraPtr = nullptr;
                if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, camera_EntityId))
                {
                    if (cameraPtr->GetProjection() == Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE)
                    {
                        cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
                    }
                    else
                    {
                        cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE);
                    }
                }
            };

        ToggleCameraProjection(mainCameraLight1_EntityId_);
        ToggleCameraProjection(mainCameraLight2_EntityId_);
        ToggleCameraProjection(mainCameraDark1_EntityId_);
        ToggleCameraProjection(mainCameraDark2_EntityId_);
        ToggleCameraProjection(mainCameraDebug_EntityId_);
    }
    else if (keyCode == Project001::KeyCode::KEY_CODE_3)
    {
        mainCamera_DistanceAway_ = 600.0f; // 800.0f;

        Project001::Camera* cameraPtr = nullptr;
        if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraLight1_EntityId_))
        {
            cameraPtr->ResetOrientation();
            cameraPtr->AddPitch(glm::quarter_pi<float>());
            cameraPtr->AddYaw(glm::pi<float>());
        }
    }
    else if (keyCode == Project001::KeyCode::KEY_CODE_4)
    {
        mainCamera_DistanceAway_ = 600.0f; // 800.0f;

        Project001::Camera* cameraPtr = nullptr;
        if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraLight1_EntityId_))
        {
            cameraPtr->ResetOrientation();
            cameraPtr->AddYaw(glm::pi<float>());
        }
    }
    else if (keyCode == Project001::KeyCode::KEY_CODE_C)
    {
        mainCamera_Locked_ = !mainCamera_Locked_;
    }
    else if (keyCode == Project001::KeyCode::KEY_CODE_P)
    {
        paused_ = !paused_;
    }
    else if (keyCode == Project001::KeyCode::KEY_CODE_L)
    {
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, testLampLight_EntityId_));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetEnabled(!collisionBody2DPtr->GetEnabled());
        }

        collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, testHouseLight_EntityId_));
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetEnabled(!collisionBody2DPtr->GetEnabled());
        }
    }
}

void TestScene103::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorPressCollisionPointIndex_].position = collisionPoints[cursorPositionCollisionPointIndex_].position;
                collisionPoints[cursorPressCollisionPointIndex_].enabled = true;
                collisionPoints[cursorReleaseCollisionPointIndex_].enabled = false;
            }
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[cursorReleaseCollisionPointIndex_].position = collisionPoints[cursorPositionCollisionPointIndex_].position;
                collisionPoints[cursorReleaseCollisionPointIndex_].enabled = true;
            }
        }
    }
}

void TestScene103::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    Project001::RenderSystem::Render(GetComponentStoresPtr(), GetRendererPtr());
}

void TestScene103::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    constexpr float speedConstant = 20.0f;

    mainCamera_DistanceAway_ += yOffset * speedConstant;
}

void TestScene103::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    if (paused_)
    {
        return;
    }

    unsigned long long timestep_ns = updateEvent.timestep_ns;
    float timestep_s = (float)timestep_ns / 1e9f;

    // Move and Calculate collisions
    // -------------------------------------------------------------------------
    constexpr size_t physicsStepsPerUpdate = 1;
    float physicsTimestep_s = timestep_s / (float)physicsStepsPerUpdate;
    for (size_t i = 0; i < physicsStepsPerUpdate; ++i)
    {
        Project001::CollisionSystem2D::ApplyMovement(GetComponentStoresPtr(), physicsTimestep_s);
        Project001::CollisionSystem2D::CalculateCollisionsWithQuadTree(GetComponentStoresPtr());

        UpdatePersonLogic(physicsTimestep_s);
        UpdatePlayerEntityVelocity();
        UpdateMainCameraEntityPosition(physicsTimestep_s);

        AnimatePlayerRenderedModel(physicsTimestep_s);

        // Update cursor because camera updates
        // -------------------------------------------------------------------------
        float cursorX_position;
        float cursorY_position;
        GetWindowPtr()->GetCursorPosition(cursorX_position, cursorY_position);
        UpdateCursorPosition(cursorX_position, cursorY_position);
    }

    // Sync rendered models
    // -------------------------------------------------------------------------
    SyncCursorRenderedModel();
    SyncPlayerRenderedModel();
    SyncPlayerLightRenderedModel();

    for (size_t i = 0; i < testPeople_Size_; ++i)
    {
        SyncTestPersonRenderedModel(testPeople_EntityIds_[i]);
    }
    SyncTestPersonRenderedModel(testPerson_EntityId_);
    SyncTestMonsterRenderedModel();
    SyncTestMonsterVisionRenderedModel();
    SyncTestLampRenderedModel();
    SyncTestLampLightRenderedModel();
    SyncTestHouseRenderedModel();
    SyncTestHouseLightRenderedModel();

    UpdateCollisionBodyQuadTreeMesh();
}

void TestScene103::InitializeInstructionScene()
{
    Project001::FontData font01_FontData;
    FAIL_CHECK(Project001::FontLoader::LoadFontDataFromMemory(
        font01_FontData,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    Project001::TextureData font01_TextureData;
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        font01_TextureData,
        g_AntonioRegular_png,
        sizeof(g_AntonioRegular_png)
    ));
    unsigned int font01_TextureId = (unsigned int)-1;
    GetRendererPtr()->CreateTexture(
        font01_TextureId,
        font01_TextureData.data,
        font01_TextureData.width,
        font01_TextureData.height,
        font01_TextureData.bytesPerPixel,
        true,
        false
    );

    const uint32_t uiCamera_Mask = 0b01000000000000000000000000000000;

    unsigned int uiCamera_EntityId = (unsigned int)-1;
    GetComponentStoresPtr()->CreateEntity(uiCamera_EntityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(uiCamera_EntityId));
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, uiCamera_EntityId));
    if (cameraPtr != nullptr)
    {
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
            float uiCameraHalfHeight = 3.5f;
            float uiCameraHalfWidth = aspectRatio * uiCameraHalfHeight;
            cameraPtr->SetAspectRatio(aspectRatio);
            cameraPtr->SetTopCutoff(uiCameraHalfHeight);
            cameraPtr->SetBottomCutoff(-uiCameraHalfHeight);
            cameraPtr->SetLeftCutoff(-uiCameraHalfWidth);
            cameraPtr->SetRightCutoff(uiCameraHalfWidth);
            cameraPtr->SetNearCutoff(-1.0f);
            cameraPtr->SetFarCutoff(1.0f);
        }
        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
        cameraPtr->SetDepthTestEnabled(false);
        cameraPtr->SetCameraMask(uiCamera_Mask);
        cameraPtr->SetPriorityValue(1000000);
    }

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "TODO:\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = &font01_FontData;
    instructionSceneInfo.fontTextureId = font01_TextureId;
    instructionSceneInfo.cameraEntityId = uiCamera_EntityId;
    instructionSceneInfo.cameraMask = uiCamera_Mask;
    instructionSceneInfo.locatedOnBottom = true;
    instructionSceneInfo.keyCode_toggleInstructions = keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene103::LoadCursorResources()
{
    cursorCircle_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*cursorCircle_MeshDataPtr_, 4.0f, 12));
}

void TestScene103::LoadPixelFontResources()
{
    pixelFont_FontDataPtr_ = &Project001::Get_PixelFont5x6_FontData();
    pixelFont_TextureDataPtr_ = &Project001::Get_PixelFont5x6_TextureData();
    GetRendererPtr()->CreateTexture(
        pixelFont_TextureId_,
        pixelFont_TextureDataPtr_->data,
        pixelFont_TextureDataPtr_->width,
        pixelFont_TextureDataPtr_->height,
        pixelFont_TextureDataPtr_->bytesPerPixel,
        false,
        false
    );
}

void TestScene103::LoadFogResources()
{
    fog_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*fog_MeshDataPtr_, 20480.0f, 20480.0f, 0.0f, 1.0f, 0.0f, 1.0f));
}

void TestScene103::LoadGroundResources()
{
    groundDark_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*groundDark_MeshDataPtr_, 2048.0f, 2048.0f, 0.0f, 32.0f, 0.0f, 32.0f));

    groundDark_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*groundDark_TextureDataPtr_, "../Textures/gridDark_64x64.png"));
    GetRendererPtr()->CreateTexture(
        groundDark_TextureId_,
        groundDark_TextureDataPtr_->data,
        groundDark_TextureDataPtr_->width,
        groundDark_TextureDataPtr_->height,
        groundDark_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    groundLit_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*groundLit_MeshDataPtr_, 2048.0f, 2048.0f, 0.0f, 32.0f, 0.0f, 32.0f));

    groundLit_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*groundLit_TextureDataPtr_, "../Textures/gridLit_64x64.png"));
    GetRendererPtr()->CreateTexture(
        groundLit_TextureId_,
        groundLit_TextureDataPtr_->data,
        groundLit_TextureDataPtr_->width,
        groundLit_TextureDataPtr_->height,
        groundLit_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    {
        Project001::MeshData tempMeshData0;
        Project001::MeshLoader::Generate2DRectangle(tempMeshData0, 128.0f, 2048.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData0, glm::vec3(-1088.0f, 0.0f, 0.0f));

        Project001::MeshData tempMeshData1;
        Project001::MeshLoader::Generate2DRectangle(tempMeshData1, 128.0f, 2048.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData1, glm::vec3(1088.0f, 0.0f, 0.0f));

        Project001::MeshData tempMeshData2;
        Project001::MeshLoader::Generate2DRectangle(tempMeshData2, 2304.0f, 128.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData2, glm::vec3(0.0f, 1088.0f, 0.0f));

        Project001::MeshData tempMeshData3;
        Project001::MeshLoader::Generate2DRectangle(tempMeshData3, 2304.0f, 128.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData3, glm::vec3(0.0f, -1088.0f, 0.0f));

        groundCollision_MeshDataPtr_ = new Project001::MeshData();
        Project001::MeshLoader::CopyMesh(*groundCollision_MeshDataPtr_, tempMeshData0);
        Project001::MeshLoader::CopyMesh(*groundCollision_MeshDataPtr_, tempMeshData1);
        Project001::MeshLoader::CopyMesh(*groundCollision_MeshDataPtr_, tempMeshData2);
        Project001::MeshLoader::CopyMesh(*groundCollision_MeshDataPtr_, tempMeshData3);
    }
}

void TestScene103::LoadGroundGridResources()
{
    groundGrid_MeshDataPtr_ = new Project001::MeshData();
    const float gridSpacing = 64.0f;
    const float gridSize = 16.0f * gridSpacing;
    const float lineWidth = 2.0f;

    for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
    {
        FAIL_CHECK(Project001::MeshLoader::Generate2DLine(
            *groundGrid_MeshDataPtr_,
            glm::vec2(-gridSize - 0.5f * lineWidth, f),
            glm::vec2(gridSize + 0.5f * lineWidth, f),
            lineWidth));
    }
    for (float f = -gridSize; f < gridSize + 1.0f; f += gridSpacing)
    {
        for (float g = -gridSize; g < gridSize - 1.0f; g += gridSpacing)
        {
            FAIL_CHECK(Project001::MeshLoader::Generate2DLine(
                *groundGrid_MeshDataPtr_,
                glm::vec2(f, g + 0.5f * lineWidth),
                glm::vec2(f, g - 0.5f * lineWidth + gridSpacing),
                lineWidth));
        }
    }

    GetRendererPtr()->CreateMesh(
        groundGrid_MeshId_,
        groundGrid_MeshDataPtr_->meshVertexArray.data(),
        (unsigned int)groundGrid_MeshDataPtr_->meshVertexArray.size(),
        groundGrid_MeshDataPtr_->meshIndexArray.data(),
        (unsigned int)groundGrid_MeshDataPtr_->meshIndexArray.size()
    );

    groundGridLabels_MeshDataPtr_ = new Project001::MeshData();

    std::vector<std::string> gridLabels;
    gridLabels.push_back("0");
    gridLabels.push_back("1");
    gridLabels.push_back("2");
    gridLabels.push_back("3");
    gridLabels.push_back("4");
    gridLabels.push_back("5");
    gridLabels.push_back("6");
    gridLabels.push_back("7");
    gridLabels.push_back("8");
    gridLabels.push_back("9");
    gridLabels.push_back("10");
    gridLabels.push_back("11");
    gridLabels.push_back("12");
    gridLabels.push_back("13");
    gridLabels.push_back("14");
    gridLabels.push_back("15");

    // positive x-axis -----------------------------------------------------

    const float pixelFont_pixelSize = 2.0f;
    float gridLabel_offsetY = pixelFont_pixelSize * 1.0f + 0.5f * lineWidth;
    float gridLabel_offsetX = pixelFont_pixelSize * 1.0f + 0.5f * lineWidth;

    for (size_t i = 0; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *pixelFont_FontDataPtr_,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*groundGridLabels_MeshDataPtr_, currentLabelMeshData);

        gridLabel_offsetX += gridSpacing;
    }

    // positive y-axis -----------------------------------------------------

    gridLabel_offsetY = pixelFont_pixelSize * 1.0f + 0.5f * lineWidth + gridSpacing;
    gridLabel_offsetX = pixelFont_pixelSize * 1.0f + 0.5f * lineWidth;

    for (size_t i = 1; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *pixelFont_FontDataPtr_,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*groundGridLabels_MeshDataPtr_, currentLabelMeshData);

        gridLabel_offsetY += gridSpacing;
    }

    // negative x-axis -----------------------------------------------------

    gridLabel_offsetY = pixelFont_pixelSize * -7.0f - 0.5f * lineWidth;
    gridLabel_offsetX = pixelFont_pixelSize * -6.0f - 0.5f * lineWidth;

    for (size_t i = 0; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        float addition_offsetX = pixelFont_pixelSize * -6.0f * (float)(currentLabel.length() - 1);

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *pixelFont_FontDataPtr_,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX + addition_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*groundGridLabels_MeshDataPtr_, currentLabelMeshData);

        gridLabel_offsetX -= gridSpacing;
    }

    // negative y-axis -----------------------------------------------------

    gridLabel_offsetY = pixelFont_pixelSize * -7.0f - 0.5f * lineWidth - gridSpacing;
    gridLabel_offsetX = pixelFont_pixelSize * -6.0f - 0.5f * lineWidth;

    for (size_t i = 1; i < gridLabels.size(); ++i)
    {
        const std::string& currentLabel = gridLabels[i];

        float addition_offsetX = pixelFont_pixelSize * -6.0f * (float)(currentLabel.length() - 1);

        Project001::MeshData currentLabelMeshData;
        FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
            currentLabelMeshData,
            *pixelFont_FontDataPtr_,
            currentLabel,
            pixelFont_pixelSize
        ));
        Project001::MeshLoader::TranslateMesh(
            currentLabelMeshData,
            glm::vec3(gridLabel_offsetX + addition_offsetX, gridLabel_offsetY, 0.0f)
        );

        Project001::MeshLoader::CopyMesh(*groundGridLabels_MeshDataPtr_, currentLabelMeshData);

        gridLabel_offsetY -= gridSpacing;
    }

    GetRendererPtr()->CreateMesh(
        groundGridLabels_MeshId_,
        groundGridLabels_MeshDataPtr_->meshVertexArray.data(),
        (unsigned int)groundGridLabels_MeshDataPtr_->meshVertexArray.size(),
        groundGridLabels_MeshDataPtr_->meshIndexArray.data(),
        (unsigned int)groundGridLabels_MeshDataPtr_->meshIndexArray.size()
    );
}

void TestScene103::LoadPlayerResources()
{
    constexpr float spriteWidth = 32.0f;
    constexpr float spriteHeight = 56.0f;

    playerDark_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*playerDark_MeshDataPtr_, spriteWidth, spriteHeight, 0.0f, 1.0f, 0.0f, 1.0f));
    Project001::MeshLoader::TranslateMesh(*playerDark_MeshDataPtr_, glm::vec3(0.0f, 0.5 * spriteHeight, 0.0f));
    Project001::MeshLoader::RotateMeshX(*playerDark_MeshDataPtr_, glm::half_pi<float>());

    playerDark_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*playerDark_TextureDataPtr_, "../Textures/personDark_32x56.png"));
    GetRendererPtr()->CreateTexture(
        playerDark_TextureId_,
        playerDark_TextureDataPtr_->data,
        playerDark_TextureDataPtr_->width,
        playerDark_TextureDataPtr_->height,
        playerDark_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    playerLit_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*playerLit_MeshDataPtr_, spriteWidth, spriteHeight, 0.0f, 1.0f, 0.0f, 1.0f));
    Project001::MeshLoader::TranslateMesh(*playerLit_MeshDataPtr_, glm::vec3(0.0f, 0.5 * spriteHeight, 0.0f));
    Project001::MeshLoader::RotateMeshX(*playerLit_MeshDataPtr_, glm::half_pi<float>());

    playerLit_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*playerLit_TextureDataPtr_, "../Textures/personLit_32x56.png"));
    GetRendererPtr()->CreateTexture(
        playerLit_TextureId_,
        playerLit_TextureDataPtr_->data,
        playerLit_TextureDataPtr_->width,
        playerLit_TextureDataPtr_->height,
        playerLit_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    playerCollision_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*playerCollision_MeshDataPtr_, 8.0f, 8));
}

void TestScene103::LoadPlayerLightResources()
{
    {
        Project001::MeshData tempMeshData0;
        Project001::MeshData tempMeshData1;
        playerLightBottom_MeshDataPtr_ = new Project001::MeshData();
        playerLightTop_MeshDataPtr_ = new Project001::MeshData();
        std::vector<glm::vec3> capCorners;

        FAIL_CHECK(Project001::MeshLoader::GenerateIceCreamCup(tempMeshData0, 112.0f, 64.0f, 8.0f, 16, 8, false));
        Project001::MeshLoader::TranslateMesh(tempMeshData0, glm::vec3(0.0f, 104.0f, 24.0f));
        // Project001::MeshLoader::SliceMeshWithAPlane(
        //     *playerLightTop_MeshDataPtr_, *playerLightBottom_MeshDataPtr_, capCorners, tempMeshData0, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
        // Project001::MeshLoader::ScaleMesh(*playerLightBottom_MeshDataPtr_, glm::vec3(1.0f, 1.0f, -1.0f));
        // Project001::MeshLoader::TurnInsideOut(*playerLightBottom_MeshDataPtr_);
        Project001::MeshLoader::SliceMeshWithAPlane(
            *playerLightTop_MeshDataPtr_, tempMeshData1, capCorners, tempMeshData0, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
        
        std::vector<glm::vec2> corners;
        for (size_t i = 0; i < capCorners.size(); ++i)
        {
            corners.emplace_back(capCorners[i].x, capCorners[i].y);
        }
        std::vector<glm::vec2> noDupCorners;
        Project001::RemoveDuplicates(noDupCorners, corners);
        Project001::OrderPointsCCW(noDupCorners, 0.0f);
        Project001::MeshLoader::Generate2DPolygon(*playerLightBottom_MeshDataPtr_, noDupCorners);
        Project001::MeshLoader::TranslateMesh(*playerLightBottom_MeshDataPtr_, glm::vec3(0.0f, -16.0f, 1.0f));

        playerLightStrong_MeshDataPtr_ = new Project001::MeshData();
        // FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*playerLightStrong_MeshDataPtr_, 320.0f, 32, 32, false));
        FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*playerLightStrong_MeshDataPtr_, 320.0f, 32, false));
        Project001::MeshLoader::TranslateMesh(*playerLightStrong_MeshDataPtr_, glm::vec3(0.0f, 0.0f, 0.1f));
    }

    playerLightCollision_MeshDataPtr_ = new Project001::MeshData();
    {
        std::vector<glm::vec2> corners;
        constexpr float radius = 60.0f;
        constexpr size_t subdivisions = 8;
        constexpr float angleRotation = glm::pi<float>() / (float)subdivisions;
        for (size_t i = 0; i <= subdivisions; ++i)
        {
            glm::vec2 radiusVector(radius, 0.0f);
            radiusVector = Project001::Rotate2DVector(radiusVector, (float)i * angleRotation);
            corners.emplace_back(radiusVector);
        }
        corners.emplace_back(-4.0f, -112.0f);
        corners.emplace_back(4.0f, -112.0f);
        for (size_t i = 0; i < corners.size(); ++i)
        {
            corners[i].y += 112.0f;
        }
        FAIL_CHECK(Project001::MeshLoader::Generate2DPolygon(*playerLightCollision_MeshDataPtr_, corners));
    }
}

void TestScene103::LoadTestUiResources()
{
    testUiBackground_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(*testUiBackground_MeshDataPtr_, 96.0f, 48.0f));

    float pixelSize = 2.0f;

    testUiText01_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *testUiText01_MeshDataPtr_,
        *pixelFont_FontDataPtr_,
        "C:99(99)",
        pixelSize
    ));

    testUiText02_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *testUiText02_MeshDataPtr_,
        *pixelFont_FontDataPtr_,
        "B:99.99",
        pixelSize
    ));

    testUiText03_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *testUiText03_MeshDataPtr_,
        *pixelFont_FontDataPtr_,
        "S:99.99",
        pixelSize
    ));
}

void TestScene103::LoadTestPersonResources()
{
    constexpr float spriteWidth = 32.0f;
    constexpr float spriteHeight = 56.0f;

    testPersonLit_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*testPersonLit_MeshDataPtr_, spriteWidth, spriteHeight, 0.0f, 1.0f, 0.0f, 1.0f));
    Project001::MeshLoader::TranslateMesh(*testPersonLit_MeshDataPtr_, glm::vec3(0.0f, 0.5 * spriteHeight, 0.0f));
    Project001::MeshLoader::RotateMeshX(*testPersonLit_MeshDataPtr_, glm::half_pi<float>());

    testPersonLit_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*testPersonLit_TextureDataPtr_, "../Textures/personLit_32x56.png"));
    GetRendererPtr()->CreateTexture(
        testPersonLit_TextureId_,
        testPersonLit_TextureDataPtr_->data,
        testPersonLit_TextureDataPtr_->width,
        testPersonLit_TextureDataPtr_->height,
        testPersonLit_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    testPersonDark_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*testPersonDark_MeshDataPtr_, spriteWidth, spriteHeight, 0.0f, 1.0f, 0.0f, 1.0f));
    Project001::MeshLoader::TranslateMesh(*testPersonDark_MeshDataPtr_, glm::vec3(0.0f, 0.5 * spriteHeight, 0.0f));
    Project001::MeshLoader::RotateMeshX(*testPersonDark_MeshDataPtr_, glm::half_pi<float>());

    testPersonDark_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*testPersonDark_TextureDataPtr_, "../Textures/personDark_32x56.png"));
    GetRendererPtr()->CreateTexture(
        testPersonDark_TextureId_,
        testPersonDark_TextureDataPtr_->data,
        testPersonDark_TextureDataPtr_->width,
        testPersonDark_TextureDataPtr_->height,
        testPersonDark_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    testPersonCollision_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*testPersonCollision_MeshDataPtr_, 8.0f, 8));
}

void TestScene103::LoadTestMonsterResources()
{
    constexpr float spriteWidth = 32.0f;
    constexpr float spriteHeight = 56.0f;

    testMonsterLit_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*testMonsterLit_MeshDataPtr_, spriteWidth, spriteHeight, 0.0f, 1.0f, 0.0f, 1.0f));
    Project001::MeshLoader::TranslateMesh(*testMonsterLit_MeshDataPtr_, glm::vec3(0.0f, 0.5 * spriteHeight, 0.0f));
    Project001::MeshLoader::RotateMeshX(*testMonsterLit_MeshDataPtr_, glm::half_pi<float>());

    testMonsterLit_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*testMonsterLit_TextureDataPtr_, "../Textures/monsterLit_32x56.png"));
    GetRendererPtr()->CreateTexture(
        testMonsterLit_TextureId_,
        testMonsterLit_TextureDataPtr_->data,
        testMonsterLit_TextureDataPtr_->width,
        testMonsterLit_TextureDataPtr_->height,
        testMonsterLit_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    testMonsterDark_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(*testMonsterDark_MeshDataPtr_, spriteWidth, spriteHeight, 0.0f, 1.0f, 0.0f, 1.0f));
    Project001::MeshLoader::TranslateMesh(*testMonsterDark_MeshDataPtr_, glm::vec3(0.0f, 0.5 * spriteHeight, 0.0f));
    Project001::MeshLoader::RotateMeshX(*testMonsterDark_MeshDataPtr_, glm::half_pi<float>());

    testMonsterDark_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*testMonsterDark_TextureDataPtr_, "../Textures/personDark_32x56.png"));
    GetRendererPtr()->CreateTexture(
        testMonsterDark_TextureId_,
        testMonsterDark_TextureDataPtr_->data,
        testMonsterDark_TextureDataPtr_->width,
        testMonsterDark_TextureDataPtr_->height,
        testMonsterDark_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    testMonsterCollision_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*testMonsterCollision_MeshDataPtr_, 8.0f, 8));
}

void TestScene103::LoadTestMonsterVisionResources()
{
    testMonsterVisionCollision_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*testMonsterVisionCollision_MeshDataPtr_, 96.0f, 16));
}

void TestScene103::LoadTestLampResources()
{
    {
        Project001::MeshData tempMeshData0; // lamp neck
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(tempMeshData0, 32.0f, 128.0f, 0.0f, 1.0f, 0.0f, 0.8f));
        Project001::MeshLoader::TranslateMesh(tempMeshData0, glm::vec3(0.0f, 64.0f, 0.0f));

        Project001::MeshData tempMeshData1; // lamp head
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(tempMeshData1, 32.0f, 32.0f, 0.0f, 1.0f, 0.8f, 1.0f));
        Project001::MeshLoader::RotateMeshX(tempMeshData1, -glm::half_pi<float>());
        Project001::MeshLoader::TranslateMesh(tempMeshData1, glm::vec3(0.0f, 128.0f, 16.0f));

        testLampLit_MeshDataPtr_ = new Project001::MeshData();
        Project001::MeshLoader::CopyMesh(*testLampLit_MeshDataPtr_, tempMeshData0);
        Project001::MeshLoader::CopyMesh(*testLampLit_MeshDataPtr_, tempMeshData1);
        Project001::MeshLoader::RotateMeshX(*testLampLit_MeshDataPtr_, glm::half_pi<float>());
    }

    testLampLit_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*testLampLit_TextureDataPtr_, "../Textures/lampLit_32x160.png"));
    GetRendererPtr()->CreateTexture(
        testLampLit_TextureId_,
        testLampLit_TextureDataPtr_->data,
        testLampLit_TextureDataPtr_->width,
        testLampLit_TextureDataPtr_->height,
        testLampLit_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    {
        testLampDark_MeshDataPtr_ = new Project001::MeshData();
        Project001::MeshLoader::CopyMesh(*testLampDark_MeshDataPtr_, *testLampLit_MeshDataPtr_);
    }

    testLampDark_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*testLampDark_TextureDataPtr_, "../Textures/lampDark_32x160.png"));
    GetRendererPtr()->CreateTexture(
        testLampDark_TextureId_,
        testLampDark_TextureDataPtr_->data,
        testLampDark_TextureDataPtr_->width,
        testLampDark_TextureDataPtr_->height,
        testLampDark_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    testLampCollision_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*testLampCollision_MeshDataPtr_, 8.0f, 8));
}

void TestScene103::LoadTestLampLightResources()
{
    testLampLightTop_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::GenerateTruncatedCone(*testLampLightTop_MeshDataPtr_, 120.0f, 8.0f, 64.0f, 16, false));
    Project001::MeshLoader::RotateMeshX(*testLampLightTop_MeshDataPtr_, glm::half_pi<float>());
    Project001::MeshLoader::TranslateMesh(*testLampLightTop_MeshDataPtr_, glm::vec3(0.0f, -16.0f, 60.0f));

    testLampLightBottom_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*testLampLightBottom_MeshDataPtr_, 64.0f, 16));
    Project001::MeshLoader::TranslateMesh(*testLampLightBottom_MeshDataPtr_, glm::vec3(0.0f, -16.0f, 1.0f));

    testLampLightCollision_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*testLampLightCollision_MeshDataPtr_, 64.0f, 16));
    Project001::MeshLoader::TranslateMesh(*testLampLightCollision_MeshDataPtr_, glm::vec3(0.0f, -16.0f, 0.0f));
}

void TestScene103::LoadTestHouseResources()
{
    {
        Project001::MeshData tempMeshData0; // front face
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData0, 128.0f, 112.0f,
            128.0f / 384.0f,
            256.0f / 384.0f,
            0.0f,
            112.0f / 256.0f
        ));
        Project001::MeshLoader::TranslateMesh(tempMeshData0, glm::vec3(0.0f, 56.0f, 64.0f));

        Project001::MeshData tempMeshData1; // left face
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData1, 128.0f, 80.0f,
            0.0f,
            128.0f / 384.0f,
            0.0f,
            80.0f / 256.0f
        ));
        Project001::MeshLoader::TranslateMesh(tempMeshData1, glm::vec3(0.0f, 40.0f, 64.0f));
        Project001::MeshLoader::RotateMeshY(tempMeshData1, -glm::half_pi<float>());

        Project001::MeshData tempMeshData2; // right face
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData2, 128.0f, 80.0f,
            256.0f / 384.0f,
            1.0f,
            0.0f,
            80.0f / 256.0f
        ));
        Project001::MeshLoader::TranslateMesh(tempMeshData2, glm::vec3(0.0f, 40.0f, 64.0f));
        Project001::MeshLoader::RotateMeshY(tempMeshData2, glm::half_pi<float>());

        Project001::MeshData tempMeshData3; // left roof
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData3, 80.0f, 144.0f,
            112.0f / 384.0f,
            192.0f / 384.0f,
            112.0f / 256.0f,
            1.0f
        ));
        Project001::MeshLoader::RotateMeshX(tempMeshData3, -glm::half_pi<float>());
        Project001::MeshLoader::TranslateMesh(tempMeshData3, glm::vec3(-40.0f, 0.0f, 0.0f));
        Project001::MeshLoader::RotateMeshZ(tempMeshData3, glm::pi<float>() / 7.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData3, glm::vec3(0.0f, 114.0f, 0.0f));

        Project001::MeshData tempMeshData4; // right roof
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData4, 80.0f, 144.0f,
            192.0f / 384.0f,
            272.0f / 384.0f,
            112.0f / 256.0f,
            1.0f
        ));
        Project001::MeshLoader::RotateMeshX(tempMeshData4, -glm::half_pi<float>());
        Project001::MeshLoader::TranslateMesh(tempMeshData4, glm::vec3(40.0f, 0.0f, 0.0f));
        Project001::MeshLoader::RotateMeshZ(tempMeshData4, -glm::pi<float>() / 7.0f);
        Project001::MeshLoader::TranslateMesh(tempMeshData4, glm::vec3(0.0f, 114.0f, 0.0f));

        Project001::MeshData tempMeshData5; // door mat
        FAIL_CHECK(Project001::MeshLoader::Generate2DSprite(
            tempMeshData5, 48.0f, 32.0f,
            0.0f,
            48.0f / 384.0f,
            224.0f / 256.0f,
            1.0f
        ));
        Project001::MeshLoader::RotateMeshX(tempMeshData5, -glm::half_pi<float>());
        Project001::MeshLoader::TranslateMesh(tempMeshData5, glm::vec3(0.0f, 0.4f, 80.0f));

        testHouseLit_MeshDataPtr_ = new Project001::MeshData();
        Project001::MeshLoader::CopyMesh(*testHouseLit_MeshDataPtr_, tempMeshData3);
        Project001::MeshLoader::CopyMesh(*testHouseLit_MeshDataPtr_, tempMeshData4);
        Project001::MeshLoader::CopyMesh(*testHouseLit_MeshDataPtr_, tempMeshData0);
        Project001::MeshLoader::CopyMesh(*testHouseLit_MeshDataPtr_, tempMeshData1);
        Project001::MeshLoader::CopyMesh(*testHouseLit_MeshDataPtr_, tempMeshData2);
        Project001::MeshLoader::CopyMesh(*testHouseLit_MeshDataPtr_, tempMeshData5);
        Project001::MeshLoader::RotateMeshX(*testHouseLit_MeshDataPtr_, glm::half_pi<float>());
    }

    testHouseLit_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*testHouseLit_TextureDataPtr_, "../Textures/houseLit_384x256.png"));
    GetRendererPtr()->CreateTexture(
        testHouseLit_TextureId_,
        testHouseLit_TextureDataPtr_->data,
        testHouseLit_TextureDataPtr_->width,
        testHouseLit_TextureDataPtr_->height,
        testHouseLit_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    {
        testHouseDark_MeshDataPtr_ = new Project001::MeshData();
        Project001::MeshLoader::CopyMesh(*testHouseDark_MeshDataPtr_, *testHouseLit_MeshDataPtr_);
    }

    testHouseDark_TextureDataPtr_ = new Project001::TextureData();
    FAIL_CHECK(Project001::TextureLoader::LoadTexture(*testHouseDark_TextureDataPtr_, "../Textures/houseDark_384x256.png"));
    GetRendererPtr()->CreateTexture(
        testHouseDark_TextureId_,
        testHouseDark_TextureDataPtr_->data,
        testHouseDark_TextureDataPtr_->width,
        testHouseDark_TextureDataPtr_->height,
        testHouseDark_TextureDataPtr_->bytesPerPixel,
        false, false
    );

    testHouseCollision_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(*testHouseCollision_MeshDataPtr_, 128.0f, 128.0f));

    testHouseText_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::FontLoader::GenerateMeshDataFromFontDataAndString(
        *testHouseText_MeshDataPtr_,
        *pixelFont_FontDataPtr_,
        "HOME",
        2.0f
    ));
    Project001::MeshLoader::RecenterMesh(*testHouseText_MeshDataPtr_);
    Project001::MeshLoader::RotateMeshX(*testHouseText_MeshDataPtr_, glm::half_pi<float>());
}

void TestScene103::LoadTestHouseLightResources()
{
    testHouseLightTop_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::GenerateHemisphere(*testHouseLightTop_MeshDataPtr_, 136.0f, 16, 8, false));
    Project001::MeshLoader::RotateMeshX(*testHouseLightTop_MeshDataPtr_, glm::half_pi<float>());
    Project001::MeshLoader::TranslateMesh(*testHouseLightTop_MeshDataPtr_, glm::vec3(0.0f, 0.0f, 68.0f));

    testHouseLightBottom_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*testHouseLightBottom_MeshDataPtr_, 136.0f, 16));
    Project001::MeshLoader::TranslateMesh(*testHouseLightBottom_MeshDataPtr_, glm::vec3(0.0f, 1.0f, 1.0f));

    testHouseLightCollision_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*testHouseLightCollision_MeshDataPtr_, 136.0f, 16));

    testHouseDoorCollision_MeshDataPtr_ = new Project001::MeshData();
    FAIL_CHECK(Project001::MeshLoader::Generate2DRectangle(*testHouseDoorCollision_MeshDataPtr_, 48.0f, 32.0f));
    Project001::MeshLoader::TranslateMesh(*testHouseDoorCollision_MeshDataPtr_, glm::vec3(0.0f, -80.0f, 0.0f));
}

void TestScene103::CreateMainCameraEntities()
{
    int aspectRatioNumerator;
    int aspectRatioDenominator;
    GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);

    float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
    constexpr float mainCameraHalfHeight = 320.0f;
    float mainCameraHalfWidth = aspectRatio * mainCameraHalfHeight;

    constexpr float mainCameraNearCutoff = mainCameraHalfHeight * 0.1f;
    constexpr float mainCameraFarCutoff = mainCameraHalfHeight * 15.0f;

    constexpr float mainCameraPitch = glm::quarter_pi<float>();
    constexpr float mainCameraYaw = glm::pi<float>();

    constexpr Project001::Camera::CameraProjection mainCameraProjection =
        Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE;

    mainCamera_DistanceAway_ = 600.0f; // 800.0f;

    auto CreateMainCamera = [&](unsigned int& camera_EntityId, const uint32_t camera_Mask, int priorityValue)
        {
            GetComponentStoresPtr()->CreateEntity(camera_EntityId);
            FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(camera_EntityId));

            Project001::Camera* cameraPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, camera_EntityId));
            if (cameraPtr != nullptr)
            {
                if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
                {
                    cameraPtr->SetAspectRatio(aspectRatio);
                    cameraPtr->SetTopCutoff(mainCameraHalfHeight);
                    cameraPtr->SetBottomCutoff(-mainCameraHalfHeight);
                    cameraPtr->SetLeftCutoff(-mainCameraHalfWidth);
                    cameraPtr->SetRightCutoff(mainCameraHalfWidth);
                    cameraPtr->SetNearCutoff(mainCameraNearCutoff);
                    cameraPtr->SetFarCutoff(mainCameraFarCutoff);
                }

                cameraPtr->AddPitch(mainCameraPitch);
                cameraPtr->AddYaw(mainCameraYaw);
                cameraPtr->SetProjection(mainCameraProjection);
                cameraPtr->SetCameraMask(camera_Mask);
                cameraPtr->SetPriorityValue(priorityValue);
            }
        };

    CreateMainCamera(mainCameraLight1_EntityId_, s_mainCameraLight1_Mask_, -100);
    CreateMainCamera(mainCameraLight2_EntityId_, s_mainCameraLight2_Mask_, -50);
    CreateMainCamera(mainCameraDark1_EntityId_, s_mainCameraDark1_Mask_, 50);
    CreateMainCamera(mainCameraDark2_EntityId_, s_mainCameraDark2_Mask_, 100);
    CreateMainCamera(mainCameraDebug_EntityId_, s_mainCameraDebug_Mask_, 150);

    {
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraDark2_EntityId_));
        if (cameraPtr != nullptr)
        {
            cameraPtr->SetTurnedOn(false);
        }
    };
}

void TestScene103::CreateUiCameraEntity()
{
    GetComponentStoresPtr()->CreateEntity(uiCamera_EntityId_);
    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(uiCamera_EntityId_));

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, uiCamera_EntityId_));
    if (cameraPtr != nullptr)
    {
        int aspectRatioNumerator;
        int aspectRatioDenominator;
        GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);
        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;
            float uiCameraHalfHeight = 320.0f;
            float uiCameraHalfWidth = aspectRatio * uiCameraHalfHeight;
            cameraPtr->SetAspectRatio(aspectRatio);
            cameraPtr->SetTopCutoff(uiCameraHalfHeight);
            cameraPtr->SetBottomCutoff(-uiCameraHalfHeight);
            cameraPtr->SetLeftCutoff(-uiCameraHalfWidth);
            cameraPtr->SetRightCutoff(uiCameraHalfWidth);
            cameraPtr->SetNearCutoff(-32.0f);
            cameraPtr->SetFarCutoff(32.0f);
        }
        cameraPtr->AddYaw(glm::pi<float>());
        cameraPtr->SetProjection(Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC);
        cameraPtr->SetDepthTestEnabled(false);
        cameraPtr->SetCameraMask(s_uiCamera_Mask_);
        cameraPtr->SetPriorityValue(1000);
    }
}

void TestScene103::CreateCursorEntity()
{
    GetComponentStoresPtr()->CreateEntity(cursor_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(cursor_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            cursorPositionRenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(cursorCircle_MeshDataPtr_);
            mesh.SetPositionZ(0.53f);
            mesh.SetColor(0.8f, 0.8f, 0.8f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            cursorPressRenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(cursorCircle_MeshDataPtr_);
            mesh.SetPositionZ(0.52f);
            mesh.SetColor(0.8f, 0.2f, 0.2f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
        }

        {
            cursorReleaseRenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(cursorCircle_MeshDataPtr_);
            mesh.SetPositionZ(0.51f);
            mesh.SetColor(0.8f, 0.8f, 0.2f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
        }
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(cursor_EntityId_));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        cursorPositionCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec2(), s_cursorPositionCollisionShapeId_, true);
        cursorPressCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec2(), s_cursorPressCollisionShapeId_, false);
        cursorReleaseCollisionPointIndex_ = collisionPoints.size();
        collisionPoints.emplace_back(glm::vec2(), s_cursorReleaseCollisionShapeId_, false);
    }
}

void TestScene103::CreateFogEntity()
{
    GetComponentStoresPtr()->CreateEntity(fog_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(fog_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, fog_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight2_Mask_);
            mesh.SetMeshDataPtr(fog_MeshDataPtr_);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.1f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark2_Mask_);
            mesh.SetMeshDataPtr(fog_MeshDataPtr_);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }
}

void TestScene103::CreateGroundEntity()
{
    GetComponentStoresPtr()->CreateEntity(ground_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(ground_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, ground_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(groundLit_MeshDataPtr_);
            mesh.SetTextureId(groundLit_TextureId_);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(groundDark_MeshDataPtr_);
            mesh.SetTextureId(groundDark_TextureId_);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshIdAndMaxBoundingRadius(groundGrid_MeshId_, groundGrid_MeshDataPtr_->maxBoundingRadius);
            mesh.SetPositionZ(-0.1f);
            mesh.SetColor(0.2f, 0.2f, 1.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshIdAndMaxBoundingRadius(groundGridLabels_MeshId_, groundGridLabels_MeshDataPtr_->maxBoundingRadius);
            mesh.SetTextureId(pixelFont_TextureId_);
            mesh.SetPositionZ(-0.1f);
            mesh.SetColor(0.2f, 0.2f, 1.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(collisionBodyQuadTreeMeshDataPtr_);
            mesh.SetPositionZ(-0.2f);
            mesh.SetColor(1.0f, 0.4f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(groundCollision_MeshDataPtr_);
            mesh.SetColor(1.0f, 0.2f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;
    collisionBody2DCreationInfo.density_ = std::numeric_limits<float>::infinity();

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(ground_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, ground_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
        collisionRectangles.emplace_back(glm::vec2(-1152.0f, -1024.0f), glm::vec2(-1024.0f, 1024.0f));
        collisionRectangles.emplace_back(glm::vec2(1024.0f, -1024.0f), glm::vec2(1152.0f, 1024.0f));
        collisionRectangles.emplace_back(glm::vec2(-1152.0f, 1024.0f), glm::vec2(1152.0f, 1152.0f));
        collisionRectangles.emplace_back(glm::vec2(-1152.0f, -1152.0f), glm::vec2(1152.0f, -1024.0f));
    }
}

void TestScene103::CreatePlayerEntity()
{
    GetComponentStoresPtr()->CreateEntity(player_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(player_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, player_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(playerLit_MeshDataPtr_);
            mesh.SetTextureId(playerLit_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(playerDark_MeshDataPtr_);
            mesh.SetTextureId(playerDark_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark2_Mask_);
            mesh.SetMeshDataPtr(playerLightStrong_MeshDataPtr_);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-100);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(playerCollision_MeshDataPtr_);
            mesh.SetColor(0.2f, 1.0f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(player_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, player_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPositionY(-64.0f);

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
        collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), 8.0f);
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<CharacterInfo>(player_EntityId_));
    CharacterInfo* characterInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<CharacterInfo>(characterInfoPtr, player_EntityId_));
    if (characterInfoPtr == nullptr)
    {
        // 
    }
}

void TestScene103::CreatePlayerLightEntity()
{
    GetComponentStoresPtr()->CreateEntity(playerLight_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(playerLight_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, playerLight_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight2_Mask_);
            mesh.SetMeshDataPtr(playerLightBottom_MeshDataPtr_);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.00f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-100);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(playerLightTop_MeshDataPtr_);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-100);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(playerLightCollision_MeshDataPtr_);
            mesh.SetColor(1.0f, 1.0f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(playerLight_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, playerLight_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
        std::vector<glm::vec2> corners;
        constexpr float radius = 60.0f;
        constexpr size_t subdivisions = 8;
        constexpr float angleRotation = glm::pi<float>() / (float)subdivisions;
        for (size_t i = 0; i <= subdivisions; ++i)
        {
            glm::vec2 radiusVector(radius, 0.0f);
            radiusVector = Project001::Rotate2DVector(radiusVector, (float)i * angleRotation);
            corners.emplace_back(radiusVector);
        }
        corners.emplace_back(-4.0f, -112.0f);
        corners.emplace_back(4.0f, -112.0f);
        for (size_t i = 0; i < corners.size(); ++i)
        {
            corners[i].y += 128.0f;
        }
        collisionConvexPolygons.emplace_back(corners);
    }
}

void TestScene103::CreateTestUiEntity()
{
    GetComponentStoresPtr()->CreateEntity(testUi_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(testUi_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testUi_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_uiCamera_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(testUiBackground_MeshDataPtr_);
            mesh.SetPosition(-480.0f + 48.0f, 296.0f, -0.1f);
            mesh.SetColor(0.2f, 0.2f, 0.2f, 1.0f);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(testUiText01_MeshDataPtr_);
            mesh.SetTextureId(pixelFont_TextureId_);
            mesh.SetPositionX(-480.0f + 2.0f);
            mesh.SetPositionY(320.0f - 14.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(testUiText02_MeshDataPtr_);
            mesh.SetTextureId(pixelFont_TextureId_);
            mesh.SetPositionX(-480.0f + 2.0f);
            mesh.SetPositionY(320.0f - 30.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(testUiText03_MeshDataPtr_);
            mesh.SetTextureId(pixelFont_TextureId_);
            mesh.SetPositionX(-480.0f + 2.0f);
            mesh.SetPositionY(320.0f - 46.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }
}

void TestScene103::CreateTestPersonEntity(unsigned int& entityId, glm::vec2 position)
{
    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(entityId));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(testPersonLit_MeshDataPtr_);
            mesh.SetTextureId(testPersonLit_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(testPersonDark_MeshDataPtr_);
            mesh.SetTextureId(testPersonDark_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(testPersonCollision_MeshDataPtr_);
            mesh.SetColor(0.2f, 1.0f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(entityId, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, entityId));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPosition(position);

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
        collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), 8.0f);
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<PersonInfo>(entityId));
}

void TestScene103::CreateTestMonsterEntity()
{
    GetComponentStoresPtr()->CreateEntity(testMonster_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(testMonster_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testMonster_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(testMonsterLit_MeshDataPtr_);
            mesh.SetTextureId(testMonsterLit_TextureId_);
            mesh.SetColor(1.0f, 0.2f, 0.2f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(testMonsterDark_MeshDataPtr_);
            mesh.SetTextureId(testMonsterDark_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(testMonsterCollision_MeshDataPtr_);
            mesh.SetColor(0.2f, 1.0f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(testMonster_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, testMonster_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPosition(64.0f, 0.0f);

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
        collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), 8.0f);
    }
}

void TestScene103::CreateTestMonsterVisionEntity()
{
    GetComponentStoresPtr()->CreateEntity(testMonsterVision_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(testMonsterVision_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testMonsterVision_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(testMonsterVisionCollision_MeshDataPtr_);
            mesh.SetColor(0.2f, 1.0f, 1.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(testMonsterVision_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, testMonsterVision_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
        collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), 96.0f);
    }
}

void TestScene103::CreateTestLampEntity()
{
    GetComponentStoresPtr()->CreateEntity(testLamp_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(testLamp_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testLamp_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(testLampLit_MeshDataPtr_);
            mesh.SetTextureId(testLampLit_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(testLampDark_MeshDataPtr_);
            mesh.SetTextureId(testLampDark_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(testLampCollision_MeshDataPtr_);
            mesh.SetColor(1.0f, 0.2f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;
    collisionBody2DCreationInfo.density_ = std::numeric_limits<float>::infinity();

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(testLamp_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, testLamp_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPosition(128.0f, 0.0f);

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
        collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), 8.0f);
    }
}

void TestScene103::CreateTestLampLightEntity()
{
    GetComponentStoresPtr()->CreateEntity(testLampLight_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(testLampLight_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testLampLight_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight2_Mask_);
            mesh.SetMeshDataPtr(testLampLightBottom_MeshDataPtr_);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.00f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-100);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(testLampLightTop_MeshDataPtr_);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-100);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(testLampLightCollision_MeshDataPtr_);
            mesh.SetColor(1.0f, 1.0f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(testLampLight_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, testLampLight_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
        collisionCircles.emplace_back(glm::vec2(0.0f, -16.0f), 64.0f);
    }
}

void TestScene103::CreateTestHouseEntity()
{
    GetComponentStoresPtr()->CreateEntity(testHouse_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(testHouse_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testHouse_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(testHouseLit_MeshDataPtr_);
            mesh.SetTextureId(testHouseLit_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(testHouseText_MeshDataPtr_);
            mesh.SetTextureId(pixelFont_TextureId_);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
            mesh.SetPosition(glm::vec3(0.0f, -66.0f, 84.0f));
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(testHouseDark_MeshDataPtr_);
            mesh.SetTextureId(testHouseDark_TextureId_);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(testHouseCollision_MeshDataPtr_);
            mesh.SetColor(1.0f, 0.2f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;
    collisionBody2DCreationInfo.density_ = std::numeric_limits<float>::infinity();

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(testHouse_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, testHouse_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPosition(-128.0f, 64.0f);

        std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
        collisionRectangles.emplace_back(glm::vec2(-64.0f, -64.0f), glm::vec2(64.0f, 64.0f));
    }
}

void TestScene103::CreateTestHouseLightEntity()
{
    GetComponentStoresPtr()->CreateEntity(testHouseLight_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(testHouseLight_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testHouseLight_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight2_Mask_);
            mesh.SetMeshDataPtr(testHouseLightBottom_MeshDataPtr_);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.00f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-100);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(testHouseLightTop_MeshDataPtr_);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-100);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(testHouseLightCollision_MeshDataPtr_);
            mesh.SetColor(1.0f, 1.0f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(testHouseDoorCollision_MeshDataPtr_);
            mesh.SetColor(1.0f, 0.2f, 1.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(testHouseLight_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, testHouseLight_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
        collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), 160.0f);

        std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
        collisionRectangles.emplace_back(glm::vec2(-24.0f, -96.0f), glm::vec2(24.0f, -64.0f));
    }
}

void TestScene103::UpdateMainCameraEntityPosition(float timestep_s)
{
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraLight1_EntityId_));
    if (cameraPtr != nullptr)
    {
        constexpr float orbitSpeed = 0.1f * glm::pi<float>();
        float cameraPitchDelta = orbitSpeed * timestep_s;

        bool pitchingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_PAGE_DOWN);
        bool pitchingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_PAGE_UP);

        const float moveSpeed = 128.0f;
        float moveSpeedDelta = moveSpeed * timestep_s;

        bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_LEFT);
        bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_RIGHT);
        bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_UP);
        bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_DOWN);

        if (pitchingUp)
        {
            cameraPtr->AddRelativeRotationX(-cameraPitchDelta);
        }

        if (pitchingDown)
        {
            cameraPtr->AddRelativeRotationX(cameraPitchDelta);
        }

        if (movingLeft)
        {
            mainCamera_LookAtPoint_.x -= moveSpeedDelta;
        }

        if (movingRight)
        {
            mainCamera_LookAtPoint_.x += moveSpeedDelta;
        }

        if (movingUp)
        {
            mainCamera_LookAtPoint_.y += moveSpeedDelta;
        }

        if (movingDown)
        {
            mainCamera_LookAtPoint_.y -= moveSpeedDelta;
        }


        if (mainCamera_Locked_)
        {
            Project001::CollisionBody2D* playerCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(playerCollisionBodyPtr, player_EntityId_));
            if (playerCollisionBodyPtr != nullptr)
            {
                glm::vec3 focalPoint(playerCollisionBodyPtr->GetPosition(), 0.0f);
                cameraPtr->FollowFocalPoint(focalPoint, mainCamera_DistanceAway_);
            }
        }
        else
        {
            glm::vec3 offsetLookAtPoint = mainCamera_LookAtPoint_;
            offsetLookAtPoint.y -= mainCamera_DistanceAway_ * 0.1f;
            cameraPtr->FollowFocalPoint(offsetLookAtPoint, mainCamera_DistanceAway_);
        }

        auto SyncMainCameras = [&](unsigned int& camera_EntityId)
            {
                Project001::Camera* otherCameraPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(otherCameraPtr, camera_EntityId));
                if (cameraPtr != nullptr)
                {
                    otherCameraPtr->SetPosition(cameraPtr->GetPosition());
                    otherCameraPtr->SetOrientation(cameraPtr->GetOrientation());
                }
            };

        SyncMainCameras(mainCameraLight2_EntityId_);
        SyncMainCameras(mainCameraDark1_EntityId_);
        SyncMainCameras(mainCameraDark2_EntityId_);
        SyncMainCameras(mainCameraDebug_EntityId_);
    }
}

void TestScene103::UpdateCursorPosition(float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    GetWindowPtr()->GetWindowSize(windowWidth, windowHeight);

    glm::vec2 viewportNormalizedCursorPosition = GetRendererPtr()->ConvertPointFromWindowToViewportNormalized(glm::vec2(xPosition, yPosition), (float)windowHeight);

    Project001::Camera* cameraPtr;
    if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraLight1_EntityId_))
    {
        glm::vec3 worldCursorPosition;
        glm::vec3 worldCursorNormal;
        bool cursorRayIntersected = cameraPtr->RaycastPointFromNormalizedViewportToPane(
            viewportNormalizedCursorPosition,
            glm::vec3(0.0f, 0.0f, 1.0f),
            0.0f,
            worldCursorPosition,
            worldCursorNormal);

        if (cursorRayIntersected)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();

                collisionPoints[cursorPositionCollisionPointIndex_].position = worldCursorPosition;
            }
        }
    }
}

void TestScene103::UpdatePlayerEntityVelocity()
{
    Project001::CollisionBody2D* playerCollisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(playerCollisionBodyPtr, player_EntityId_));
    if (playerCollisionBodyPtr != nullptr)
    {
        constexpr float walkSpeed = 64.0f;
        constexpr float runSpeed = 128.0f;

        const bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
        const bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
        const bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
        const bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
        const bool running = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_SPACE);

        const glm::vec2& velocity = playerCollisionBodyPtr->GetVelocity();
        float velocityMagnitude = glm::length(velocity);

        glm::vec2 velocityDirection(0.0f, 0.0);
        if (movingLeft)
        {
            velocityDirection.x -= 1.0f;
        }
        if (movingRight)
        {
            velocityDirection.x += 1.0f;
        }
        if (movingUp)
        {
            velocityDirection.y += 1.0f;
        }
        if (movingDown)
        {
            velocityDirection.y -= 1.0f;
        }

        float accelerationMagnitude = glm::length(velocityDirection);
        if (accelerationMagnitude > 0.0f)
        {
            velocityDirection /= accelerationMagnitude;
        }

        if (running)
        {
            playerCollisionBodyPtr->SetVelocity(velocityDirection * runSpeed);
        }
        else
        {
            playerCollisionBodyPtr->SetVelocity(velocityDirection * walkSpeed);
        }

        CharacterInfo* characterInfoPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<CharacterInfo>(characterInfoPtr, player_EntityId_));
        if (characterInfoPtr != nullptr)
        {
            if (accelerationMagnitude > 0.0f)
            {
                if (running)
                {
                    characterInfoPtr->state = CharacterInfo::State::STATE_RUNNING;
                }
                else
                {
                    characterInfoPtr->state = CharacterInfo::State::STATE_WALKING;
                }
            }
            else
            {
                characterInfoPtr->state = CharacterInfo::State::STATE_STANDING;
            }
        }
    }

    Project001::CollisionBody2D* playerLightCollisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(playerLightCollisionBodyPtr, playerLight_EntityId_));
    if (playerLightCollisionBodyPtr != nullptr)
    {
        const bool lightOn = GetWindowPtr()->GetMouseButtonPressed(Project001::MouseButton::MOUSE_BUTTON_LEFT);

        playerLightCollisionBodyPtr->SetEnabled(lightOn);

        Project001::CollisionBody2D* cursorCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(cursorCollisionBodyPtr, cursor_EntityId_));
        if (cursorCollisionBodyPtr != nullptr)
        {
            std::vector<Project001::CollisionPoint2D>& collisionPoints = cursorCollisionBodyPtr->GetCollisionPoints();
            const glm::vec2& cursorPosition = collisionPoints[cursorPositionCollisionPointIndex_].position;
            glm::vec2 collisionBodyDirection = playerLightCollisionBodyPtr->GetForwardVector();
            glm::vec2 collisionBodyToCursor = cursorPosition - playerLightCollisionBodyPtr->GetPosition();

            float angleToCursor = Project001::Get2DVectorAngle(collisionBodyDirection, collisionBodyToCursor);
            playerLightCollisionBodyPtr->AddRotation(angleToCursor);
        }
    }

    if (playerCollisionBodyPtr != nullptr && playerLightCollisionBodyPtr != nullptr)
    {
        playerLightCollisionBodyPtr->SetPosition(playerCollisionBodyPtr->GetPosition());
    }
}

void TestScene103::SyncCursorRenderedModel()
{
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        Project001::CollisionPoint2D collisionPoint01 = collisionPoints[cursorPositionCollisionPointIndex_];
        Project001::CollisionPoint2D collisionPoint02 = collisionPoints[cursorPressCollisionPointIndex_];
        Project001::CollisionPoint2D collisionPoint03 = collisionPoints[cursorReleaseCollisionPointIndex_];

        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            Project001::RenderedMesh& circleMesh01 = renderedMeshes[cursorPositionRenderedMeshIndex_];
            Project001::RenderedMesh& circleMesh02 = renderedMeshes[cursorPressRenderedMeshIndex_];
            Project001::RenderedMesh& circleMesh03 = renderedMeshes[cursorReleaseRenderedMeshIndex_];

            circleMesh01.SetPositionX(collisionPoint01.position.x);
            circleMesh01.SetPositionY(collisionPoint01.position.y);
            circleMesh01.SetVisible(collisionPoint01.enabled);


            circleMesh02.SetPositionX(collisionPoint02.position.x);
            circleMesh02.SetPositionY(collisionPoint02.position.y);
            circleMesh02.SetVisible(collisionPoint02.enabled);

            circleMesh03.SetPositionX(collisionPoint03.position.x);
            circleMesh03.SetPositionY(collisionPoint03.position.y);
            circleMesh03.SetVisible(collisionPoint03.enabled);
        }
    }
}

void TestScene103::SyncPlayerRenderedModel()
{
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, player_EntityId_));
    if (collisionBodyPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, player_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
        }
    }
}

void TestScene103::SyncPlayerLightRenderedModel()
{
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, playerLight_EntityId_));
    if (collisionBodyPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, playerLight_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
            renderedModelPtr->SetVisible(collisionBodyPtr->GetEnabled());
        }
    }
}

void TestScene103::SyncTestPersonRenderedModel(unsigned int& entityId)
{
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, entityId));
    if (collisionBodyPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
        }
    }
}

void TestScene103::SyncTestMonsterRenderedModel()
{
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, testMonster_EntityId_));
    if (collisionBodyPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testMonster_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
        }

        Project001::CollisionBody2D* visionCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(visionCollisionBodyPtr, testMonsterVision_EntityId_));
        if (visionCollisionBodyPtr != nullptr)
        {
            visionCollisionBodyPtr->SetPosition(collisionBodyPtr->GetPosition());
        }
    }
}

void TestScene103::SyncTestMonsterVisionRenderedModel()
{
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, testMonsterVision_EntityId_));
    if (collisionBodyPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testMonsterVision_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
            renderedModelPtr->SetVisible(collisionBodyPtr->GetEnabled());
        }
    }
}

void TestScene103::SyncTestLampRenderedModel()
{
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, testLamp_EntityId_));
    if (collisionBodyPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testLamp_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
        }

        Project001::CollisionBody2D* lightCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(lightCollisionBodyPtr, testLampLight_EntityId_));
        if (lightCollisionBodyPtr != nullptr)
        {
            lightCollisionBodyPtr->SetPosition(collisionBodyPtr->GetPosition());
        }
    }
}

void TestScene103::SyncTestLampLightRenderedModel()
{
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, testLampLight_EntityId_));
    if (collisionBodyPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testLampLight_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
            renderedModelPtr->SetVisible(collisionBodyPtr->GetEnabled());
        }
    }
}

void TestScene103::SyncTestHouseRenderedModel()
{
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, testHouse_EntityId_));
    if (collisionBodyPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testHouse_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
        }

        Project001::CollisionBody2D* lightCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(lightCollisionBodyPtr, testHouseLight_EntityId_));
        if (lightCollisionBodyPtr != nullptr)
        {
            lightCollisionBodyPtr->SetPosition(collisionBodyPtr->GetPosition());
        }
    }
}

void TestScene103::SyncTestHouseLightRenderedModel()
{
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, testHouseLight_EntityId_));
    if (collisionBodyPtr != nullptr)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, testHouseLight_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
            renderedModelPtr->SetVisible(collisionBodyPtr->GetEnabled());
        }
    }
}

void TestScene103::AnimatePlayerRenderedModel(float timestep_s)
{

}

void TestScene103::UpdatePersonLogic(float timestep_s)
{
    PersonInfo* personInfoPtrs = nullptr;
    size_t personInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<PersonInfo>(personInfoPtrs, personInfoCount);
    for (size_t i = 0; i < personInfoCount; ++i)
    {
        PersonInfo& currentPersonInfo = personInfoPtrs[i];
        unsigned int personEntityId = (unsigned int)-1;
        if (GetComponentStoresPtr()->GetComponentEntityId<PersonInfo>(personEntityId, &currentPersonInfo))
        {
            if (currentPersonInfo.state == PersonInfo::State::STATE_STANDING)
            {
                if (currentPersonInfo.stateDuration_s <= 0.0f)
                {
                    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
                    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, personEntityId));
                    if (collisionBodyPtr != nullptr)
                    {
                        PersonInfo* personInfoPtr = nullptr;
                        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PersonInfo>(personInfoPtr, personEntityId));
                        if (personInfoPtr != nullptr)
                        {
                            std::uniform_real_distribution<float> distributionX(-1.0f, 1.0f);
                            std::uniform_real_distribution<float> distributionY(-1.0f, 1.0f);
                            glm::vec2 velocityDirection = glm::normalize(glm::vec2(distributionX(randomNumberEngine_), distributionY(randomNumberEngine_)));

                            // avoid sticking to outside border
                            const glm::vec2 collisionBodyPosition = collisionBodyPtr->GetPosition();
                            if (collisionBodyPosition.x > 992.0f && velocityDirection.x > 0.0f)
                            {
                                velocityDirection.x = -velocityDirection.x;
                            }
                            else if (collisionBodyPosition.x < -992.0f && velocityDirection.x < 0.0f)
                            {
                                velocityDirection.x = -velocityDirection.x;
                            }
                            if (collisionBodyPosition.y > 992.0f && velocityDirection.y > 0.0f)
                            {
                                velocityDirection.y = -velocityDirection.y;
                            }
                            else if (collisionBodyPosition.y < -992.0f && velocityDirection.y < 0.0f)
                            {
                                velocityDirection.y = -velocityDirection.y;
                            }

                            constexpr float walkSpeed = 64.0f;
                            collisionBodyPtr->SetVelocity(velocityDirection * walkSpeed);

                            personInfoPtr->state = PersonInfo::State::STATE_WALKING;

                            std::uniform_real_distribution<float> distributionT(3.0f, 5.0f);
                            personInfoPtr->stateDuration_s = distributionT(randomNumberEngine_);
                        }
                    }
                }
                else
                {
                    currentPersonInfo.stateDuration_s -= timestep_s;
                }
            }
            else if (currentPersonInfo.state == PersonInfo::State::STATE_WALKING)
            {
                if (currentPersonInfo.stateDuration_s <= 0.0f)
                {
                    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
                    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, personEntityId));
                    if (collisionBodyPtr != nullptr)
                    {
                        PersonInfo* personInfoPtr = nullptr;
                        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PersonInfo>(personInfoPtr, personEntityId));
                        if (personInfoPtr != nullptr)
                        {
                            collisionBodyPtr->SetVelocity(glm::vec2(0.0f, 0.0f));

                            personInfoPtr->state = PersonInfo::State::STATE_STANDING;

                            std::uniform_real_distribution<float> distributionT(3.0f, 5.0f);
                            personInfoPtr->stateDuration_s = distributionT(randomNumberEngine_);
                        }
                    }
                }
                else
                {
                    currentPersonInfo.stateDuration_s -= timestep_s;
                }
            }
        }
    }
}

#include <stack>

void TestScene103::UpdateCollisionBodyQuadTreeMesh()
{
    const Project001::CollisionBodyQuadTreeNode2D* rootNodePtr = Project001::CollisionSystem2D::GetCollisionBodyQuadTree2D().GetRootNode();

    const float lineWidth = 4.0f;

    collisionBodyQuadTreeMeshDataPtr_->Clear();

    if ((!rootNodePtr->leafNode || !rootNodePtr->bodyPtrs.empty()))
    {
        Project001::MeshLoader::Generate2DRectangleFrame(*collisionBodyQuadTreeMeshDataPtr_, rootNodePtr->min, rootNodePtr->max, lineWidth);

        std::stack<const Project001::CollisionBodyQuadTreeNode2D*> nodePtrStack;
        nodePtrStack.push(rootNodePtr);

        while (!nodePtrStack.empty())
        {
            const Project001::CollisionBodyQuadTreeNode2D* nodePtr = nodePtrStack.top();
            nodePtrStack.pop();

            if (!nodePtr->leafNode)
            {
                const glm::vec2& min = nodePtr->min;
                const glm::vec2& max = nodePtr->max;
                glm::vec2 mid((nodePtr->max + nodePtr->min) * 0.5f);
                glm::vec2 top(mid.x, max.y);
                glm::vec2 bottom(mid.x, min.y);
                glm::vec2 left(min.x, mid.y);
                glm::vec2 right(max.x, mid.y);

                Project001::MeshLoader::Generate2DLine(*collisionBodyQuadTreeMeshDataPtr_, top, bottom, lineWidth);
                Project001::MeshLoader::Generate2DLine(*collisionBodyQuadTreeMeshDataPtr_, left, right, lineWidth);

                for (size_t i = 0; i < 4; ++i)
                {
                    if (nodePtr->childrenPtrs[i])
                    {
                        nodePtrStack.push(nodePtr->childrenPtrs[i]);
                    }
                }
            }
        }
    }
}
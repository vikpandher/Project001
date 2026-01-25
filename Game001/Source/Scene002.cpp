// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-24

#include "Scene002.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedModel.h"
#include "Utilities/FontUtility.h"
#include "Utilities/MeshUtility.h"
#include "CollisionSystem2D.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "SoundPlayer.h"
#include "Window.h"

#include <stack>



struct BaseInfo
{
    unsigned int light_EntityId = static_cast<unsigned int>(-1);
};

struct HouseInfo
{
    unsigned int light_EntityId = static_cast<unsigned int>(-1);

    enum class State
    {
        STATE_DARK,
        STATE_LIT,
        STATE_WAITING
    };

    State state = State::STATE_DARK;
    float stateDuration_s = 0.0f;
};

struct LightInfo
{
    unsigned int owner_EntityId = static_cast<unsigned int>(-1);
};

struct MonsterInfo
{
    unsigned int vision_EntityId = static_cast<unsigned int>(-1);

    enum class State
    {
        STATE_STANDING,
        STATE_WALKING,
        STATE_RUNNING,
        STATE_CELEBRATING
    };

    State state = State::STATE_STANDING;
    float stateDuration_s = 0.0f;

    State animationState = State::STATE_STANDING;
    float animationStateDuration_s = 0.0f;

    float currentJumpDuration_s = 0.0f;
    float currentJumpHeight = 0.0f;
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

    State animationState = State::STATE_STANDING;
    float animationStateDuration_s = 0.0f;

    float currentJumpDuration_s = 0.0f;
    float currentJumpHeight = 0.0f;
};

struct PlayerInfo
{
    unsigned int light_EntityId = static_cast<unsigned int>(-1);

    enum class State
    {
        STATE_STANDING,
        STATE_WALKING,
        STATE_RUNNING
    };

    State state = State::STATE_STANDING;
    bool safe = false;

    float battery_s = 0.0f;
    float stamina_s = 0.0f;

    unsigned int score = 0;

    State animationState = State::STATE_STANDING;
    float animationStateDuration_s = 0.0f;

    float currentJumpDuration_s = 0.0f;
    float currentJumpHeight = 0.0f;
};

struct VisionInfo
{
    unsigned int owner_EntityId = static_cast<unsigned int>(-1);
};

// public ----------------------------------------------------------------------

Scene002::Scene002(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , mainCamera_LookAtPoint_(0.0f, 0.0f, 0.0f)
    , miniMapLocation_(0.0f, 0.0f)
{
    sharedDataPtr_ = GetSharedDataPtr<SharedApplicationData>();
    sharedDataPtr_->scene002Id = GetId();
}

Scene002::~Scene002()
{
}

void Scene002::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&Scene002::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&Scene002::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&Scene002::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&Scene002::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&Scene002::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&Scene002::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&Scene002::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&Scene002::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected -------------------------------------------------------------------

void Scene002::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   Scene002:            " << GetId());

    sharedDataPtr_->score = 0;
    sharedDataPtr_->nextHouseTurnOnWait_s = sharedDataPtr_->house_randomTurnOnWait_s;
    sharedDataPtr_->remainingTime_s = sharedDataPtr_->startingGameTime_s;

    randomNumberEngine_.seed(sharedDataPtr_->randomNumberSeed);

    CreateMainCameraEntities();
    CreateUiCameraEntity();
    CreateUiTextEntity();
    CreateUiPauseTextEntity();
    CreateUiMiniMapEntity();
    CreateCursorEntity();

    constexpr float houseYOffset = 64.0f;
    CreateBaseEntity(base_EntityId_, glm::vec2(0.0f, houseYOffset));

    constexpr float houseSpacing = 384.0f;
    for (int i = -2; i <= 2; ++i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }

            glm::vec2 position(static_cast<float>(i) * houseSpacing, static_cast<float>(j) * houseSpacing + houseYOffset);

            house_EntityIds_.push_back(static_cast<unsigned int>(-1));
            unsigned int& entityId = house_EntityIds_.back();

            CreateHouseEntity(entityId, position);
        }
    }

    std::vector<unsigned int> houseIndicies;
    for (size_t i = 0; i < 24; ++i)
    {
        houseIndicies.push_back(i);
    }
    std::shuffle(houseIndicies.begin(), houseIndicies.end(), randomNumberEngine_);

    for (size_t i = 0; i < houseIndicies.size() && i < sharedDataPtr_->house_initialOnCount; ++i)
    {
        const unsigned int& entityId = house_EntityIds_[houseIndicies[i]];

        HouseInfo* houseInfoPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<HouseInfo>(houseInfoPtr, entityId));
        if (houseInfoPtr != nullptr)
        {
            houseInfoPtr->state = HouseInfo::State::STATE_LIT;
        }
    }

    CreateGroundEntity();

    std::uniform_real_distribution<float> distributionX(-1024.0f, 1024.0f);
    std::uniform_real_distribution<float> distributionY(-1024.0f, 1024.0f);
    for (size_t i = 0; i < sharedDataPtr_->person_initalCount; ++i)
    {
        glm::vec2 currentPosition(distributionX(randomNumberEngine_), distributionY(randomNumberEngine_));

        person_EntityIds_.push_back(static_cast<unsigned int>(-1));
        unsigned int& entityId = person_EntityIds_.back();

        CreatePersonEntity(entityId, currentPosition);
    }

    constexpr float playerYOffset = -16.0f;
    CreatePlayerEntity(glm::vec2(0.0f, playerYOffset));

    for (size_t i = 0; i < sharedDataPtr_->monster_initialCount; ++i)
    {
        glm::vec2 currentPosition(distributionX(randomNumberEngine_), distributionY(randomNumberEngine_));

        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, player_EntityId_));
        if (collisionBody2DPtr != nullptr)
        {
            const glm::vec2& playerPosition = collisionBody2DPtr->GetPosition();

            glm::vec2 current_to_player = playerPosition - currentPosition;
            float distance_to_player = glm::length(current_to_player);

            if (distance_to_player < sharedDataPtr_->playerVision_radius)
            {
                glm::vec2 move_away_vector = -current_to_player;
                move_away_vector *= (sharedDataPtr_->playerVision_radius - distance_to_player) / (distance_to_player + 2.0f);

                currentPosition += move_away_vector;
            }
        }

        monster_EntityIds_.push_back(static_cast<unsigned int>(-1));
        unsigned int& entityId = monster_EntityIds_.back();

        CreateMonsterEntity(entityId, currentPosition);
    }
}

void Scene002::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    LOG_INFO("DEINITIALIZING: Scene002:            " << GetId());

    // -------------------------------------------------------------------------

    GetComponentStoresPtr()->DeleteAllEntities();

    // Entity Ids --------------------------------------------------------------

    mainCameraLight1_EntityId_ = static_cast<unsigned int>(-1);
    mainCameraLight2_EntityId_ = static_cast<unsigned int>(-1);
    mainCameraDark1_EntityId_ = static_cast<unsigned int>(-1);
    mainCameraDark2_EntityId_ = static_cast<unsigned int>(-1);
    mainCameraDebug_EntityId_ = static_cast<unsigned int>(-1);

    uiCamera_EntityId_ = static_cast<unsigned int>(-1);

    uiText_EntityId_ = static_cast<unsigned int>(-1);
    uiPauseText_EntityId_ = static_cast<unsigned int>(-1);
    uiMiniMaphouse_RenderedMeshIndies.clear();
    uiMiniMapPlayer_RenderedMeshIndex_ = static_cast<size_t>(-1);
    uiMiniMap_EntityId_ = static_cast<unsigned int>(-1);

    cursor_EntityId_ = static_cast<unsigned int>(-1);

    base_EntityId_ = static_cast<unsigned int>(-1);
    house_EntityIds_.clear();
    ground_EntityId_ = static_cast<unsigned int>(-1);
    player_EntityId_ = static_cast<unsigned int>(-1);
    person_EntityIds_.clear();
    monster_EntityIds_.clear();

    // -------------------------------------------------------------------------

    mainCamera_LookAtPoint_ = glm::vec3(0.0f, 0.0f, 0.0f);
    mainCamera_DistanceAway_ = 0.0f;
    mainCamera_Locked_ = true;

    miniMapLocation_ = glm::vec2(0.0f, 0.0f);

    paused_ = false;

    pause_joystickButtonPressRested_ = false;
}

void Scene002::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent)
{
    UpdateCursorPosition(cursorPositionEvent.xPosition, cursorPositionEvent.yPosition);

    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        collisionPoints[s_cursorPosition_CollisionPointIndex_].enabled = true;
    }
}

void Scene002::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (sharedDataPtr_->pause_usesKeyboard && keyCode == sharedDataPtr_->pause_KeyCode)
        {
            paused_ = !paused_;
        }
        if (sharedDataPtr_->quit_usesKeyboard && keyCode == sharedDataPtr_->quit_KeyCode)
        {
            if (paused_)
            {
                SendEventToApplication(Project001::SwitchSceneEvent(sharedDataPtr_->scene001Id));
                if (GetActiveScene()->GetId() == sharedDataPtr_->scene001Id)
                {
                    SendEventToScene(GetId(), Project001::DeinitializeEvent());
                    SendEventToApplication(Project001::InitializeEvent());
                }
                return;
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_0)
        {
            Project001::Camera* cameraPtr = nullptr;
            if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraDebug_EntityId_))
            {
                cameraPtr->SetTurnedOn(!cameraPtr->GetTurnedOn());
            }
        }
        // else if (keyCode == Project001::KeyCode::KEY_CODE_9)
        // {
        //     mainCamera_Locked_ = !mainCamera_Locked_;
        // }
    }
}

void Scene002::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (!sharedDataPtr_->pause_usesKeyboard && mouseButton == sharedDataPtr_->pause_MouseButton)
        {
            paused_ = !paused_;
        }
        if (!sharedDataPtr_->quit_usesKeyboard && mouseButton == sharedDataPtr_->quit_MouseButton)
        {
            if (paused_)
            {
                SendEventToApplication(Project001::SwitchSceneEvent(sharedDataPtr_->scene001Id));
                if (GetActiveScene()->GetId() == sharedDataPtr_->scene001Id)
                {
                    SendEventToScene(GetId(), Project001::DeinitializeEvent());
                    SendEventToApplication(Project001::InitializeEvent());
                }
                return;
            }
        }
    }

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[s_cursorPress_CollisionPointIndex_].position = collisionPoints[s_cursorPosition_CollisionPointIndex_].position;
                collisionPoints[s_cursorPress_CollisionPointIndex_].enabled = true;
                collisionPoints[s_cursorRelease_CollisionPointIndex_].enabled = false;
            }
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[s_cursorRelease_CollisionPointIndex_].position = collisionPoints[s_cursorPosition_CollisionPointIndex_].position;
                collisionPoints[s_cursorRelease_CollisionPointIndex_].enabled = true;
            }
        }
    }
}

void Scene002::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    // if (renderEvent.timestep_ns > 17000000)
    // {
    //     LOG_WARNING_F("Slow Render Frame (ns): " << renderEvent.timestep_ns);
    // }

    GetRenderSystemPtr()->Render();
}

void Scene002::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    // float& yOffset = scrollEvent.yOffset;
    // constexpr float speedConstant = 20.0f;
    // mainCamera_DistanceAway_ += yOffset * speedConstant;
}

void Scene002::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    UpdateUiTextEntity();
    UpdateUiPauseTextEntity();
    UpdateUiMiniMapEntity();

    std::vector<bool> buttonValues;
    GetWindowPtr()->GetJoystickButtons(0, buttonValues);

    bool pause_joystickButtonPressed = false;
    if (buttonValues.size() > sharedDataPtr_->pause_joystickButtonIndex)
    {
        pause_joystickButtonPressed |= buttonValues[sharedDataPtr_->pause_joystickButtonIndex];
    }

    if (!pause_joystickButtonPressed)
    {
        pause_joystickButtonPressRested_ = true;
    }

    if (pause_joystickButtonPressRested_ && pause_joystickButtonPressed)
    {
        pause_joystickButtonPressRested_ = false;
        paused_ = !paused_;
    }

    if (paused_)
    {
        bool quit = false;
        if (buttonValues.size() > sharedDataPtr_->quit_joystickButtonIndex)
        {
            quit |= buttonValues[sharedDataPtr_->quit_joystickButtonIndex];
        }

        if (quit)
        {
            SendEventToApplication(Project001::SwitchSceneEvent(sharedDataPtr_->scene001Id));
            if (GetActiveScene()->GetId() == sharedDataPtr_->scene001Id)
            {
                SendEventToScene(GetId(), Project001::DeinitializeEvent());
                SendEventToApplication(Project001::InitializeEvent());
            }
        }

        return;
    }

    unsigned long long timestep_ns = updateEvent.timestep_ns;
    float timestep_s = static_cast<float>(timestep_ns) / 1e9f;

    constexpr size_t physicsStepsPerUpdate = 1;
    float physicsTimestep_s = timestep_s / static_cast<float>(physicsStepsPerUpdate);
    for (size_t i = 0; i < physicsStepsPerUpdate; ++i)
    {
        GetCollisionSystemPtr()->ApplyMovement(physicsTimestep_s);
        GetCollisionSystemPtr()->CalculateCollisionsWithQuadTree();

        UpdateMainCameraEntity(physicsTimestep_s);

        float cursorX_position;
        float cursorY_position;
        GetWindowPtr()->GetCursorPosition(cursorX_position, cursorY_position);
        UpdateCursorPosition(cursorX_position, cursorY_position); // because camera updated

        UpdateGroundCollisionBodyQuadTreeMesh();
        UpdateHouseEntities(physicsTimestep_s);
        UpdatePersonEntities(physicsTimestep_s);
        UpdatePlayerEntity(physicsTimestep_s);
        UpdateMonsterEntities(physicsTimestep_s);
        UpdateWorld(physicsTimestep_s);
    }

    AnimatePersonEntities(timestep_s);
    AnimatePlayerEntity(timestep_s);
    AnimateMonsterEntities(timestep_s);

    // Sync rendered models
    // -------------------------------------------------------------------------

    SyncCursorRenderedModel();
    SyncBaseRenderedModels();
    SyncHouseRenderedModels();
    SyncPersonRenderedModels();
    SyncPlayerRenderedModels();
    SyncMonsterRenderedModels();
}

void Scene002::CreateMainCameraEntities()
{
    mainCamera_DistanceAway_ = sharedDataPtr_->mainCamera_initialDistanceAway;

    int aspectRatioNumerator;
    int aspectRatioDenominator;
    GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);

    float aspectRatio = static_cast<float>(aspectRatioNumerator) / static_cast<float>(aspectRatioDenominator);
    constexpr float mainCameraHalfHeight = 320.0f;
    float mainCameraHalfWidth = aspectRatio * mainCameraHalfHeight;

    constexpr float mainCameraNearCutoff = mainCameraHalfHeight * 0.1f;
    constexpr float mainCameraFarCutoff = mainCameraHalfHeight * 15.0f;

    constexpr float mainCameraPitch = glm::quarter_pi<float>();
    constexpr float mainCameraYaw = glm::pi<float>();

    constexpr Project001::Camera::CameraProjection mainCameraProjection =
        Project001::Camera::CameraProjection::CAMERA_PROJECTION_PERSPECTIVE;

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

                cameraPtr->FollowFocalPoint(glm::vec3(0.0f, 0.0f, 0.0f), mainCamera_DistanceAway_);
            }
        };

    CreateMainCamera(mainCameraLight1_EntityId_, s_mainCameraLight1_Mask_, -100);
    CreateMainCamera(mainCameraLight2_EntityId_, s_mainCameraLight2_Mask_, -50);
    CreateMainCamera(mainCameraDark1_EntityId_, s_mainCameraDark1_Mask_, 50);
    CreateMainCamera(mainCameraDark2_EntityId_, s_mainCameraDark2_Mask_, 100);
    CreateMainCamera(mainCameraDebug_EntityId_, s_mainCameraDebug_Mask_, 150);

    {
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraDebug_EntityId_));
        if (cameraPtr != nullptr)
        {
            cameraPtr->SetTurnedOn(false);
        }
    };

    {
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraDark2_EntityId_));
        if (cameraPtr != nullptr)
        {
            cameraPtr->SetTurnedOn(false);
        }
    };
}

void Scene002::CreateUiCameraEntity()
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
            float aspectRatio = static_cast<float>(aspectRatioNumerator) / static_cast<float>(aspectRatioDenominator);
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
        // cameraPtr->SetDepthTestEnabled(false);
        cameraPtr->SetCameraMask(s_uiCamera_Mask_);
        cameraPtr->SetPriorityValue(1000);
    }
}

void Scene002::CreateUiTextEntity()
{
    GetComponentStoresPtr()->CreateEntity(uiText_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(uiText_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, uiText_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_uiCamera_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiLeftBackground_MeshDataPtr);
            mesh.SetPosition(-480.0f + 48.0f, 320.0f - 24.0f, -0.1f);
            mesh.SetColor(0.2f, 0.2f, 0.2f, 1.0f);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiLeftText01_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_TextureId);
            mesh.SetPositionX(-480.0f + 2.0f);
            mesh.SetPositionY(320.0f - 14.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiLeftText02_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_TextureId);
            mesh.SetPositionX(-480.0f + 2.0f);
            mesh.SetPositionY(320.0f - 30.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiLeftText03_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_TextureId);
            mesh.SetPositionX(-480.0f + 2.0f);
            mesh.SetPositionY(320.0f - 46.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMiddleBackground_MeshDataPtr);
            mesh.SetPosition(0.0f, 320.0f - 8.0f, -0.1f);
            mesh.SetColor(0.2f, 0.2f, 0.2f, 1.0f);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMiddleText01_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_TextureId);
            mesh.SetPositionY(320.0f - 14.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }
}

void Scene002::CreateUiPauseTextEntity()
{
    GetComponentStoresPtr()->CreateEntity(uiPauseText_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(uiPauseText_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, uiPauseText_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_uiCamera_Mask_);
        renderedModelPtr->SetVisible(paused_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiPauseBackground_MeshDataPtr);
            mesh.SetPosition(0.0f, 320.0f - 41.0f - 2.0f, -0.1f);
            mesh.SetColor(0.2f, 0.2f, 0.2f, 1.0f);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiPauseText01_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_TextureId);
            mesh.SetPositionY(320.0f - 30.0f - 2.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiPauseText02_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_TextureId);
            mesh.SetPositionY(320.0f - 46.0f - 2.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiPauseText03_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_TextureId);
            mesh.SetPositionY(320.0f - 64.0f - 2.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }
}

void Scene002::CreateUiMiniMapEntity()
{
    miniMapLocation_ = glm::vec2(480.0f - 32.0f, 320.0f - 32.0f);

    GetComponentStoresPtr()->CreateEntity(uiMiniMap_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(uiMiniMap_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, uiMiniMap_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_uiCamera_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        constexpr float houseYOffset = 2.0f;
        constexpr float houseSpacing = 12.0f;
        for (int i = -2; i <= 2; ++i)
        {
            for (int j = -2; j <= 2; ++j)
            {
                if (i == 0 && j == 0)
                {
                    continue;
                }

                uiMiniMaphouse_RenderedMeshIndies.push_back(renderedMeshes.size());

                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_uiCamera_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->uiMiniMapHouse_MeshDataPtr);
                mesh.SetPosition(
                    miniMapLocation_.x + static_cast<float>(i) * houseSpacing,
                    miniMapLocation_.y + static_cast<float>(j) * houseSpacing + houseYOffset,
                    0.0f
                );
                mesh.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
                mesh.SetUseLighting(false);
            }
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMiniMapHouse_MeshDataPtr);
            mesh.SetPosition(miniMapLocation_.x, miniMapLocation_.y + houseYOffset, 0.0f);
            mesh.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMiniMapBackground_MeshDataPtr);
            mesh.SetPosition(miniMapLocation_.x, miniMapLocation_.y, -0.1f);
            mesh.SetColor(0.2f, 0.2f, 0.2f, 1.0f);
            mesh.SetUseLighting(false);
        }

        {
            uiMiniMapPlayer_RenderedMeshIndex_ = renderedMeshes.size();
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_uiCamera_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMiniMapPlayer_MeshDataPtr);
            mesh.SetPosition(miniMapLocation_.x, miniMapLocation_.y, 0.1f);
            mesh.SetColor(0.6f, 0.6f, 0.6f, 1.0f);
            mesh.SetUseLighting(false);
        }
    }
}

void Scene002::CreateCursorEntity()
{
    GetComponentStoresPtr()->CreateEntity(cursor_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(cursor_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(3);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_cursorPosition_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursorCircle_MeshDataPtr);
            mesh.SetPositionZ(0.53f);
            mesh.SetColor(0.8f, 0.8f, 0.8f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_cursorPress_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursorCircle_MeshDataPtr);
            mesh.SetPositionZ(0.52f);
            mesh.SetColor(0.8f, 0.2f, 0.2f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_cursorRelease_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursorCircle_MeshDataPtr);
            mesh.SetPositionZ(0.51f);
            mesh.SetColor(0.8f, 0.8f, 0.2f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(cursor_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        collisionPoints.resize(3);

        collisionPoints[s_cursorPosition_CollisionPointIndex_] = Project001::CollisionPoint2D(
            glm::vec2(), s_cursorPosition_CollisionShapeId_, true
        );

        collisionPoints[s_cursorPress_CollisionPointIndex_] = Project001::CollisionPoint2D(
            glm::vec2(), s_cursorPress_CollisionShapeId_, false
        );

        collisionPoints[s_cursorRelease_CollisionPointIndex_] = Project001::CollisionPoint2D(
            glm::vec2(), s_cursorRelease_CollisionShapeId_, false
        );
    }
}

void Scene002::CreateBaseEntity(unsigned int& entityId, const glm::vec2& position)
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
            mesh.SetMeshDataPtr(sharedDataPtr_->houseLit_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->houseLit_TextureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->houseText_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_TextureId);
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
            mesh.SetMeshDataPtr(sharedDataPtr_->houseDark_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->houseDark_TextureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->houseCollision_MeshDataPtr);
            mesh.SetColor(1.0f, 0.2f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;
    collisionBody2DCreationInfo.density_ = std::numeric_limits<float>::infinity();
    collisionBody2DCreationInfo.collisionGroupMask = s_wall_collisionGroupMask_;
    collisionBody2DCreationInfo.allowedCollisionFilterMask = s_actor_collisionGroupMask_;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(entityId, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, entityId));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPosition(position);

        std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
        collisionRectangles.emplace_back(glm::vec2(-64.0f, -64.0f), glm::vec2(64.0f, 64.0f));
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<BaseInfo>(entityId));
    BaseInfo* baseInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<BaseInfo>(baseInfoPtr, entityId));
    if (baseInfoPtr != nullptr)
    {
        unsigned int& light_EntityId = baseInfoPtr->light_EntityId;

        GetComponentStoresPtr()->CreateEntity(light_EntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<LightInfo>(light_EntityId));
        LightInfo* lightInfoPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<LightInfo>(lightInfoPtr, light_EntityId));
        if (lightInfoPtr != nullptr)
        {
            lightInfoPtr->owner_EntityId = entityId;
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(light_EntityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, light_EntityId));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraLight2_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->houseLightBottom_MeshDataPtr);
                mesh.SetColor(0.0f, 0.0f, 0.0f, 0.00f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
                mesh.SetRenderPriorityOverride(-100);
            }

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDark1_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->houseLightTop_MeshDataPtr);
                mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
                mesh.SetRenderPriorityOverride(-100);
            }

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDark2_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->houseLightTop_MeshDataPtr);
                mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
                mesh.SetRenderPriorityOverride(-100);
            }

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDebug_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->houseLightCollision_MeshDataPtr);
                mesh.SetColor(1.0f, 1.0f, 0.2f, 0.2f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
            }

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDebug_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->houseDoorCollision_MeshDataPtr);
                mesh.SetColor(1.0f, 0.2f, 1.0f, 0.2f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
            }
        }

        Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo2;
        collisionBody2DCreationInfo2.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR;
        collisionBody2DCreationInfo2.collisionGroupMask = s_sensor_collisionGroupMask_;
        collisionBody2DCreationInfo2.allowedCollisionFilterMask = s_actor_collisionGroupMask_;

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(light_EntityId, collisionBody2DCreationInfo2));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, light_EntityId));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), sharedDataPtr_->houseLight_radius, s_light_collisionShapeTag_, true);

            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(glm::vec2(-24.0f, -96.0f), glm::vec2(24.0f, -64.0f), s_base_collisionShapeTag_, true);
        }
    }
}

void Scene002::CreateGroundEntity()
{
    GetCollisionSystemPtr()->ResetCollisionBodyQuadTree2D(
        glm::vec2(-1160.0f, -1160.0f),
        glm::vec2(1160.0f, 1160.0f),
        3,
        16
    );

    GetCollisionSystemPtr()->FullyLoadCollisionBodyQuadTree2D();

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
            mesh.SetMeshDataPtr(sharedDataPtr_->groundFog_MeshDataPtr);
            mesh.SetPositionZ(-1.0f);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->groundLit_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->groundLit_TextureId);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraLight2_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->groundFog_MeshDataPtr);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.1f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->groundFog_MeshDataPtr);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
            mesh.SetPositionZ(-1.0f);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->groundDark_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->groundDark_TextureId);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark2_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->groundFog_MeshDataPtr);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->groundGrid_MeshId, sharedDataPtr_->groundGrid_MeshDataPtr->maxBoundingRadius);
            mesh.SetPositionZ(-0.1f);
            mesh.SetColor(0.2f, 0.2f, 1.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->groundGridLabels_MeshId, sharedDataPtr_->groundGridLabels_MeshDataPtr->maxBoundingRadius);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_TextureId);
            mesh.SetPositionZ(-0.1f);
            mesh.SetColor(0.2f, 0.2f, 1.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->groundCollisionQuadTree_MeshDataPtr);
            mesh.SetPositionZ(-0.2f);
            mesh.SetColor(1.0f, 0.4f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->groundCollision_MeshDataPtr);
            mesh.SetColor(1.0f, 0.2f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;
    collisionBody2DCreationInfo.density_ = std::numeric_limits<float>::infinity();
    collisionBody2DCreationInfo.collisionGroupMask = s_wall_collisionGroupMask_;
    collisionBody2DCreationInfo.allowedCollisionFilterMask = s_actor_collisionGroupMask_;

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

void Scene002::CreateHouseEntity(unsigned int& entityId, const glm::vec2& position)
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
            mesh.SetMeshDataPtr(sharedDataPtr_->houseLit_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->houseLit_TextureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->houseDark_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->houseDark_TextureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->houseCollision_MeshDataPtr);
            mesh.SetColor(1.0f, 0.2f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;
    collisionBody2DCreationInfo.density_ = std::numeric_limits<float>::infinity();
    collisionBody2DCreationInfo.collisionGroupMask = s_wall_collisionGroupMask_;
    collisionBody2DCreationInfo.allowedCollisionFilterMask = s_actor_collisionGroupMask_;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(entityId, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, entityId));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPosition(position);

        std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
        collisionRectangles.emplace_back(glm::vec2(-64.0f, -64.0f), glm::vec2(64.0f, 64.0f));
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<HouseInfo>(entityId));
    HouseInfo* houseInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<HouseInfo>(houseInfoPtr, entityId));
    if (houseInfoPtr != nullptr)
    {
        unsigned int& light_EntityId = houseInfoPtr->light_EntityId;

        GetComponentStoresPtr()->CreateEntity(light_EntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<LightInfo>(light_EntityId));
        LightInfo* lightInfoPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<LightInfo>(lightInfoPtr, light_EntityId));
        if (lightInfoPtr != nullptr)
        {
            lightInfoPtr->owner_EntityId = entityId;
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(light_EntityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, light_EntityId));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraLight2_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->houseLightBottom_MeshDataPtr);
                mesh.SetColor(0.0f, 0.0f, 0.0f, 0.00f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
                mesh.SetRenderPriorityOverride(-100);
            }

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDark1_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->houseLightTop_MeshDataPtr);
                mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
                mesh.SetRenderPriorityOverride(-100);
            }

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDark2_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->houseLightTop_MeshDataPtr);
                mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
                mesh.SetRenderPriorityOverride(-100);
            }

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDebug_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->houseLightCollision_MeshDataPtr);
                mesh.SetColor(1.0f, 1.0f, 0.2f, 0.2f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
            }

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDebug_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->houseDoorCollision_MeshDataPtr);
                mesh.SetColor(1.0f, 0.2f, 1.0f, 0.2f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
            }
        }

        Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo2;
        collisionBody2DCreationInfo2.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR;
        collisionBody2DCreationInfo2.collisionGroupMask = s_sensor_collisionGroupMask_;
        collisionBody2DCreationInfo2.allowedCollisionFilterMask = s_actor_collisionGroupMask_;

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(light_EntityId, collisionBody2DCreationInfo2));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, light_EntityId));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), sharedDataPtr_->houseLight_radius, s_light_collisionShapeTag_, true);

            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(glm::vec2(-24.0f, -96.0f), glm::vec2(24.0f, -64.0f), s_door_collisionShapeTag_, true);
        }
    }
}

void Scene002::CreatePersonEntity(unsigned int& entityId, const glm::vec2& position)
{
    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(entityId));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(2);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_personLit_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->personLit_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->personLit_TextureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_personDark_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->unknownDark_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->unknownDark_TextureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->personCollision_MeshDataPtr);
            mesh.SetColor(0.2f, 1.0f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;
    collisionBody2DCreationInfo.collisionGroupMask = s_actor_collisionGroupMask_;
    collisionBody2DCreationInfo.allowedCollisionFilterMask = s_wall_collisionGroupMask_ | s_actor_collisionGroupMask_;

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
    PersonInfo* personInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PersonInfo>(personInfoPtr, entityId));
    if (personInfoPtr != nullptr)
    {
        // 
    }
}

void Scene002::CreatePlayerEntity(const glm::vec2& position)
{
    GetComponentStoresPtr()->CreateEntity(player_EntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(player_EntityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, player_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(2);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_playerLit_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->personLit_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->personLit_TextureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_playerDark_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->unknownDark_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->unknownDark_TextureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDark2_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->playerLightStrong_MeshDataPtr);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-100);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->personCollision_MeshDataPtr);
            mesh.SetColor(0.2f, 1.0f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;
    collisionBody2DCreationInfo.collisionGroupMask = s_actor_collisionGroupMask_;
    collisionBody2DCreationInfo.allowedCollisionFilterMask = s_wall_collisionGroupMask_ | s_actor_collisionGroupMask_ | s_sensor_collisionGroupMask_;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(player_EntityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, player_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPosition(position);

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
        collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), 8.0f, s_player_collisionShapeTag_, true);
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<PlayerInfo>(player_EntityId_));
    PlayerInfo* playerInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PlayerInfo>(playerInfoPtr, player_EntityId_));
    if (playerInfoPtr != nullptr)
    {
        playerInfoPtr->battery_s = sharedDataPtr_->player_initialBattery_s;
        playerInfoPtr->stamina_s = sharedDataPtr_->player_initialStamina_s;

        unsigned int& light_EntityId = playerInfoPtr->light_EntityId;

        GetComponentStoresPtr()->CreateEntity(light_EntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<LightInfo>(light_EntityId));
        LightInfo* lightInfoPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<LightInfo>(lightInfoPtr, light_EntityId));
        if (lightInfoPtr != nullptr)
        {
            lightInfoPtr->owner_EntityId = player_EntityId_;
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(light_EntityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, light_EntityId));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraLight2_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->playerLightBottom_MeshDataPtr);
                mesh.SetColor(0.0f, 0.0f, 0.0f, 0.00f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
                mesh.SetRenderPriorityOverride(-100);
            }

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDark1_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->playerLightTop_MeshDataPtr);
                mesh.SetColor(0.0f, 0.0f, 0.0f, 0.0f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
                mesh.SetRenderPriorityOverride(-100);
            }

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDebug_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->playerLightCollision_MeshDataPtr);
                mesh.SetColor(1.0f, 1.0f, 0.2f, 0.2f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
            }
        }

        Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo2;
        collisionBody2DCreationInfo2.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR;
        collisionBody2DCreationInfo2.collisionGroupMask = s_sensor_collisionGroupMask_;
        collisionBody2DCreationInfo2.allowedCollisionFilterMask = s_actor_collisionGroupMask_;

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(light_EntityId, collisionBody2DCreationInfo2));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, light_EntityId));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
            std::vector<glm::vec2> corners;
            constexpr float radius = 60.0f;
            constexpr size_t subdivisions = 8;
            constexpr float angleRotation = glm::pi<float>() / static_cast<float>(subdivisions);
            for (size_t i = 0; i <= subdivisions; ++i)
            {
                glm::vec2 radiusVector(radius, 0.0f);
                radiusVector = Project001::Math::Rotate2DVector(radiusVector, static_cast<float>(i) * angleRotation);
                corners.emplace_back(radiusVector);
            }
            corners.emplace_back(-4.0f, -112.0f);
            corners.emplace_back(4.0f, -112.0f);
            for (size_t i = 0; i < corners.size(); ++i)
            {
                corners[i].y += 128.0f;
            }
            collisionConvexPolygons.emplace_back(corners, s_light_collisionShapeTag_, true);
        }
    }
}

void Scene002::CreateMonsterEntity(unsigned int& entityId, const glm::vec2& position)
{
    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(entityId));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(2);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_monsterLit_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCameraLight1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->monsterLit_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->monsterLit_TextureId);
            mesh.SetColor(1.0f, 0.6f, 0.6f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[s_monsterDark_RenderedMeshIndex_];
            mesh.SetCameraMask(s_mainCameraDark1_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->unknownDark_MeshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->unknownDark_TextureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_Mask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->monsterCollision_MeshDataPtr);
            mesh.SetColor(1.0f, 0.2f, 1.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.fixedTranslation = true;
    collisionBody2DCreationInfo.fixedRotation = true;
    collisionBody2DCreationInfo.collisionGroupMask = s_actor_collisionGroupMask_;
    collisionBody2DCreationInfo.allowedCollisionFilterMask = s_wall_collisionGroupMask_ | s_actor_collisionGroupMask_ | s_sensor_collisionGroupMask_;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(entityId, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, entityId));
    if (collisionBody2DPtr != nullptr)
    {
        collisionBody2DPtr->SetPosition(position);

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
        collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), 8.0f);
    }

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<MonsterInfo>(entityId));
    MonsterInfo* monsterInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<MonsterInfo>(monsterInfoPtr, entityId));
    if (monsterInfoPtr != nullptr)
    {
        unsigned int& vision_EntityId = monsterInfoPtr->vision_EntityId;

        GetComponentStoresPtr()->CreateEntity(vision_EntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<VisionInfo>(vision_EntityId));
        VisionInfo* visionInfoPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<VisionInfo>(visionInfoPtr, vision_EntityId));
        if (visionInfoPtr != nullptr)
        {
            visionInfoPtr->owner_EntityId = entityId;
        }

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(vision_EntityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, vision_EntityId));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetCameraMask(s_mainCameraGroup_Mask_);
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            {
                renderedMeshes.emplace_back();
                Project001::RenderedMesh& mesh = renderedMeshes.back();
                mesh.SetCameraMask(s_mainCameraDebug_Mask_);
                mesh.SetMeshDataPtr(sharedDataPtr_->monsterVisionCollision_MeshDataPtr);
                mesh.SetColor(1.0f, 1.0f, 0.2f, 0.2f);
                mesh.SetTranslucent(true);
                mesh.SetUseLighting(false);
            }
        }

        Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo2;
        collisionBody2DCreationInfo2.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR;
        collisionBody2DCreationInfo2.collisionGroupMask = s_sensor_collisionGroupMask_;
        collisionBody2DCreationInfo2.allowedCollisionFilterMask = s_actor_collisionGroupMask_;

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(vision_EntityId, collisionBody2DCreationInfo2));
        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, vision_EntityId));
        if (collisionBody2DPtr != nullptr)
        {
            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(glm::vec2(0.0f, 0.0f), sharedDataPtr_->monsterVision_radius);
        }
    }
}

void Scene002::UpdateMainCameraEntity(float timestep_s)
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

void Scene002::UpdateUiTextEntity()
{
    constexpr float pixelSize = 2.0f;

    sharedDataPtr_->uiLeftText01_MeshDataPtr->Clear();
    sharedDataPtr_->uiLeftText02_MeshDataPtr->Clear();
    sharedDataPtr_->uiLeftText03_MeshDataPtr->Clear();

    unsigned int handScore = 0;
    unsigned int totalScore = sharedDataPtr_->score;
    float battery_s = 0.0f;
    float stamina_s = 0.0f;
    float remainingTime_s = sharedDataPtr_->remainingTime_s;

    PlayerInfo* playerInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PlayerInfo>(playerInfoPtr, player_EntityId_));
    if (playerInfoPtr != nullptr)
    {
        battery_s = playerInfoPtr->battery_s;
        stamina_s = playerInfoPtr->stamina_s;
        handScore = playerInfoPtr->score;
    }

    if (handScore > 99) handScore = 99;
    if (totalScore > 99) totalScore = 99;
    if (battery_s > 999.99f) battery_s = 999.99f;
    if (stamina_s > 999.99f) stamina_s = 999.99f;
    if (remainingTime_s > 999.99f) remainingTime_s = 999.99f;

    std::string candyString =
        "C:" + std::to_string(totalScore) + "(" + std::to_string(handScore) + ")";

    char buffer[20];

    snprintf(buffer, sizeof(buffer), "%.2f", battery_s);
    std::string batteryString = "B:" + std::string(buffer);

    snprintf(buffer, sizeof(buffer), "%.2f", stamina_s);
    std::string staminaString = "S:" + std::string(buffer);

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiLeftText01_MeshDataPtr,
        *sharedDataPtr_->pixelFont_FontDataPtr,
        candyString,
        pixelSize
    ));

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiLeftText02_MeshDataPtr,
        *sharedDataPtr_->pixelFont_FontDataPtr,
        batteryString,
        pixelSize
    ));

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiLeftText03_MeshDataPtr,
        *sharedDataPtr_->pixelFont_FontDataPtr,
        staminaString,
        pixelSize
    ));

    sharedDataPtr_->uiMiddleText01_MeshDataPtr->Clear();

    snprintf(buffer, sizeof(buffer), "%.2f", remainingTime_s);
    std::string timeString = std::string(buffer);

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMiddleText01_MeshDataPtr,
        *sharedDataPtr_->pixelFont_FontDataPtr,
        timeString,
        pixelSize
    ));
    Project001::Mesh::TranslateMesh(
        *sharedDataPtr_->uiMiddleText01_MeshDataPtr,
        glm::vec3(sharedDataPtr_->uiMiddleText01_MeshDataPtr->GetSize().x * -0.5f, 0.0f, 0.0f)
    );

    // FAIL_CHECK(Project001::GenerateMeshDataFromFontDataAndString(
    //     *sharedDataPtr_->uiTopMiddleText_MeshDataPtr,
    //     *sharedDataPtr_->pixelFont_FontDataPtr,
    //     middleString,
    //     pixelSize,
    //     1
    // ));
    // Project001::Mesh::TranslateMesh(
    //     *sharedDataPtr_->uiTopMiddleText_MeshDataPtr,
    //     glm::vec3((middleString.length() * pixelSize * 6.0f) * -0.5f, 0.0f, 0.0f)
    // );

    // sharedDataPtr_->uiTopRightText_MeshDataPtr->Clear();

    // FAIL_CHECK(Project001::GenerateMeshDataFromFontDataAndString(
    //     *sharedDataPtr_->uiTopRightText_MeshDataPtr,
    //     *sharedDataPtr_->pixelFont_FontDataPtr,
    //     rightString,
    //     pixelSize,
    //     2
    // ));
    // Project001::Mesh::TranslateMesh(
    //     *sharedDataPtr_->uiTopRightText_MeshDataPtr,
    //     glm::vec3(pixelSize, 0.0f, 0.0f)
    // );
    // Project001::Mesh::TranslateMesh(
    //     *sharedDataPtr_->uiTopRightText_MeshDataPtr,
    //     glm::vec3((rightString.length() - 1) * pixelSize * -6.0f, 0.0f, 0.0f)
    // );
}

void Scene002::UpdateUiPauseTextEntity()
{
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, uiPauseText_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetVisible(paused_);
    }
}

void Scene002::UpdateUiMiniMapEntity()
{
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, uiMiniMap_EntityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_uiCamera_Mask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        for (size_t i = 0; i < house_EntityIds_.size() && i < uiMiniMaphouse_RenderedMeshIndies.size(); ++i)
        {
            const unsigned int& entityId = house_EntityIds_[i];
            const unsigned int& renderedMeshIndex = uiMiniMaphouse_RenderedMeshIndies[i];
            Project001::RenderedMesh& renderedMesh = renderedMeshes[renderedMeshIndex];

            HouseInfo* houseInfoPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<HouseInfo>(houseInfoPtr, entityId));
            if (houseInfoPtr != nullptr)
            {
                if (houseInfoPtr->state == HouseInfo::State::STATE_DARK)
                {
                    renderedMesh.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
                }
                else
                {
                    renderedMesh.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
                }
            }
        }

        Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, player_EntityId_));
        if (collisionBody2DPtr != nullptr)
        {
            Project001::RenderedMesh& renderedMesh = renderedMeshes[uiMiniMapPlayer_RenderedMeshIndex_];
            renderedMesh.SetPosition(
                miniMapLocation_.x + collisionBody2DPtr->GetPosition().x / 32.0f,
                miniMapLocation_.y + collisionBody2DPtr->GetPosition().y / 32.0f,
                0.1f
            );
        }
    }
}

void Scene002::UpdateCursorPosition(float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    GetWindowPtr()->GetWindowSize(windowWidth, windowHeight);

    glm::vec2 viewportNormalizedCursorPosition = GetRendererPtr()->ConvertPointFromWindowToViewportNormalized(glm::vec2(xPosition, yPosition), static_cast<float>(windowHeight));

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

                collisionPoints[s_cursorPosition_CollisionPointIndex_].position = worldCursorPosition;
            }
        }
    }
}

void Scene002::UpdateGroundCollisionBodyQuadTreeMesh()
{
    const Project001::CollisionBodyQuadTreeNode2D* rootNodePtr = GetCollisionSystemPtr()->GetCollisionBodyQuadTree2D().GetRootNode();

    const float lineWidth = 4.0f;

    sharedDataPtr_->groundCollisionQuadTree_MeshDataPtr->Clear();

    Project001::Mesh::Generate2DRectangleFrame(*sharedDataPtr_->groundCollisionQuadTree_MeshDataPtr, rootNodePtr->min, rootNodePtr->max, lineWidth);

    if ((!rootNodePtr->leafNode || !rootNodePtr->bodyPtrs.empty()))
    {
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

                Project001::Mesh::Generate2DLine(*sharedDataPtr_->groundCollisionQuadTree_MeshDataPtr, top, bottom, lineWidth);
                Project001::Mesh::Generate2DLine(*sharedDataPtr_->groundCollisionQuadTree_MeshDataPtr, left, right, lineWidth);

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

void Scene002::UpdateHouseEntities(float timestep_s)
{
    std::vector<unsigned int> darkHouseIndices;

    HouseInfo* houseInfoPtrs = nullptr;
    size_t houseInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<HouseInfo>(houseInfoPtrs, houseInfoCount);
    for (size_t i = 0; i < houseInfoCount; ++i)
    {
        HouseInfo& houseInfo = houseInfoPtrs[i];

        if (houseInfo.state == HouseInfo::State::STATE_WAITING)
        {
            if (houseInfo.stateDuration_s <= 0.0f)
            {
                houseInfo.state = HouseInfo::State::STATE_DARK;
            }
            else
            {
                houseInfo.stateDuration_s -= timestep_s;
            }
        }

        Project001::CollisionBody2D* lightCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(lightCollisionBodyPtr, houseInfo.light_EntityId));
        if (lightCollisionBodyPtr != nullptr)
        {
            if (houseInfo.state == HouseInfo::State::STATE_DARK)
            {
                lightCollisionBodyPtr->SetEnabled(false);

                darkHouseIndices.push_back(i);
            }
            else
            {
                lightCollisionBodyPtr->SetEnabled(true);
            }
        }
    }


    if (!darkHouseIndices.empty() && sharedDataPtr_->nextHouseTurnOnWait_s <= 0)
    {
        std::shuffle(darkHouseIndices.begin(), darkHouseIndices.end(), randomNumberEngine_);

        HouseInfo& houseInfo = houseInfoPtrs[darkHouseIndices.back()];

        houseInfo.state = HouseInfo::State::STATE_LIT;
        sharedDataPtr_->nextHouseTurnOnWait_s = sharedDataPtr_->house_randomTurnOnWait_s;
    }
    else
    {
        sharedDataPtr_->nextHouseTurnOnWait_s -= timestep_s;
    }
}

void Scene002::UpdatePersonEntities(float timestep_s)
{
    PersonInfo* personInfoPtrs = nullptr;
    size_t personInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<PersonInfo>(personInfoPtrs, personInfoCount);
    for (size_t i = 0; i < personInfoCount; ++i)
    {
        PersonInfo& personInfo = personInfoPtrs[i];

        unsigned int personEntityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<PersonInfo>(personEntityId, &personInfo);

        if (personInfo.state == PersonInfo::State::STATE_STANDING)
        {
            if (personInfo.stateDuration_s <= 0.0f)
            {
                Project001::CollisionBody2D* collisionBodyPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, personEntityId));
                if (collisionBodyPtr != nullptr)
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

                    collisionBodyPtr->SetVelocity(velocityDirection * sharedDataPtr_->person_walkSpeed);

                    personInfo.state = PersonInfo::State::STATE_WALKING;

                    std::uniform_real_distribution<float> distributionT(sharedDataPtr_->person_walkDurationMin_s, sharedDataPtr_->person_walkDurationMax_s);
                    personInfo.stateDuration_s = distributionT(randomNumberEngine_);
                }
            }
            else
            {
                personInfo.stateDuration_s -= timestep_s;
            }
        }
        else if (personInfo.state == PersonInfo::State::STATE_WALKING)
        {
            if (personInfo.stateDuration_s <= 0.0f)
            {
                Project001::CollisionBody2D* collisionBodyPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, personEntityId));
                if (collisionBodyPtr != nullptr)
                {
                    collisionBodyPtr->SetVelocity(glm::vec2(0.0f, 0.0f));

                    personInfo.state = PersonInfo::State::STATE_STANDING;

                    std::uniform_real_distribution<float> distributionT(sharedDataPtr_->person_walkDurationMin_s, sharedDataPtr_->person_walkDurationMax_s);
                    personInfo.stateDuration_s = distributionT(randomNumberEngine_);
                }
            }
            else
            {
                personInfo.stateDuration_s -= timestep_s;
            }
        }
    }
}

void Scene002::UpdatePlayerEntity(float timestep_s)
{
    PlayerInfo* playerInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PlayerInfo>(playerInfoPtr, player_EntityId_));
    Project001::CollisionBody2D* playerCollisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(playerCollisionBodyPtr, player_EntityId_));
    if (playerInfoPtr != nullptr && playerCollisionBodyPtr != nullptr)
    {
        bool movingLeft = false;
        if (sharedDataPtr_->left_usesKeyboard)
        {
            movingLeft = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->left_KeyCode);
        }
        else
        {
            movingLeft = GetWindowPtr()->GetMouseButtonPressed(sharedDataPtr_->left_MouseButton);
        }

        bool movingRight = false;
        if (sharedDataPtr_->right_usesKeyboard)
        {
            movingRight = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->right_KeyCode);
        }
        else
        {
            movingRight = GetWindowPtr()->GetMouseButtonPressed(sharedDataPtr_->right_MouseButton);
        }

        bool movingUp = false;
        if (sharedDataPtr_->up_usesKeyboard)
        {
            movingUp = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->up_KeyCode);
        }
        else
        {
            movingUp = GetWindowPtr()->GetMouseButtonPressed(sharedDataPtr_->up_MouseButton);
        }

        bool movingDown = false;
        if (sharedDataPtr_->down_usesKeyboard)
        {
            movingDown = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->down_KeyCode);
        }
        else
        {
            movingDown = GetWindowPtr()->GetMouseButtonPressed(sharedDataPtr_->down_MouseButton);
        }

        bool running = false;
        if (sharedDataPtr_->sprint_usesKeyboard)
        {
            running = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->sprint_KeyCode);
        }
        else
        {
            running = GetWindowPtr()->GetMouseButtonPressed(sharedDataPtr_->sprint_MouseButton);
        }

        std::vector<bool> buttonValues;
        GetWindowPtr()->GetJoystickButtons(0, buttonValues);

        if (buttonValues.size() > sharedDataPtr_->left_joystickButtonIndex)
        {
            movingLeft |= buttonValues[sharedDataPtr_->left_joystickButtonIndex];
        }

        if (buttonValues.size() > sharedDataPtr_->right_joystickButtonIndex)
        {
            movingRight |= buttonValues[sharedDataPtr_->right_joystickButtonIndex];
        }

        if (buttonValues.size() > sharedDataPtr_->down_joystickButtonIndex)
        {
            movingDown |= buttonValues[sharedDataPtr_->down_joystickButtonIndex];
        }

        if (buttonValues.size() > sharedDataPtr_->up_joystickButtonIndex)
        {
            movingUp |= buttonValues[sharedDataPtr_->up_joystickButtonIndex];
        }

        if (buttonValues.size() > sharedDataPtr_->sprint_joystickButtonIndex)
        {
            running |= buttonValues[sharedDataPtr_->sprint_joystickButtonIndex];
        }

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

        std::vector<float> axisValues;
        GetWindowPtr()->GetJoystickAxis(0, axisValues);

        glm::vec2 moveAxisValue(0.0f, 0.0f);
        if (axisValues.size() > sharedDataPtr_->moveRightLeft_joystickAxisIndex)
        {
            moveAxisValue.x = axisValues[sharedDataPtr_->moveRightLeft_joystickAxisIndex];
        }
        if (axisValues.size() > sharedDataPtr_->moveDownUp_joystickAxisIndex)
        {
            moveAxisValue.y = -axisValues[sharedDataPtr_->moveDownUp_joystickAxisIndex];
        }

        if (glm::length(moveAxisValue) > sharedDataPtr_->move_joystickAxisDeadzone)
        {
            velocityDirection += moveAxisValue;
        }

        float accelerationMagnitude = glm::length(velocityDirection);
        if (accelerationMagnitude > 0.0f)
        {
            velocityDirection /= accelerationMagnitude;

            if (running)
            {
                playerInfoPtr->stamina_s -= timestep_s;
                if (playerInfoPtr->stamina_s < 0.0f)
                {
                    playerInfoPtr->stamina_s = 0.0f;
                }
            }

            if (running && playerInfoPtr->stamina_s > 0.0f)
            {
                playerInfoPtr->state = PlayerInfo::State::STATE_RUNNING;

                playerCollisionBodyPtr->SetVelocity(velocityDirection * sharedDataPtr_->player_runSpeed);
            }
            else
            {
                playerInfoPtr->state = PlayerInfo::State::STATE_WALKING;
                playerInfoPtr->stamina_s += sharedDataPtr_->player_staminaRechargeRate * timestep_s;
                if (playerInfoPtr->stamina_s > sharedDataPtr_->player_initialStamina_s)
                {
                    playerInfoPtr->stamina_s = sharedDataPtr_->player_initialStamina_s;
                }

                playerCollisionBodyPtr->SetVelocity(velocityDirection * sharedDataPtr_->player_walkSpeed);
            }
        }
        else
        {
            playerInfoPtr->state = PlayerInfo::State::STATE_STANDING;
            playerInfoPtr->stamina_s += sharedDataPtr_->player_staminaRechargeRate * timestep_s;
            if (playerInfoPtr->stamina_s > sharedDataPtr_->player_initialStamina_s)
            {
                playerInfoPtr->stamina_s = sharedDataPtr_->player_initialStamina_s;
            }

            playerCollisionBodyPtr->SetVelocity(glm::vec2(0.0f, 0.0f));
        }

        Project001::CollisionBody2D* playerLightCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(playerLightCollisionBodyPtr, playerInfoPtr->light_EntityId));
        if (playerLightCollisionBodyPtr != nullptr)
        {
            glm::vec2 aimAxisValue(0.0f, 0.0f);
            if (axisValues.size() > sharedDataPtr_->aimRightLeft_joystickAxisIndex)
            {
                aimAxisValue.x = axisValues[sharedDataPtr_->aimRightLeft_joystickAxisIndex];
            }
            if (axisValues.size() > sharedDataPtr_->aimDownUp_joystickAxisIndex)
            {
                aimAxisValue.y = -axisValues[sharedDataPtr_->aimDownUp_joystickAxisIndex];
            }

            if (glm::length(aimAxisValue) > sharedDataPtr_->aim_joystickAxisDeadzone)
            {
                playerInfoPtr->battery_s -= timestep_s;
                if (playerInfoPtr->battery_s < 0.0f)
                {
                    playerInfoPtr->battery_s = 0.0f;
                }

                playerLightCollisionBodyPtr->SetEnabled(true);

                glm::vec2 collisionBodyDirection = playerLightCollisionBodyPtr->GetForwardVector();

                float angleToCursor = Project001::Math::Get2DVectorAngle(collisionBodyDirection, aimAxisValue);
                playerLightCollisionBodyPtr->AddRotation(angleToCursor);
            }
            else
            {
                bool lightOn = false;

                if (sharedDataPtr_->shine_usesKeyboard)
                {
                    lightOn = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->shine_KeyCode);
                }
                else
                {
                    lightOn = GetWindowPtr()->GetMouseButtonPressed(sharedDataPtr_->shine_MouseButton);
                }

                if (lightOn)
                {
                    playerInfoPtr->battery_s -= timestep_s;
                    if (playerInfoPtr->battery_s < 0.0f)
                    {
                        playerInfoPtr->battery_s = 0.0f;
                    }
                }

                if (playerInfoPtr->battery_s <= 0.0f)
                {
                    lightOn = false;
                }

                playerLightCollisionBodyPtr->SetEnabled(lightOn);

                Project001::CollisionBody2D* cursorCollisionBodyPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(cursorCollisionBodyPtr, cursor_EntityId_));
                if (cursorCollisionBodyPtr != nullptr)
                {
                    std::vector<Project001::CollisionPoint2D>& collisionPoints = cursorCollisionBodyPtr->GetCollisionPoints();
                    const glm::vec2& cursorPosition = collisionPoints[s_cursorPosition_CollisionPointIndex_].position;
                    glm::vec2 collisionBodyDirection = playerLightCollisionBodyPtr->GetForwardVector();
                    glm::vec2 collisionBodyToCursor = cursorPosition - playerLightCollisionBodyPtr->GetPosition();

                    float angleToCursor = Project001::Math::Get2DVectorAngle(collisionBodyDirection, collisionBodyToCursor);
                    playerLightCollisionBodyPtr->AddRotation(angleToCursor);
                }
            }
        }

        playerInfoPtr->safe = false;

        const std::vector<Project001::CollisionOverlapData2D>& collisionOverlaps = playerCollisionBodyPtr->GetCollisionOverlaps();
        for (size_t i = 0; i < collisionOverlaps.size(); ++i)
        {
            const Project001::CollisionOverlapData2D& collisionOverlapData = collisionOverlaps[i];

            if (collisionOverlapData.otherShapeTag == s_base_collisionShapeTag_)
            {
                if (playerInfoPtr->score > 0)
                {
                    GetSoundPlayerPtr()->PlaySoundSource(sharedDataPtr_->dropCoin_SoundSourceId);
                }

                sharedDataPtr_->score += playerInfoPtr->score;
                playerInfoPtr->score = 0;

                playerInfoPtr->battery_s = sharedDataPtr_->player_initialBattery_s;
            }
            else if (collisionOverlapData.otherShapeTag == s_door_collisionShapeTag_)
            {
                LightInfo* lightInfoPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<LightInfo>(lightInfoPtr, collisionOverlapData.otherEntityId));
                if (lightInfoPtr != nullptr)
                {
                    HouseInfo* houseInfoPtr = nullptr;
                    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<HouseInfo>(houseInfoPtr, lightInfoPtr->owner_EntityId));
                    if (houseInfoPtr != nullptr)
                    {
                        if (houseInfoPtr->state == HouseInfo::State::STATE_LIT)
                        {
                            playerInfoPtr->score++;

                            GetSoundPlayerPtr()->PlaySoundSource(sharedDataPtr_->pickupCoin_SoundSourceId);
                        }

                        houseInfoPtr->state = HouseInfo::State::STATE_WAITING;
                        houseInfoPtr->stateDuration_s = sharedDataPtr_->house_turnOffWait_s;
                    }
                }
            }
            else if (collisionOverlapData.otherShapeTag == s_light_collisionShapeTag_)
            {
                playerInfoPtr->safe = true;
            }
        }
    }
}

void Scene002::UpdateMonsterEntities(float timestep_s)
{
    MonsterInfo* monsterInfoPtrs = nullptr;
    size_t monsterInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<MonsterInfo>(monsterInfoPtrs, monsterInfoCount);
    for (size_t i = 0; i < monsterInfoCount; ++i)
    {
        MonsterInfo& monsterInfo = monsterInfoPtrs[i];

        unsigned int monsterEntityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<MonsterInfo>(monsterEntityId, &monsterInfo);

        bool distracted = false;

        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, monsterEntityId));
        if (collisionBodyPtr != nullptr)
        {
            const std::vector<Project001::CollisionOverlapData2D>& collisionOverlaps = collisionBodyPtr->GetCollisionOverlaps();
            for (size_t i = 0; i < collisionOverlaps.size(); ++i)
            {
                const Project001::CollisionOverlapData2D& collisionOverlapData = collisionOverlaps[i];

                if (collisionOverlapData.otherShapeTag == s_player_collisionShapeTag_)
                {
                    PlayerInfo* playerInfoPtr = nullptr;
                    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PlayerInfo>(playerInfoPtr, collisionOverlapData.otherEntityId));
                    if (playerInfoPtr != nullptr)
                    {
                        if (playerInfoPtr->score > 0)
                        {
                            playerInfoPtr->score = 0;

                            GetSoundPlayerPtr()->PlaySoundSource(sharedDataPtr_->hitHurt_SoundSourceId);

                            Project001::CollisionBody2D* collisionBodyPtr = nullptr;
                            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, monsterEntityId));
                            if (collisionBodyPtr != nullptr)
                            {
                                collisionBodyPtr->SetVelocity(glm::vec2(0.0f, 0.0f));
                                monsterInfo.state = MonsterInfo::State::STATE_CELEBRATING;
                                monsterInfo.stateDuration_s = 0.5f; // make sure this is greater then monsterInfo.animationStateDuration_s for walking and running
                                distracted = true;
                                break;
                            }
                        }
                    }
                }
                else if (collisionOverlapData.otherShapeTag == s_light_collisionShapeTag_)
                {
                    Project001::CollisionBody2D* otherCollisionBodyPtr = nullptr;
                    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(otherCollisionBodyPtr, collisionOverlapData.otherEntityId));
                    if (otherCollisionBodyPtr != nullptr)
                    {
                        const glm::vec2& position = collisionBodyPtr->GetPosition();
                        const glm::vec2& otherPosition = otherCollisionBodyPtr->GetPosition();

                        glm::vec2 towardOther = otherPosition - position;
                        float towardOtherMagnitude = glm::length(towardOther);

                        if (towardOtherMagnitude > 0)
                        {
                            Project001::CollisionBody2D* collisionBodyPtr = nullptr;
                            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, monsterEntityId));
                            if (collisionBodyPtr != nullptr)
                            {
                                glm::vec2 newVelocity = -towardOther * sharedDataPtr_->monster_runSpeed / towardOtherMagnitude;
                                collisionBodyPtr->SetVelocity(newVelocity);
                                monsterInfo.state = MonsterInfo::State::STATE_RUNNING;
                                monsterInfo.stateDuration_s = 0.0f;
                                distracted = true;
                            }
                        }
                    }
                }
            }
        }

        if (distracted)
        {
            continue;
        }

        Project001::CollisionBody2D* visionCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(visionCollisionBodyPtr, monsterInfo.vision_EntityId));
        if (visionCollisionBodyPtr != nullptr)
        {
            const std::vector<Project001::CollisionOverlapData2D>& collisionOverlaps = visionCollisionBodyPtr->GetCollisionOverlaps();
            for (size_t i = 0; i < collisionOverlaps.size(); ++i)
            {
                const Project001::CollisionOverlapData2D& collisionOverlapData = collisionOverlaps[i];

                if (collisionOverlapData.otherShapeTag == s_player_collisionShapeTag_)
                {
                    PlayerInfo* playerInfoPtr = nullptr;
                    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PlayerInfo>(playerInfoPtr, collisionOverlapData.otherEntityId));
                    if (playerInfoPtr != nullptr)
                    {
                        if (playerInfoPtr->safe || playerInfoPtr->score == 0)
                        {
                            continue;
                        }
                    }

                    Project001::CollisionBody2D* otherCollisionBodyPtr = nullptr;
                    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(otherCollisionBodyPtr, collisionOverlapData.otherEntityId));
                    if (otherCollisionBodyPtr != nullptr)
                    {
                        const glm::vec2& position = visionCollisionBodyPtr->GetPosition();
                        const glm::vec2& otherPosition = otherCollisionBodyPtr->GetPosition();

                        glm::vec2 towardOther = otherPosition - position;
                        float towardOtherMagnitude = glm::length(towardOther);

                        if (towardOtherMagnitude > 0)
                        {
                            Project001::CollisionBody2D* collisionBodyPtr = nullptr;
                            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, monsterEntityId));
                            if (collisionBodyPtr != nullptr)
                            {
                                glm::vec2 newVelocity = towardOther * sharedDataPtr_->monster_runSpeed / towardOtherMagnitude;
                                collisionBodyPtr->SetVelocity(newVelocity);
                                monsterInfo.state = MonsterInfo::State::STATE_RUNNING;
                                monsterInfo.stateDuration_s = 0.0f;
                                distracted = true;
                            }
                        }
                    }
                }
            }
        }

        if (distracted)
        {
            continue;
        }

        if (collisionBodyPtr != nullptr)
        {
            if (monsterInfo.state != MonsterInfo::State::STATE_WALKING)
            {
                if (monsterInfo.stateDuration_s <= 0.0f)
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

                    collisionBodyPtr->SetVelocity(velocityDirection * sharedDataPtr_->monster_walkSpeed);

                    monsterInfo.state = MonsterInfo::State::STATE_WALKING;

                    std::uniform_real_distribution<float> distributionT(sharedDataPtr_->monster_walkDurationMin_s, sharedDataPtr_->monster_walkDurationMax_s);
                    monsterInfo.stateDuration_s = distributionT(randomNumberEngine_);
                }
                else
                {
                    monsterInfo.stateDuration_s -= timestep_s;
                }
            }
            else if (monsterInfo.state == MonsterInfo::State::STATE_WALKING)
            {
                if (monsterInfo.stateDuration_s <= 0.0f)
                {
                    collisionBodyPtr->SetVelocity(glm::vec2(0.0f, 0.0f));

                    monsterInfo.state = MonsterInfo::State::STATE_STANDING;

                    std::uniform_real_distribution<float> distributionT(sharedDataPtr_->monster_walkDurationMin_s, sharedDataPtr_->monster_walkDurationMax_s);
                    monsterInfo.stateDuration_s = distributionT(randomNumberEngine_);
                }
                else
                {
                    monsterInfo.stateDuration_s -= timestep_s;
                }
            }
        }
    }
}

void Scene002::UpdateWorld(float timestep_s)
{
    sharedDataPtr_->remainingTime_s -= timestep_s;

    if (sharedDataPtr_->remainingTime_s < 0.0f)
    {
        sharedDataPtr_->remainingTime_s = 0.0f;

        for (size_t i = 0; i < 3; ++i)
        {
            GetSoundPlayerPtr()->PlaySoundSource(sharedDataPtr_->pickupCoin_SoundSourceId);

            while (GetSoundPlayerPtr()->GetIsPlayingSoundSource(sharedDataPtr_->pickupCoin_SoundSourceId))
            {
                continue;
            }
        }

        SendEventToApplication(Project001::SwitchSceneEvent(sharedDataPtr_->scene001Id));
        if (GetActiveScene()->GetId() == sharedDataPtr_->scene001Id)
        {
            SendEventToScene(GetId(), Project001::DeinitializeEvent());
            SendEventToApplication(Project001::InitializeEvent());
        }
        return;
    }
}

void Scene002::AnimatePersonEntities(float timestep_s)
{
    PersonInfo* personInfoPtrs = nullptr;
    size_t personInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<PersonInfo>(personInfoPtrs, personInfoCount);
    for (size_t i = 0; i < personInfoCount; ++i)
    {
        PersonInfo& personInfo = personInfoPtrs[i];

        unsigned int personEntityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<PersonInfo>(personEntityId, &personInfo);

        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, personEntityId));
        if (renderedModelPtr != nullptr)
        {
            if (personInfo.animationStateDuration_s == 0.0f)
            {
                if (personInfo.state == PersonInfo::State::STATE_STANDING)
                {
                    personInfo.animationState = PersonInfo::State::STATE_STANDING;
                    personInfo.currentJumpDuration_s = 0.0f;
                    personInfo.currentJumpHeight = 0.0f;

                }
                else if (personInfo.state == PersonInfo::State::STATE_WALKING)
                {
                    personInfo.animationState = PersonInfo::State::STATE_WALKING;
                    personInfo.animationStateDuration_s = 0.375f;
                    personInfo.currentJumpDuration_s = 0.375f;
                    personInfo.currentJumpHeight = 2.0f;
                }
            }

            if (personInfo.currentJumpDuration_s > 0.0f)
            {
                std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[s_personLit_RenderedMeshIndex_];
                    mesh.SetPositionZ(
                        personInfo.currentJumpHeight * std::abs(glm::sin(personInfo.animationStateDuration_s * glm::pi<float>() / personInfo.currentJumpDuration_s))
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[s_personDark_RenderedMeshIndex_];
                    mesh.SetPositionZ(
                        personInfo.currentJumpHeight * std::abs(glm::sin(personInfo.animationStateDuration_s * glm::pi<float>() / personInfo.currentJumpDuration_s))
                    );
                }

                personInfo.animationStateDuration_s -= timestep_s;
                if (personInfo.animationStateDuration_s < 0.0f)
                {
                    personInfo.animationStateDuration_s = 0.0f;
                }
            }
        }
    }
}

void Scene002::AnimatePlayerEntity(float timestep_s)
{
    PlayerInfo* playerInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PlayerInfo>(playerInfoPtr, player_EntityId_));
    Project001::RenderedModel* playerRenderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(playerRenderedModelPtr, player_EntityId_));
    if (playerInfoPtr != nullptr && playerRenderedModelPtr != nullptr)
    {
        if (playerInfoPtr->animationStateDuration_s == 0.0f)
        {
            if (playerInfoPtr->state == PlayerInfo::State::STATE_STANDING)
            {
                playerInfoPtr->animationState = PlayerInfo::State::STATE_STANDING;
                playerInfoPtr->animationStateDuration_s = 0.0f;
                playerInfoPtr->currentJumpDuration_s = 0.0f;
                playerInfoPtr->currentJumpHeight = 0.0f;
                
            }
            else if(playerInfoPtr->state == PlayerInfo::State::STATE_WALKING)
            {
                playerInfoPtr->animationState = PlayerInfo::State::STATE_WALKING;
                playerInfoPtr->animationStateDuration_s = 0.375f;
                playerInfoPtr->currentJumpDuration_s = 0.375f;
                playerInfoPtr->currentJumpHeight = 2.0f;
            }
            else if (playerInfoPtr->state == PlayerInfo::State::STATE_RUNNING)
            {
                playerInfoPtr->animationState = PlayerInfo::State::STATE_RUNNING;
                playerInfoPtr->animationStateDuration_s = 0.25f;
                playerInfoPtr->currentJumpDuration_s = 0.25f;
                playerInfoPtr->currentJumpHeight = 4.0f;
            }
        }

        if (playerInfoPtr->currentJumpDuration_s > 0.0f)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = playerRenderedModelPtr->GetRenderedMeshes();

            {
                Project001::RenderedMesh& mesh = renderedMeshes[s_playerLit_RenderedMeshIndex_];
                mesh.SetPositionZ(
                    playerInfoPtr->currentJumpHeight * std::abs(glm::sin(playerInfoPtr->animationStateDuration_s * glm::pi<float>() / playerInfoPtr->currentJumpDuration_s))
                );
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[s_playerDark_RenderedMeshIndex_];
                mesh.SetPositionZ(
                    playerInfoPtr->currentJumpHeight * std::abs(glm::sin(playerInfoPtr->animationStateDuration_s * glm::pi<float>() / playerInfoPtr->currentJumpDuration_s))
                );
            }
        }

        playerInfoPtr->animationStateDuration_s -= timestep_s;
        if (playerInfoPtr->animationStateDuration_s < 0.0f)
        {
            playerInfoPtr->animationStateDuration_s = 0.0f;
        }
    }
}

void Scene002::AnimateMonsterEntities(float timestep_s)
{
    MonsterInfo* monsterInfoPtrs = nullptr;
    size_t monsterInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<MonsterInfo>(monsterInfoPtrs, monsterInfoCount);
    for (size_t i = 0; i < monsterInfoCount; ++i)
    {
        MonsterInfo& monsterInfo = monsterInfoPtrs[i];

        unsigned int monsterEntityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<MonsterInfo>(monsterEntityId, &monsterInfo);

        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, monsterEntityId));
        if (renderedModelPtr != nullptr)
        {
            if (monsterInfo.animationStateDuration_s == 0.0f)
            {
                if (monsterInfo.state == MonsterInfo::State::STATE_STANDING)
                {
                    monsterInfo.animationState = MonsterInfo::State::STATE_STANDING;
                    monsterInfo.animationStateDuration_s = 0.0f;
                    monsterInfo.currentJumpDuration_s = 0.0f;
                    monsterInfo.currentJumpHeight = 0.0f;

                }
                else if (monsterInfo.state == MonsterInfo::State::STATE_WALKING)
                {
                    monsterInfo.animationState = MonsterInfo::State::STATE_WALKING;
                    monsterInfo.animationStateDuration_s = 0.375f;
                    monsterInfo.currentJumpDuration_s = 0.375f;
                    monsterInfo.currentJumpHeight = 2.0f;
                }
                else if (monsterInfo.state == MonsterInfo::State::STATE_RUNNING)
                {
                    monsterInfo.animationState = MonsterInfo::State::STATE_RUNNING;
                    monsterInfo.animationStateDuration_s = 0.25f;
                    monsterInfo.currentJumpDuration_s = 0.25f;
                    monsterInfo.currentJumpHeight = 4.0f;
                }
            }
            else if (monsterInfo.state == MonsterInfo::State::STATE_CELEBRATING &&
                monsterInfo.animationState != MonsterInfo::State::STATE_CELEBRATING)
            {
                monsterInfo.animationState = MonsterInfo::State::STATE_CELEBRATING;
                monsterInfo.animationStateDuration_s = 1.125f;
                monsterInfo.currentJumpDuration_s = 0.375f;
                monsterInfo.currentJumpHeight = 16.0f;
            }

            if (monsterInfo.currentJumpDuration_s > 0.0f)
            {
                std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[s_monsterLit_RenderedMeshIndex_];
                    mesh.SetPositionZ(
                        monsterInfo.currentJumpHeight * std::abs(glm::sin(monsterInfo.animationStateDuration_s * glm::pi<float>() / monsterInfo.currentJumpDuration_s))
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[s_monsterDark_RenderedMeshIndex_];
                    mesh.SetPositionZ(
                        monsterInfo.currentJumpHeight * std::abs(glm::sin(monsterInfo.animationStateDuration_s * glm::pi<float>() / monsterInfo.currentJumpDuration_s))
                    );

                    if (monsterInfo.animationState == MonsterInfo::State::STATE_CELEBRATING)
                    {
                        mesh.SetMeshDataPtr(sharedDataPtr_->monsterLit_MeshDataPtr);
                        mesh.SetTextureId(sharedDataPtr_->monsterLit_TextureId);
                        mesh.SetColor(1.0f, 0.6f, 0.6f, 1.0f);
                    }
                    else
                    {
                        mesh.SetMeshDataPtr(sharedDataPtr_->unknownDark_MeshDataPtr);
                        mesh.SetTextureId(sharedDataPtr_->unknownDark_TextureId);
                        mesh.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
                    }
                }
            }

            monsterInfo.animationStateDuration_s -= timestep_s;
            if (monsterInfo.animationStateDuration_s < 0.0f)
            {
                monsterInfo.animationStateDuration_s = 0.0f;
            }
        }
    }
}

void Scene002::SyncCursorRenderedModel()
{
    Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_EntityId_));
    if (collisionBody2DPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        Project001::CollisionPoint2D collisionPoint01 = collisionPoints[s_cursorPosition_CollisionPointIndex_];
        Project001::CollisionPoint2D collisionPoint02 = collisionPoints[s_cursorPress_CollisionPointIndex_];
        Project001::CollisionPoint2D collisionPoint03 = collisionPoints[s_cursorRelease_CollisionPointIndex_];

        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_EntityId_));
        if (renderedModelPtr != nullptr)
        {
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            Project001::RenderedMesh& circleMesh01 = renderedMeshes[s_cursorPosition_RenderedMeshIndex_];
            Project001::RenderedMesh& circleMesh02 = renderedMeshes[s_cursorPress_RenderedMeshIndex_];
            Project001::RenderedMesh& circleMesh03 = renderedMeshes[s_cursorRelease_RenderedMeshIndex_];

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

void Scene002::SyncBaseRenderedModels()
{
    BaseInfo* baseInfoPtrs = nullptr;
    size_t baseInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<BaseInfo>(baseInfoPtrs, baseInfoCount);
    for (size_t i = 0; i < baseInfoCount; ++i)
    {
        BaseInfo& baseInfo = baseInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<BaseInfo>(entityId, &baseInfo);

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

            unsigned int& light_EntityId = baseInfo.light_EntityId;

            Project001::CollisionBody2D* lightCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(lightCollisionBodyPtr, light_EntityId));
            if (lightCollisionBodyPtr != nullptr)
            {
                lightCollisionBodyPtr->SetPosition(collisionBodyPtr->GetPosition());

                Project001::RenderedModel* lightRenderedModelPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(lightRenderedModelPtr, light_EntityId));
                if (lightRenderedModelPtr != nullptr)
                {
                    lightRenderedModelPtr->SetPositionX(lightCollisionBodyPtr->GetPosition().x);
                    lightRenderedModelPtr->SetPositionY(lightCollisionBodyPtr->GetPosition().y);
                    lightRenderedModelPtr->ResetOrientation();
                    lightRenderedModelPtr->AddRelativeRotationZ(lightCollisionBodyPtr->GetRotation());
                    lightRenderedModelPtr->SetVisible(lightCollisionBodyPtr->GetEnabled());
                }
            }
        }
    }
}

void Scene002::SyncHouseRenderedModels()
{
    HouseInfo* houseInfoPtrs = nullptr;
    size_t houseInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<HouseInfo>(houseInfoPtrs, houseInfoCount);
    for (size_t i = 0; i < houseInfoCount; ++i)
    {
        HouseInfo& houseInfo = houseInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<HouseInfo>(entityId, &houseInfo);

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

            unsigned int& light_EntityId = houseInfo.light_EntityId;

            Project001::CollisionBody2D* lightCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(lightCollisionBodyPtr, light_EntityId));
            if (lightCollisionBodyPtr != nullptr)
            {
                lightCollisionBodyPtr->SetPosition(collisionBodyPtr->GetPosition());

                Project001::RenderedModel* lightRenderedModelPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(lightRenderedModelPtr, light_EntityId));
                if (lightRenderedModelPtr != nullptr)
                {
                    lightRenderedModelPtr->SetPositionX(lightCollisionBodyPtr->GetPosition().x);
                    lightRenderedModelPtr->SetPositionY(lightCollisionBodyPtr->GetPosition().y);
                    lightRenderedModelPtr->ResetOrientation();
                    lightRenderedModelPtr->AddRelativeRotationZ(lightCollisionBodyPtr->GetRotation());
                    lightRenderedModelPtr->SetVisible(lightCollisionBodyPtr->GetEnabled());
                }
            }
        }
    }
}

void Scene002::SyncPersonRenderedModels()
{
    PersonInfo* personInfoPtrs = nullptr;
    size_t personInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<PersonInfo>(personInfoPtrs, personInfoCount);
    for (size_t i = 0; i < personInfoCount; ++i)
    {
        PersonInfo& personInfo = personInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<PersonInfo>(entityId, &personInfo);

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
}

void Scene002::SyncPlayerRenderedModels()
{
    PlayerInfo* playerInfoPtrs = nullptr;
    size_t playerInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<PlayerInfo>(playerInfoPtrs, playerInfoCount);
    for (size_t i = 0; i < playerInfoCount; ++i)
    {
        PlayerInfo& playerInfo = playerInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<PlayerInfo>(entityId, &playerInfo);

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

            unsigned int& light_EntityId = playerInfo.light_EntityId;

            Project001::CollisionBody2D* lightCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(lightCollisionBodyPtr, light_EntityId));
            if (lightCollisionBodyPtr != nullptr)
            {
                lightCollisionBodyPtr->SetPosition(collisionBodyPtr->GetPosition());

                Project001::RenderedModel* lightRenderedModelPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(lightRenderedModelPtr, light_EntityId));
                if (lightRenderedModelPtr != nullptr)
                {
                    lightRenderedModelPtr->SetPositionX(lightCollisionBodyPtr->GetPosition().x);
                    lightRenderedModelPtr->SetPositionY(lightCollisionBodyPtr->GetPosition().y);
                    lightRenderedModelPtr->ResetOrientation();
                    lightRenderedModelPtr->AddRelativeRotationZ(lightCollisionBodyPtr->GetRotation());
                    lightRenderedModelPtr->SetVisible(lightCollisionBodyPtr->GetEnabled());
                }
            }
        }
    }
}

void Scene002::SyncMonsterRenderedModels()
{
    MonsterInfo* monsterInfoPtrs = nullptr;
    size_t monsterInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<MonsterInfo>(monsterInfoPtrs, monsterInfoCount);
    for (size_t i = 0; i < monsterInfoCount; ++i)
    {
        MonsterInfo& monsterInfo = monsterInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<MonsterInfo>(entityId, &monsterInfo);

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

            unsigned int& vision_EntityId = monsterInfo.vision_EntityId;

            Project001::CollisionBody2D* visionCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(visionCollisionBodyPtr, vision_EntityId));
            if (visionCollisionBodyPtr != nullptr)
            {
                visionCollisionBodyPtr->SetPosition(collisionBodyPtr->GetPosition());

                Project001::RenderedModel* visionRenderedModelPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(visionRenderedModelPtr, vision_EntityId));
                if (visionRenderedModelPtr != nullptr)
                {
                    visionRenderedModelPtr->SetPositionX(visionCollisionBodyPtr->GetPosition().x);
                    visionRenderedModelPtr->SetPositionY(visionCollisionBodyPtr->GetPosition().y);
                    visionRenderedModelPtr->ResetOrientation();
                    visionRenderedModelPtr->AddRelativeRotationZ(visionCollisionBodyPtr->GetRotation());
                    visionRenderedModelPtr->SetVisible(visionCollisionBodyPtr->GetEnabled());
                }
            }
        }
    }
}
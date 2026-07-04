// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-07-03

#include "Scene002.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/LightSource.h"
#include "Components/RenderedModel.h"
#include "Utilities/FontUtility.h"
#include "Utilities/MathUtility.h"
#include "Utilities/MeshUtility.h"
#include "ActorInfo.h"
#include "CollisionSystem2D.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "SoundPlayer.h"
#include "Window.h"

#include <stack>



// public ----------------------------------------------------------------------

Scene002::Scene002(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , mainCamera_lookAtPoint_(0.0f, 0.0f, 0.0f)
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

    randomNumberEngine_.seed(sharedDataPtr_->randomNumberSeed);

    CreateMainCameraEntities();
    CreateUiCameraEntity();
    CreateUiTextEntity();
    CreateUiPauseTextEntity();

    if (s_cursorEnabled) CreateCursorEntity();

    CreateStageEntity();
    CreateStageLightEntity();

    if (sharedDataPtr_->playerInfos[0].turnedOn)
    {
        CreatePenguinEntity(player_entityIds_[0], sharedDataPtr_->playerInfos[0], glm::vec2(-128.0f, 0.0f), glm::pi<float>());
    }
    if (sharedDataPtr_->playerInfos[1].turnedOn)
    {
        CreatePenguinEntity(player_entityIds_[1], sharedDataPtr_->playerInfos[1], glm::vec2(-96.0f, 0.0f), glm::pi<float>());
    }
    if (sharedDataPtr_->playerInfos[2].turnedOn)
    {
        CreatePenguinEntity(player_entityIds_[2], sharedDataPtr_->playerInfos[2], glm::vec2(-64.0f, 0.0f), glm::pi<float>());
    }
    if (sharedDataPtr_->playerInfos[3].turnedOn)
    {
        CreatePenguinEntity(player_entityIds_[3], sharedDataPtr_->playerInfos[3], glm::vec2(-32.0f, 0.0f), glm::pi<float>());
    }

    CreateSharkEntity(stageShark_entityId_, glm::vec2(0.0f, SharedApplicationData::s_stageSharkCircleOffset_size), -glm::half_pi<float>());
    // CreateSharkEntity(stageShark_entityId_, glm::vec2(0.0f, 0.0f), 0.0f);

    // unsigned int snowball_entityId = static_cast<unsigned int>(-1);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(0.0f, 0.0f), glm::vec2(-sharedDataPtr_->penguin_throwSpeed_s, 0.0f), 8.0f);

    // CreateSnowballEntity(snowball_entityId, glm::vec2(-128.0f, -128.0f), 8.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(-96.0f, -96.0f), 16.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(-64.0f, -64.0f), 24.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(-32.0f, -32.0f), 32.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(32.0f, 32.0f), 40.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(64.0f, 64.0f), 48.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(96.0f, 96.0f), 56.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(128.0f, 128.0f), 64.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(-128.0f, 128.0f), 8.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(-96.0f, 128.0f), 8.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(-64.0f, 128.0f), 8.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(-32.0f, 128.0f), 8.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(0.0f, 128.0f), 8.0f);
    // CreateSnowballEntity(snowball_entityId, glm::vec2(32.0f, 128.0f), 8.0f);
}

void Scene002::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    LOG_INFO("DEINITIALIZING: Scene002:            " << GetId());

    // -------------------------------------------------------------------------

    GetComponentStoresPtr()->DeleteAllEntities();

    // Entity Ids --------------------------------------------------------------

    mainCamera_entityId_ = static_cast<unsigned int>(-1);
    mainCameraDebug_entityId_ = static_cast<unsigned int>(-1);

    uiCamera_entityId_ = static_cast<unsigned int>(-1);

    uiText_entityId_ = static_cast<unsigned int>(-1);
    uiPauseText_entityId_ = static_cast<unsigned int>(-1);

    cursor_entityId_ = static_cast<unsigned int>(-1);

    stage_entityId_ = static_cast<unsigned int>(-1);
    stageLight_entityId_ = static_cast<unsigned int>(-1);

    stageShark_entityId_ = static_cast<unsigned int>(-1);

    for (size_t i = 0; i < SharedApplicationData::s_player_count; ++i)
    {
        player_entityIds_[i] = static_cast<unsigned int>(-1);
    }

    // -------------------------------------------------------------------------

    mainCamera_lookAtPoint_ = glm::vec3(0.0f, 0.0f, 0.0f);
    mainCamera_distanceAway_ = s_mainCamera_initialDistanceAway_;
    mainCamera_lockedToPlayers_ = true;
    debugCamera_turnedOn_ = false;

    paused_ = false;
}

void Scene002::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    sharedDataPtr_->UpdateKeyboardButtonPresses(keyEvent);
}

void Scene002::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    sharedDataPtr_->UpdateMouseButtonPresses(mouseButtonEvent);
}

void Scene002::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    GetRenderSystemPtr()->Render();
}

void Scene002::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;
    constexpr float speedConstant = 20.0f;
    mainCamera_distanceAway_ += yOffset * speedConstant;
}

void Scene002::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    sharedDataPtr_->UpdateButtonPressCounts(GetWindowPtr());

    UpdateUiTextEntity();
    UpdateUiPauseTextEntity();

    unsigned long long timestep_ns = updateEvent.timestep_ns;
    float timestep_s = static_cast<float>(timestep_ns) / 1e9f;

    constexpr size_t physicsStepsPerUpdate = 1;
    float physicsTimestep_s = timestep_s / static_cast<float>(physicsStepsPerUpdate);
    for (size_t i = 0; i < physicsStepsPerUpdate; ++i)
    {
        GetCollisionSystemPtr()->CalculateCollisionsWithQuadTree();

        UpdateMainCameraEntity(physicsTimestep_s);

        if (s_cursorEnabled) UpdateCursorEntity(physicsTimestep_s);

        UpdateStageEntity(physicsTimestep_s);

        // Update snowballs before penguins because penguins can spawn snowballs
        UpdateSnowballEntities(physicsTimestep_s);

        if (sharedDataPtr_->playerInfos[0].turnedOn)
        {
            UpdatePenguinEntity(player_entityIds_[0], sharedDataPtr_->playerInfos[0], physicsTimestep_s);
        }
        if (sharedDataPtr_->playerInfos[1].turnedOn)
        {
            UpdatePenguinEntity(player_entityIds_[1], sharedDataPtr_->playerInfos[1], physicsTimestep_s);
        }
        if (sharedDataPtr_->playerInfos[2].turnedOn)
        {
            UpdatePenguinEntity(player_entityIds_[2], sharedDataPtr_->playerInfos[2], physicsTimestep_s);
        }
        if (sharedDataPtr_->playerInfos[3].turnedOn)
        {
            UpdatePenguinEntity(player_entityIds_[3], sharedDataPtr_->playerInfos[3], physicsTimestep_s);
        }

        UpdateSharkEntity(stageShark_entityId_, physicsTimestep_s);

        UpdateWorld(physicsTimestep_s);

        GetCollisionSystemPtr()->ApplyMovement(physicsTimestep_s);
    }

    if (s_cursorEnabled) AnimateCursorEntity(timestep_s);

    AnimatePenguinEntities(timestep_s);
    AnimateSharkEntities(timestep_s);
    AnimateSnowballEntities(timestep_s);

    // Sync rendered models
    // -------------------------------------------------------------------------

    if (s_cursorEnabled) SyncCursorRenderedModels();

    SyncPenguinRenderedModels();
    SyncSharkRenderedModels();
    SyncSnowballRenderedModels();
}

void Scene002::CreateMainCameraEntities()
{
    int aspectRatioNumerator;
    int aspectRatioDenominator;
    GetWindowPtr()->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);

    float aspectRatio = static_cast<float>(aspectRatioNumerator) / static_cast<float>(aspectRatioDenominator);
    constexpr float mainCameraHalfHeight = 320.0f;
    float mainCameraHalfWidth = aspectRatio * mainCameraHalfHeight;

    constexpr float mainCameraNearCutoff = mainCameraHalfHeight * 0.1f;
    constexpr float mainCameraFarCutoff = mainCameraHalfHeight * 24.0f;

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

                cameraPtr->FollowFocalPoint(glm::vec3(0.0f, 0.0f, 0.0f), mainCamera_distanceAway_);
            }
        };

    CreateMainCamera(mainCamera_entityId_, s_mainCamera_cameraMask_, 0);
    CreateMainCamera(mainCameraDebug_entityId_, s_mainCameraDebug_cameraMask_, 100);

    {
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraDebug_entityId_));
        if (cameraPtr != nullptr)
        {
            cameraPtr->SetTurnedOn(debugCamera_turnedOn_);
        }
    };
}

void Scene002::CreateUiCameraEntity()
{
    GetComponentStoresPtr()->CreateEntity(uiCamera_entityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::Camera>(uiCamera_entityId_));
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, uiCamera_entityId_));
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
        cameraPtr->SetDepthTestEnabled(false);
        cameraPtr->SetCameraMask(s_uiCamera_cameraMask_);
        cameraPtr->SetPriorityValue(1000);
    }
}

void Scene002::CreateUiTextEntity()
{
    // TODO:
}

void Scene002::CreateUiPauseTextEntity()
{
    // TODO:
}

void Scene002::CreateCursorEntity()
{
    GetComponentStoresPtr()->CreateEntity(cursor_entityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<CursorInfo>(cursor_entityId_));
    CursorInfo* cursorInfoPtr = nullptr;
    GetComponentStoresPtr()->GetComponent<CursorInfo>(cursorInfoPtr, cursor_entityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(cursor_entityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_entityId_));

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(cursor_entityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, cursor_entityId_));
    if (cursorInfoPtr != nullptr && renderedModelPtr != nullptr && collisionBodyPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_cameraMask_ | s_uiCamera_cameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(CursorInfo::s_renderedMeshCount);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_position_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursor_marker_meshDataPtr);
            mesh.SetPositionZ(0.61f);
            mesh.SetColor(0.2f, 0.8f, 0.8f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_press_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursor_marker_meshDataPtr);
            mesh.SetPositionZ(0.62f);
            mesh.SetColor(0.8f, 0.2f, 0.8f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_release_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursor_marker_meshDataPtr);
            mesh.SetPositionZ(0.61f);
            mesh.SetColor(0.8f, 0.8f, 0.2f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_handBase_renderedMeshIndex];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_handOpen_renderedMeshIndex];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursorHandOpen_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->cursor_textureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
            mesh.SetParentMeshIndex(CursorInfo::s_handBase_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_handPointer_renderedMeshIndex];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursorHandPointer_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->cursor_textureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
            mesh.SetParentMeshIndex(CursorInfo::s_handBase_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_handGrab_renderedMeshIndex];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursorHandGrab_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->cursor_textureId);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetVisible(false);
            mesh.SetParentMeshIndex(CursorInfo::s_handBase_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_grabZone_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(sharedDataPtr_->dotted_1_1_textureId);
            mesh.SetMeshDataPtr(sharedDataPtr_->player_grabZone_meshDataPtr);
            mesh.SetPositionZ(0.6f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetParentMeshIndex(CursorInfo::s_position_renderedMeshIndex);
            mesh.SetRenderPriorityOverride(2);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_aimRay1_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursor_aimRay1_meshDataPtr);
            mesh.SetPositionZ(0.6f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_aimRay2_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(sharedDataPtr_->dotted_1_3_textureId);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursor_aimRay2_meshDataPtr);
            mesh.SetPositionZ(0.6f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[CursorInfo::s_aimRay3_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->cursor_aimRay3_meshDataPtr);
            mesh.SetPositionZ(0.6f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
        }

        collisionBodyPtr->SetPhysicsType(Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SENSOR);

        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBodyPtr->GetCollisionPoints();
        collisionPoints.resize(CursorInfo::s_collisionPointCount);

        collisionPoints[CursorInfo::s_position_collisionPointIndex].tag = s_cursorPosition_collisionShapeTag_;

        collisionPoints[CursorInfo::s_press_collisionPointIndex].tag = s_cursorPress_collisionShapeTag_;
        collisionPoints[CursorInfo::s_press_collisionPointIndex].enabled = false;

        collisionPoints[CursorInfo::s_release_collisionPointIndex].tag = s_cursorRelease_collisionShapeTag_;
        collisionPoints[CursorInfo::s_release_collisionPointIndex].enabled = false;

        collisionPoints[CursorInfo::s_onScreen_collisionPointIndex].tag = s_ui_CollisionShapeTag_;

        std::vector<Project001::CollisionRay2D>& collisionRays = collisionBodyPtr->GetCollisionRays();
        collisionRays.resize(CursorInfo::s_collisionRayCount);

        {
            Project001::CollisionRay2D& collisionRay = collisionRays[CursorInfo::s_aimRay_collisionRayIndex];
            collisionRay = Project001::CollisionRay2D(
                glm::vec2(0.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                s_aimRay_collisionShapeTag_
            );
        }

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBodyPtr->GetCollisionCircles();
        collisionCircles.resize(CursorInfo::s_collisionCircleCount);

        {
            Project001::CollisionCircle2D& collisionCircle = collisionCircles[CursorInfo::s_grabZone_collisionCircleIndex];
            collisionCircle = Project001::CollisionCircle2D(
                glm::vec2(0.0f, SharedApplicationData::s_penguin_grabOffset),
                SharedApplicationData::s_penguin_grabRadius,
                s_grab_collisionShapeTag_,
                true,
                Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR
            );
        }
    }
}

void Scene002::CreateStageEntity()
{
    GetCollisionSystemPtr()->ResetCollisionBodyQuadTree2D(
        glm::vec2(-(SharedApplicationData::s_maxStage_size + 8.0f), -(SharedApplicationData::s_maxStage_size + 8.0f)),
        glm::vec2(SharedApplicationData::s_maxStage_size + 8.0f, SharedApplicationData::s_maxStage_size + 8.0f),
        3,
        16
    );

    GetCollisionSystemPtr()->FullyLoadCollisionBodyQuadTree2D();

    GetComponentStoresPtr()->CreateEntity(stage_entityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<StageInfo>(stage_entityId_));
    StageInfo* stageInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<StageInfo>(stageInfoPtr, stage_entityId_));

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(stage_entityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, stage_entityId_));
    if (stageInfoPtr != nullptr && renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_cameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(StageInfo::s_renderedMeshCount);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[StageInfo::s_ground_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->ground_meshDataPtr);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[StageInfo::s_water_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->water_meshDataPtr);
            mesh.SetPositionZ(s_waterHeight);
            mesh.SetColor(0.4f, 0.6f, 1.0f, 0.5f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[StageInfo::s_deadZone_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->deadZone_meshDataPtr);
            mesh.SetColor(0.8f, 0.6f, 0.2f, 1.0f);
            mesh.SetTextureId(sharedDataPtr_->hazard_textureId);
            mesh.SetRenderPriorityOverride(3);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[StageInfo::s_pathPoints_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->pathPoints_meshDataPtr);
            mesh.SetPositionZ(0.3f);
            mesh.SetColor(1.0f, 0.4f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[StageInfo::s_collisionQuadTree_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->stageCollisionQuadTree_meshDataPtr);
            mesh.SetPositionZ(0.3f);
            mesh.SetColor(1.0f, 0.4f, 0.2f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[StageInfo::s_grid_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->stageGrid_meshId, sharedDataPtr_->stageGrid_meshDataPtr->maxBoundingRadius);
            mesh.SetPositionZ(0.2f);
            mesh.SetColor(0.2f, 0.2f, 0.4f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[StageInfo::s_gridLabels_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->stageGridLabels_meshId, sharedDataPtr_->stageGridLabels_meshDataPtr->maxBoundingRadius);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetPositionZ(0.2f);
            mesh.SetColor(0.2f, 0.2f, 0.4f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
        }
    }

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.physicsType = Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(stage_entityId_, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, stage_entityId_));
    if (collisionBodyPtr != nullptr)
    {
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBodyPtr->GetCollisionPoints();
        collisionPoints.resize(StageInfo::s_collisionPointCount);
        for (size_t i = 0; i < StageInfo::s_collisionPointCount; ++i)
        {
            collisionPoints[i].tag = s_path_collisionShapeTag_;
        }

        std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBodyPtr->GetCollisionConvexPolygons();
        collisionConvexPolygons.resize(StageInfo::s_collisionConvexPolygonCount);

        // convex polygon corners will be added durring the update because it resizes
        collisionConvexPolygons[StageInfo::s_ground_collisionConvexPolygonIndex].corners.reserve(8);
        collisionConvexPolygons[StageInfo::s_ground_collisionConvexPolygonIndex].tag = s_ground_collisionShapeTag_;
    }
}

void Scene002::CreateStageLightEntity()
{
    GetComponentStoresPtr()->CreateEntity(stageLight_entityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::LightSource>(stageLight_entityId_));
    Project001::LightSource* lightSourcePtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::LightSource>(lightSourcePtr, stageLight_entityId_));
    if (lightSourcePtr != nullptr)
    {
        lightSourcePtr->SetCameraMask(s_mainCamera_cameraMask_);
        lightSourcePtr->SetPosition(0.0f, 0.0f, 2048.0f);
        lightSourcePtr->SetAmbientColor(0.6f, 0.6f, 0.6f);
        lightSourcePtr->SetDiffuseColor(0.4f, 0.4f, 0.4f);
        lightSourcePtr->TurnOn();
    }
}

void Scene002::CreatePenguinEntity(unsigned int& entityId, PlayerInfo& playerInfo, const glm::vec2& position, float rotation)
{
    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<PenguinInfo>(entityId));
    PenguinInfo* penguinInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PenguinInfo>(penguinInfoPtr, entityId));

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(entityId));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(entityId, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, entityId));
    if (penguinInfoPtr != nullptr && renderedModelPtr != nullptr && collisionBodyPtr != nullptr)
    {
        unsigned int textureId = static_cast<unsigned int>(-1);
        if (playerInfo.playerNumber == 0)
        {
            penguinInfoPtr->glassesType = 0;

            collisionBodyPtr->SetCollisionGroupMask(s_player_collisionGroupMasks_[playerInfo.playerNumber]);

            textureId = sharedDataPtr_->penguin_textureId;
        }
        else if (playerInfo.playerNumber == 1)
        {
            penguinInfoPtr->glassesType = 1;

            collisionBodyPtr->SetCollisionGroupMask(s_player_collisionGroupMasks_[playerInfo.playerNumber]);

            textureId = sharedDataPtr_->penguin_v2_textureId;
        }
        else if (playerInfo.playerNumber == 2)
        {
            penguinInfoPtr->glassesType = 1;

            collisionBodyPtr->SetCollisionGroupMask(s_player_collisionGroupMasks_[playerInfo.playerNumber]);

            textureId = sharedDataPtr_->penguin_v3_textureId;
        }
        else if (playerInfo.playerNumber == 3)
        {
            penguinInfoPtr->glassesType = 1;

            collisionBodyPtr->SetCollisionGroupMask(s_player_collisionGroupMasks_[playerInfo.playerNumber]);

            textureId = sharedDataPtr_->penguin_v4_textureId;
        }

        renderedModelPtr->SetCameraMask(s_mainCameraGroup_cameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(PenguinInfo::s_renderedMeshCount);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_body_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(textureId);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_body_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_body_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_right_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_body_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_left_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_body_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_head_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_body_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_eyes_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_head_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_beak_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_head_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_glasses_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_head_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_shadow_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->circle_meshDataPtr);
            mesh.SetScale(glm::vec3(SharedApplicationData::s_penguin_collisionRadius));
            mesh.SetPositionZ(0.5f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.8f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_grabZone_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(sharedDataPtr_->dotted_1_1_textureId);
            mesh.SetMeshDataPtr(sharedDataPtr_->player_grabZone_meshDataPtr);
            mesh.SetPositionY(SharedApplicationData::s_penguin_grabOffset);
            mesh.SetPositionZ(0.6f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_aimRay1_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            if (playerInfo.playerNumber == 0)
            {
                mesh.SetMeshDataPtr(sharedDataPtr_->player1_aimRay1_meshDataPtr);
            }
            else if (playerInfo.playerNumber == 1)
            {
                mesh.SetMeshDataPtr(sharedDataPtr_->player2_aimRay1_meshDataPtr);

            }
            else if (playerInfo.playerNumber == 2)
            {
                mesh.SetMeshDataPtr(sharedDataPtr_->player3_aimRay1_meshDataPtr);
            }
            else if (playerInfo.playerNumber == 3)
            {
                mesh.SetMeshDataPtr(sharedDataPtr_->player4_aimRay1_meshDataPtr);
            }
            mesh.SetPositionZ(0.6f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_aimRay2_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetTextureId(sharedDataPtr_->dotted_1_3_textureId);
            if (playerInfo.playerNumber == 0)
            {
                mesh.SetMeshDataPtr(sharedDataPtr_->player1_aimRay2_meshDataPtr);
            }
            else if (playerInfo.playerNumber == 1)
            {
                mesh.SetMeshDataPtr(sharedDataPtr_->player2_aimRay2_meshDataPtr);

            }
            else if (playerInfo.playerNumber == 2)
            {
                mesh.SetMeshDataPtr(sharedDataPtr_->player3_aimRay2_meshDataPtr);
            }
            else if (playerInfo.playerNumber == 3)
            {
                mesh.SetMeshDataPtr(sharedDataPtr_->player4_aimRay2_meshDataPtr);
            }
            mesh.SetPositionZ(0.6f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.2f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_orientationArrow_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->orientationArrow_meshDataPtr);
            mesh.SetScale(glm::vec3(SharedApplicationData::s_penguin_collisionRadius));
            mesh.SetPositionZ(0.1f);
            mesh.SetColor(0.2f, 0.6f, 0.6f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_grabAttractorCollision_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->circle_meshDataPtr);
            // mesh.SetScale(glm::vec3(2.0f));
            mesh.SetPositionZ(0.2f);
            mesh.SetColor(0.2f, 0.2f, 0.6f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(-2);
        }

        collisionBodyPtr->SetPosition(position);
        collisionBodyPtr->SetRotation(rotation);

        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBodyPtr->GetCollisionPoints();
        collisionPoints.resize(PenguinInfo::s_collisionPointCount);

        {
            Project001::CollisionPoint2D& collisionPoint = collisionPoints[PenguinInfo::s_grabAttractor_collisionPointIndex];
            collisionPoint = Project001::CollisionPoint2D(
                glm::vec2(0.0f, penguinInfoPtr->grabAttractionRadius),
                s_grabAttractor_collisionShapeTag_
            );
        }

        std::vector<Project001::CollisionRay2D>& collisionRays = collisionBodyPtr->GetCollisionRays();
        collisionRays.resize(PenguinInfo::s_collisionRayCount);

        {
            Project001::CollisionRay2D& collisionRay = collisionRays[PenguinInfo::s_aimRay_collisionRayIndex];
            collisionRay = Project001::CollisionRay2D(
                glm::vec2(0.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                s_aimRay_collisionShapeTag_
            );
        }
        

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBodyPtr->GetCollisionCircles();
        collisionCircles.resize(PenguinInfo::s_collisionCircleCount);

        {
            Project001::CollisionCircle2D& collisionCircle = collisionCircles[PenguinInfo::s_body_collisionCircleIndex];
            collisionCircle = Project001::CollisionCircle2D(
                glm::vec2(0.0f, 0.0f),
                SharedApplicationData::s_penguin_collisionRadius,
                s_player_collisionShapeTag_
            );
        }

        {
            Project001::CollisionCircle2D& collisionCircle = collisionCircles[PenguinInfo::s_grabZone_collisionCircleIndex];
            collisionCircle = Project001::CollisionCircle2D(
                glm::vec2(0.0f, SharedApplicationData::s_penguin_grabOffset),
                SharedApplicationData::s_penguin_grabRadius,
                s_grab_collisionShapeTag_,
                true,
                Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR
            );
        }
    }
}

void Scene002::CreateSharkEntity(unsigned int& entityId, const glm::vec2& position, float rotation)
{
    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<SharkInfo>(entityId));
    SharkInfo* sharkInfoPtr = nullptr;
    GetComponentStoresPtr()->GetComponent<SharkInfo>(sharkInfoPtr, entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(entityId));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(entityId, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, entityId));
    if (sharkInfoPtr != nullptr && renderedModelPtr != nullptr && collisionBodyPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_cameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(SharkInfo::s_renderedMeshCount);

        // renderedModelPtr->SetPositionX(position.x);
        // renderedModelPtr->SetPositionY(position.y);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SharkInfo::s_front_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->shark_front_meshId, sharedDataPtr_->shark_front_meshDataPtr->maxBoundingRadius);
            mesh.SetScale(2.0f, 2.0f, 2.0f);
            mesh.SetTextureId(sharedDataPtr_->shark_textureId);
            // mesh.SetColorAlpha(0.2f);
            // mesh.SetRenderPriorityOverride(2);
            // mesh.SetTranslucent(true);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SharkInfo::s_back_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->shark_back_meshId, sharedDataPtr_->shark_back_meshDataPtr->maxBoundingRadius);
            mesh.SetScale(2.0f, 2.0f, 2.0f);
            mesh.SetTextureId(sharedDataPtr_->shark_textureId);
            mesh.SetParentMeshIndex(SharkInfo::s_front_renderedMeshIndex);
            // mesh.SetColorAlpha(0.2f);
            // mesh.SetRenderPriorityOverride(2);
            // mesh.SetTranslucent(true);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SharkInfo::s_jaw_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->shark_jaw_meshId, sharedDataPtr_->shark_jaw_meshDataPtr->maxBoundingRadius);
            mesh.SetScale(2.0f, 2.0f, 2.0f);
            mesh.SetTextureId(sharedDataPtr_->shark_textureId);
            mesh.SetParentMeshIndex(SharkInfo::s_front_renderedMeshIndex);
            // mesh.SetColorAlpha(0.2f);
            // mesh.SetRenderPriorityOverride(2);
            // mesh.SetTranslucent(true);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SharkInfo::s_frontCollision_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->shark_front_collision_meshDataPtr);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.25f);
            mesh.SetTranslucent(true);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SharkInfo::s_backCollision_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->shark_back_collision_meshDataPtr);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.25f);
            mesh.SetTranslucent(true);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SharkInfo::s_jawCollision_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->shark_jaw_collision_meshDataPtr);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.25f);
            mesh.SetTranslucent(true);
        }

        collisionBodyPtr->SetPosition(position);
        collisionBodyPtr->SetRotation(rotation);

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBodyPtr->GetCollisionCircles();
        collisionCircles.resize(SharkInfo::s_collisionCircleCount);

        {
            Project001::CollisionCircle2D& collisionCircle = collisionCircles[SharkInfo::s_jaw_collisionCircleIndex];
            collisionCircle = Project001::CollisionCircle2D(
                glm::vec2(0.0f, 80.0f),
                30.0f,
                s_sharkJaw_collisionShapeTag_
            );
        }

        std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBodyPtr->GetCollisionRectangles();
        collisionRectangles.resize(SharkInfo::s_collisionRectangleCount);

        {
            Project001::CollisionRectangle2D& collisionRectangle = collisionRectangles[SharkInfo::s_body_collisionRectangleIndex];
            collisionRectangle = Project001::CollisionRectangle2D(
                glm::vec2(-30.0f, 14.0f), glm::vec2(30.0f, 82.0f),
                s_sharkBody_collisionShapeTag_
            );
        }

        std::vector<Project001::CollisionTriangle2D>& collisionTriangles = collisionBodyPtr->GetCollisionTriangles();
        collisionTriangles.resize(SharkInfo::s_collisionTriangleCount);

        {
            Project001::CollisionTriangle2D& collisionTriangle = collisionTriangles[SharkInfo::s_body_collisionTriangleIndex];
            collisionTriangle = Project001::CollisionTriangle2D(
                glm::vec2(30.0f, 14.0f),
                glm::vec2(-30.0f, 14.0f),
                glm::vec2(0.0f, -130.0f),
                s_sharkBody_collisionShapeTag_
            );
        }
    }
}

void Scene002::CreateSnowballEntity(unsigned int& entityId, const glm::vec2& position, const glm::vec2& velocity, float radius)
{
    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<SnowballInfo>(entityId));
    SnowballInfo* snowballInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, entityId));

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(entityId));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::CollisionBody2D>(entityId, collisionBody2DCreationInfo));
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, entityId));
    if (snowballInfoPtr != nullptr && renderedModelPtr != nullptr && collisionBodyPtr != nullptr)
    {
        snowballInfoPtr->radius = radius;

        // static bool exploding = true;
        // if (exploding)
        // {
        //     snowballInfoPtr->state = SnowballInfo::State::STATE_EXPLODING;
        // }
        // exploding = !exploding;

        renderedModelPtr->SetCameraMask(s_mainCameraGroup_cameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(SnowballInfo::s_renderedMeshCount);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->snowball_meshId, sharedDataPtr_->snowball_meshDataPtr->maxBoundingRadius);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_01_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->snowball_meshId, sharedDataPtr_->snowball_meshDataPtr->maxBoundingRadius);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_02_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->snowball_meshId, sharedDataPtr_->snowball_meshDataPtr->maxBoundingRadius);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_03_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->snowball_meshId, sharedDataPtr_->snowball_meshDataPtr->maxBoundingRadius);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_04_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->snowball_meshId, sharedDataPtr_->snowball_meshDataPtr->maxBoundingRadius);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_05_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->snowball_meshId, sharedDataPtr_->snowball_meshDataPtr->maxBoundingRadius);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_06_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->snowball_meshId, sharedDataPtr_->snowball_meshDataPtr->maxBoundingRadius);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
            mesh.SetVisible(false);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_shadow_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->circle_meshDataPtr);
            mesh.SetPositionZ(0.5f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.8f);
            mesh.SetTranslucent(true);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_orientationArrow_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->orientationArrow_meshDataPtr);
            mesh.SetPositionZ(0.1f);
            mesh.SetColor(0.2f, 0.6f, 0.2f, 0.4f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(-4);
            mesh.SetUseLighting(false);
        }

        collisionBodyPtr->SetPosition(position);
        collisionBodyPtr->SetVelocity(velocity);

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBodyPtr->GetCollisionCircles();
        collisionCircles.resize(SnowballInfo::s_collisionCircleCount);

        {
            Project001::CollisionCircle2D& collisionCircle = collisionCircles[SnowballInfo::s_snowball_collisionCircleIndex];
            collisionCircle = Project001::CollisionCircle2D(
                glm::vec2(0.0f, 0.0f),
                snowballInfoPtr->radius,
                s_snowball_collisionShapeTag_
            );
        }
    }
}

void Scene002::UpdateCursorPositionUsingWindowCoordinates(unsigned int entityId, float xPosition, float yPosition)
{
    int windowWidth, windowHeight;
    GetWindowPtr()->GetWindowSize(windowWidth, windowHeight);

    glm::vec2 viewportNormalizedCursorPosition =
        GetRendererPtr()->ConvertPointFromWindowToViewportNormalized(glm::vec2(xPosition, yPosition), static_cast<float>(windowHeight));

    Project001::Camera* mainCameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(mainCameraPtr, mainCamera_entityId_));
    if (mainCameraPtr != nullptr)
    {
        glm::vec3 worldCursorPosition;
        glm::vec3 worldCursorNormal;
        bool cursorRayIntersected = mainCameraPtr->RaycastPointFromNormalizedViewportToPane(
            viewportNormalizedCursorPosition,
            glm::vec3(0.0f, 0.0f, 1.0f),
            0.0f,
            worldCursorPosition,
            worldCursorNormal);

        if (cursorRayIntersected)
        {
            CursorInfo* cursorInfoPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<CursorInfo>(cursorInfoPtr, entityId));
            Project001::CollisionBody2D* cursorCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(cursorCollisionBodyPtr, entityId));
            if (cursorInfoPtr != nullptr && cursorCollisionBodyPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& cursorCollisionPoints = cursorCollisionBodyPtr->GetCollisionPoints();
                glm::vec2& cursorPosition = cursorCollisionPoints[CursorInfo::s_position_collisionPointIndex].position;
                cursorPosition = worldCursorPosition;

                std::vector<Project001::CollisionCircle2D>& collisionCircles = cursorCollisionBodyPtr->GetCollisionCircles();
                Project001::CollisionCircle2D& collisionCircle = collisionCircles[CursorInfo::s_grabZone_collisionCircleIndex];
                collisionCircle.position = worldCursorPosition;

                if (cursorInfoPtr->snowball_entityId != static_cast<unsigned int>(-1))
                {
                    Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
                    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, cursorInfoPtr->snowball_entityId));
                    if (snowballCollisionBodyPtr != nullptr)
                    {
                        const glm::vec2& snowballPosition = snowballCollisionBodyPtr->GetPosition();

                        glm::vec2 aimDirection = snowballPosition - cursorPosition;
                        float aimMagnitude = glm::length(aimDirection);
                        if (aimMagnitude > 0.0f)
                        {
                            std::vector<Project001::CollisionRay2D>& cursorCollisionRays = cursorCollisionBodyPtr->GetCollisionRays();
                            cursorCollisionRays[CursorInfo::s_aimRay_collisionRayIndex].position = cursorPosition;
                            cursorCollisionRays[CursorInfo::s_aimRay_collisionRayIndex].direction = aimDirection / aimMagnitude;
                        }
                    }
                }
            }
        }
    }

    Project001::Camera* uiCameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(uiCameraPtr, uiCamera_entityId_));
    if (uiCameraPtr != nullptr)
    {
        glm::vec3 worldCursorPosition;
        glm::vec3 worldCursorNormal;
        bool cursorRayIntersected = uiCameraPtr->RaycastPointFromNormalizedViewportToPane(
            viewportNormalizedCursorPosition,
            glm::vec3(0.0f, 0.0f, 1.0f),
            0.0f,
            worldCursorPosition,
            worldCursorNormal);

        if (cursorRayIntersected)
        {
            Project001::CollisionBody2D* collisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, entityId));
            if (collisionBodyPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBodyPtr->GetCollisionPoints();

                collisionPoints[CursorInfo::s_onScreen_collisionPointIndex].position = worldCursorPosition;
            }
        }
    }
}

void Scene002::UpdateMainCameraEntity(float timestep_s)
{
    if (sharedDataPtr_->debug_keyboard_toggleDebugCamera_pressCount == 1)
    {
        debugCamera_turnedOn_ = !debugCamera_turnedOn_;
    }

    if (sharedDataPtr_->debug_keyboard_toggleCameraLock_pressCount == 1)
    {
        mainCamera_lockedToPlayers_ = !mainCamera_lockedToPlayers_;
    }

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_entityId_));
    if (cameraPtr != nullptr)
    {
        constexpr float orbitSpeed = 0.1f * glm::pi<float>();
        float cameraPitchDelta = orbitSpeed * timestep_s;

        bool pitchingUp = sharedDataPtr_->debug_keyboard_pitchCameraDown_pressCount > 0;
        bool pitchingDown = sharedDataPtr_->debug_keyboard_pitchCameraUp_pressCount > 0;

        const float moveSpeed = 128.0f;
        float moveSpeedDelta = moveSpeed * timestep_s;

        bool movingLeft = sharedDataPtr_->debug_keyboard_moveCameraLeft_pressCount > 0;
        bool movingRight = sharedDataPtr_->debug_keyboard_moveCameraRight_pressCount > 0;
        bool movingUp = sharedDataPtr_->debug_keyboard_moveCameraUp_pressCount > 0;
        bool movingDown = sharedDataPtr_->debug_keyboard_moveCameraDown_pressCount > 0;

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
            mainCamera_lookAtPoint_.x -= moveSpeedDelta;
        }

        if (movingRight)
        {
            mainCamera_lookAtPoint_.x += moveSpeedDelta;
        }

        if (movingUp)
        {
            mainCamera_lookAtPoint_.y += moveSpeedDelta;
        }

        if (movingDown)
        {
            mainCamera_lookAtPoint_.y -= moveSpeedDelta;
        }

        const float& cameraFieldOfView = cameraPtr->GetFieldOfView();
        float cameraFieldOfViewRemainder = glm::mod(cameraFieldOfView, glm::pi<float>());
        if (mainCamera_lockedToPlayers_ && sharedDataPtr_->s_player_count > 0 && cameraFieldOfViewRemainder != 0.0f)
        {
            glm::vec2 maxPlayerPosition(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
            glm::vec2 minPlayerPosition(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
            for (size_t i = 0; i < sharedDataPtr_->s_player_count; ++i)
            {
                if (sharedDataPtr_->playerInfos[i].turnedOn)
                {
                    Project001::CollisionBody2D* playerCollisionBodyPtr = nullptr;
                    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(playerCollisionBodyPtr, player_entityIds_[i]));
                    if (playerCollisionBodyPtr != nullptr)
                    {
                        const glm::vec2& playerPosition = playerCollisionBodyPtr->GetPosition();

                        if (playerPosition.x > maxPlayerPosition.x) maxPlayerPosition.x = playerPosition.x;
                        if (playerPosition.x < minPlayerPosition.x) minPlayerPosition.x = playerPosition.x;
                        if (playerPosition.y > maxPlayerPosition.y) maxPlayerPosition.y = playerPosition.y;
                        if (playerPosition.y < minPlayerPosition.y) minPlayerPosition.y = playerPosition.y;
                    }
                }
            }

            // glm::vec2 centerPlayerPosition = (maxPlayerPosition + minPlayerPosition) * 0.5f;

            glm::vec3 cameraForward = cameraPtr->GetForwardVector();
            float currentCameraAngle = Project001::Math::Get3DVectorAngle(glm::vec3(0.0f, 0.0f, -1.0f), cameraForward);
            currentCameraAngle = glm::mod(currentCameraAngle, glm::half_pi<float>());
            float cameraMaxPositionInclusionRatio = 1.0f - currentCameraAngle / glm::half_pi<float>();
            glm::vec2 centerPlayerPosition = (glm::vec2(maxPlayerPosition.x, maxPlayerPosition.y * cameraMaxPositionInclusionRatio) + minPlayerPosition) * 0.5f;

            mainCamera_lookAtPoint_.x = centerPlayerPosition.x;
            mainCamera_lookAtPoint_.y = centerPlayerPosition.y;
            mainCamera_lookAtPoint_.z = 0.0f;

            float playerPositionBoundingRaidus = glm::length(centerPlayerPosition - minPlayerPosition);

            glm::vec3 forwardVector = cameraPtr->GetForwardVector();
            glm::vec3 forwardHorizontalVector = forwardVector;
            forwardHorizontalVector.z = 0.0f;

            // Calculate mainCamera_distanceAway_ with law of sines to include all players
            // 
            //                                  . <- Camera Point
            //                                //
            //                              / /
            //                            /  /
            //                          /   /
            //                        /    /
            //                      /     /
            //                    /      /
            // Player Center -> /_______/
            //                      ^
            //                      Player Spread
            // 
            //                                  . <- angleA
            //                                //
            //                              / /
            //                            /  /
            //                 sideC -> /   /
            //                        /    / <- sideB
            //                      /     /
            //                    /      /
            //        angleB -> /_______/ <- angleC
            //                      ^
            //                      sideA
            // 
            // sideA / sin(angleA) = sideB / sin(angleB) = sideC / sin(angleC)
            // sideA / sin(angleA) = sideC / sin(angleC)
            // sideC = sideA * sin(angleC) / sin(angleA)

            float angleA = cameraFieldOfView * 0.5f;
            float angleB = Project001::Math::Get3DVectorAngle(forwardHorizontalVector, forwardVector);
            float angleC = glm::pi<float>() - angleA - angleB;

            float sideA = playerPositionBoundingRaidus + s_mainCamera_playerToEdgeSpacing_;
            float sideC = sideA * sin(angleC) / sin(angleA);

            mainCamera_distanceAway_ = sideC;

            if (mainCamera_distanceAway_ < s_mainCamera_minimumDistanceAway_)
            {
                mainCamera_distanceAway_ = s_mainCamera_minimumDistanceAway_;
            }
        }

        if (sharedDataPtr_->debug_keyboard_setCameraPitch1_pressCount == 1)
        {
            cameraPtr->ResetOrientation();

            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->AddPitch(-glm::quarter_pi<float>());
        }
        else if (sharedDataPtr_->debug_keyboard_setCameraPitch2_pressCount == 1)
        {
            cameraPtr->ResetOrientation();

            cameraPtr->AddYaw(glm::pi<float>());
        }

        glm::vec3 offsetLookAtPoint = mainCamera_lookAtPoint_;
        // offsetLookAtPoint.y -= mainCamera_distanceAway_ * 0.1f;
        cameraPtr->FollowFocalPoint(offsetLookAtPoint, mainCamera_distanceAway_);

        Project001::Camera* otherCameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(otherCameraPtr, mainCameraDebug_entityId_));
        if (otherCameraPtr != nullptr)
        {
            otherCameraPtr->SetTurnedOn(debugCamera_turnedOn_);
            otherCameraPtr->SetPosition(cameraPtr->GetPosition());
            otherCameraPtr->SetOrientation(cameraPtr->GetOrientation());
        }
    }
}

void Scene002::UpdateUiTextEntity()
{
    // TODO:
}

void Scene002::UpdateUiPauseTextEntity()
{
    // TODO:
}

void Scene002::UpdateCursorEntity(float timestep_s)
{
    glm::vec2 cursorPosition;
    GetWindowPtr()->GetCursorPosition(cursorPosition.x, cursorPosition.y);
    UpdateCursorPositionUsingWindowCoordinates(cursor_entityId_, cursorPosition.x, cursorPosition.y);

    // Creating new components will cause old pointers from the ECS to become,
    // invalid. Flags will be used to preform these actions at the end.
    bool actionCreateSnowballEntity = false;

    CursorInfo* cursorInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<CursorInfo>(cursorInfoPtr, cursor_entityId_));
    Project001::CollisionBody2D* cursorCollisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(cursorCollisionBodyPtr, cursor_entityId_));
    if (cursorInfoPtr != nullptr && cursorCollisionBodyPtr != nullptr)
    {
        // Analyzing collisions
        // ---------------------------------------------------------------------

        unsigned int grabable_entityId = static_cast<unsigned int>(-1);
        float grabable_entityDistance = std::numeric_limits<float>::infinity();
        bool snowballSpawnPointOnLand = false;
        bool hoveringOverAlreadyGrabbedEntity = false;

        const std::vector<Project001::CollisionOverlapData2D>& cursorCollisionOverlaps = cursorCollisionBodyPtr->GetCollisionOverlaps();
        for (size_t i = 0; i < cursorCollisionOverlaps.size(); ++i)
        {
            const Project001::CollisionOverlapData2D& cursorCollisionOverlapData = cursorCollisionOverlaps[i];

            if (cursorCollisionOverlapData.myShapeTag == s_cursorPosition_collisionShapeTag_ &&
                cursorCollisionOverlapData.otherShapeTag == s_ground_collisionShapeTag_)
            {
                snowballSpawnPointOnLand = true;
            }

            if (cursorCollisionOverlapData.myShapeTag == s_grab_collisionShapeTag_ &&
                cursorInfoPtr->snowball_entityId == cursorCollisionOverlapData.otherEntityId)
            {
                hoveringOverAlreadyGrabbedEntity = true;
            }

            if (cursorInfoPtr->snowball_entityId == static_cast<unsigned int>(-1) &&
                cursorCollisionOverlapData.myShapeTag == s_grab_collisionShapeTag_ &&
                cursorCollisionOverlapData.otherShapeTag == s_snowball_collisionShapeTag_)
            {
                SnowballInfo* snowballInfoPtr = nullptr;
                GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, cursorCollisionOverlapData.otherEntityId);
                Project001::CollisionBody2D* otherCollisionBodyPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(otherCollisionBodyPtr, cursorCollisionOverlapData.otherEntityId));
                if (snowballInfoPtr != nullptr && otherCollisionBodyPtr != nullptr &&
                    snowballInfoPtr->owner_entityId == static_cast<unsigned int>(-1))
                {
                    float otherRadius = 0.0f;

                    std::vector<Project001::CollisionCircle2D>& collisionCircles = otherCollisionBodyPtr->GetCollisionCircles();
                    const Project001::CollisionCircle2D& collisionCircle2D = collisionCircles[SnowballInfo::s_snowball_collisionCircleIndex];
                    otherRadius = collisionCircle2D.radius;

                    const glm::vec2& cursorPosition = cursorCollisionBodyPtr->GetPosition();
                    const glm::vec2& otherPosition = otherCollisionBodyPtr->GetPosition();

                    float distance = glm::length(otherPosition - cursorPosition) - otherRadius;
                    if (distance < grabable_entityDistance)
                    {
                        grabable_entityId = cursorCollisionOverlapData.otherEntityId;
                        grabable_entityDistance = distance;
                    }
                }
            }
        }

        // static unsigned int prevGrabable_entityId = static_cast<unsigned int>(-1);
        // if (prevGrabable_entityId != grabable_entityId)
        // {
        //     if (grabable_entityId != static_cast<unsigned int>(-1))
        //     {
        //         LOG_INFO("grabable_entityId = " << grabable_entityId);
        //     }
        //     prevGrabable_entityId = grabable_entityId;
        // }

        // static bool prevSnowballSpawnPointOnLand = false;
        // if (prevSnowballSpawnPointOnLand != snowballSpawnPointOnLand)
        // {
        //     LOG_INFO("snowballSpawnPointOnLand = " << snowballSpawnPointOnLand);
        //     prevSnowballSpawnPointOnLand = snowballSpawnPointOnLand;
        // }

        float impulseMagntidueSum = 0.0f;

        if (cursorInfoPtr->snowball_entityId != static_cast<unsigned int>(-1))
        {
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, cursorInfoPtr->snowball_entityId));
            if (snowballCollisionBodyPtr != nullptr)
            {
                std::vector<Project001::CollisionImpulseData2D>& snowballCollisionImpulses = snowballCollisionBodyPtr->GetCollisionImpulses();
                for (size_t i = 0; i < snowballCollisionImpulses.size(); ++i)
                {
                    Project001::CollisionImpulseData2D& snowballCollisionImpulseData = snowballCollisionImpulses[i];
                    impulseMagntidueSum += glm::length(snowballCollisionImpulseData.impulse);
                }
            }
        }

        bool hitHard = false;
        if (impulseMagntidueSum > 128.0f)
        {
            hitHard = true;
        }

        // Update aim ray
        // ---------------------------------------------------------------------

        const std::vector<Project001::CollisionRaycastData2D>& cursorCollisionRaycasts = cursorCollisionBodyPtr->GetCollisionRaycasts();

        float pullIntersectionScalar = std::numeric_limits<float>::infinity();
        for (size_t i = 0; i < cursorCollisionRaycasts.size(); ++i)
        {
            const Project001::CollisionRaycastData2D& cursorCollisionRaycastData = cursorCollisionRaycasts[i];

            if (cursorCollisionRaycastData.myShapeTag == s_aimRay_collisionShapeTag_)
            {
                if (cursorCollisionRaycastData.otherShapeTag == s_snowball_collisionShapeTag_ &&
                    cursorCollisionRaycastData.otherEntityId == cursorInfoPtr->snowball_entityId)
                {
                    pullIntersectionScalar = cursorCollisionRaycastData.intersectionScalar;
                }
            }
        }

        float minAimIntersectionScalar = std::numeric_limits<float>::infinity();
        for (size_t i = 0; i < cursorCollisionRaycasts.size(); ++i)
        {
            const Project001::CollisionRaycastData2D& cursorCollisionRaycastData = cursorCollisionRaycasts[i];

            if (cursorCollisionRaycastData.myShapeTag == s_aimRay_collisionShapeTag_)
            {
                if ((cursorCollisionRaycastData.otherShapeTag == s_player_collisionShapeTag_ ||
                    (cursorCollisionRaycastData.otherShapeTag == s_snowball_collisionShapeTag_ && cursorCollisionRaycastData.otherEntityId != cursorInfoPtr->snowball_entityId)) &&
                    cursorCollisionRaycastData.intersectionScalar < minAimIntersectionScalar &&
                    cursorCollisionRaycastData.intersectionScalar > pullIntersectionScalar)
                {
                    minAimIntersectionScalar = cursorCollisionRaycastData.intersectionScalar;
                }
            }
        }

        // LOG_INFO("pullIntersectionScalar = " << pullIntersectionScalar);
        // LOG_INFO("minAimIntersectionScalar = " << minAimIntersectionScalar);

        sharedDataPtr_->cursor_aimRay1_meshDataPtr->Clear();
        sharedDataPtr_->cursor_aimRay2_meshDataPtr->Clear();
        sharedDataPtr_->cursor_aimRay3_meshDataPtr->Clear();

        if (pullIntersectionScalar > 0.0f && !hoveringOverAlreadyGrabbedEntity)
        {
            const std::vector<Project001::CollisionRay2D>& cursorCollisionRays = cursorCollisionBodyPtr->GetCollisionRays();
            const Project001::CollisionRay2D& aimRay = cursorCollisionRays[CursorInfo::s_aimRay_collisionRayIndex];

            if (minAimIntersectionScalar > SharedApplicationData::s_maxAimLineLength)
            {
                FAIL_CHECK(Project001::Mesh::Generate2DLine(
                    *sharedDataPtr_->cursor_aimRay1_meshDataPtr,
                    aimRay.position + aimRay.direction * pullIntersectionScalar,
                    aimRay.position + aimRay.direction * SharedApplicationData::s_maxAimLineLength,
                    SharedApplicationData::s_aimLineWidth
                ));
            }
            else
            {
                FAIL_CHECK(Project001::Mesh::Generate2DLine(
                    *sharedDataPtr_->cursor_aimRay1_meshDataPtr,
                    aimRay.position + aimRay.direction * pullIntersectionScalar,
                    aimRay.position + aimRay.direction * minAimIntersectionScalar,
                    SharedApplicationData::s_aimLineWidth
                ));

                FAIL_CHECK(Project001::Mesh::Generate2DLine(
                    *sharedDataPtr_->cursor_aimRay2_meshDataPtr,
                    aimRay.position + aimRay.direction * minAimIntersectionScalar,
                    aimRay.position + aimRay.direction * (minAimIntersectionScalar + SharedApplicationData::s_maxAimLineLength),
                    SharedApplicationData::s_aimLineWidth
                ));
                Project001::Mesh::RotateTextureCoordinates(
                    *sharedDataPtr_->cursor_aimRay2_meshDataPtr,
                    -glm::half_pi<float>()
                );
                Project001::Mesh::ScaleTextureCoordinates(
                    *sharedDataPtr_->cursor_aimRay2_meshDataPtr,
                    glm::vec2(1.0f, SharedApplicationData::s_maxAimLineLength * 0.125f)
                );
            }

            FAIL_CHECK(Project001::Mesh::Generate2DLine(
                *sharedDataPtr_->cursor_aimRay3_meshDataPtr,
                aimRay.position,
                aimRay.position + aimRay.direction * pullIntersectionScalar,
                SharedApplicationData::s_aimLineWidth * 2.0f
            ));

            // TODO: Update aim ray meshes
        }

        // Gathering input
        // ---------------------------------------------------------------------

        const bool grabPressed = sharedDataPtr_->debug_mouse_grab_pressCount == 1;
        const bool grabHeld = sharedDataPtr_->debug_mouse_grab_pressCount > 0;
        const bool releasePressed = sharedDataPtr_->debug_mouse_grab_releaseCount == 1;

        if (grabPressed)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_entityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[CursorInfo::s_press_collisionPointIndex].position = collisionPoints[CursorInfo::s_position_collisionPointIndex].position;
                collisionPoints[CursorInfo::s_press_collisionPointIndex].enabled = true;
                collisionPoints[CursorInfo::s_release_collisionPointIndex].enabled = false;
            }
        }
        else if (releasePressed)
        {
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, cursor_entityId_));
            if (collisionBody2DPtr != nullptr)
            {
                std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
                collisionPoints[CursorInfo::s_release_collisionPointIndex].position = collisionPoints[CursorInfo::s_position_collisionPointIndex].position;
                collisionPoints[CursorInfo::s_release_collisionPointIndex].enabled = true;
            }
        }

        // Applying physics
        // ---------------------------------------------------------------------

        if (cursorInfoPtr->snowball_entityId != static_cast<unsigned int>(-1))
        {
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, cursorInfoPtr->snowball_entityId));
            if (snowballCollisionBodyPtr != nullptr)
            {
                constexpr float friction = 256.0f;

                const glm::vec2& velocity = snowballCollisionBodyPtr->GetVelocity();
                float velocityMagnitude = glm::length(velocity);
                if (velocityMagnitude > 0.0f)
                {
                    glm::vec2 velocityDirection = velocity / velocityMagnitude;

                    float decelerationStep = friction * timestep_s;
                    float decelerationStepMagnitude = std::abs(decelerationStep);
                    if (velocityMagnitude < decelerationStepMagnitude)
                    {
                        snowballCollisionBodyPtr->SetVelocity(glm::vec2(0.0f, 0.0f));
                    }
                    else
                    {
                        snowballCollisionBodyPtr->SetVelocity(velocity - velocityDirection * decelerationStep);
                    }
                }

                constexpr float angularFriction = glm::pi<float>();

                const float& angularVelocity = snowballCollisionBodyPtr->GetAngularVelocity();
                float angularVelocityMagnitude = glm::abs(angularVelocity);
                if (angularVelocityMagnitude > 0.0f)
                {
                    float angularVelocityDirection = glm::sign(angularVelocity);

                    float angularDecelerationStep = angularFriction * timestep_s;
                    float angularDecelerationStepMagnitude = glm::abs(angularDecelerationStep);
                    if (angularVelocityMagnitude < angularDecelerationStepMagnitude)
                    {
                        snowballCollisionBodyPtr->SetAngularVelocity(0.0f);
                    }
                    else
                    {
                        snowballCollisionBodyPtr->SetAngularVelocity(angularVelocity - angularVelocityDirection * angularDecelerationStep);
                    }
                }
            }
        }

        // Changing state
        // ---------------------------------------------------------------------

        const bool grabableInReach = grabable_entityId != static_cast<unsigned int>(-1);

        enum class SnowballAction
        {
            SNOWBALL_ACTION_NONE = 0,
            SNOWBALL_ACTION_GRAB,
            SNOWBALL_ACTION_SPAWN,
            SNOWBALL_ACTION_DROP,
            SNOWBALL_ACTION_THROW
        };
        SnowballAction snowballAciton = SnowballAction::SNOWBALL_ACTION_NONE;

        switch (cursorInfoPtr->state)
        {
        case CursorInfo::State::STATE_OPEN:
        {
            if (grabableInReach)
            {
                if (grabPressed)
                {
                    snowballAciton = SnowballAction::SNOWBALL_ACTION_GRAB;
                    cursorInfoPtr->state = CursorInfo::State::STATE_GRABING;
                }
                else
                {
                    cursorInfoPtr->state = CursorInfo::State::STATE_POINTING;
                }
            }
            else if (snowballSpawnPointOnLand)
            {
                if (grabPressed)
                {
                    cursorInfoPtr->makeSnowballCountDown_s = CursorInfo::s_makeSnowballTime_s;
                    cursorInfoPtr->state = CursorInfo::State::STATE_MAKING;
                }
            }
            break;
        }
        case CursorInfo::State::STATE_POINTING:
        {
            if (grabableInReach)
            {
                if (grabPressed)
                {
                    snowballAciton = SnowballAction::SNOWBALL_ACTION_GRAB;
                    cursorInfoPtr->state = CursorInfo::State::STATE_GRABING;
                }
            }
            else // !grabableInReach
            {
                cursorInfoPtr->state = CursorInfo::State::STATE_OPEN;
            }
            break;
        }
        case CursorInfo::State::STATE_MAKING:
        {
            if (grabHeld) // && snowballSpawnPointOnLand)
            {
                cursorInfoPtr->makeSnowballCountDown_s -= timestep_s;
                if (cursorInfoPtr->makeSnowballCountDown_s <= 0.0f)
                {
                    snowballAciton = SnowballAction::SNOWBALL_ACTION_SPAWN;
                    cursorInfoPtr->state = CursorInfo::State::STATE_GRABING;
                }
            }
            else // !grabHeld
            {
                cursorInfoPtr->state = CursorInfo::State::STATE_OPEN;
            }
            break;
        }
        case CursorInfo::State::STATE_GRABING:
        {
            if (!grabHeld)
            {
                if (hoveringOverAlreadyGrabbedEntity)
                {
                    snowballAciton = SnowballAction::SNOWBALL_ACTION_DROP;
                    cursorInfoPtr->state = CursorInfo::State::STATE_POINTING;
                }
                else
                {
                    snowballAciton = SnowballAction::SNOWBALL_ACTION_THROW;
                    cursorInfoPtr->state = CursorInfo::State::STATE_OPEN;
                }
            }
            break;
        }
        }

        // static SnowballAction previousSnowballAction = SnowballAction::SNOWBALL_ACTION_NONE;
        // if (previousSnowballAction != snowballAciton)
        // {
        //     LOG_INFO("snowballAciton = " << static_cast<size_t>(snowballAciton));
        //     previousSnowballAction = snowballAciton;
        // }

        // Perform snowball actions
        // ---------------------------------------------------------------------

        switch (snowballAciton)
        {
        case SnowballAction::SNOWBALL_ACTION_NONE:
        {
            break;
        }
        case SnowballAction::SNOWBALL_ACTION_GRAB:
        {
            SnowballInfo* snowballInfoPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, grabable_entityId);
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, grabable_entityId);
            if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
            {
                cursorInfoPtr->snowball_entityId = grabable_entityId;
                snowballInfoPtr->owner_entityId = cursor_entityId_;
            }
            break;
        }
        case SnowballAction::SNOWBALL_ACTION_SPAWN:
        {
            actionCreateSnowballEntity = true;
            break;
        }
        case SnowballAction::SNOWBALL_ACTION_DROP:
        {
            SnowballInfo* snowballInfoPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, cursorInfoPtr->snowball_entityId);
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, cursorInfoPtr->snowball_entityId);
            if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
            {
                cursorInfoPtr->snowball_entityId = static_cast<unsigned int>(-1);
                snowballInfoPtr->owner_entityId = static_cast<unsigned int>(-1);
            }
            break;
        }
        case SnowballAction::SNOWBALL_ACTION_THROW:
        {
            SnowballInfo* snowballInfoPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, cursorInfoPtr->snowball_entityId);
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, cursorInfoPtr->snowball_entityId);
            if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
            {
                cursorInfoPtr->snowball_entityId = static_cast<unsigned int>(-1);
                snowballInfoPtr->owner_entityId = static_cast<unsigned int>(-1);

                std::vector<Project001::CollisionRay2D>& cursorCollisionRays = cursorCollisionBodyPtr->GetCollisionRays();
                Project001::CollisionRay2D& collisionRay = cursorCollisionRays[CursorInfo::s_aimRay_collisionRayIndex];

                snowballCollisionBodyPtr->SetVelocity(
                    snowballCollisionBodyPtr->GetVelocity() +
                    collisionRay.direction * SharedApplicationData::s_penguin_throwSpeed_s
                );
            }
            break;
        }
        }
    }

    if (actionCreateSnowballEntity)
    {
        CreateSnowballEntity(
            cursorInfoPtr->snowball_entityId,
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            SnowballInfo::s_initialSnowballRadius
        );

        Project001::CollisionBody2D* cursorCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(cursorCollisionBodyPtr, cursor_entityId_));
        SnowballInfo* snowballInfoPtr = nullptr;
        GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, cursorInfoPtr->snowball_entityId);
        Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
        GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, cursorInfoPtr->snowball_entityId);
        if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
        {
            snowballInfoPtr->owner_entityId = cursor_entityId_;

            std::vector<Project001::CollisionPoint2D>& cursorCollisionPoints = cursorCollisionBodyPtr->GetCollisionPoints();
            Project001::CollisionPoint2D& cursorPress = cursorCollisionPoints[CursorInfo::s_press_collisionPointIndex];
            snowballCollisionBodyPtr->SetPosition(cursorPress.position);

            cursorCollisionBodyPtr->FlagMassToBeRecalculated();
        }
    }
}

void Scene002::UpdateStageEntity(float timestep_s)
{
    StageInfo* stageInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<StageInfo>(stageInfoPtr, stage_entityId_));
    Project001::CollisionBody2D* collisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, stage_entityId_));
    if (stageInfoPtr != nullptr && collisionBodyPtr != nullptr)
    {
        stageInfoPtr->groundSize -= stageInfoPtr->groundSkrinkRate_s * timestep_s;
        if (stageInfoPtr->groundSize < 0.0f)
        {
            stageInfoPtr->groundSize = 0.0f;
        }

        // Update Ground Rendered Mesh

        sharedDataPtr_->ground_meshDataPtr->Clear();

        if (stageInfoPtr->groundSize > 0.0f)
        {
            constexpr float height = 256.0f;
            const float groundRadius = stageInfoPtr->groundSize * 1.0829f;
            const float groundCorner = stageInfoPtr->groundSize * 0.41421357f; // sqrt(2) - 1

            FAIL_CHECK(Project001::Mesh::GenerateCylinder(
                *sharedDataPtr_->ground_meshDataPtr, height, groundRadius, 8, false
            ));
            Project001::Mesh::RotateMeshX(*sharedDataPtr_->ground_meshDataPtr, glm::half_pi<float>());
            Project001::Mesh::RotateMeshZ(*sharedDataPtr_->ground_meshDataPtr, glm::pi<float>() / 8.0f);
            Project001::Mesh::TranslateMesh(*sharedDataPtr_->ground_meshDataPtr, glm::vec3(0.0f, 0.0f, -0.5f * height));
        }

        // Update Ground Collision

        std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBodyPtr->GetCollisionConvexPolygons();
        std::vector<glm::vec2>& corners = collisionConvexPolygons[StageInfo::s_ground_collisionConvexPolygonIndex].corners;
        corners.clear();

        if (stageInfoPtr->groundSize > 0.0f)
        {
            const float groundCorner = stageInfoPtr->groundSize * 0.41421357f; // sqrt(2) - 1

            corners.emplace_back(stageInfoPtr->groundSize, groundCorner);
            corners.emplace_back(groundCorner, stageInfoPtr->groundSize);
            corners.emplace_back(-groundCorner, stageInfoPtr->groundSize);
            corners.emplace_back(-stageInfoPtr->groundSize, groundCorner);
            corners.emplace_back(-stageInfoPtr->groundSize, -groundCorner);
            corners.emplace_back(-groundCorner, -stageInfoPtr->groundSize);
            corners.emplace_back(groundCorner, -stageInfoPtr->groundSize);
            corners.emplace_back(stageInfoPtr->groundSize, -groundCorner);
        }

        // Update Path Points Mesh & Collision

        sharedDataPtr_->pathPoints_meshDataPtr->Clear();

        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBodyPtr->GetCollisionPoints();

        glm::vec2 pathOffsetVector(0.0f, stageInfoPtr->groundSize + SharedApplicationData::s_stageSharkCircleOffset_size);
        pathOffsetVector = Project001::Math::Rotate2DVector(pathOffsetVector, glm::pi<float>() / 8.0f);

        for (size_t i = 0; i < 8; ++i)
        {
            FAIL_CHECK(Project001::Mesh::Generate2DRegularPolygon(*sharedDataPtr_->pathPoints_meshDataPtr, 8.0f, 4, pathOffsetVector));
            pathOffsetVector = Project001::Math::Rotate2DVector(pathOffsetVector, glm::pi<float>() / -4.0f);
        }
    }

    UpdateStageCollisionBodyQuadTreeMesh();
}

void Scene002::UpdateStageCollisionBodyQuadTreeMesh()
{
    const Project001::CollisionBodyQuadTreeNode2D* rootNodePtr = GetCollisionSystemPtr()->GetCollisionBodyQuadTree2D().GetRootNode();

    const float lineWidth = 4.0f;

    sharedDataPtr_->stageCollisionQuadTree_meshDataPtr->Clear();

    Project001::Mesh::Generate2DRectangleFrame(*sharedDataPtr_->stageCollisionQuadTree_meshDataPtr, rootNodePtr->min, rootNodePtr->max, lineWidth);

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

                Project001::Mesh::Generate2DLine(*sharedDataPtr_->stageCollisionQuadTree_meshDataPtr, top, bottom, lineWidth);
                Project001::Mesh::Generate2DLine(*sharedDataPtr_->stageCollisionQuadTree_meshDataPtr, left, right, lineWidth);

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

void Scene002::UpdatePenguinEntity(unsigned int& entityId, PlayerInfo& playerInfo, float timestep_s)
{
    // Creating new components will cause old pointers from the ECS to become,
    // invalid. Flags will be used to preform these actions at the end.
    bool actionCreateSnowballEntity = false;

    PenguinInfo* penguinInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PenguinInfo>(penguinInfoPtr, entityId));
    Project001::CollisionBody2D* penguinCollisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(penguinCollisionBodyPtr, entityId));
    if (penguinInfoPtr != nullptr && penguinCollisionBodyPtr != nullptr)
    {
        // Analyzing collisions
        // ---------------------------------------------------------------------

        penguinInfoPtr->onLand = false;
        unsigned int grabable_entityId = static_cast<unsigned int>(-1);
        float grabable_entityDistance = std::numeric_limits<float>::infinity();
        bool snowballSpawnPointOnLand = false;

        const std::vector<Project001::CollisionOverlapData2D>& penguinCollisionOverlaps = penguinCollisionBodyPtr->GetCollisionOverlaps();
        for (size_t i = 0; i < penguinCollisionOverlaps.size(); ++i)
        {
            const Project001::CollisionOverlapData2D& penguinCollisionOverlapData = penguinCollisionOverlaps[i];

            if (penguinCollisionOverlapData.myShapeTag == s_player_collisionShapeTag_ &&
                penguinCollisionOverlapData.otherShapeTag == s_ground_collisionShapeTag_)
            {
                penguinInfoPtr->onLand = true;
            }

            if (penguinCollisionOverlapData.myShapeTag == s_grabAttractor_collisionShapeTag_ &&
                penguinCollisionOverlapData.otherShapeTag == s_ground_collisionShapeTag_)
            {
                snowballSpawnPointOnLand = true;
            }

            if (penguinInfoPtr->snowball_entityId == static_cast<unsigned int>(-1) &&
                penguinCollisionOverlapData.myShapeTag == s_grab_collisionShapeTag_ &&
                penguinCollisionOverlapData.otherShapeTag == s_snowball_collisionShapeTag_)
            {
                SnowballInfo* snowballInfoPtr = nullptr;
                GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinCollisionOverlapData.otherEntityId);
                Project001::CollisionBody2D* otherCollisionBodyPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(otherCollisionBodyPtr, penguinCollisionOverlapData.otherEntityId));
                if (snowballInfoPtr != nullptr && otherCollisionBodyPtr != nullptr &&
                    snowballInfoPtr->owner_entityId == static_cast<unsigned int>(-1) && snowballInfoPtr->onLand)
                {
                    float otherRadius = 0.0f;

                    std::vector<Project001::CollisionCircle2D>& collisionCircles = otherCollisionBodyPtr->GetCollisionCircles();
                    const Project001::CollisionCircle2D& collisionCircle2D = collisionCircles[SnowballInfo::s_snowball_collisionCircleIndex];
                    otherRadius = collisionCircle2D.radius;

                    const glm::vec2& penguinPosition = penguinCollisionBodyPtr->GetPosition();
                    const glm::vec2& otherPosition = otherCollisionBodyPtr->GetPosition();

                    float distance = glm::length(otherPosition - penguinPosition) - otherRadius;
                    if (distance < grabable_entityDistance)
                    {
                        grabable_entityId = penguinCollisionOverlapData.otherEntityId;
                        grabable_entityDistance = distance;
                    }
                }
            }
        }

        float impulseMagntidueSum = 0.0f;

        const std::vector<Project001::CollisionImpulseData2D>& penguinCollisionImpulses = penguinCollisionBodyPtr->GetCollisionImpulses();
        for (size_t i = 0; i < penguinCollisionImpulses.size(); ++i)
        {
            const Project001::CollisionImpulseData2D& penguinCollisionImpulseData = penguinCollisionImpulses[i];

            impulseMagntidueSum += glm::length(penguinCollisionImpulseData.impulse);
        }

        if (penguinInfoPtr->snowball_entityId != static_cast<unsigned int>(-1))
        {
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_entityId));
            if (snowballCollisionBodyPtr != nullptr)
            {
                std::vector<Project001::CollisionImpulseData2D>& snowballCollisionImpulses = snowballCollisionBodyPtr->GetCollisionImpulses();
                for (size_t i = 0; i < snowballCollisionImpulses.size(); ++i)
                {
                    Project001::CollisionImpulseData2D& snowballCollisionImpulseData = snowballCollisionImpulses[i];

                    impulseMagntidueSum += glm::length(snowballCollisionImpulseData.impulse);

                    const float& penguinMass = penguinCollisionBodyPtr->GetMass();
                    const float& snowballMass = snowballCollisionBodyPtr->GetMass();
                    float snowballMassRatio = snowballMass / (penguinMass + snowballMass);

                    snowballCollisionImpulseData.impulse *= snowballMassRatio;
                    snowballCollisionImpulseData.angularImpulse *= snowballMassRatio;
                }

                // Adjust the impulses felt by the penguin if it is holding a
                // snowball to prevent the penguin from being pushed around too
                // easily.
                std::vector<Project001::CollisionImpulseData2D>& penguinCollisionImpulses = penguinCollisionBodyPtr->GetCollisionImpulses();
                for (size_t i = 0; i < penguinCollisionImpulses.size(); ++i)
                {
                    Project001::CollisionImpulseData2D& penguinCollisionImpulseData = penguinCollisionImpulses[i];

                    const float& penguinMass = penguinCollisionBodyPtr->GetMass();
                    const float& snowballMass = snowballCollisionBodyPtr->GetMass();
                    float penguinMassRatio = penguinMass / (penguinMass + snowballMass);

                    penguinCollisionImpulseData.impulse *= penguinMassRatio;
                    penguinCollisionImpulseData.angularImpulse *= penguinMassRatio;
                }
            }
        }

        // if (impulseMagntidueSum > 0.0f)
        // {
        //     LOG_INFO(entityId << " impulse: " << impulseMagntidueSum);
        // }

        bool hitHard = false;
        if (impulseMagntidueSum > 128.0f)
        {
            hitHard = true;
        }

        // if (snowballSpawnPointOnLand)
        // {
        //     Project001::RenderedModel* renderedModelPtr = nullptr;
        //     FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
        //     if (renderedModelPtr != nullptr)
        //     {
        //         std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        //         Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_grabAttractorCollision_renderedMeshIndex];
        //         mesh.SetColor(0.2f, 0.8f, 0.2f, 1.0f);
        //     }
        // }
        // else
        // {
        //     Project001::RenderedModel* renderedModelPtr = nullptr;
        //     FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
        //     if (renderedModelPtr != nullptr)
        //     {
        //         std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        //         Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_grabAttractorCollision_renderedMeshIndex];
        //         mesh.SetColor(0.8f, 0.2f, 0.2f, 1.0f);
        //     }
        // }

        // Update aim ray
        // ---------------------------------------------------------------------

        float minIntersectionScalar = std::numeric_limits<float>::infinity();

        const std::vector<Project001::CollisionRaycastData2D>& penguinCollisionRaycasts = penguinCollisionBodyPtr->GetCollisionRaycasts();
        for (size_t i = 0; i < penguinCollisionRaycasts.size(); ++i)
        {
            const Project001::CollisionRaycastData2D& penguinCollisionRaycastData = penguinCollisionRaycasts[i];

            if (penguinCollisionRaycastData.myShapeTag == s_aimRay_collisionShapeTag_ &&
                (penguinCollisionRaycastData.otherShapeTag == s_player_collisionShapeTag_ ||
                    (penguinCollisionRaycastData.otherShapeTag == s_snowball_collisionShapeTag_ && penguinCollisionRaycastData.otherEntityId != penguinInfoPtr->snowball_entityId)) &&
                penguinCollisionRaycastData.intersectionScalar < minIntersectionScalar)
            {
                minIntersectionScalar = penguinCollisionRaycastData.intersectionScalar;
            }
        }

        Project001::MeshData* aimRay1_MeshDataPtr = nullptr;
        Project001::MeshData* aimRay2_MeshDataPtr = nullptr;
        if (playerInfo.playerNumber == 0)
        {
            aimRay1_MeshDataPtr = sharedDataPtr_->player1_aimRay1_meshDataPtr;
            aimRay2_MeshDataPtr = sharedDataPtr_->player1_aimRay2_meshDataPtr;
        }
        else if (playerInfo.playerNumber == 1)
        {
            aimRay1_MeshDataPtr = sharedDataPtr_->player2_aimRay1_meshDataPtr;
            aimRay2_MeshDataPtr = sharedDataPtr_->player2_aimRay2_meshDataPtr;

        }
        else if (playerInfo.playerNumber == 2)
        {
            aimRay1_MeshDataPtr = sharedDataPtr_->player3_aimRay1_meshDataPtr;
            aimRay2_MeshDataPtr = sharedDataPtr_->player3_aimRay2_meshDataPtr;
        }
        else if (playerInfo.playerNumber == 3)
        {
            aimRay1_MeshDataPtr = sharedDataPtr_->player4_aimRay1_meshDataPtr;
            aimRay2_MeshDataPtr = sharedDataPtr_->player4_aimRay2_meshDataPtr;
        }

        if (aimRay1_MeshDataPtr != nullptr && aimRay2_MeshDataPtr != nullptr)
        {
            aimRay1_MeshDataPtr->Clear();
            aimRay2_MeshDataPtr->Clear();

            if (minIntersectionScalar > SharedApplicationData::s_penguin_grabOffset + SharedApplicationData::s_penguin_grabRadius)
            {
                if (minIntersectionScalar > SharedApplicationData::s_maxAimLineLength)
                {
                    FAIL_CHECK(Project001::Mesh::Generate2DLine(
                        *aimRay1_MeshDataPtr,
                        glm::vec2(0.0f, SharedApplicationData::s_penguin_grabOffset + SharedApplicationData::s_penguin_grabRadius),
                        glm::vec2(0.0f, SharedApplicationData::s_maxAimLineLength),
                        SharedApplicationData::s_aimLineWidth
                    ));
                }
                else
                {
                    FAIL_CHECK(Project001::Mesh::Generate2DLine(
                        *aimRay1_MeshDataPtr,
                        glm::vec2(0.0f, SharedApplicationData::s_penguin_grabOffset + SharedApplicationData::s_penguin_grabRadius),
                        glm::vec2(0.0f, minIntersectionScalar),
                        SharedApplicationData::s_aimLineWidth
                    ));

                    FAIL_CHECK(Project001::Mesh::Generate2DSprite(
                        *aimRay2_MeshDataPtr,
                        glm::vec3(-0.5f * SharedApplicationData::s_aimLineWidth, minIntersectionScalar, 0.0f),
                        glm::vec3(0.5f * SharedApplicationData::s_aimLineWidth, minIntersectionScalar, 0.0f),
                        glm::vec3(0.5f * SharedApplicationData::s_aimLineWidth, SharedApplicationData::s_maxAimLineLength, 0.0f),
                        glm::vec3(-0.5f * SharedApplicationData::s_aimLineWidth, SharedApplicationData::s_maxAimLineLength, 0.0f),
                        0.0f, 1.0f,
                        0.0f, (SharedApplicationData::s_maxAimLineLength - minIntersectionScalar) * 0.125f
                    ));
                }
            }
        }

        // Gathering input
        // ---------------------------------------------------------------------

        const bool& grabPressed = playerInfo.grab_pressCount > 0;
        const bool& throwPressed = playerInfo.throw_pressCount == 1; // TODO

        glm::vec2 moveDirection(0.0f, 0.0);
        if (playerInfo.left_pressCount > 0)
        {
            moveDirection.x -= 1.0f;
        }
        if (playerInfo.right_pressCount > 0)
        {
            moveDirection.x += 1.0f;
        }
        if (playerInfo.up_pressCount > 0)
        {
            moveDirection.y += 1.0f;
        }
        if (playerInfo.down_pressCount > 0)
        {
            moveDirection.y -= 1.0f;
        }
        if (glm::abs(playerInfo.leftRightAxisValue) > playerInfo.axisDeadzone)
        {
            moveDirection.x += playerInfo.leftRightAxisValue;
        }
        if (glm::abs(playerInfo.upDownAxisValue) > playerInfo.axisDeadzone)
        {
            moveDirection.y += playerInfo.upDownAxisValue;
        }

        float moveMagnitude = glm::length(moveDirection);
        if (moveMagnitude > 0.0f)
        {
            moveDirection /= moveMagnitude;
        }

        glm::vec2 penguinCollisionBodyDirection = penguinCollisionBodyPtr->GetForwardVector();

        // Changing state
        // ---------------------------------------------------------------------

        const bool& penguinOnLand = penguinInfoPtr->onLand;
        const bool grabableInReach = grabable_entityId != static_cast<unsigned int>(-1);
        const bool moving = moveMagnitude > 0.0f;

        enum class SnowballAction
        {
            SNOWBALL_ACTION_NONE,
            SNOWBALL_ACTION_GRAB,
            SNOWBALL_ACTION_SPAWN,
            SNOWBALL_ACTION_DROP,
            SNOWBALL_ACTION_THROW
        };
        SnowballAction snowballAciton = SnowballAction::SNOWBALL_ACTION_NONE;

        switch (penguinInfoPtr->state)
        {
        case PenguinInfo::State::STATE_STANDING:
        case PenguinInfo::State::STATE_WALKING:
        case PenguinInfo::State::STATE_TREADING_WATER:
        case PenguinInfo::State::STATE_SWIMMING:
        {
            if (hitHard)
            {
                moveMagnitude = 0.0f;
                penguinInfoPtr->state = PenguinInfo::State::STATE_HITSTUN;
                penguinInfoPtr->hitstunCoolDown_s = impulseMagntidueSum / 512.0f;

                break;
            }

            if (penguinOnLand)
            {
                if (grabPressed)
                {
                    if (grabableInReach && penguinInfoPtr->regrabSnowballCoolDown_s <= 0.0f)
                    {
                        penguinInfoPtr->regrabSnowballCoolDown_s = PenguinInfo::s_regrabSnowballTime_s;
                        snowballAciton = SnowballAction::SNOWBALL_ACTION_GRAB;

                        if (moving)
                        {
                            penguinInfoPtr->state = PenguinInfo::State::STATE_WALKING_SNOWBALL;
                        }
                        else // !moving
                        {
                            penguinInfoPtr->state = PenguinInfo::State::STATE_STANDING_SNOWBALL;
                        }
                    }
                    else if (snowballSpawnPointOnLand) // && !grabableInReach
                    {
                        penguinInfoPtr->makeSnowballCountDown_s = PenguinInfo::s_makeSnowballTime_s;
                        penguinInfoPtr->state = PenguinInfo::State::STATE_MAKING_SNOWBALL;
                    }
                }
                else // !grabPressed
                {
                    if (moving)
                    {
                        penguinInfoPtr->state = PenguinInfo::State::STATE_WALKING;
                    }
                    else // !moving
                    {
                        penguinInfoPtr->state = PenguinInfo::State::STATE_STANDING;
                    }
                }
            }
            else // !penguinOnLand
            {
                if (moving)
                {
                    penguinInfoPtr->state = PenguinInfo::State::STATE_SWIMMING;
                }
                else // !moving
                {
                    penguinInfoPtr->state = PenguinInfo::State::STATE_TREADING_WATER;
                }
            }
            break;
        }
        case PenguinInfo::State::STATE_MAKING_SNOWBALL:
        {
            if (hitHard)
            {
                moveMagnitude = 0.0f;
                penguinInfoPtr->state = PenguinInfo::State::STATE_HITSTUN;
                penguinInfoPtr->hitstunCoolDown_s = impulseMagntidueSum / 512.0f;

                break;
            }

            if (penguinOnLand)
            {
                if (grabPressed)
                {
                    penguinInfoPtr->makeSnowballCountDown_s -= timestep_s;
                    if (penguinInfoPtr->makeSnowballCountDown_s <= 0.0f)
                    {
                        snowballAciton = SnowballAction::SNOWBALL_ACTION_SPAWN;

                        if (moving)
                        {
                            penguinInfoPtr->state = PenguinInfo::State::STATE_WALKING_SNOWBALL;
                        }
                        else // !moving
                        {
                            penguinInfoPtr->state = PenguinInfo::State::STATE_STANDING_SNOWBALL;
                        }
                    }
                    else // penguinInfoPtr->makeSnowballCountDown_s > 0.0f
                    {
                        moveMagnitude = 0.0f;
                    }
                }
                else // !grabPressed
                {
                    if (moving)
                    {
                        penguinInfoPtr->state = PenguinInfo::State::STATE_WALKING;
                    }
                    else // !moving
                    {
                        penguinInfoPtr->state = PenguinInfo::State::STATE_STANDING;
                    }
                }
            }
            else // !penguinOnLand
            {
                if (moving)
                {
                    penguinInfoPtr->state = PenguinInfo::State::STATE_SWIMMING;
                }
                else // !moving
                {
                    penguinInfoPtr->state = PenguinInfo::State::STATE_TREADING_WATER;
                }
            }
            break;
        }
        case PenguinInfo::State::STATE_STANDING_SNOWBALL:
        case PenguinInfo::State::STATE_WALKING_SNOWBALL:
        {
            if (hitHard)
            {
                moveMagnitude = 0.0f;
                penguinInfoPtr->state = PenguinInfo::State::STATE_HITSTUN;
                penguinInfoPtr->hitstunCoolDown_s = impulseMagntidueSum / 512.0f;
        
                penguinInfoPtr->regrabSnowballCoolDown_s = PenguinInfo::s_regrabSnowballTime_s;
                snowballAciton = SnowballAction::SNOWBALL_ACTION_DROP;

                break;
            }
        
            if (penguinOnLand)
            {
                if (grabPressed)
                {
                    bool snowballOnLand = false;
                    SnowballInfo* snowballInfoPtr = nullptr;
                    GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfoPtr->snowball_entityId);
                    if (snowballInfoPtr != nullptr)
                    {
                        snowballOnLand = snowballInfoPtr->onLand;
                    }

                    if (snowballOnLand)
                    {
                        if (moving)
                        {
                            penguinInfoPtr->state = PenguinInfo::State::STATE_WALKING_SNOWBALL;
                        }
                        else // !moving
                        {
                            penguinInfoPtr->state = PenguinInfo::State::STATE_STANDING_SNOWBALL;
                        }
                    }
                    else // !snowballOnLand
                    {
                        penguinInfoPtr->regrabSnowballCoolDown_s = PenguinInfo::s_regrabSnowballTime_s;
                        snowballAciton = SnowballAction::SNOWBALL_ACTION_DROP;
        
                        if (moving)
                        {
                            penguinInfoPtr->state = PenguinInfo::State::STATE_WALKING;
                        }
                        else // !moving
                        {
                            penguinInfoPtr->state = PenguinInfo::State::STATE_STANDING;
                        }
                    }
                }
                else // !grabPressed
                {
                    penguinInfoPtr->regrabSnowballCoolDown_s = PenguinInfo::s_regrabSnowballTime_s;
                    snowballAciton = SnowballAction::SNOWBALL_ACTION_THROW;
                
                    if (moving)
                    {
                        penguinInfoPtr->state = PenguinInfo::State::STATE_WALKING;
                    }
                    else // !moving
                    {
                        penguinInfoPtr->state = PenguinInfo::State::STATE_STANDING;
                    }
                }
            }
            else // !penguinOnLand
            {
                penguinInfoPtr->regrabSnowballCoolDown_s = PenguinInfo::s_regrabSnowballTime_s;
                snowballAciton = SnowballAction::SNOWBALL_ACTION_DROP;
        
                if (moving)
                {
                    penguinInfoPtr->state = PenguinInfo::State::STATE_SWIMMING;
                }
                else // !moving
                {
                    penguinInfoPtr->state = PenguinInfo::State::STATE_TREADING_WATER;
                }
            }
            break;
        }
        case PenguinInfo::State::STATE_HITSTUN:
        {
            if (penguinInfoPtr->hitstunCoolDown_s > 0.0f)
            {
                moveMagnitude = 0.0f;
                penguinInfoPtr->hitstunCoolDown_s -= timestep_s;
            }
            else
            {
                penguinInfoPtr->state = PenguinInfo::State::STATE_STANDING;
            }
            break;
        }
        }

        penguinInfoPtr->regrabSnowballCoolDown_s -= timestep_s;

        // Applying physics
        // ---------------------------------------------------------------------

        float maxSpeed = 128.0f;
        float acceleration = 256.0f;
        float friction = 192.0f;

        float maxAngularSpeed = glm::pi<float>() * 8.0f;
        float angularAcceleration = glm::pi<float>() * 16.0f;
        float angularFriction = glm::pi<float>() * 32.0f;

        if (penguinInfoPtr->snowball_entityId != static_cast<unsigned int>(-1))
        {
            const float& penguinMass = penguinCollisionBodyPtr->GetMass();
            float snowballMass = 0.0f;
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_entityId);
            if (snowballCollisionBodyPtr != nullptr)
            {
                snowballMass = snowballCollisionBodyPtr->GetMass();
            }

            float penguinMassRatio = penguinMass / (penguinMass + 0.25f * snowballMass);

            maxSpeed = 0.8f * maxSpeed * penguinMassRatio;
            acceleration = 0.8f * acceleration * penguinMassRatio;
            friction = 0.8f * friction * penguinMassRatio;

            maxAngularSpeed = 0.4f * maxAngularSpeed * penguinMassRatio * penguinMassRatio;
            angularAcceleration = 0.4f * angularAcceleration * penguinMassRatio * penguinMassRatio;
            angularFriction = angularFriction * penguinMassRatio * penguinMassRatio;
        }

        if (penguinInfoPtr->state == PenguinInfo::State::STATE_HITSTUN)
        {
            angularAcceleration = 0.0f;

            if (penguinInfoPtr->onLand)
            {
                friction = SharedApplicationData::s_snowball_landFriction;
            }
            else
            {
                friction = SharedApplicationData::s_snowball_waterFriction;
            }

            angularAcceleration = 0.0f;
            angularFriction = SharedApplicationData::s_snowball_angularFriction;
        }

        constexpr float turnAngleMovementThreshold = glm::pi<float>() * 0.25f;

        float turnAngle = 0.0f;
        if (moveMagnitude > 0.0f)
        {
            turnAngle = Project001::Math::Get2DVectorAngle(penguinCollisionBodyDirection, moveDirection);
        }
        float turnAngleMagnitude = glm::abs(turnAngle);
        float turnAngleDirection = glm::sign(turnAngle);

        const glm::vec2& velocity = penguinCollisionBodyPtr->GetVelocity();
        float velocityMagnitude = glm::length(velocity);
        glm::vec2 velocityDirection(1.0f, 0.0f);
        if (velocityMagnitude > 0.0f)
        {
            velocityDirection = velocity / velocityMagnitude;
        }

        const float& angularVelocity = penguinCollisionBodyPtr->GetAngularVelocity();
        float angularVelocityMagnitude = glm::abs(angularVelocity);
        float angularVelocityDirection = glm::sign(angularVelocity);

        if (turnAngleDirection == angularVelocityDirection || angularVelocityDirection == 0.0f) // apply angular acceleration
        {
            float targetAngularVelocity = turnAngleDirection * maxAngularSpeed;
            float neededAngularVelocity = targetAngularVelocity - angularVelocity;
            float neededAngularVelocityMagnitude = std::abs(neededAngularVelocity);
            if (neededAngularVelocityMagnitude > 0.0f)
            {
                float neededAngularVelocityDirection = glm::sign(neededAngularVelocity);
                float angularAccelerationStep = neededAngularVelocityDirection * angularAcceleration * timestep_s;
                float angularAccelerationStepMagnitude = glm::abs(angularAccelerationStep);
                if (angularAccelerationStepMagnitude > neededAngularVelocityMagnitude)
                {
                    angularAccelerationStep = neededAngularVelocity; // prevent angular acceleration from overshooting max angular speed
                }

                float angularDifference = turnAngle - angularVelocity * timestep_s;
                float angularDifferenceMagnitude = glm::abs(angularDifference);
                if (angularAccelerationStepMagnitude > angularDifferenceMagnitude)
                {
                    angularAccelerationStep = angularDifference; // prevent angular acceleration from jittering when low
                }

                penguinCollisionBodyPtr->SetAngularVelocity(angularVelocity + angularAccelerationStep);
            }
        }
        else // apply angular deceleration
        {
            float angularVelocityMagnitude = glm::abs(angularVelocity);
            if (angularVelocityMagnitude > 0.0f)
            {
                float angularDecelerationStep = angularFriction * timestep_s;
                float angularDecelerationStepMagnitude = std::abs(angularFriction);
                if (angularVelocityMagnitude < angularDecelerationStepMagnitude)
                {
                    penguinCollisionBodyPtr->SetAngularVelocity(0.0f);
                }
                else
                {
                    penguinCollisionBodyPtr->SetAngularVelocity(angularVelocity - angularVelocityDirection * angularDecelerationStep);
                }
            }
        }

        if (glm::abs(turnAngle) < turnAngleMovementThreshold && moveMagnitude > 0.0f) // apply acceleration
        {
            glm::vec2 targetVelocity = moveDirection * maxSpeed;
            glm::vec2 neededVelocity = targetVelocity - velocity;
            float neededVelocityMagnitude = glm::length(neededVelocity);
            if (neededVelocityMagnitude > 0.0f)
            {
                glm::vec2 neededVelocityDirection = neededVelocity / neededVelocityMagnitude;
                glm::vec2 accelerationStep = neededVelocityDirection * acceleration * timestep_s;
                float accelerationStepMagnitude = glm::length(accelerationStep);
                if (accelerationStepMagnitude > neededVelocityMagnitude)
                {
                    accelerationStep = neededVelocity; // prevent acceleration from overshooting max speed
                }
                penguinCollisionBodyPtr->SetVelocity(velocity + accelerationStep);
            }
        }
        else // apply deceleration
        {
            if (velocityMagnitude > 0.0f)
            {
                float decelerationStep = friction * timestep_s;
                float decelerationStepMagnitude = std::abs(decelerationStep);
                if (velocityMagnitude < decelerationStepMagnitude)
                {
                    penguinCollisionBodyPtr->SetVelocity(glm::vec2(0.0f, 0.0f));
                }
                else
                {
                    penguinCollisionBodyPtr->SetVelocity(velocity - velocityDirection * decelerationStep);
                }
            }
        }

        // Perform snowball actions
        // ---------------------------------------------------------------------

        switch (snowballAciton)
        {
        case SnowballAction::SNOWBALL_ACTION_NONE:
        {
            break;
        }
        case SnowballAction::SNOWBALL_ACTION_GRAB:
        {
            SnowballInfo* snowballInfoPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, grabable_entityId);
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, grabable_entityId);
            if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
            {
                penguinInfoPtr->snowball_entityId = grabable_entityId;
                snowballInfoPtr->owner_entityId = entityId;

                const float& penguinMass = penguinCollisionBodyPtr->GetMass();
                const float& snowballMass = snowballCollisionBodyPtr->GetMass();

                if (penguinMass > 0.0f)
                {
                    float snowballMassRatio = snowballMass / (penguinMass + snowballMass);
                    constexpr float extraSlowdownConstant = 0.8f;

                    penguinCollisionBodyPtr->SetVelocity(
                        snowballCollisionBodyPtr->GetVelocity() * snowballMassRatio * extraSlowdownConstant
                    );
                    penguinCollisionBodyPtr->SetAngularVelocity(
                        -snowballCollisionBodyPtr->GetAngularVelocity() * snowballMassRatio
                    );
                }

                snowballCollisionBodyPtr->SetAllowedCollisionFilterMask(
                    snowballCollisionBodyPtr->GetAllowedCollisionFilterMask() & ~penguinCollisionBodyPtr->GetCollisionGroupMask()
                );
            }
            break;
        }
        case SnowballAction::SNOWBALL_ACTION_SPAWN:
        {
            actionCreateSnowballEntity = true;
            break;
        }
        case SnowballAction::SNOWBALL_ACTION_DROP:
        {
            SnowballInfo* snowballInfoPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfoPtr->snowball_entityId);
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_entityId);
            if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
            {
                penguinInfoPtr->snowball_entityId = static_cast<unsigned int>(-1);
                snowballInfoPtr->owner_entityId = static_cast<unsigned int>(-1);

                snowballCollisionBodyPtr->SetAllowedCollisionFilterMask(
                    snowballCollisionBodyPtr->GetAllowedCollisionFilterMask() | penguinCollisionBodyPtr->GetCollisionGroupMask()
                );
            }
            break;
        }
        case SnowballAction::SNOWBALL_ACTION_THROW:
        {
            SnowballInfo* snowballInfoPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfoPtr->snowball_entityId);
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_entityId);
            if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
            {
                penguinInfoPtr->snowball_entityId = static_cast<unsigned int>(-1);
                snowballInfoPtr->owner_entityId = static_cast<unsigned int>(-1);

                snowballCollisionBodyPtr->SetAllowedCollisionFilterMask(
                    snowballCollisionBodyPtr->GetAllowedCollisionFilterMask() | penguinCollisionBodyPtr->GetCollisionGroupMask()
                );

                if (glm::abs(turnAngle) < turnAngleMovementThreshold && moveMagnitude > 0.0f)
                {
                    snowballCollisionBodyPtr->SetVelocity(
                        snowballCollisionBodyPtr->GetVelocity() +
                        penguinCollisionBodyDirection * SharedApplicationData::s_penguin_throwSpeed_s
                    );
                }
            }
            break;
        }
        }

        SnowballInfo* snowballInfoPtr = nullptr;
        GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfoPtr->snowball_entityId);
        Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
        GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_entityId);
        if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
        {
            const glm::vec2& penguinPosition = penguinCollisionBodyPtr->GetPosition();
            const float& penguinRotation = penguinCollisionBodyPtr->GetRotation();
            const float& penguinRadius = SharedApplicationData::s_penguin_collisionRadius;
            const glm::vec2& snowballPosition = snowballCollisionBodyPtr->GetPosition();
            const float& snowballRadius = snowballInfoPtr->radius;

            penguinInfoPtr->grabAttractionRadius = penguinRadius + snowballRadius;

            // Snowball attraction logic:
            // -----------------------------------------------------------------

            constexpr float penguinRotationSpeed = glm::pi<float>() * 16.0f;
            const float penguinRotationStep = penguinRotationSpeed * timestep_s;

            glm::vec2 penguinToSnowball = snowballPosition - penguinPosition;
            float penguinToSnowballMagnitude = glm::length(penguinToSnowball);
            glm::vec2 penguinToSnowballDirection = penguinToSnowball / penguinToSnowballMagnitude;
            float angleToSnowball = Project001::Math::Get2DVectorAngle(penguinCollisionBodyPtr->GetForwardVector(), penguinToSnowballDirection);

            float angleToSnowballMagnitude = glm::abs(angleToSnowball);
            if (angleToSnowballMagnitude > penguinRotationStep)
            {
                float angleToSnowballSign = glm::sign(angleToSnowball);
                penguinCollisionBodyPtr->AddRotation(angleToSnowballSign * penguinRotationStep);
            }
            penguinCollisionBodyDirection = penguinCollisionBodyPtr->GetForwardVector(); // get this again because rotation was applied

            glm::vec2 attractionPoint = penguinPosition + penguinCollisionBodyDirection * penguinInfoPtr->grabAttractionRadius;
            glm::vec2 snowballToAttractionPointDirection = attractionPoint - snowballPosition;
            float snowballToAttractionPointMagnitude = glm::length(snowballToAttractionPointDirection);

            const float attractionSpeed = 16.0f * snowballToAttractionPointMagnitude;
            const float attractionStep = attractionSpeed * timestep_s;

            if (snowballToAttractionPointMagnitude > attractionStep)
            {
                snowballToAttractionPointDirection /= snowballToAttractionPointMagnitude;
                snowballCollisionBodyPtr->SetPosition(
                    snowballCollisionBodyPtr->GetPosition() + snowballToAttractionPointDirection * attractionStep
                );
            }
            else
            {
                snowballCollisionBodyPtr->SetPosition(attractionPoint);
            }

            snowballCollisionBodyPtr->SetVelocity(
                penguinCollisionBodyPtr->GetVelocity() + penguinInfoPtr->grabAttractionRadius *
                penguinCollisionBodyPtr->GetRightVector() * -penguinCollisionBodyPtr->GetAngularVelocity()
            );

            // -----------------------------------------------------------------

            if (moveMagnitude > 0.0f)
            {
                constexpr float snowballGrowthRate_s = 2.0f;

                snowballInfoPtr->radius += snowballGrowthRate_s * timestep_s;

                if (snowballInfoPtr->radius > SnowballInfo::s_maxRadius)
                {
                    snowballInfoPtr->radius = SnowballInfo::s_maxRadius;
                }

                snowballCollisionBodyPtr->FlagMassToBeRecalculated();
            }
        }
        else
        {
            penguinInfoPtr->grabAttractionRadius = PenguinInfo::s_initialGrabAttractionRadius;
        }
    }

    if (actionCreateSnowballEntity)
    {
        CreateSnowballEntity(
            penguinInfoPtr->snowball_entityId,
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            SnowballInfo::s_initialSnowballRadius
        );

        Project001::CollisionBody2D* penguinCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(penguinCollisionBodyPtr, entityId));
        SnowballInfo* snowballInfoPtr = nullptr;
        GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfoPtr->snowball_entityId);
        Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
        GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_entityId);
        if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
        {
            snowballInfoPtr->owner_entityId = entityId;

            const glm::vec2& penguinPosition = penguinCollisionBodyPtr->GetPosition();
            const float& penguinRadius = SharedApplicationData::s_penguin_collisionRadius;
            const float& snowballRadius = snowballInfoPtr->radius;

            penguinInfoPtr->grabAttractionRadius = penguinRadius + snowballRadius;
            glm::vec2 collisionBodyDirection = penguinCollisionBodyPtr->GetForwardVector();
            glm::vec2 attractionPoint = penguinPosition + collisionBodyDirection * penguinInfoPtr->grabAttractionRadius;
            snowballCollisionBodyPtr->SetPosition(attractionPoint);

            penguinCollisionBodyPtr->FlagMassToBeRecalculated();
        }
    }
}

void Scene002::UpdateSharkEntity(unsigned int& entityId, float timestep_s)
{
    SharkInfo* sharkInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<SharkInfo>(sharkInfoPtr, entityId));
    Project001::CollisionBody2D* sharkCollisionBodyPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(sharkCollisionBodyPtr, entityId));
    if (sharkInfoPtr != nullptr && sharkCollisionBodyPtr != nullptr)
    {
        // Analyzing collisions
        // ---------------------------------------------------------------------

        sharkInfoPtr->onLand = false;

        const std::vector<Project001::CollisionOverlapData2D>& sharkCollisionOverlaps = sharkCollisionBodyPtr->GetCollisionOverlaps();
        for (size_t i = 0; i < sharkCollisionOverlaps.size(); ++i)
        {
            const Project001::CollisionOverlapData2D& sharkCollisionOverlapData = sharkCollisionOverlaps[i];

            if ((sharkCollisionOverlapData.myShapeTag == s_sharkBody_collisionShapeTag_ || sharkCollisionOverlapData.myShapeTag == s_sharkJaw_collisionShapeTag_) &&
                sharkCollisionOverlapData.otherShapeTag == s_ground_collisionShapeTag_)
            {
                sharkInfoPtr->onLand = true;
            }
        }

        float impulseMagntidueSum = 0.0f;

        const std::vector<Project001::CollisionImpulseData2D>& sharkCollisionImpulses = sharkCollisionBodyPtr->GetCollisionImpulses();
        for (size_t i = 0; i < sharkCollisionImpulses.size(); ++i)
        {
            const Project001::CollisionImpulseData2D& sharkCollisionImpulseData = sharkCollisionImpulses[i];

            impulseMagntidueSum += glm::length(sharkCollisionImpulseData.impulse);
        }

        // TODO:
    }
}

void Scene002::UpdateSnowballEntities(float timestep_s)
{
    SnowballInfo* snowballInfoPtrs = nullptr;
    size_t snowballInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<SnowballInfo>(snowballInfoPtrs, snowballInfoCount);
    for (size_t i = 0; i < snowballInfoCount; ++i)
    {
        SnowballInfo& snowballInfo = snowballInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<SnowballInfo>(entityId, &snowballInfo);

        Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, entityId));
        if (snowballCollisionBodyPtr != nullptr)
        {
            snowballInfo.onLand = false;

            const std::vector<Project001::CollisionOverlapData2D>& snowballCollisionOverlaps = snowballCollisionBodyPtr->GetCollisionOverlaps();
            for (size_t i = 0; i < snowballCollisionOverlaps.size(); ++i)
            {
                const Project001::CollisionOverlapData2D& snowballCollisionOverlapData = snowballCollisionOverlaps[i];

                if (snowballCollisionOverlapData.otherShapeTag == s_ground_collisionShapeTag_)
                {
                    snowballInfo.onLand = true;
                }
            }

            float impulseMagntidueSum = 0.0f;

            const std::vector<Project001::CollisionImpulseData2D>& snowballCollisionImpulses = snowballCollisionBodyPtr->GetCollisionImpulses();
            for (size_t i = 0; i < snowballCollisionImpulses.size(); ++i)
            {
                const Project001::CollisionImpulseData2D& snowballCollisionImpulseData = snowballCollisionImpulses[i];

                impulseMagntidueSum += glm::length(snowballCollisionImpulseData.impulse);
            }

            float friction = SharedApplicationData::s_snowball_landFriction;
            if (!snowballInfo.onLand)
            {
                friction = SharedApplicationData::s_snowball_waterFriction;
            }

            const glm::vec2& velocity = snowballCollisionBodyPtr->GetVelocity();
            float velocityMagnitude = glm::length(velocity);
            if (velocityMagnitude > 0.0f)
            {
                glm::vec2 velocityDirection = velocity / velocityMagnitude;

                float decelerationStep = friction * timestep_s;
                float decelerationStepMagnitude = std::abs(decelerationStep);
                if (velocityMagnitude < decelerationStepMagnitude)
                {
                    snowballCollisionBodyPtr->SetVelocity(glm::vec2(0.0f, 0.0f));
                }
                else
                {
                    snowballCollisionBodyPtr->SetVelocity(velocity - velocityDirection * decelerationStep);
                }
            }

            float angularFriction = SharedApplicationData::s_snowball_angularFriction;

            const float& angularVelocity = snowballCollisionBodyPtr->GetAngularVelocity();
            float angularVelocityMagnitude = glm::abs(angularVelocity);
            if (angularVelocityMagnitude > 0.0f)
            {
                float angularVelocityDirection = glm::sign(angularVelocity);

                float angularDecelerationStep = angularFriction * timestep_s;
                float angularDecelerationStepMagnitude = glm::abs(angularDecelerationStep);
                if (angularVelocityMagnitude < angularDecelerationStepMagnitude)
                {
                    snowballCollisionBodyPtr->SetAngularVelocity(0.0f);
                }
                else
                {
                    snowballCollisionBodyPtr->SetAngularVelocity(angularVelocity - angularVelocityDirection * angularDecelerationStep);
                }
            }

            std::vector<Project001::CollisionCircle2D>& collisionCircles = snowballCollisionBodyPtr->GetCollisionCircles();

            {
                Project001::CollisionCircle2D& collisionCircle = collisionCircles[SnowballInfo::s_snowball_collisionCircleIndex];
                collisionCircle.radius = snowballInfo.radius;
            }
        }
    }
}

void Scene002::UpdateWorld(float timestep_s)
{
    sharedDataPtr_->gameTime_s += timestep_s;
}

void Scene002::AnimateCursorEntity(float timestep_s)
{
    CursorInfo* cursorInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<CursorInfo>(cursorInfoPtr, cursor_entityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, cursor_entityId_));
    if (cursorInfoPtr != nullptr && renderedModelPtr != nullptr)
    {
        // static CursorInfo::State previousCursorState = CursorInfo::State::STATE_OPEN;
        // if (previousCursorState != cursorInfoPtr->state)
        // {
        //     LOG_INFO("CursorState = " << CursorInfo::s_stateToString.at(static_cast<size_t>(cursorInfoPtr->state)));
        //     previousCursorState = cursorInfoPtr->state;
        // }

        // Animations interrupt others
        if (cursorInfoPtr->animationState != cursorInfoPtr->state)
        {
            cursorInfoPtr->animationStateCountDown_s = 0.0f;
            cursorInfoPtr->animaitionFlipper = false;
        }

        if (cursorInfoPtr->animationStateCountDown_s <= 0.0f)
        {
            cursorInfoPtr->animationState = cursorInfoPtr->state;

            if (cursorInfoPtr->animationState == CursorInfo::State::STATE_MAKING)
            {
                cursorInfoPtr->animationStateCountDown_s = 0.125f;
                cursorInfoPtr->animaitionFlipper = !cursorInfoPtr->animaitionFlipper;
            }
            else
            {
                cursorInfoPtr->animationStateCountDown_s = 0.0f;
            }
        }

        // Reset the mesh to default
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        Project001::RenderedMesh& zoneMesh = renderedMeshes[CursorInfo::s_grabZone_renderedMeshIndex];
        zoneMesh.SetVisible(true);
        zoneMesh.SetTextureId(static_cast<unsigned int>(-1));

        Project001::RenderedMesh& openMesh = renderedMeshes[CursorInfo::s_handOpen_renderedMeshIndex];
        openMesh.SetVisible(false);

        Project001::RenderedMesh& pointerMesh = renderedMeshes[CursorInfo::s_handPointer_renderedMeshIndex];
        pointerMesh.SetVisible(false);

        Project001::RenderedMesh& grabMesh = renderedMeshes[CursorInfo::s_handGrab_renderedMeshIndex];
        grabMesh.SetVisible(false);

        if (cursorInfoPtr->animationState == CursorInfo::State::STATE_OPEN)
        {
            openMesh.SetVisible(true);
        }
        else if (cursorInfoPtr->animationState == CursorInfo::State::STATE_POINTING)
        {
            pointerMesh.SetVisible(true);
        }
        else if (cursorInfoPtr->animationState == CursorInfo::State::STATE_MAKING)
        {
            zoneMesh.SetTextureId(sharedDataPtr_->dotted_1_1_textureId);

            if (cursorInfoPtr->animaitionFlipper)
            {
                openMesh.SetVisible(true);
            }
            else
            {
                grabMesh.SetVisible(true);
            }
        }
        else if (cursorInfoPtr->animationState == CursorInfo::State::STATE_GRABING)
        {
            zoneMesh.SetTextureId(sharedDataPtr_->dotted_1_1_textureId);

            grabMesh.SetVisible(true);
        }

        cursorInfoPtr->animationStateCountDown_s -= timestep_s;
    }
}

void Scene002::AnimatePenguinEntities(float timestep_s)
{
    PenguinInfo* penguinInfoPtrs = nullptr;
    size_t penguinInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<PenguinInfo>(penguinInfoPtrs, penguinInfoCount);
    for (size_t i = 0; i < penguinInfoCount; ++i)
    {
        PenguinInfo& penguinInfo = penguinInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<PenguinInfo>(entityId, &penguinInfo);

        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
        if (renderedModelPtr != nullptr)
        {
            // static PenguinInfo::State previousState = PenguinInfo::State::STATE_STANDING;
            // if (previousState != penguinInfo.state)
            // {
            //     LOG_INFO("State: " << PenguinInfo::s_stateToString.at(static_cast<size_t>(penguinInfo.state)));
            //     previousState = penguinInfo.state;
            // }

            // Some animations can be interrupted
            if ((penguinInfo.animationState == PenguinInfo::State::STATE_STANDING ||
                penguinInfo.animationState == PenguinInfo::State::STATE_TREADING_WATER ||
                penguinInfo.animationState == PenguinInfo::State::STATE_MAKING_SNOWBALL ||
                penguinInfo.animationState == PenguinInfo::State::STATE_STANDING_SNOWBALL) &&
                penguinInfo.animationState != penguinInfo.state)
            {
                penguinInfo.animationStateCountDown_s = 0.0f;
            }

            // Some animations interrupt others
            if ((penguinInfo.animationState == PenguinInfo::State::STATE_STANDING && penguinInfo.state == PenguinInfo::State::STATE_MAKING_SNOWBALL) ||
                (penguinInfo.animationState == PenguinInfo::State::STATE_WALKING && penguinInfo.state == PenguinInfo::State::STATE_MAKING_SNOWBALL) ||
                (penguinInfo.animationState == PenguinInfo::State::STATE_WALKING && penguinInfo.state == PenguinInfo::State::STATE_SWIMMING) ||
                (penguinInfo.animationState == PenguinInfo::State::STATE_SWIMMING && penguinInfo.state == PenguinInfo::State::STATE_WALKING) ||
                (penguinInfo.animationState == PenguinInfo::State::STATE_WALKING_SNOWBALL && penguinInfo.state == PenguinInfo::State::STATE_SWIMMING) ||
                (penguinInfo.animationState == PenguinInfo::State::STATE_SWIMMING && penguinInfo.state == PenguinInfo::State::STATE_WALKING_SNOWBALL) ||
                (penguinInfo.animationState != PenguinInfo::State::STATE_HITSTUN && penguinInfo.state == PenguinInfo::State::STATE_HITSTUN) ||
                (penguinInfo.animationState == PenguinInfo::State::STATE_SWIMMING && penguinInfo.state == PenguinInfo::State::STATE_MAKING_SNOWBALL))
            {
                penguinInfo.animationStateCountDown_s = 0.0f;
            }

            // Set animationState to state when the animation is done
            if (penguinInfo.animationStateCountDown_s <= 0.0f)
            {
                penguinInfo.animationState = penguinInfo.state;

                if (penguinInfo.animationState == PenguinInfo::State::STATE_STANDING)
                {
                    penguinInfo.animationStateCountDown_s = 2.0f;
                    penguinInfo.mirrorAnimation = !penguinInfo.mirrorAnimation;
                }
                else if (penguinInfo.animationState == PenguinInfo::State::STATE_WALKING)
                {
                    penguinInfo.animationStateCountDown_s = 0.25f;
                    penguinInfo.mirrorAnimation = !penguinInfo.mirrorAnimation;
                }
                else if (penguinInfo.animationState == PenguinInfo::State::STATE_TREADING_WATER)
                {
                    penguinInfo.animationStateCountDown_s = 1.0f;
                    penguinInfo.mirrorAnimation = !penguinInfo.mirrorAnimation;
                }
                else if (penguinInfo.animationState == PenguinInfo::State::STATE_SWIMMING)
                {
                    penguinInfo.animationStateCountDown_s = 1.0f;
                    penguinInfo.mirrorAnimation = !penguinInfo.mirrorAnimation;
                }
                else if (penguinInfo.animationState == PenguinInfo::State::STATE_HITSTUN)
                {
                    penguinInfo.animationStateCountDown_s = 0.25f;
                    penguinInfo.mirrorAnimation = !penguinInfo.mirrorAnimation;
                }
                else if (penguinInfo.animationState == PenguinInfo::State::STATE_MAKING_SNOWBALL)
                {
                    penguinInfo.animationStateCountDown_s = 0.5f;
                    penguinInfo.mirrorAnimation = !penguinInfo.mirrorAnimation;
                }
                else if (penguinInfo.animationState == PenguinInfo::State::STATE_STANDING_SNOWBALL)
                {
                    penguinInfo.animationStateCountDown_s = 2.0f;
                    penguinInfo.mirrorAnimation = !penguinInfo.mirrorAnimation;
                }
                else if (penguinInfo.animationState == PenguinInfo::State::STATE_WALKING_SNOWBALL)
                {
                    penguinInfo.animationStateCountDown_s = 0.25f;
                    penguinInfo.mirrorAnimation = !penguinInfo.mirrorAnimation;
                }
                else
                {
                    penguinInfo.animationStateCountDown_s = 0.0f;
                }
            }

            // Update positionZ based on the state
            float desiredPositionZ = 0.0f;

            if (!penguinInfo.onLand)
            {
                desiredPositionZ += s_waterHeight;
            }

            if (penguinInfo.animationState == PenguinInfo::State::STATE_TREADING_WATER)
            {
                desiredPositionZ -= 24.0f;
            }
            else if (penguinInfo.animationState == PenguinInfo::State::STATE_SWIMMING)
            {
                desiredPositionZ -= 20.0f;
            }
            else if (!penguinInfo.onLand && penguinInfo.animationState == PenguinInfo::State::STATE_HITSTUN)
            {
                desiredPositionZ -= 20.0f;
            }

            float riseSpeed = timestep_s * 120.0f;
            float sinkSpeed = timestep_s * 120.0f;

            if (penguinInfo.positionZ < desiredPositionZ - riseSpeed)
            {
                penguinInfo.positionZ += riseSpeed;
            }
            else if (penguinInfo.positionZ > desiredPositionZ + sinkSpeed)
            {
                penguinInfo.positionZ -= sinkSpeed;
            }
            else
            {
                penguinInfo.positionZ = desiredPositionZ;
            }

            // Reset the mesh to default
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_body_renderedMeshIndex];
                mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_body_meshId, sharedDataPtr_->penguin_body_meshDataPtr->maxBoundingRadius);
                mesh.SetPosition(0.0f, 0.0f, penguinInfo.positionZ);
                mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
                mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_flipper_right_meshId, sharedDataPtr_->penguin_flipper_right_meshDataPtr->maxBoundingRadius);
                mesh.SetPosition(9.0f, 0.0f, 23.0f);
                mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
                mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_flipper_left_meshId, sharedDataPtr_->penguin_flipper_left_meshDataPtr->maxBoundingRadius);
                mesh.SetPosition(-9.0f, 0.0f, 23.0f);
                mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_right_renderedMeshIndex];
                mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_foot_right_meshId, sharedDataPtr_->penguin_foot_right_meshDataPtr->maxBoundingRadius);
                mesh.SetPosition(7.0f, -2.0f, 3.0f);
                mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_left_renderedMeshIndex];
                mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_foot_left_meshId, sharedDataPtr_->penguin_foot_left_meshDataPtr->maxBoundingRadius);
                mesh.SetPosition(-7.0f, -2.0f, 3.0f);
                mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_head_renderedMeshIndex];
                mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_head_meshId, sharedDataPtr_->penguin_head_meshDataPtr->maxBoundingRadius);
                mesh.SetPosition(0.0f, 0.0f, 26.0f);
                mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);
            }
            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_eyes_renderedMeshIndex];
                mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_eyes_meshId, sharedDataPtr_->penguin_eyes_meshDataPtr->maxBoundingRadius);
                mesh.SetPosition(0.0f, 0.0f, 0.0f);
                mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_beak_renderedMeshIndex];
                mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_beak_meshId, sharedDataPtr_->penguin_beak_meshDataPtr->maxBoundingRadius);
                mesh.SetPosition(0.0f, 0.0f, 0.0f);
                mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_glasses_renderedMeshIndex];
                if (penguinInfo.glassesType == 0)
                {
                    mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_glasses_meshId, sharedDataPtr_->penguin_glasses_meshDataPtr->maxBoundingRadius);
                }
                else
                {
                    mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_glasses_v2_meshId, sharedDataPtr_->penguin_glasses_v2_meshDataPtr->maxBoundingRadius);
                }
                mesh.SetPosition(0.0f, 0.0f, 0.0f);
                mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);
            }

            // Animate the mesh based on the state
            if (penguinInfo.animationState == PenguinInfo::State::STATE_STANDING)
            {
                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
                    mesh.AddRelativeRotationY(
                        glm::pi<float>() / 48.0f * glm::sin(glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
                    mesh.AddRelativeRotationY(
                        glm::pi<float>() / -48.0f * glm::sin(glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }
            }
            else if (penguinInfo.animationState == PenguinInfo::State::STATE_WALKING)
            {
                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_body_renderedMeshIndex];
                    mesh.AddTranslationZ(
                        4.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                    float mirrorConst = 1.0f;
                    if (penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationX(glm::pi<float>() / -32.0f);
                    mesh.AddRelativeRotationY(
                        mirrorConst * glm::pi<float>() / 24.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    if (!penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationY(
                        mirrorConst* glm::pi<float>() / 16.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    if (!penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationY(
                        mirrorConst * glm::pi<float>() / 16.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_right_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    if (penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationX(glm::pi<float>() / 32.0f);
                    mesh.AddRelativeRotationX(
                        mirrorConst* glm::pi<float>() / 24.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_left_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    if (!penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationX(glm::pi<float>() / 32.0f);
                    mesh.AddRelativeRotationX(
                        mirrorConst* glm::pi<float>() / 24.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }
            }
            else if (penguinInfo.animationState == PenguinInfo::State::STATE_TREADING_WATER)
            {
                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_body_renderedMeshIndex];
                    float addedDuration_s = 0.0f;
                    if (!penguinInfo.mirrorAnimation)
                    {
                        addedDuration_s = 1.0f;
                    }
                    mesh.AddTranslationZ(
                        2.0f * glm::sin(glm::pi<float>() * (penguinInfo.animationStateCountDown_s + addedDuration_s))
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    if (!penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationY(
                        mirrorConst * glm::pi<float>() / 12.0f * glm::sin(glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    if (penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationY(
                        mirrorConst * glm::pi<float>() / 12.0f * glm::sin(glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_right_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    mesh.AddRelativeRotationX(glm::pi<float>() / -8.0f);
                    mesh.AddRelativeRotationX(
                        glm::pi<float>() / 16.0f * glm::sin(2.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_left_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    mesh.AddRelativeRotationX(glm::pi<float>() / -8.0f);
                    mesh.AddRelativeRotationX(
                        glm::pi<float>() / 16.0f * glm::sin(-2.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }
            }
            else if (penguinInfo.animationState == PenguinInfo::State::STATE_SWIMMING)
            {
                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_body_renderedMeshIndex];
                    float addedDuration_s = 0.0f;
                    if (!penguinInfo.mirrorAnimation)
                    {
                        addedDuration_s = 1.0f;
                    }
                    mesh.AddTranslationZ(
                        2.0f * glm::sin(glm::pi<float>() * (penguinInfo.animationStateCountDown_s + addedDuration_s))
                    );
                    mesh.AddRelativeRotationX(glm::pi<float>() / -4.0f);
                    mesh.AddTranslationY(-12.0f);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_head_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 4.0f);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
                    mesh.AddRelativeRotationY(glm::pi<float>() / -24.0f);
                    mesh.AddRelativeRotationY(
                        glm::pi<float>() / 12.0f * glm::sin(2.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
                    mesh.AddRelativeRotationY(glm::pi<float>() / 24.0f);
                    mesh.AddRelativeRotationY(
                        glm::pi<float>() / 12.0f * glm::sin(-2.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_right_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    mesh.AddRelativeRotationX(glm::pi<float>() / -8.0f);
                    mesh.AddRelativeRotationX(
                        glm::pi<float>() / 16.0f * glm::sin(2.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_left_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    mesh.AddRelativeRotationX(glm::pi<float>() / -8.0f);
                    mesh.AddRelativeRotationX(
                        glm::pi<float>() / 16.0f * glm::sin(-2.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }
            }
            else if (penguinInfo.animationState == PenguinInfo::State::STATE_MAKING_SNOWBALL)
            {
                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_body_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / -16.0f);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_head_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / -6.0f);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 3.0f);
                    mesh.AddRelativeRotationY(glm::pi<float>() / 16.0f);
                    mesh.AddRelativeRotationZ(glm::pi<float>() / 8.0f);
                    mesh.AddRelativeRotationY(
                        glm::pi<float>() / 8.0f * glm::sin(2.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 3.0f);
                    mesh.AddRelativeRotationY(glm::pi<float>() / -16.0f);
                    mesh.AddRelativeRotationZ(glm::pi<float>() / -8.0f);
                    mesh.AddRelativeRotationY(
                        glm::pi<float>() / -8.0f * glm::sin(2.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_right_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 16.0f);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_left_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 16.0f);
                }
            }
            else if (penguinInfo.animationState == PenguinInfo::State::STATE_STANDING_SNOWBALL)
            {
                float bodyTilt = 0.0f;
                float headTilt = 0.0f;
                float flipperTilt = 0.0f;

                SnowballInfo* snowballInfoPtr = nullptr;
                GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfo.snowball_entityId);
                if (snowballInfoPtr != nullptr && penguinInfo.grabAttractionRadius > 0.0f)
                {
                    headTilt = glm::atan((2.0f * snowballInfoPtr->radius - 32.0f) / penguinInfo.grabAttractionRadius);
                    if (headTilt > glm::quarter_pi<float>())
                    {
                        bodyTilt = headTilt - glm::quarter_pi<float>();
                        headTilt = glm::quarter_pi<float>();
                    }
                    else if (headTilt < -glm::quarter_pi<float>())
                    {
                        bodyTilt = headTilt + glm::quarter_pi<float>();
                        headTilt = -glm::quarter_pi<float>();
                    }
                    flipperTilt = glm::atan((snowballInfoPtr->radius - 16.0f) / penguinInfo.grabAttractionRadius);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_body_renderedMeshIndex];
                    mesh.AddRelativeRotationX(bodyTilt);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_head_renderedMeshIndex];
                    mesh.AddRelativeRotationX(headTilt);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 3.0f);
                    mesh.AddRelativeRotationY(glm::pi<float>() / 16.0f);
                    mesh.AddRelativeRotationZ(glm::pi<float>() / 8.0f);
                    mesh.AddRelativeRotationY(-flipperTilt);
                    mesh.AddRelativeRotationY(
                        glm::pi<float>() / 48.0f * glm::sin(glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 3.0f);
                    mesh.AddRelativeRotationY(glm::pi<float>() / -16.0f);
                    mesh.AddRelativeRotationZ(glm::pi<float>() / -8.0f);
                    mesh.AddRelativeRotationY(flipperTilt);
                    mesh.AddRelativeRotationY(
                        glm::pi<float>() / -48.0f * glm::sin(glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_right_renderedMeshIndex];
                    mesh.AddRelativeRotationX(-bodyTilt);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_left_renderedMeshIndex];
                    mesh.AddRelativeRotationX(-bodyTilt);
                }
            }
            else if (penguinInfo.animationState == PenguinInfo::State::STATE_WALKING_SNOWBALL)
            {
                float bodyTilt = 0.0f;
                float headTilt = 0.0f;
                float flipperTilt = 0.0f;

                SnowballInfo* snowballInfoPtr = nullptr;
                GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfo.snowball_entityId);
                if (snowballInfoPtr != nullptr && penguinInfo.grabAttractionRadius > 0.0f)
                {
                    headTilt = glm::atan((2.0f * snowballInfoPtr->radius - 32.0f) / penguinInfo.grabAttractionRadius);
                    if (headTilt > glm::quarter_pi<float>())
                    {
                        bodyTilt = headTilt - glm::quarter_pi<float>();
                        headTilt = glm::quarter_pi<float>();
                    }
                    else if (headTilt < -glm::quarter_pi<float>())
                    {
                        bodyTilt = headTilt + glm::quarter_pi<float>();
                        headTilt = -glm::quarter_pi<float>();
                    }
                    flipperTilt = glm::atan((snowballInfoPtr->radius - 16.0f) / penguinInfo.grabAttractionRadius);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_body_renderedMeshIndex];
                    mesh.AddTranslationZ(
                        4.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                    float mirrorConst = 1.0f;
                    if (penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationX(bodyTilt);
                    mesh.AddRelativeRotationY(
                        mirrorConst * glm::pi<float>() / 24.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_head_renderedMeshIndex];
                    mesh.AddRelativeRotationX(headTilt);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 3.0f);
                    mesh.AddRelativeRotationY(glm::pi<float>() / 16.0f);
                    mesh.AddRelativeRotationZ(glm::pi<float>() / 8.0f);
                    mesh.AddRelativeRotationY(-flipperTilt);
                    float mirrorConst = 1.0f;
                    if (!penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationY(
                        mirrorConst * glm::pi<float>() / 16.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 3.0f);
                    mesh.AddRelativeRotationY(glm::pi<float>() / -16.0f);
                    mesh.AddRelativeRotationZ(glm::pi<float>() / -8.0f);
                    mesh.AddRelativeRotationY(flipperTilt);
                    float mirrorConst = 1.0f;
                    if (!penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationY(
                        mirrorConst * glm::pi<float>() / 16.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_right_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    if (penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationX(-bodyTilt);
                    mesh.AddRelativeRotationX(
                        mirrorConst * glm::pi<float>() / 24.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_left_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    if (!penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationX(-bodyTilt);
                    mesh.AddRelativeRotationX(
                        mirrorConst * glm::pi<float>() / 24.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                }
            }
            else if (penguinInfo.animationState == PenguinInfo::State::STATE_HITSTUN)
            {
                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_body_renderedMeshIndex];
                    float mirrorConst = 1.0f;
                    if (penguinInfo.mirrorAnimation)
                    {
                        mirrorConst = -1.0f;
                    }
                    mesh.AddRelativeRotationX(glm::pi<float>() / 8.0f);
                    mesh.AddRelativeRotationY(
                        mirrorConst * glm::pi<float>() / 128.0f * glm::sin(4.0f * glm::pi<float>() * penguinInfo.animationStateCountDown_s)
                    );
                    mesh.AddTranslationY(8.0f);
                    mesh.AddTranslationZ(4.0f);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_head_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 8.0f);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_eyes_renderedMeshIndex];
                    mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_eyes_v2_meshId, sharedDataPtr_->penguin_eyes_v2_meshDataPtr->maxBoundingRadius);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 4.0f);
                    mesh.AddRelativeRotationY(glm::pi<float>() / -4.0f);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 4.0f);
                    mesh.AddRelativeRotationY(glm::pi<float>() / 4.0f);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_right_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 16.0f);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_left_renderedMeshIndex];
                    mesh.AddRelativeRotationX(glm::pi<float>() / 16.0f);
                }
            }

            // Animate the shadow and grab zone

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_shadow_renderedMeshIndex];

                if (penguinInfo.onLand)
                {
                    mesh.SetPositionZ(0.01f);
                }
                else
                {
                    mesh.SetPositionZ(s_waterHeight + 0.01f);
                }
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_grabZone_renderedMeshIndex];

                if (penguinInfo.onLand)
                {
                    mesh.SetVisible(true);
                }
                else
                {
                    mesh.SetVisible(false);
                }

                if (penguinInfo.regrabSnowballCoolDown_s > 0.0f ||
                    penguinInfo.snowball_entityId != static_cast<unsigned int>(-1) ||
                    penguinInfo.animationState == PenguinInfo::State::STATE_MAKING_SNOWBALL)
                {
                    mesh.SetTextureId(sharedDataPtr_->dotted_1_1_textureId);
                }
                else
                {
                    mesh.SetTextureId(static_cast<unsigned int>(-1));
                }
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_aimRay1_renderedMeshIndex];

                if (penguinInfo.snowball_entityId != static_cast<unsigned int>(-1))
                {
                    mesh.SetVisible(true);
                }
                else
                {
                    mesh.SetVisible(false);
                }
            }

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_aimRay2_renderedMeshIndex];

                if (penguinInfo.snowball_entityId != static_cast<unsigned int>(-1))
                {
                    mesh.SetVisible(true);
                }
                else
                {
                    mesh.SetVisible(false);
                }
            }

            penguinInfo.animationStateCountDown_s -= timestep_s;
        }
    }
}

void Scene002::AnimateSharkEntities(float timestep_s)
{
    SharkInfo* sharkInfoPtrs = nullptr;
    size_t sharkInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<SharkInfo>(sharkInfoPtrs, sharkInfoCount);
    for (size_t i = 0; i < sharkInfoCount; ++i)
    {
        SharkInfo& sharkInfo = sharkInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<SharkInfo>(entityId, &sharkInfo);

        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
        if (renderedModelPtr != nullptr)
        {
            float desiredPositionZ = 0.0f;

            if (sharkInfo.onLand)
            {
                desiredPositionZ += 32.0f;
            }
            else
            {
                desiredPositionZ += s_waterHeight;
            }

            if (sharkInfo.animationState == SharkInfo::State::STATE_SWIMMING)
            {
                desiredPositionZ -= 24.0f;
            }
            else if (sharkInfo.animationState == SharkInfo::State::STATE_CHASING)
            {
                desiredPositionZ -= 8.0f;
            }
            else if (sharkInfo.animationState == SharkInfo::State::STATE_HITSTUN)
            {
                desiredPositionZ -= 28.0f;
            }

            float riseSpeed = timestep_s * 120.0f;
            float sinkSpeed = timestep_s * 120.0f;

            if (sharkInfo.positionZ < desiredPositionZ - riseSpeed)
            {
                sharkInfo.positionZ += riseSpeed;
            }
            else if (sharkInfo.positionZ > desiredPositionZ + sinkSpeed)
            {
                sharkInfo.positionZ -= sinkSpeed;
            }
            else
            {
                sharkInfo.positionZ = desiredPositionZ;
            }

            // sharkInfo.positionZ = 64.0f;

            // Reset the mesh to default
            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            Project001::RenderedMesh& front_mesh = renderedMeshes[SharkInfo::s_front_renderedMeshIndex];
            front_mesh.SetPosition(0.0f, 0.0f, sharkInfo.positionZ);
            front_mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);

            Project001::RenderedMesh& back_mesh = renderedMeshes[SharkInfo::s_back_renderedMeshIndex];
            back_mesh.SetPosition(0.0f, 0.0f, 0.0f);
            back_mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);

            Project001::RenderedMesh& jaw_mesh = renderedMeshes[SharkInfo::s_jaw_renderedMeshIndex];
            jaw_mesh.SetPosition(0.0f, 96.0f, 10.0f);
            jaw_mesh.SetOrientation(1.0f, 0.0f, 0.0f, 0.0f);

            Project001::RenderedMesh& frontCollision_mesh = renderedMeshes[SharkInfo::s_frontCollision_renderedMeshIndex];
            frontCollision_mesh.SetPosition(0.0f, 0.0f, s_waterHeight);

            Project001::RenderedMesh& backCollision_mesh = renderedMeshes[SharkInfo::s_backCollision_renderedMeshIndex];
            backCollision_mesh.SetPosition(0.0f, 0.0f, s_waterHeight);

            Project001::RenderedMesh& jawCollision_mesh = renderedMeshes[SharkInfo::s_jawCollision_renderedMeshIndex];
            jawCollision_mesh.SetPosition(0.0f, 0.0f, s_waterHeight);

            if (sharkInfo.animationState == SharkInfo::State::STATE_SWIMMING)
            {
                if (sharkInfo.animaitionFlipper)
                {
                    sharkInfo.backRotationZ += glm::pi<float>() / 480.0f;
                }
                else
                {
                    sharkInfo.backRotationZ -= glm::pi<float>() / 480.0f;
                }

                back_mesh.AddRelativeRotationZ(sharkInfo.backRotationZ);

                if (glm::abs(sharkInfo.backRotationZ) > glm::pi<float>() / 18.0f)
                {
                    sharkInfo.animaitionFlipper = !sharkInfo.animaitionFlipper;
                }
            }
            else if (sharkInfo.animationState == SharkInfo::State::STATE_CHASING)
            {
                front_mesh.AddRelativeRotationX(glm::pi<float>() / 12.0f);
                jaw_mesh.AddRelativeRotationX(glm::pi<float>() / -3.0f);

                if (sharkInfo.animaitionFlipper)
                {
                    sharkInfo.backRotationZ += glm::pi<float>() / 160.0f;
                }
                else
                {
                    sharkInfo.backRotationZ -= glm::pi<float>() / 160.0f;
                }

                back_mesh.AddRelativeRotationZ(sharkInfo.backRotationZ);

                if (glm::abs(sharkInfo.backRotationZ) > glm::pi<float>() / 24.0f)
                {
                    sharkInfo.animaitionFlipper = !sharkInfo.animaitionFlipper;
                }
            }
            else if (sharkInfo.animationState == SharkInfo::State::STATE_HITSTUN)
            {
                front_mesh.AddRelativeRotationX(glm::pi<float>() / 12.0f);
                back_mesh.AddRelativeRotationX(glm::pi<float>() / -6.0f);
                jaw_mesh.AddRelativeRotationX(glm::pi<float>() / -6.0f);
            }

            if (!sharkInfo.onLand)
            {
                front_mesh.AddTranslationZ(
                    2.0f * glm::sin(glm::pi<float>() * (sharkInfo.animationStateCountDown_s))
                );
            }

            sharkInfo.animationStateCountDown_s -= timestep_s;
        }

        // TODO:
    }
}

void Scene002::AnimateSnowballEntities(float timestep_s)
{
    SnowballInfo* snowballInfoPtrs = nullptr;
    size_t snowballInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<SnowballInfo>(snowballInfoPtrs, snowballInfoCount);
    for (size_t i = 0; i < snowballInfoCount; ++i)
    {
        SnowballInfo& snowballInfo = snowballInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<SnowballInfo>(entityId, &snowballInfo);

        Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, entityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
        if (snowballCollisionBodyPtr != nullptr && renderedModelPtr != nullptr)
        {
            // Set animationState to state when the animation is done
            if (snowballInfo.animationStateCountDown_s <= 0.0f)
            {
                snowballInfo.animationState = snowballInfo.state;

                if (snowballInfo.animationState == SnowballInfo::State::STATE_EXPLODING)
                {
                    snowballInfo.animationStateCountDown_s = SnowballInfo::s_explosionTime_s;

                    std::uniform_real_distribution<float> randomRotation(0.0f, glm::two_pi<float>());
                    snowballInfo.animationRandomRotation = randomRotation(randomNumberEngine_);
                }
                else
                {
                    snowballInfo.animationStateCountDown_s = 0.0f;
                }
            }

            const glm::vec3 maxSizeColor(0.8f, 0.9f, 1.0f);
            const glm::vec3 regularColor(1.0f, 1.0f, 1.0f);

            constexpr float regularAlpha = 1.0f;

            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            if (snowballInfo.animationState == SnowballInfo::State::STATE_REGULAR)
            {
                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_renderedMeshIndex];

                    float adjustedScale = snowballInfo.radius * SnowballInfo::s_renderedMeshRadiusScaler;
                    mesh.SetScale(adjustedScale, adjustedScale, adjustedScale);

                    const glm::vec2& velocity = snowballCollisionBodyPtr->GetVelocity();
                    float velocityMagnitude = glm::length(velocity);
                    glm::vec3 rotationAxis(1.0f, 0.0f, 0.0f);
                    float rotationAngle = 0.0f;
                    if (velocityMagnitude > 0.0f)
                    {
                        glm::vec2 velocityDirection = velocity / velocityMagnitude;
                        glm::vec2 normalDirection = glm::vec2(-velocityDirection.y, velocityDirection.x);
                        normalDirection = Project001::Math::Rotate2DVector(normalDirection, snowballCollisionBodyPtr->GetRotation());
                        rotationAxis = glm::vec3(normalDirection, 0.0f);

                        rotationAngle = velocityMagnitude * timestep_s / snowballInfo.radius;
                    }
                    mesh.AddWorldRotation(rotationAngle, rotationAxis);

                    float desiredPositionZ = 0.0f;
                    if (snowballInfo.onLand)
                    {
                        desiredPositionZ = snowballInfo.radius;
                    }
                    else
                    {
                        desiredPositionZ = -0.8f * snowballInfo.radius + s_waterHeight;
                    }

                    float riseSpeed = timestep_s * 120.0f;
                    float sinkSpeed = timestep_s * 120.0f;

                    if (snowballInfo.positionZ < desiredPositionZ - riseSpeed)
                    {
                        snowballInfo.positionZ += riseSpeed;
                    }
                    else if (snowballInfo.positionZ > desiredPositionZ + sinkSpeed)
                    {
                        snowballInfo.positionZ -= sinkSpeed;
                    }
                    else
                    {
                        snowballInfo.positionZ = desiredPositionZ;
                    }

                    mesh.SetPositionZ(snowballInfo.positionZ);

                    if (snowballInfo.radius == SnowballInfo::s_maxRadius)
                    {
                        mesh.SetColor(maxSizeColor, regularAlpha);
                    }
                    else
                    {
                        mesh.SetColor(regularColor, regularAlpha);
                    }
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_01_renderedMeshIndex];
                    mesh.SetVisible(false);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_02_renderedMeshIndex];
                    mesh.SetVisible(false);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_03_renderedMeshIndex];
                    mesh.SetVisible(false);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_04_renderedMeshIndex];
                    mesh.SetVisible(false);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_05_renderedMeshIndex];
                    mesh.SetVisible(false);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_06_renderedMeshIndex];
                    mesh.SetVisible(false);
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_shadow_renderedMeshIndex];

                    if (snowballInfo.onLand)
                    {
                        mesh.SetScale(snowballInfo.radius, snowballInfo.radius, snowballInfo.radius);
                        mesh.SetVisible(true);
                    }
                    else
                    {
                        mesh.SetVisible(false);
                    }
                }

            }
            else if (snowballInfo.animationState == SnowballInfo::State::STATE_EXPLODING)
            {
                float adjustedScale = snowballInfo.radius * SnowballInfo::s_renderedMeshRadiusScaler * snowballInfo.animationStateCountDown_s;

                float desiredPositionZ = 0.0f;
                if (snowballInfo.onLand)
                {
                    desiredPositionZ = snowballInfo.radius;
                }
                else
                {
                    desiredPositionZ = -0.8f * snowballInfo.radius + s_waterHeight;
                }

                float riseSpeed = timestep_s * 160.0f;
                float sinkSpeed = timestep_s * 160.0f;

                if (snowballInfo.positionZ < desiredPositionZ - riseSpeed)
                {
                    snowballInfo.positionZ += riseSpeed;
                }
                else if (snowballInfo.positionZ > desiredPositionZ + sinkSpeed)
                {
                    snowballInfo.positionZ -= sinkSpeed;
                }
                else
                {
                    snowballInfo.positionZ = desiredPositionZ;
                }

                glm::vec2 outwardVector = Project001::Math::Rotate2DVector(snowballCollisionBodyPtr->GetForwardVector(), snowballInfo.animationRandomRotation);
                constexpr float outwardVectorRotation = glm::two_pi<float>() / 6.0f;

                const float timeRatio = (SnowballInfo::s_explosionTime_s - snowballInfo.animationStateCountDown_s) / SnowballInfo::s_explosionTime_s;
                const float outwardVectorLength = snowballInfo.radius * SnowballInfo::s_renderedMeshRadiusScaler * timeRatio;

                float zDrop = 0.0f;
                float zBounce = 0.0f;
                if (snowballInfo.onLand)
                {
                    zDrop = -snowballInfo.radius * timeRatio;
                    zBounce = snowballInfo.radius * glm::sin(glm::pi<float>() * timeRatio);
                }
                else
                {
                    zDrop = snowballInfo.radius * timeRatio;
                }

                const float explodeAlpha = snowballInfo.animationStateCountDown_s / SnowballInfo::s_explosionTime_s;

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_renderedMeshIndex];
                    mesh.SetScale(adjustedScale, adjustedScale, adjustedScale);
                    mesh.SetPositionZ(snowballInfo.positionZ + zDrop);

                    if (snowballInfo.radius == SnowballInfo::s_maxRadius)
                    {
                        mesh.SetColor(maxSizeColor, regularAlpha);
                    }
                    else
                    {
                        mesh.SetColor(regularColor, regularAlpha);
                    }
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_01_renderedMeshIndex];
                    mesh.SetVisible(true);
                    mesh.SetScale(adjustedScale, adjustedScale, adjustedScale);

                    mesh.SetPosition(glm::vec3(outwardVector, 0.0f)* outwardVectorLength);
                    mesh.SetPositionZ(snowballInfo.positionZ + zDrop + zBounce);
                    outwardVector = Project001::Math::Rotate2DVector(outwardVector, outwardVectorRotation);

                    if (snowballInfo.radius == SnowballInfo::s_maxRadius)
                    {
                        mesh.SetColor(maxSizeColor, explodeAlpha);
                    }
                    else
                    {
                        mesh.SetColor(regularColor, explodeAlpha);
                    }
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_02_renderedMeshIndex];
                    mesh.SetVisible(true);
                    mesh.SetScale(adjustedScale, adjustedScale, adjustedScale);

                    mesh.SetPosition(glm::vec3(outwardVector, 0.0f) * outwardVectorLength);
                    mesh.SetPositionZ(snowballInfo.positionZ + zDrop);
                    outwardVector = Project001::Math::Rotate2DVector(outwardVector, outwardVectorRotation);

                    if (snowballInfo.radius == SnowballInfo::s_maxRadius)
                    {
                        mesh.SetColor(maxSizeColor, explodeAlpha);
                    }
                    else
                    {
                        mesh.SetColor(regularColor, explodeAlpha);
                    }
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_03_renderedMeshIndex];
                    mesh.SetVisible(true);
                    mesh.SetScale(adjustedScale, adjustedScale, adjustedScale);

                    mesh.SetPosition(glm::vec3(outwardVector, 0.0f)* outwardVectorLength);
                    mesh.SetPositionZ(snowballInfo.positionZ + zDrop + zBounce);
                    outwardVector = Project001::Math::Rotate2DVector(outwardVector, outwardVectorRotation);

                    if (snowballInfo.radius == SnowballInfo::s_maxRadius)
                    {
                        mesh.SetColor(maxSizeColor, explodeAlpha);
                    }
                    else
                    {
                        mesh.SetColor(regularColor, explodeAlpha);
                    }
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_04_renderedMeshIndex];
                    mesh.SetVisible(true);
                    mesh.SetScale(adjustedScale, adjustedScale, adjustedScale);

                    mesh.SetPosition(glm::vec3(outwardVector, 0.0f) * outwardVectorLength);
                    mesh.SetPositionZ(snowballInfo.positionZ + zDrop);
                    outwardVector = Project001::Math::Rotate2DVector(outwardVector, outwardVectorRotation);

                    if (snowballInfo.radius == SnowballInfo::s_maxRadius)
                    {
                        mesh.SetColor(maxSizeColor, explodeAlpha);
                    }
                    else
                    {
                        mesh.SetColor(regularColor, explodeAlpha);
                    }
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_05_renderedMeshIndex];
                    mesh.SetVisible(true);
                    mesh.SetScale(adjustedScale, adjustedScale, adjustedScale);

                    mesh.SetPosition(glm::vec3(outwardVector, 0.0f) * outwardVectorLength);
                    mesh.SetPositionZ(snowballInfo.positionZ + zDrop + zBounce);
                    outwardVector = Project001::Math::Rotate2DVector(outwardVector, outwardVectorRotation);

                    if (snowballInfo.radius == SnowballInfo::s_maxRadius)
                    {
                        mesh.SetColor(maxSizeColor, explodeAlpha);
                    }
                    else
                    {
                        mesh.SetColor(regularColor, explodeAlpha);
                    }
                }

                {
                    Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_snowball_break_06_renderedMeshIndex];
                    mesh.SetVisible(true);
                    mesh.SetScale(adjustedScale, adjustedScale, adjustedScale);

                    mesh.SetPosition(glm::vec3(outwardVector, 0.0f)* outwardVectorLength);
                    mesh.SetPositionZ(snowballInfo.positionZ + zDrop);
                    outwardVector = Project001::Math::Rotate2DVector(outwardVector, outwardVectorRotation);

                    if (snowballInfo.radius == SnowballInfo::s_maxRadius)
                    {
                        mesh.SetColor(maxSizeColor, explodeAlpha);
                    }
                    else
                    {
                        mesh.SetColor(regularColor, explodeAlpha);
                    }
                }
            }

            snowballInfo.animationStateCountDown_s -= timestep_s;
        }
    }
}

void Scene002::SyncCursorRenderedModels()
{
    CursorInfo* cursorInfoPtrs = nullptr;
    size_t cursorInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<CursorInfo>(cursorInfoPtrs, cursorInfoCount);
    for (size_t i = 0; i < cursorInfoCount; ++i)
    {
        CursorInfo& cursorInfo = cursorInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<CursorInfo>(entityId, &cursorInfo);

        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, entityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
        if (collisionBodyPtr != nullptr && renderedModelPtr != nullptr)
        {
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBodyPtr->GetCollisionPoints();
            Project001::CollisionPoint2D& positionPoint = collisionPoints[CursorInfo::s_position_collisionPointIndex];
            Project001::CollisionPoint2D& pressPoint = collisionPoints[CursorInfo::s_press_collisionPointIndex];
            Project001::CollisionPoint2D& releasePoint = collisionPoints[CursorInfo::s_release_collisionPointIndex];
            Project001::CollisionPoint2D& onScreenPoint = collisionPoints[CursorInfo::s_onScreen_collisionPointIndex];

            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
            Project001::RenderedMesh& positionMesh = renderedMeshes[CursorInfo::s_position_renderedMeshIndex];
            Project001::RenderedMesh& pressMesh = renderedMeshes[CursorInfo::s_press_renderedMeshIndex];
            Project001::RenderedMesh& releaseMesh = renderedMeshes[CursorInfo::s_release_renderedMeshIndex];
            Project001::RenderedMesh& handBaseMesh = renderedMeshes[CursorInfo::s_handBase_renderedMeshIndex];

            positionMesh.SetPositionX(positionPoint.position.x);
            positionMesh.SetPositionY(positionPoint.position.y);
            positionMesh.SetVisible(positionPoint.enabled);

            pressMesh.SetPositionX(pressPoint.position.x);
            pressMesh.SetPositionY(pressPoint.position.y);
            pressMesh.SetVisible(pressPoint.enabled);

            releaseMesh.SetPositionX(releasePoint.position.x);
            releaseMesh.SetPositionY(releasePoint.position.y);
            releaseMesh.SetVisible(releasePoint.enabled);

            handBaseMesh.SetPositionX(onScreenPoint.position.x);
            handBaseMesh.SetPositionY(onScreenPoint.position.y);
            handBaseMesh.SetVisible(onScreenPoint.enabled);
        }
    }
}

void Scene002::SyncPenguinRenderedModels()
{
    PenguinInfo* penguinInfoPtrs = nullptr;
    size_t penguinInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<PenguinInfo>(penguinInfoPtrs, penguinInfoCount);
    for (size_t i = 0; i < penguinInfoCount; ++i)
    {
        PenguinInfo& penguinInfo = penguinInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<PenguinInfo>(entityId, &penguinInfo);

        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, entityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
        if (collisionBodyPtr != nullptr && renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());

            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            {
                Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_grabAttractorCollision_renderedMeshIndex];
                mesh.SetPositionY(penguinInfo.grabAttractionRadius);
            }
        }
    }
}

void Scene002::SyncSharkRenderedModels()
{
    SharkInfo* sharkInfoPtrs = nullptr;
    size_t sharkInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<SharkInfo>(sharkInfoPtrs, sharkInfoCount);
    for (size_t i = 0; i < sharkInfoCount; ++i)
    {
        SharkInfo& sharkInfo = sharkInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<SharkInfo>(entityId, &sharkInfo);

        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, entityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
        if (collisionBodyPtr != nullptr && renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());
        }
    }
}

void Scene002::SyncSnowballRenderedModels()
{
    SnowballInfo* snowballInfoPtrs = nullptr;
    size_t snowballInfoCount = 0;
    GetComponentStoresPtr()->GetAllComponents<SnowballInfo>(snowballInfoPtrs, snowballInfoCount);
    for (size_t i = 0; i < snowballInfoCount; ++i)
    {
        SnowballInfo& snowballInfo = snowballInfoPtrs[i];

        unsigned int entityId = static_cast<unsigned int>(-1);
        GetComponentStoresPtr()->GetComponentEntityId<SnowballInfo>(entityId, &snowballInfo);

        Project001::CollisionBody2D* collisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(collisionBodyPtr, entityId));
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
        if (collisionBodyPtr != nullptr && renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetPositionX(collisionBodyPtr->GetPosition().x);
            renderedModelPtr->SetPositionY(collisionBodyPtr->GetPosition().y);
            renderedModelPtr->ResetOrientation();
            renderedModelPtr->AddRelativeRotationZ(collisionBodyPtr->GetRotation());

            std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

            {
                Project001::RenderedMesh& mesh = renderedMeshes[SnowballInfo::s_orientationArrow_renderedMeshIndex];

                std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBodyPtr->GetCollisionCircles();
                Project001::CollisionCircle2D& collisionCircle = collisionCircles[SnowballInfo::s_snowball_collisionCircleIndex];

                mesh.SetScale(collisionCircle.radius, collisionCircle.radius, collisionCircle.radius);
            }
        }
    }
}

const uint32_t Scene002::s_player_collisionGroupMasks_[SharedApplicationData::s_player_count] = {
        0b00000000000000000000000000000010,
        0b00000000000000000000000000000100,
        0b00000000000000000000000000001000,
        0b00000000000000000000000000010000
};
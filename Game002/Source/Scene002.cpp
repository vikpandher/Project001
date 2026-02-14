// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-02-13

#include "Scene002.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/LightSource.h"
#include "Components/RenderedModel.h"
#include "Utilities/FontUtility.h"
#include "Utilities/MathUtility.h"
#include "Utilities/MeshUtility.h"
#include "CollisionSystem2D.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "SoundPlayer.h"
#include "Window.h"

#include <stack>



struct PenguinInfo
{
    size_t playerNumber = 1;
    size_t controlScheme = 1;

    unsigned int snowball_EntityId = static_cast<unsigned int>(-1);

    enum class State
    {
        STATE_STANDING,
        STATE_WALKING,
        STATE_TREADING_WATER,
        STATE_SWIMMING,
        STATE_MAKING_SNOWBALL,
        STATE_STANDING_SNOWBALL,
        STATE_WALKING_SNOWBALL,
        STATE_HITSTUN
    };

    State state = State::STATE_STANDING;
    float makeSnowballCountDown_s = 0.0f;
    static constexpr float s_makeSnowballTime_s = 0.5f;
    float regrabSnowballCoolDown_s = 0.0f;
    static constexpr float s_regrabSnowballTime_s = 0.0f;
    float hitstunCoolDown_s = 0.0f;

    bool onLand = false;

    State animationState = State::STATE_STANDING;
    float animationStateCountDown_s = 0.0f;

    bool mirrorAnimation = false;

    float positionZ = 0.0f;

    static constexpr float s_initialGrabAttractionRadius = 12.0f + 8.0f; // penguinRadius + snowballRadius
    float grabAttractionRadius = s_initialGrabAttractionRadius;

    static const size_t s_body_renderedMeshIndex = 0;
    static const size_t s_flipper_right_renderedMeshIndex = 1;
    static const size_t s_flipper_left_renderedMeshIndex = 2;
    static const size_t s_foot_right_renderedMeshIndex = 3;
    static const size_t s_foot_left_renderedMeshIndex = 4;
    static const size_t s_head_renderedMeshIndex = 5;
    static const size_t s_eyes_renderedMeshIndex = 6;
    static const size_t s_beak_renderedMeshIndex = 7;
    static const size_t s_glasses_renderedMeshIndex = 8;
    static const size_t s_shadow_renderedMeshIndex = 9;
    static const size_t s_grabZone_renderedMeshIndex = 10;
    static const size_t s_orientationArrow_renderedMeshIndex = 11;
    static const size_t s_grabAttractorCollision_renderedMeshIndex = 12;
    static const size_t s_renderedMeshIndices = 13;

    static const size_t s_body_collisionCircleIndex = 0;
    static const size_t s_grabZone_collisionCircleIndex = 1;
    static const size_t s_collisionCircleCount = 2;

    static const size_t s_grabAttractor_collisionPointIndex = 0;
    static const size_t s_collisionPointCount = 1;
};

struct SnowballInfo
{
    unsigned int penguin_EntityId = static_cast<unsigned int>(-1);

    enum class State
    {
        STATE_REGULAR,
        STATE_EXPLODING
    };

    State state = State::STATE_REGULAR;
    static constexpr float s_explosionTime_s = 0.75f;

    State animationState = State::STATE_REGULAR;
    float animationStateCountDown_s = 0.0f;

    float animationRandomRotation = 0.0f;

    bool onLand = false;

    static constexpr float s_initialSnowballRadius = 8.0f;
    float radius = s_initialSnowballRadius;
    static constexpr float s_maxRadius = 48.0f;
    static constexpr float s_renderedMeshRadiusScaler = 1.2f;

    float positionZ = 0.0f;

    static const size_t s_snowball_renderedMeshIndex = 0;
    static const size_t s_snowball_break_01_renderedMeshIndex = 1;
    static const size_t s_snowball_break_02_renderedMeshIndex = 2;
    static const size_t s_snowball_break_03_renderedMeshIndex = 3;
    static const size_t s_snowball_break_04_renderedMeshIndex = 4;
    static const size_t s_snowball_break_05_renderedMeshIndex = 5;
    static const size_t s_snowball_break_06_renderedMeshIndex = 6;
    static const size_t s_shadow_renderedMeshIndex = 7;
    static const size_t s_orientationArrow_renderedMeshIndex = 8;
    static const size_t s_renderedMeshIndices = 9;

    static const size_t s_snowball_collisionCircleIndex = 0;
    static const size_t s_collisionCircleCount = 1;
};

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

    CreateStageEntity();
    CreateStageLightEntity();
    CreatePenguinEntity(player1_entityId_, glm::vec2(-128.0f, 0.0f), glm::pi<float>(), 1, 1);
    CreatePenguinEntity(player2_entityId_, glm::vec2(-96.0f, 0.0f), glm::pi<float>(), 2, 1);
    CreatePenguinEntity(player3_entityId_, glm::vec2(-64.0f, 0.0f), glm::pi<float>(), 3, 1);
    CreatePenguinEntity(player4_entityId_, glm::vec2(-32.0f, 0.0f), glm::pi<float>(), 4, 1);

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

    stage_entityId_ = static_cast<unsigned int>(-1);
    stageLight_entityId_ = static_cast<unsigned int>(-1);

    player1_entityId_ = static_cast<unsigned int>(-1);
    player2_entityId_ = static_cast<unsigned int>(-1);
    player3_entityId_ = static_cast<unsigned int>(-1);
    player4_entityId_ = static_cast<unsigned int>(-1);

    // -------------------------------------------------------------------------

    mainCamera_lookAtPoint_ = glm::vec3(0.0f, 0.0f, 0.0f);
    mainCamera_distanceAway_ = 0.0f;
    mainCamera_playerLock_ = 0;

    paused_ = false;
}

void Scene002::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == sharedDataPtr_->pause_keyCode)
        {
            paused_ = !paused_;
        }
        if (paused_ && keyCode == sharedDataPtr_->quit_keyCode)
        {
            SendEventToApplication(Project001::SwitchSceneEvent(sharedDataPtr_->scene001Id));
            if (GetActiveScene()->GetId() == sharedDataPtr_->scene001Id)
            {
                SendEventToScene(GetId(), Project001::DeinitializeEvent());
                SendEventToApplication(Project001::InitializeEvent());
            }
            return;
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_1)
        {
            mainCamera_playerLock_++;
            if (mainCamera_playerLock_ > 3)
            {
                mainCamera_playerLock_ = 0;
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_2)
        {
            constexpr float mainCameraPitch = glm::quarter_pi<float>();
            constexpr float mainCameraYaw = glm::pi<float>();
            mainCamera_distanceAway_ = sharedDataPtr_->mainCamera_initialDistanceAway;

            Project001::Camera* cameraPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_entityId_));
            if (cameraPtr != nullptr)
            {
                cameraPtr->ResetOrientation();

                cameraPtr->AddPitch(mainCameraPitch);
                cameraPtr->AddYaw(mainCameraYaw);

                cameraPtr->FollowFocalPoint(glm::vec3(0.0f, 0.0f, 0.0f), mainCamera_distanceAway_);
            }

            Project001::Camera* otherCameraPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(otherCameraPtr, mainCameraDebug_entityId_));
            if (cameraPtr != nullptr)
            {
                otherCameraPtr->SetOrientation(cameraPtr->GetOrientation());
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_3)
        {
            mainCamera_distanceAway_ = 1400.0f;

            Project001::Camera* cameraPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_entityId_));
            if (cameraPtr != nullptr)
            {
                cameraPtr->ResetOrientation();
                cameraPtr->AddYaw(glm::pi<float>());
                // cameraPtr->AddPitch(glm::half_pi<float>());
            }

            Project001::Camera* otherCameraPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(otherCameraPtr, mainCameraDebug_entityId_));
            if (cameraPtr != nullptr)
            {
                otherCameraPtr->SetOrientation(cameraPtr->GetOrientation());
            }
        }
        else if (keyCode == Project001::KeyCode::KEY_CODE_0)
        {
            Project001::Camera* cameraPtr = nullptr;
            if (GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraDebug_entityId_))
            {
                cameraPtr->SetTurnedOn(!cameraPtr->GetTurnedOn());
            }
        }
    }
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

        UpdateStageCollisionBodyQuadTreeMesh();
        UpdatePenguinEntity(player1_entityId_, physicsTimestep_s);
        UpdatePenguinEntity(player2_entityId_, physicsTimestep_s);
        UpdatePenguinEntity(player3_entityId_, physicsTimestep_s);
        UpdatePenguinEntity(player4_entityId_, physicsTimestep_s);
        UpdateSnowballEntities(physicsTimestep_s);
        UpdateWorld(physicsTimestep_s);

        GetCollisionSystemPtr()->ApplyMovement(physicsTimestep_s);
    }

    AnimatePenguinEntities(timestep_s);
    AnimateSnowballEntities(timestep_s);

    // Sync rendered models
    // -------------------------------------------------------------------------

    SyncPenguinRenderedModels();
    SyncSnowballRenderedModels();
}

void Scene002::CreateMainCameraEntities()
{
    mainCamera_distanceAway_ = sharedDataPtr_->mainCamera_initialDistanceAway;

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
            // cameraPtr->SetTurnedOn(false);
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

void Scene002::CreateStageEntity()
{
    GetCollisionSystemPtr()->ResetCollisionBodyQuadTree2D(
        glm::vec2(-(sharedDataPtr_->maxStage_size + 8.0f), -(sharedDataPtr_->maxStage_size + 8.0f)),
        glm::vec2(sharedDataPtr_->maxStage_size + 8.0f, sharedDataPtr_->maxStage_size + 8.0f),
        3,
        16
    );

    GetCollisionSystemPtr()->FullyLoadCollisionBodyQuadTree2D();

    GetComponentStoresPtr()->CreateEntity(stage_entityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(stage_entityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, stage_entityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCameraGroup_cameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->ground_meshDataPtr);
        }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->deadZone_meshDataPtr);
            mesh.SetColor(0.8f, 0.6f, 0.2f, 1.0f);
            mesh.SetTextureId(sharedDataPtr_->hazard_textureId);
            mesh.SetRenderPriorityOverride(3);
        }

        // {
        //     renderedMeshes.emplace_back();
        //     Project001::RenderedMesh& mesh = renderedMeshes.back();
        //     mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
        //     mesh.SetMeshDataPtr(sharedDataPtr_->groundCollision_meshDataPtr);
        //     mesh.SetPositionZ(0.1f);
        //     mesh.SetColor(0.2f, 1.0f, 0.2f, 0.2f);
        //     mesh.SetTranslucent(true);
        //     mesh.SetUseLighting(false);
        // }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->water_meshDataPtr);
            mesh.SetPositionZ(s_waterHeight);
            mesh.SetColor(0.6f, 0.6f, 1.0f, 0.6f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(1);
        }

        // {
        //     renderedMeshes.emplace_back();
        //     Project001::RenderedMesh& mesh = renderedMeshes.back();
        //     mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
        //     mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->stageGrid_meshId, sharedDataPtr_->stageGrid_meshDataPtr->maxBoundingRadius);
        //     mesh.SetPositionZ(0.2f);
        //     mesh.SetColor(0.2f, 0.2f, 1.0f, 0.2f);
        //     mesh.SetTranslucent(true);
        //     mesh.SetUseLighting(false);
        // }

        // {
        //     renderedMeshes.emplace_back();
        //     Project001::RenderedMesh& mesh = renderedMeshes.back();
        //     mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
        //     mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->stageGridLabels_meshId, sharedDataPtr_->stageGridLabels_meshDataPtr->maxBoundingRadius);
        //     mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
        //     mesh.SetPositionZ(0.2f);
        //     mesh.SetColor(0.2f, 0.2f, 1.0f, 0.2f);
        //     mesh.SetTranslucent(true);
        //     mesh.SetUseLighting(false);
        // }

        {
            renderedMeshes.emplace_back();
            Project001::RenderedMesh& mesh = renderedMeshes.back();
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->stageCollisionQuadTree_meshDataPtr);
            mesh.SetPositionZ(0.3f);
            mesh.SetColor(1.0f, 0.4f, 0.2f, 0.2f);
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
        const float groundCorner = sharedDataPtr_->ground_size * 0.41421357f; // sqrt(2) - 1
        std::vector<glm::vec2> groundCollisionCorners;
        groundCollisionCorners.reserve(8);
        groundCollisionCorners.emplace_back(sharedDataPtr_->ground_size, groundCorner);
        groundCollisionCorners.emplace_back(groundCorner, sharedDataPtr_->ground_size);
        groundCollisionCorners.emplace_back(-groundCorner, sharedDataPtr_->ground_size);
        groundCollisionCorners.emplace_back(-sharedDataPtr_->ground_size, groundCorner);
        groundCollisionCorners.emplace_back(-sharedDataPtr_->ground_size, -groundCorner);
        groundCollisionCorners.emplace_back(-groundCorner, -sharedDataPtr_->ground_size);
        groundCollisionCorners.emplace_back(groundCorner, -sharedDataPtr_->ground_size);
        groundCollisionCorners.emplace_back(sharedDataPtr_->ground_size, -groundCorner);
        std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBodyPtr->GetCollisionConvexPolygons();
        collisionConvexPolygons.emplace_back(groundCollisionCorners, s_ground_collisionShapeTag_);
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

void Scene002::CreatePenguinEntity(unsigned int& entityId, const glm::vec2& position, float rotation, size_t playerNumber, size_t controlScheme)
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
        penguinInfoPtr->playerNumber = playerNumber;
        penguinInfoPtr->controlScheme = controlScheme;

        unsigned int textureId = static_cast<unsigned int>(-1);
        if (penguinInfoPtr->playerNumber == 1)
        {
            collisionBodyPtr->SetCollisionGroupMask(s_player1_collisionGroupMask_);

            textureId = sharedDataPtr_->penguin_textureId;
        }
        else if (penguinInfoPtr->playerNumber == 2)
        {
            collisionBodyPtr->SetCollisionGroupMask(s_player2_collisionGroupMask_);

            textureId = sharedDataPtr_->penguin_v2_textureId;
        }
        else if (penguinInfoPtr->playerNumber == 3)
        {
            collisionBodyPtr->SetCollisionGroupMask(s_player3_collisionGroupMask_);

            textureId = sharedDataPtr_->penguin_v3_textureId;
        }
        else if (penguinInfoPtr->playerNumber == 4)
        {
            collisionBodyPtr->SetCollisionGroupMask(s_player4_collisionGroupMask_);

            textureId = sharedDataPtr_->penguin_v4_textureId;
        }

        renderedModelPtr->SetCameraMask(s_mainCameraGroup_cameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(PenguinInfo::s_renderedMeshIndices);

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
            mesh.SetScale(glm::vec3(sharedDataPtr_->penguin_collisionRadius));
            mesh.SetPositionZ(0.01f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.8f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_grabZone_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->hallowCircle_meshDataPtr);
            mesh.SetScale(glm::vec3(sharedDataPtr_->penguin_grabRadius));
            mesh.SetPositionY(sharedDataPtr_->penguin_grabOffset);
            mesh.SetPositionZ(0.02f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.1f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_orientationArrow_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCameraDebug_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->orientationArrow_meshDataPtr);
            mesh.SetScale(glm::vec3(sharedDataPtr_->penguin_collisionRadius));
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

        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBodyPtr->GetCollisionCircles();
        collisionCircles.resize(PenguinInfo::s_collisionCircleCount);

        {
            Project001::CollisionCircle2D& collisionCircle = collisionCircles[PenguinInfo::s_body_collisionCircleIndex];
            collisionCircle = Project001::CollisionCircle2D(
                glm::vec2(0.0f, 0.0f),
                sharedDataPtr_->penguin_collisionRadius,
                s_player_collisionShapeTag_
            );
        }

        {
            Project001::CollisionCircle2D& collisionCircle = collisionCircles[PenguinInfo::s_grabZone_collisionCircleIndex];
            collisionCircle = Project001::CollisionCircle2D(
                glm::vec2(0.0f, sharedDataPtr_->penguin_grabOffset),
                sharedDataPtr_->penguin_grabRadius,
                s_grab_collisionShapeTag_,
                true,
                Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_SIMPLE_SENSOR
            );
        }

        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBodyPtr->GetCollisionPoints();
        collisionPoints.resize(PenguinInfo::s_collisionPointCount);

        {
            Project001::CollisionPoint2D& collisionPoint = collisionPoints[PenguinInfo::s_grabAttractor_collisionPointIndex];
            collisionPoint = Project001::CollisionPoint2D(
                glm::vec2(0.0f, penguinInfoPtr->grabAttractionRadius),
                s_grabAttractor_collisionShapeTag_
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
        renderedMeshes.resize(SnowballInfo::s_renderedMeshIndices);

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
            mesh.SetPositionZ(0.01f);
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

void Scene002::UpdateMainCameraEntity(float timestep_s)
{
    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_entityId_));
    if (cameraPtr != nullptr)
    {
        constexpr float orbitSpeed = 0.1f * glm::pi<float>();
        float cameraPitchDelta = orbitSpeed * timestep_s;

        bool pitchingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_PAGE_DOWN);
        bool pitchingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_PAGE_UP);

        const float moveSpeed = 128.0f;
        float moveSpeedDelta = moveSpeed * timestep_s;

        bool movingLeft = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_KP_4);
        bool movingRight = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_KP_6);
        bool movingUp = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_KP_8);
        bool movingDown = GetWindowPtr()->GetKeyPressed(Project001::KeyCode::KEY_CODE_KP_2);

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

        if (mainCamera_playerLock_ == 1)
        {
            Project001::CollisionBody2D* playerCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(playerCollisionBodyPtr, player1_entityId_));
            if (playerCollisionBodyPtr != nullptr)
            {
                mainCamera_lookAtPoint_ = glm::vec3(playerCollisionBodyPtr->GetPosition(), 0.0f);
            }
        }
        else if (mainCamera_playerLock_ == 2)
        {
            Project001::CollisionBody2D* playerCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(playerCollisionBodyPtr, player2_entityId_));
            if (playerCollisionBodyPtr != nullptr)
            {
                mainCamera_lookAtPoint_ = glm::vec3(playerCollisionBodyPtr->GetPosition(), 0.0f);
            }
        }
        else if (mainCamera_playerLock_ == 3)
        {
            Project001::CollisionBody2D* player1CollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(player1CollisionBodyPtr, player1_entityId_));
            Project001::CollisionBody2D* player2CollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(player2CollisionBodyPtr, player2_entityId_));
            if (player1CollisionBodyPtr != nullptr && player2CollisionBodyPtr != nullptr)
            {
                mainCamera_lookAtPoint_ =glm::vec3((player1CollisionBodyPtr->GetPosition() + player2CollisionBodyPtr->GetPosition()) * 0.5f, 0.0f);
            }
        }

        glm::vec3 offsetLookAtPoint = mainCamera_lookAtPoint_;
        // offsetLookAtPoint.y -= mainCamera_distanceAway_ * 0.1f;
        cameraPtr->FollowFocalPoint(offsetLookAtPoint, mainCamera_distanceAway_);

        Project001::Camera* otherCameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(otherCameraPtr, mainCameraDebug_entityId_));
        if (otherCameraPtr != nullptr)
        {
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

void Scene002::UpdatePenguinEntity(unsigned int& entityId, float timestep_s)
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

            if (penguinInfoPtr->snowball_EntityId == static_cast<unsigned int>(-1) &&
                penguinCollisionOverlapData.myShapeTag == s_grab_collisionShapeTag_ &&
                penguinCollisionOverlapData.otherShapeTag == s_snowball_collisionShapeTag_)
            {
                SnowballInfo* snowballInfoPtr = nullptr;
                GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinCollisionOverlapData.otherEntityId);
                Project001::CollisionBody2D* otherCollisionBodyPtr = nullptr;
                FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(otherCollisionBodyPtr, penguinCollisionOverlapData.otherEntityId));
                if (snowballInfoPtr != nullptr && otherCollisionBodyPtr != nullptr && snowballInfoPtr->penguin_EntityId == static_cast<unsigned int>(-1) &&
                    snowballInfoPtr->onLand && otherCollisionBodyPtr->GetPhysicsType() == Project001::CollisionShape2D::PhysicsType::PHYSICS_TYPE_RIGID_BODY)
                {
                    float otherRadius = 0.0f;

                    std::vector<Project001::CollisionCircle2D>& collisionCircles = otherCollisionBodyPtr->GetCollisionCircles();
                    if (collisionCircles.size() > 0)
                    {
                        const Project001::CollisionCircle2D& collisionCircle2D = collisionCircles[SnowballInfo::s_snowball_collisionCircleIndex];
                        otherRadius = collisionCircle2D.radius;
                    }

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

        if (penguinInfoPtr->snowball_EntityId != static_cast<unsigned int>(-1))
        {
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_EntityId));
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

        // Gathering input
        // ---------------------------------------------------------------------

        bool movingLeft = false;
        bool movingRight = false;
        bool movingUp = false;
        bool movingDown = false;
        bool grabPressed = false;

        if (penguinInfoPtr->controlScheme == 1)
        {
            movingLeft = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->player1_left_keyCode);
            movingRight = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->player1_right_keyCode);
            movingUp = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->player1_up_keyCode);
            movingDown = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->player1_down_keyCode);
            grabPressed = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->player1_snowball_keyCode);
        }
        else if (penguinInfoPtr->controlScheme == 2)
        {
            movingLeft = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->player2_left_keyCode);
            movingRight = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->player2_right_keyCode);
            movingUp = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->player2_up_keyCode);
            movingDown = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->player2_down_keyCode);
            grabPressed = GetWindowPtr()->GetKeyPressed(sharedDataPtr_->player2_snowball_keyCode);
        }

        glm::vec2 moveDirection(0.0f, 0.0);
        if (movingLeft)
        {
            moveDirection.x -= 1.0f;
        }
        if (movingRight)
        {
            moveDirection.x += 1.0f;
        }
        if (movingUp)
        {
            moveDirection.y += 1.0f;
        }
        if (movingDown)
        {
            moveDirection.y -= 1.0f;
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
                    if (penguinInfoPtr->regrabSnowballCoolDown_s <= 0.0f)
                    {
                        penguinInfoPtr->regrabSnowballCoolDown_s = PenguinInfo::s_regrabSnowballTime_s;

                        if (grabableInReach)
                        {
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
                    GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfoPtr->snowball_EntityId);
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

        if (penguinInfoPtr->snowball_EntityId != static_cast<unsigned int>(-1))
        {
            const float& penguinMass = penguinCollisionBodyPtr->GetMass();
            float snowballMass = 0.0f;
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_EntityId);
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
                friction = sharedDataPtr_->snowball_landFriction;
            }
            else
            {
                friction = sharedDataPtr_->snowball_waterFriction;
            }

            angularAcceleration = 0.0f;
            angularFriction = sharedDataPtr_->snowball_angularFriction;
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
                penguinInfoPtr->snowball_EntityId = grabable_entityId;
                snowballInfoPtr->penguin_EntityId = entityId;

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
        }
        case SnowballAction::SNOWBALL_ACTION_DROP:
        {
            SnowballInfo* snowballInfoPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfoPtr->snowball_EntityId);
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_EntityId);
            if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
            {
                penguinInfoPtr->snowball_EntityId = static_cast<unsigned int>(-1);
                snowballInfoPtr->penguin_EntityId = static_cast<unsigned int>(-1);

                snowballCollisionBodyPtr->SetAllowedCollisionFilterMask(
                    snowballCollisionBodyPtr->GetAllowedCollisionFilterMask() | penguinCollisionBodyPtr->GetCollisionGroupMask()
                );
            }

            break;
        }
        case SnowballAction::SNOWBALL_ACTION_THROW:
        {
            SnowballInfo* snowballInfoPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfoPtr->snowball_EntityId);
            Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
            GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_EntityId);
            if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
            {
                penguinInfoPtr->snowball_EntityId = static_cast<unsigned int>(-1);
                snowballInfoPtr->penguin_EntityId = static_cast<unsigned int>(-1);

                snowballCollisionBodyPtr->SetAllowedCollisionFilterMask(
                    snowballCollisionBodyPtr->GetAllowedCollisionFilterMask() | penguinCollisionBodyPtr->GetCollisionGroupMask()
                );

                if (glm::abs(turnAngle) < turnAngleMovementThreshold && moveMagnitude > 0.0f)
                {
                    snowballCollisionBodyPtr->SetVelocity(
                        snowballCollisionBodyPtr->GetVelocity() +
                        penguinCollisionBodyDirection * sharedDataPtr_->penguin_throwSpeed_s
                    );
                }
            }

            break;
        }
        }

        SnowballInfo* snowballInfoPtr = nullptr;
        GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfoPtr->snowball_EntityId);
        Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
        GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_EntityId);
        if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
        {
            const glm::vec2& penguinPosition = penguinCollisionBodyPtr->GetPosition();
            const float& penguinRotation = penguinCollisionBodyPtr->GetRotation();
            const float& penguinRadius = sharedDataPtr_->penguin_collisionRadius;
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
            penguinInfoPtr->snowball_EntityId,
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            SnowballInfo::s_initialSnowballRadius
        );

        Project001::CollisionBody2D* penguinCollisionBodyPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(penguinCollisionBodyPtr, entityId));
        SnowballInfo* snowballInfoPtr = nullptr;
        GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfoPtr->snowball_EntityId);
        Project001::CollisionBody2D* snowballCollisionBodyPtr = nullptr;
        GetComponentStoresPtr()->GetComponent<Project001::CollisionBody2D>(snowballCollisionBodyPtr, penguinInfoPtr->snowball_EntityId);
        if (snowballInfoPtr != nullptr && snowballCollisionBodyPtr != nullptr)
        {
            snowballInfoPtr->penguin_EntityId = entityId;

            const glm::vec2& penguinPosition = penguinCollisionBodyPtr->GetPosition();
            const float& penguinRadius = sharedDataPtr_->penguin_collisionRadius;
            const float& snowballRadius = snowballInfoPtr->radius;

            penguinInfoPtr->grabAttractionRadius = penguinRadius + snowballRadius;
            glm::vec2 collisionBodyDirection = penguinCollisionBodyPtr->GetForwardVector();
            glm::vec2 attractionPoint = penguinPosition + collisionBodyDirection * penguinInfoPtr->grabAttractionRadius;
            snowballCollisionBodyPtr->SetPosition(attractionPoint);

            penguinCollisionBodyPtr->FlagMassToBeRecalculated();
        }
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

            float friction = sharedDataPtr_->snowball_landFriction;
            if (!snowballInfo.onLand)
            {
                friction = sharedDataPtr_->snowball_waterFriction;
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

            float angularFriction = sharedDataPtr_->snowball_angularFriction;

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
            if (penguinInfo.animationState == PenguinInfo::State::STATE_TREADING_WATER)
            {
                desiredPositionZ = s_waterHeight - 24.0f;
            }
            else if (penguinInfo.animationState == PenguinInfo::State::STATE_SWIMMING)
            {
                desiredPositionZ = s_waterHeight - 20.0f;
            }
            else if (!penguinInfo.onLand && penguinInfo.animationState == PenguinInfo::State::STATE_HITSTUN)
            {
                desiredPositionZ = s_waterHeight - 20.0f;
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
                if (penguinInfo.playerNumber == 1)
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
                GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfo.snowball_EntityId);
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
                GetComponentStoresPtr()->GetComponent<SnowballInfo>(snowballInfoPtr, penguinInfo.snowball_EntityId);
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
            }

            penguinInfo.animationStateCountDown_s -= timestep_s;
        }
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
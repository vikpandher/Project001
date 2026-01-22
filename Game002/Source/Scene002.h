// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-22

#pragma once

#include "Scene.h"

#include "SharedApplicationData.h"

#include "glm/glm.hpp"

#include <random>
#include <stack>
#include <vector>



class Scene002 : public Project001::Scene
{
public:
    Scene002(Project001::Application* applicationPtr);
    ~Scene002();

    Scene002(Scene002& other) = delete;
    void operator=(const Scene002&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void CreateMainCameraEntities();
    void CreateUiCameraEntity();
    void CreateUiTextEntity();
    void CreateUiPauseTextEntity();
    void CreateStageEntity();
    void CreateStageLightEntity();
    void CreatePenguinEntity(unsigned int& entityId, const glm::vec2& position, float rotation, size_t playerNumber);
    void CreateSnowballEntity(unsigned int& entityId, const glm::vec2& position, const glm::vec2& velocity, float radius);

    void UpdateMainCameraEntity(float timestep_s);
    void UpdateUiTextEntity();
    void UpdateUiPauseTextEntity();
    void UpdateStageCollisionBodyQuadTreeMesh();
    void UpdatePenguinEntity(unsigned int& entityId, float timestep_s);
    void UpdateSnowballEntities(float timestep_s);
    void UpdateWorld(float timestep_s);

    void AnimatePenguinEntities(float timestep_s);
    void AnimateSnowballEntities(float timestep_s);

    void SyncPenguinRenderedModels();
    void SyncSnowballRenderedModels();

    // -------------------------------------------------------------------------

    SharedApplicationData* sharedDataPtr_ = nullptr;

    // Entity Ids --------------------------------------------------------------

    unsigned int mainCamera_entityId_ = static_cast<unsigned int>(-1);
    unsigned int mainCameraDebug_entityId_ = static_cast<unsigned int>(-1);
    static const uint32_t s_mainCamera_cameraMask_ = 0b00000000000000000000000000000001;
    static const uint32_t s_mainCameraDebug_cameraMask_ = 0b00000000000000000000000000000010;
    static const uint32_t s_mainCameraGroup_cameraMask_ = s_mainCamera_cameraMask_ | s_mainCameraDebug_cameraMask_;

    unsigned int uiCamera_entityId_ = static_cast<unsigned int>(-1);
    static const uint32_t s_uiCamera_cameraMask_ = 0b10000000000000000000000000000000;

    unsigned int uiText_entityId_ = static_cast<unsigned int>(-1);
    unsigned int uiPauseText_entityId_ = static_cast<unsigned int>(-1);

    unsigned int stage_entityId_ = static_cast<unsigned int>(-1);
    unsigned int stageLight_entityId_ = static_cast<unsigned int>(-1);

    unsigned int player1_entityId_ = static_cast<unsigned int>(-1);
    unsigned int player2_entityId_ = static_cast<unsigned int>(-1);

    // -------------------------------------------------------------------------

    glm::vec3 mainCamera_lookAtPoint_;
    float mainCamera_distanceAway_ = 0.0f;

    // 0 = unlocked
    // 1 = locked to player 1
    // 2 = locked to player 2
    // 3 = locked between player 1 & 2
    size_t mainCamera_playerLock_ = 0;

    bool paused_ = false;

    std::mt19937 randomNumberEngine_;

    static const int s_main_renderPriority = 0;

    static const unsigned int s_player_collisionShapeTag_ = 1;
    static const unsigned int s_grab_collisionShapeTag_ = 2;
    static const unsigned int s_snowball_collisionShapeTag_ = 3;
    static const unsigned int s_ground_collisionShapeTag_ = 4;

    static const uint32_t s_actor_collisionGroupMask_ = 0b00000000000000000000000000000010;
    static const uint32_t s_sensor_collisionGroupMask_ = 0b00000000000000000000000000000100;
    static const uint32_t s_ground_collisionGroupMask_ = 0b00000000000000000000000000001000;

    static constexpr float s_waterHeight = -8.0f;
};
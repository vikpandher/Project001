// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-06-29

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
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void CreateMainCameraEntities();
    void CreateUiCameraEntity();
    void CreateUiTextEntity();
    void CreateUiPauseTextEntity();
    void CreateCursorEntity();
    void CreateStageEntity();
    void CreateStageLightEntity();
    void CreatePenguinEntity(unsigned int& entityId, PlayerInfo& playerInfo, const glm::vec2& position, float rotation);
    void CreateSharkEntity(unsigned int& entityId, const glm::vec2& position, float rotation);
    void CreateSnowballEntity(unsigned int& entityId, const glm::vec2& position, const glm::vec2& velocity, float radius);

    void UpdateCursorPositionUsingWindowCoordinates(unsigned int entityId, float xPosition, float yPosition);

    void UpdateMainCameraEntity(float timestep_s);
    void UpdateUiTextEntity();
    void UpdateUiPauseTextEntity();
    void UpdateCursorEntity(float timestep_s);
    void UpdateStageCollisionBodyQuadTreeMesh();
    void UpdatePenguinEntity(unsigned int& entityId, PlayerInfo& playerInfo, float timestep_s);
    void UpdateSharkEntity(unsigned int& entityId, float timestep_s);
    void UpdateSnowballEntities(float timestep_s);
    void UpdateWorld(float timestep_s);

    void AnimateCursorEntity(float timestep_s);
    void AnimatePenguinEntities(float timestep_s);
    void AnimateSharkEntities(float timestep_s);
    void AnimateSnowballEntities(float timestep_s);

    void SyncCursorRenderedModels();
    void SyncPenguinRenderedModels();
    void SyncSharkRenderedModels();
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

    unsigned int cursor_entityId_ = static_cast<unsigned int>(-1);

    unsigned int stage_entityId_ = static_cast<unsigned int>(-1);
    unsigned int stageLight_entityId_ = static_cast<unsigned int>(-1);

    unsigned int stageShark_entityId_ = static_cast<unsigned int>(-1);

    unsigned int player_entityIds_[SharedApplicationData::s_player_count] = {
        static_cast<unsigned int>(-1),
        static_cast<unsigned int>(-1),
        static_cast<unsigned int>(-1),
        static_cast<unsigned int>(-1)
    };

    // -------------------------------------------------------------------------

    glm::vec3 mainCamera_lookAtPoint_;

    static constexpr float s_mainCamera_initialDistanceAway_ = 600.0f;
    static constexpr float s_mainCamera_minimumDistanceAway_ = 600.0f;
    static constexpr float s_mainCamera_playerToEdgeSpacing_ = 128.0f;
    float mainCamera_distanceAway_ = s_mainCamera_initialDistanceAway_;
    bool mainCamera_lockedToPlayers_ = true;
    bool debugCamera_turnedOn_ = false;

    bool paused_ = false;

    std::mt19937 randomNumberEngine_;

    static const uint32_t s_player_collisionGroupMasks_[SharedApplicationData::s_player_count];

    static constexpr unsigned int s_ui_CollisionShapeTag_ = 1;
    static constexpr unsigned int s_player_collisionShapeTag_ = 2;
    static constexpr unsigned int s_grab_collisionShapeTag_ = 3;
    static constexpr unsigned int s_grabAttractor_collisionShapeTag_ = 4;
    static constexpr unsigned int s_aimRay_collisionShapeTag_ = 5;
    static constexpr unsigned int s_snowball_collisionShapeTag_ = 6;
    static constexpr unsigned int s_ground_collisionShapeTag_ = 7;
    static constexpr unsigned int s_cursorPosition_collisionShapeTag_ = 9;
    static constexpr unsigned int s_cursorPress_collisionShapeTag_ = 10;
    static constexpr unsigned int s_cursorRelease_collisionShapeTag_ = 11;

    static constexpr float s_waterHeight = -8.0f;

    static constexpr bool s_cursorEnabled = true;
};
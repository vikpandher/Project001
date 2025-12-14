// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#pragma once

#include "Scene.h"

#include "SharedApplicationData.h"

#include "glm/glm.hpp"

#include <random>
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

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessScrollEvent(Project001::ScrollEvent& scrollEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void CreateMainCameraEntities();
    void CreateUiCameraEntity();
    void CreateUiTextEntity();
    void CreateUiPauseTextEntity();
    void CreateUiMiniMapEntity();
    void CreateCursorEntity();

    void CreateBaseEntity(unsigned int& entityId, const glm::vec2& position);
    void CreateGroundEntity();
    void CreateHouseEntity(unsigned int& entityId, const glm::vec2& position);
    void CreatePersonEntity(unsigned int& entityId, const glm::vec2& position);
    void CreatePlayerEntity(const glm::vec2& position);
    void CreateMonsterEntity(unsigned int& entityId, const glm::vec2& position);

    void UpdateMainCameraEntity(float timestep_s);
    void UpdateUiTextEntity();
    void UpdateUiPauseTextEntity();
    void UpdateUiMiniMapEntity();
    void UpdateCursorPosition(float xPosition, float yPosition);

    void UpdateGroundCollisionBodyQuadTreeMesh();
    void UpdateHouseEntities(float timestep_s);
    void UpdatePersonEntities(float timestep_s);
    void UpdatePlayerEntity(float timestep_s);
    void UpdateMonsterEntities(float timestep_s);
    void UpdateWorld(float timestep_s);

    void AnimatePersonEntities(float timestep_s);
    void AnimatePlayerEntity(float timestep_s);
    void AnimateMonsterEntities(float timestep_s);

    void SyncCursorRenderedModel();
    void SyncBaseRenderedModels();
    void SyncHouseRenderedModels();
    void SyncPersonRenderedModels();
    void SyncPlayerRenderedModels();
    void SyncMonsterRenderedModels();

    // -------------------------------------------------------------------------

    SharedApplicationData* sharedDataPtr_ = nullptr;

    // Entity Ids --------------------------------------------------------------

    unsigned int mainCameraLight1_EntityId_ = static_cast<unsigned int>(-1);
    unsigned int mainCameraLight2_EntityId_ = static_cast<unsigned int>(-1);
    unsigned int mainCameraDark1_EntityId_ = static_cast<unsigned int>(-1);
    unsigned int mainCameraDark2_EntityId_ = static_cast<unsigned int>(-1);
    unsigned int mainCameraDebug_EntityId_ = static_cast<unsigned int>(-1);
    static const uint32_t s_mainCameraLight1_Mask_ = 0b00000000000000000000000000000001;
    static const uint32_t s_mainCameraLight2_Mask_ = 0b00000000000000000000000000000010;
    static const uint32_t s_mainCameraDark1_Mask_ = 0b00000000000000000000000000000100;
    static const uint32_t s_mainCameraDark2_Mask_ = 0b00000000000000000000000000001000;
    static const uint32_t s_mainCameraDebug_Mask_ = 0b00000000000000000000000000010000;
    static const uint32_t s_mainCameraGroup_Mask_ = s_mainCameraLight1_Mask_ | s_mainCameraLight2_Mask_ | s_mainCameraDark1_Mask_ | s_mainCameraDark2_Mask_ | s_mainCameraDebug_Mask_;

    unsigned int uiCamera_EntityId_ = static_cast<unsigned int>(-1);
    static const uint32_t s_uiCamera_Mask_ = 0b10000000000000000000000000000000;

    unsigned int uiText_EntityId_ = static_cast<unsigned int>(-1);
    unsigned int uiPauseText_EntityId_ = static_cast<unsigned int>(-1);

    std::vector<unsigned int> uiMiniMaphouse_RenderedMeshIndies;
    unsigned int uiMiniMapPlayer_RenderedMeshIndex_ = static_cast<unsigned int>(-1);
    unsigned int uiMiniMap_EntityId_ = static_cast<unsigned int>(-1);

    unsigned int cursor_EntityId_ = static_cast<unsigned int>(-1);
    static const unsigned int s_cursorPosition_RenderedMeshIndex_ = 0;
    static const unsigned int s_cursorPress_RenderedMeshIndex_ = 1;
    static const unsigned int s_cursorRelease_RenderedMeshIndex_ = 2;
    static const unsigned int s_cursorPosition_CollisionPointIndex_ = 0;
    static const unsigned int s_cursorPress_CollisionPointIndex_ = 1;
    static const unsigned int s_cursorRelease_CollisionPointIndex_ = 2;
    static const unsigned int s_cursorPosition_CollisionShapeId_ = 100;
    static const unsigned int s_cursorPress_CollisionShapeId_ = 101;
    static const unsigned int s_cursorRelease_CollisionShapeId_ = 102;

    unsigned int base_EntityId_ = static_cast<unsigned int>(-1);

    std::vector<unsigned int> house_EntityIds_;

    unsigned int ground_EntityId_ = static_cast<unsigned int>(-1);

    std::vector<unsigned int> person_EntityIds_;
    static const unsigned int s_personLit_RenderedMeshIndex_ = 0;
    static const unsigned int s_personDark_RenderedMeshIndex_ = 1;

    unsigned int player_EntityId_ = static_cast<unsigned int>(-1);
    static const unsigned int s_playerLit_RenderedMeshIndex_ = 0;
    static const unsigned int s_playerDark_RenderedMeshIndex_ = 1;

    std::vector<unsigned int> monster_EntityIds_;
    static const unsigned int s_monsterLit_RenderedMeshIndex_ = 0;
    static const unsigned int s_monsterDark_RenderedMeshIndex_ = 1;

    // -------------------------------------------------------------------------

    glm::vec3 mainCamera_LookAtPoint_;
    float mainCamera_DistanceAway_ = 0.0f;
    bool mainCamera_Locked_ = true;

    glm::vec2 miniMapLocation_;

    bool paused_ = false;

    bool pause_joystickButtonPressRested_ = false;

    std::mt19937 randomNumberEngine_;

    static const unsigned int s_base_collisionShapeTag_ = 1;
    static const unsigned int s_door_collisionShapeTag_ = 2;
    static const unsigned int s_light_collisionShapeTag_ = 3;
    static const unsigned int s_player_collisionShapeTag_ = 4;

    static const uint32_t s_wall_collisionGroupMask_ = 0b00000000000000000000000000000010;
    static const uint32_t s_actor_collisionGroupMask_ = 0b00000000000000000000000000000100;
    static const uint32_t s_sensor_collisionGroupMask_ = 0b00000000000000000000000000001000;
};
// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-10

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
    void CreateCursorEntity();

    void CreateBaseEntity(unsigned int& entityId, const glm::vec2& position);
    void CreateGroundEntity();
    void CreateHouseEntity(unsigned int& entityId, const glm::vec2& position);
    void CreatePersonEntity(unsigned int& entityId, const glm::vec2& position);
    void CreatePlayerEntity(const glm::vec2& position);
    void CreateMonsterEntity(unsigned int& entityId, const glm::vec2& position);

    void UpdateMainCameraEntity(float timestep_s);
    void UpdateUiTextEntity();
    void UpdateCursorPosition(float xPosition, float yPosition);

    void UpdateGroundCollisionBodyQuadTreeMesh();
    void UpdateHouseEntities(float timestep_s);
    void UpdatePersonEntities(float timestep_s);
    void UpdatePlayerEntity(float timestep_s);
    void UpdateMonsterEntities(float timestep_s);
    void UpdateWorld(float timestep_s);

    void SyncCursorRenderedModel();
    void SyncBaseRenderedModels();
    void SyncHouseRenderedModels();
    void SyncPersonRenderedModels();
    void SyncPlayerRenderedModels();
    void SyncMonsterRenderedModels();

    // -------------------------------------------------------------------------

    SharedApplicationData* sharedDataPtr_;

    // Entity Ids --------------------------------------------------------------

    unsigned int mainCameraLight1_EntityId_;
    unsigned int mainCameraLight2_EntityId_;
    unsigned int mainCameraDark1_EntityId_;
    unsigned int mainCameraDark2_EntityId_;
    unsigned int mainCameraDebug_EntityId_;
    static const uint32_t s_mainCameraLight1_Mask_ = 0b00000000000000000000000000000001;
    static const uint32_t s_mainCameraLight2_Mask_ = 0b00000000000000000000000000000010;
    static const uint32_t s_mainCameraDark1_Mask_ = 0b00000000000000000000000000000100;
    static const uint32_t s_mainCameraDark2_Mask_ = 0b00000000000000000000000000001000;
    static const uint32_t s_mainCameraDebug_Mask_ = 0b00000000000000000000000000010000;
    static const uint32_t s_mainCameraGroup_Mask_ = s_mainCameraLight1_Mask_ | s_mainCameraLight2_Mask_ | s_mainCameraDark1_Mask_ | s_mainCameraDark2_Mask_ | s_mainCameraDebug_Mask_;

    unsigned int uiCamera_EntityId_;
    static const uint32_t s_uiCamera_Mask_ = 0b10000000000000000000000000000000;

    unsigned int uiText_EntityId_;

    unsigned int cursor_EntityId_;
    unsigned int cursorPositionRenderedMeshIndex_;
    unsigned int cursorPressRenderedMeshIndex_;
    unsigned int cursorReleaseRenderedMeshIndex_;
    unsigned int cursorPositionCollisionPointIndex_;
    unsigned int cursorPressCollisionPointIndex_;
    unsigned int cursorReleaseCollisionPointIndex_;
    static const unsigned int s_cursorPositionCollisionShapeId_ = 100;
    static const unsigned int s_cursorPressCollisionShapeId_ = 101;
    static const unsigned int s_cursorReleaseCollisionShapeId_ = 102;

    unsigned int base_EntityId_;

    std::vector<unsigned int> house_EntityIds_;

    unsigned int ground_EntityId_;

    std::vector<unsigned int> person_EntityIds_;

    unsigned int player_EntityId_;

    std::vector<unsigned int> monster_EntityIds_;

    // -------------------------------------------------------------------------

    glm::vec3 mainCamera_LookAtPoint_;
    float mainCamera_DistanceAway_;
    bool mainCamera_Locked_;

    bool paused_;

    std::mt19937 randomNumberEngine_;

    static const unsigned int s_base_collisionShapeTag_ = 1;
    static const unsigned int s_door_collisionShapeTag_ = 2;
    static const unsigned int s_light_collisionShapeTag_ = 3;
    static const unsigned int s_player_collisionShapeTag_ = 4;
    static const unsigned int s_monster_collisionShapeTag_ = 5;
};
// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"

#include <random>



class TestScene103 : public Project001::Scene
{
public:
    TestScene103(Project001::Application* applicationPtr);
    ~TestScene103();

    TestScene103(TestScene103& other) = delete;
    void operator=(const TestScene103&) = delete;

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

    void InitializeInstructionScene();

    void LoadCursorResources();
    void LoadPixelFontResources();
    void LoadFogResources();
    void LoadGroundResources();
    void LoadGroundGridResources();
    void LoadPlayerResources();
    void LoadPlayerLightResources();

    void LoadTestPersonResources();
    void LoadTestMonsterResources();
    void LoadTestMonsterVisionResources();
    void LoadTestLampResources();
    void LoadTestLampLightResources();
    void LoadTestHouseResources();
    void LoadTestHouseLightResources();

    void CreateMainCameraEntities();
    void CreateUiCameraEntity();
    void CreateCursorEntity();
    void CreateFogEntity();
    void CreateGroundEntity();
    void CreatePlayerEntity();
    void CreatePlayerLightEntity();

    void CreateTestPersonEntity(unsigned int& entityId, glm::vec2 position);
    void CreateTestMonsterEntity();
    void CreateTestMonsterVisionEntity();
    void CreateTestLampEntity();
    void CreateTestLampLightEntity();
    void CreateTestHouseEntity();
    void CreateTestHouseLightEntity();

    void UpdateMainCameraEntityPosition(float timestep_s);
    void UpdatePlayerEntityVelocity();

    void UpdateCursorPosition(float xPosition, float yPosition);

    void SyncCursorRenderedModel();
    void SyncPlayerRenderedModel();
    void SyncPlayerLightRenderedModel();

    void SyncTestPersonRenderedModel(unsigned int& entityId);
    void SyncTestMonsterRenderedModel();
    void SyncTestMonsterVisionRenderedModel();
    void SyncTestLampRenderedModel();
    void SyncTestLampLightRenderedModel();
    void SyncTestHouseRenderedModel();
    void SyncTestHouseLightRenderedModel();

    void AnimatePlayerRenderedModel(float timestep_s);

    void UpdatePersonLogic(float timestep_s);

    void UpdateCollisionBodyQuadTreeMesh();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Resources ---------------------------------------------------------------

    Project001::MeshData* collisionBodyQuadTreeMeshDataPtr_;

    Project001::MeshData* cursorCircle_MeshDataPtr_;

    const Project001::FontData* pixelFont_FontDataPtr_;
    const Project001::TextureData* pixelFont_TextureDataPtr_;
    unsigned int pixelFont_TextureId_;

    Project001::MeshData* fog_MeshDataPtr_;

    Project001::MeshData* groundDark_MeshDataPtr_;
    Project001::TextureData* groundDark_TextureDataPtr_;
    unsigned int groundDark_TextureId_;
    Project001::MeshData* groundLit_MeshDataPtr_;
    Project001::TextureData* groundLit_TextureDataPtr_;
    unsigned int groundLit_TextureId_;
    Project001::MeshData* groundGrid_MeshDataPtr_;
    unsigned int groundGrid_MeshId_;
    Project001::MeshData* groundGridLabels_MeshDataPtr_;
    unsigned int groundGridLabels_MeshId_;
    Project001::MeshData* groundCollision_MeshDataPtr_;

    Project001::MeshData* playerDark_MeshDataPtr_;
    Project001::TextureData* playerDark_TextureDataPtr_;
    unsigned int playerDark_TextureId_;
    Project001::MeshData* playerLit_MeshDataPtr_;
    Project001::TextureData* playerLit_TextureDataPtr_;
    unsigned int playerLit_TextureId_;
    Project001::MeshData* playerCollision_MeshDataPtr_;

    Project001::MeshData* playerLightBottom_MeshDataPtr_;
    Project001::MeshData* playerLightTop_MeshDataPtr_;
    Project001::MeshData* playerLightStrong_MeshDataPtr_;
    Project001::MeshData* playerLightCollision_MeshDataPtr_;

    Project001::MeshData* testPersonLit_MeshDataPtr_;
    Project001::TextureData* testPersonLit_TextureDataPtr_;
    unsigned int testPersonLit_TextureId_;
    Project001::MeshData* testPersonDark_MeshDataPtr_;
    Project001::TextureData* testPersonDark_TextureDataPtr_;
    unsigned int testPersonDark_TextureId_;
    Project001::MeshData* testPersonCollision_MeshDataPtr_;

    Project001::MeshData* testMonsterLit_MeshDataPtr_;
    Project001::TextureData* testMonsterLit_TextureDataPtr_;
    unsigned int testMonsterLit_TextureId_;
    Project001::MeshData* testMonsterDark_MeshDataPtr_;
    Project001::TextureData* testMonsterDark_TextureDataPtr_;
    unsigned int testMonsterDark_TextureId_;
    Project001::MeshData* testMonsterCollision_MeshDataPtr_;

    Project001::MeshData* testMonsterVisionCollision_MeshDataPtr_;

    Project001::MeshData* testLampLit_MeshDataPtr_;
    Project001::TextureData* testLampLit_TextureDataPtr_;
    unsigned int testLampLit_TextureId_;
    Project001::MeshData* testLampDark_MeshDataPtr_;
    Project001::TextureData* testLampDark_TextureDataPtr_;
    unsigned int testLampDark_TextureId_;
    Project001::MeshData* testLampCollision_MeshDataPtr_;

    Project001::MeshData* testLampLightBottom_MeshDataPtr_;
    Project001::MeshData* testLampLightTop_MeshDataPtr_;
    Project001::MeshData* testLampLightCollision_MeshDataPtr_;

    Project001::MeshData* testHouseLit_MeshDataPtr_;
    Project001::TextureData* testHouseLit_TextureDataPtr_;
    unsigned int testHouseLit_TextureId_;
    Project001::MeshData* testHouseDark_MeshDataPtr_;
    Project001::TextureData* testHouseDark_TextureDataPtr_;
    unsigned int testHouseDark_TextureId_;
    Project001::MeshData* testHouseCollision_MeshDataPtr_;

    Project001::MeshData* testHouseText_MeshDataPtr_;

    Project001::MeshData* testHouseLightBottom_MeshDataPtr_;
    Project001::MeshData* testHouseLightTop_MeshDataPtr_;
    Project001::MeshData* testHouseLightCollision_MeshDataPtr_;
    Project001::MeshData* testHouseDoorCollision_MeshDataPtr_;

    // Entities ----------------------------------------------------------------

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
    glm::vec3 mainCamera_LookAtPoint_;
    float mainCamera_DistanceAway_;
    bool mainCamera_Locked_;

    static const uint32_t s_uiCamera_Mask_ = 0b10000000000000000000000000000000;
    unsigned int uiCamera_EntityId_;

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

    unsigned int fog_EntityId_;

    unsigned int ground_EntityId_;

    unsigned int player_EntityId_;

    unsigned int playerLight_EntityId_;

    static const size_t testPeople_Size_ = 500;
    unsigned int testPeople_EntityIds_[testPeople_Size_];

    unsigned int testPerson_EntityId_;

    unsigned int testMonster_EntityId_;

    unsigned int testMonsterVision_EntityId_;

    unsigned int testLamp_EntityId_;

    unsigned int testLampLight_EntityId_;

    unsigned int testHouse_EntityId_;

    unsigned int testHouseLight_EntityId_;

    bool paused_;

    std::mt19937 randomNumberEngine_;
};
// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-20

#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"



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
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void InitializeInstructionScene();

    void LoadCursorResources();
    void LoadPixelFontResources();
    void LoadGroundResources();
    void LoadGroundGridResources();
    void LoadPlayerResources();
    void LoadLightResources();

    void CreateMainCameraEntities();
    void CreateUiCameraEntity();
    void CreateCursorEntity();
    void CreateGroundEntity();
    void CreatePlayerEntity();
    void CreateLightEntities();

    void UpdateMainCameraEntityPosition(float timestep_s);

    void UpdateCursorPosition(float xPosition, float yPosition);

    void SyncCursorRenderedModel();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Resources ---------------------------------------------------------------

    Project001::MeshData* cursorCircle_MeshDataPtr_;

    const Project001::FontData* pixelFont_FontDataPtr_;
    const Project001::TextureData* pixelFont_TextureDataPtr_;
    unsigned int pixelFont_TextureId_;

    Project001::MeshData* groundGrid_MeshDataPtr_;
    unsigned int groundGrid_MeshId_;
    Project001::MeshData* groundGridLabels_MeshDataPtr_;
    unsigned int groundGridLabels_MeshId_;

    Project001::MeshData* groundDark_MeshDataPtr_;
    Project001::TextureData* groundDark_TextureDataPtr_;
    unsigned int groundDark_TextureId_;
    Project001::MeshData* groundLight_MeshDataPtr_;
    Project001::TextureData* groundLight_TextureDataPtr_;
    unsigned int groundLight_TextureId_;

    Project001::MeshData* playerDark_MeshDataPtr_;
    Project001::TextureData* playerDark_TextureDataPtr_;
    unsigned int playerDark_TextureId_;
    Project001::MeshData* playerLight_MeshDataPtr_;
    Project001::TextureData* playerLight_TextureDataPtr_;
    unsigned int playerLight_TextureId_;

    Project001::MeshData* light01_MeshDataPtr_;

    // Entities ----------------------------------------------------------------

    unsigned int mainCameraDark_EntityId_;
    unsigned int mainCameraLight_EntityId_;
    static const uint32_t s_mainCameraDark_Mask_ = 0b00000000000000000000000000000001;
    static const uint32_t s_mainCameraLight_Mask_ = 0b00000000000000000000000000000010;
    static const uint32_t s_mainCameraDebug_Mask_ = 0b00000000000000000000000000000100;
    static const uint32_t s_mainCameraGroup_Mask_ = s_mainCameraDark_Mask_ | s_mainCameraLight_Mask_ | s_mainCameraDebug_Mask_;
    glm::vec3 mainCamera_LookAtPoint_;
    float mainCamera_DistanceAway_;

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

    unsigned int ground_EntityId_;

    unsigned int player_EntityId_;

    unsigned int light01_EntityId_;
};
// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-18

#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"

#include <vector>



struct TestApplicationData;

namespace Project001
{
    struct SoundData;
}

class TestScene102 : public Project001::Scene
{
public:
    TestScene102(Project001::Application* applicationPtr);
    ~TestScene102();

    TestScene102(TestScene102& other) = delete;
    void operator=(const TestScene102&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void LoadFontData();
    void LoadMeshData();
    void LoadTextureData();

    void CreateCameraEntities();
    void CreateCursorEntity();
    void CreateFloorEntity();
    void CreatePlayerEntity();

    void UpdateMainCameraEntityPosition(float timestep_s);
    void UpdatePlayerEntityVelocity(float timestep_s);
    void UpdatePreviousWorldCursorPosition(float xPosition, float yPosition);

    void CapVelocities();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Font Data ---------------------------------------------------------------

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    const float pixelFont_pixelSize_ = 2.0f;
    const Project001::FontData* pixelFont_FontDataPtr_;
    const Project001::TextureData* pixelFont_TextureDataPtr_;
    unsigned int pixelFont_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* circle_MeshDataPtr_;
    Project001::MeshData* floorGrid_MeshDataPtr_;
    unsigned int floorGrid_MeshId_;
    float floorGrid_MaxBoundingRadius_;
    Project001::MeshData* floorGridLabels_MeshDataPtr_;
    unsigned int floorGridLabels_MeshId_;
    float floorGridLabels_MaxBoundingRadius_;
    Project001::MeshData* mainCameraNearFrustum_MeshDataPtr_;
    Project001::MeshData* mainCameraFarFrustum_MeshDataPtr_;
    Project001::MeshData* ship_MeshDataPtr_;
    Project001::MeshData* shipBeamSight_MeshDataPtr_;
    Project001::MeshData* shipCollisionBody_MeshDataPtr_;

    // Texture Data ------------------------------------------------------------

    unsigned int border96x64_TextureId_;
    unsigned int numbers16x4_TextureId_;

    // Entities ----------------------------------------------------------------

    static const uint32_t s_mainCameraMask_ = 0b00000000000000000000000000000001;
    unsigned int mainCamera_EntityId_;
    static const uint32_t s_uiCameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCamera_EntityId_;

    unsigned int cursor_EntityId_;
    unsigned int cursorPositionRenderedMeshIndex_;
    unsigned int cursorPressRenderedMeshIndex_;
    unsigned int cursorReleaseRenderedMeshIndex_;
    static const unsigned int s_cursorPositionCollisionShapeId_ = 100;
    static const unsigned int s_cursorPressCollisionShapeId_ = 101;
    static const unsigned int s_cursorReleaseCollisionShapeId_ = 102;
    unsigned int cursorPositionCollisionPointIndex_;
    unsigned int cursorPressCollisionPointIndex_;
    unsigned int cursorReleaseCollisionPointIndex_;

    unsigned int floor_EntityId_;

    unsigned int player_EntityId_;

    // -------------------------------------------------------------------------

    float cameraDistanceFromPlayer_;
    float maxCollisionBodyVelocity_;
    size_t physicsStepsPerUpdate_;

    static const uint32_t s_mainCollisionGroupMask_ = 0b00000000000000000000000000000010;

    static const Project001::KeyCode s_keyCode_toggleInstructions_ = Project001::KeyCode::KEY_CODE_TAB;
};
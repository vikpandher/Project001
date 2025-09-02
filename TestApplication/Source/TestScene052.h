// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-01

#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"

#include <vector>



struct TestApplicationData;

namespace Project001
{
    struct SoundData;
}

class TestScene052 : public Project001::Scene
{
public:
    TestScene052(Project001::Application* applicationPtr);
    ~TestScene052();

    TestScene052(TestScene052& other) = delete;
    void operator=(const TestScene052&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateMainCameraEntityPositionAndRoll(unsigned long long timestep_ns);
    void UpdatePreviousWorldCursorPosition(float xPosition, float yPosition);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Font Data ---------------------------------------------------------------

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* circleMeshDataPtr_;
    Project001::MeshData* floorBackgroundMeshDataPtr_;

    // Entity Ids --------------------------------------------------------------

    static const uint32_t s_mainCameraMask_ = 0b00000000000000000000000000000001;
    unsigned int mainCameraEntityId_;
    static const uint32_t s_uiCameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCameraEntityId_;

    unsigned int cursorEntityId_;
    unsigned int cursorPositionRenderedMeshIndex_;
    unsigned int cursorPressRenderedMeshIndex_;
    unsigned int cursorReleaseRenderedMeshIndex_;
    static const unsigned int s_cursorPositionCollisionShapeId_ = 100;
    static const unsigned int s_cursorPressCollisionShapeId_ = 101;
    static const unsigned int s_cursorReleaseCollisionShapeId_ = 102;
    unsigned int cursorPositionCollisionPointIndex_;
    unsigned int cursorPressCollisionPointIndex_;
    unsigned int cursorReleaseCollisionPointIndex_;

    unsigned int floorEntityId_;
    unsigned int floorBackgroundMeshIndex_;

    // -------------------------------------------------------------------------

    glm::vec2 previousWorldCursorPosition_;
    glm::vec2 previousWorldCursorPress_;
    glm::vec2 previousWorldCursorRelease_;

    static const Project001::KeyCode s_keyCode_toggleInstructions_ = Project001::KeyCode::KEY_CODE_TAB;
};
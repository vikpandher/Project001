// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-12

#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"

#include <vector>



class TestScene001 : public Project001::Scene
{
public:
    TestScene001(Project001::Application* applicationPtr);
    ~TestScene001();

    TestScene001(TestScene001& other) = delete;
    void operator=(const TestScene001&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdateCursorPosition(float xPosition, float yPosition);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // FontUtils Data ---------------------------------------------------------------

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* rectangleMeshDataPtr_;
    unsigned int rectangularMeshId_;

    Project001::MeshData* selectorMeshDataPtr_;
    unsigned int selectorMeshId_;

    Project001::MeshData* cursorCircleMeshDataPtr_;

    const float buttonFontPixelSize_ = 0.0048f;
    std::vector<Project001::MeshData*> buttonTextMeshDataPtrs_;

    // Entity Ids --------------------------------------------------------------

    static const uint32_t s_mainCameraMask_ = 0b00000000000000000000000000000001;
    unsigned int mainCameraEntityId_;
    static const uint32_t s_uiCameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCameraEntityId_;

    unsigned int cursorEntityId_;
    size_t cursorPositionRenderedMeshIndex_;
    size_t cursorPressRenderedMeshIndex_;
    size_t cursorReleaseRenderedMeshIndex_;
    size_t cursorPositionCollisionPointIndex_;
    size_t cursorPressCollisionPointIndex_;
    size_t cursorReleaseCollisionPointIndex_;
    static const unsigned int s_cursorPositionCollisionShapeId_ = 100;
    static const unsigned int s_cursorPressCollisionShapeId_ = 101;
    static const unsigned int s_cursorReleaseCollisionShapeId_ = 102;

    std::vector<unsigned int> buttonEntityIds_;
    static const glm::vec4 s_buttonColor_;
    static const glm::vec4 s_buttonColor2_;

    unsigned int selectorEntityId_;

    // -------------------------------------------------------------------------

    unsigned int selectedEntityId_;

    static const Project001::KeyCode s_keyCode_toggleInstructions_ = Project001::KeyCode::KEY_CODE_TAB;
    static const Project001::KeyCode s_keyCode_moveUp_ = Project001::KeyCode::KEY_CODE_W;
    static const Project001::KeyCode s_keyCode_moveLeft_ = Project001::KeyCode::KEY_CODE_A;
    static const Project001::KeyCode s_keyCode_moveDown_ = Project001::KeyCode::KEY_CODE_S;
    static const Project001::KeyCode s_keyCode_moveRight_ = Project001::KeyCode::KEY_CODE_D;
    static const Project001::KeyCode s_keyCode_select_ = Project001::KeyCode::KEY_CODE_SPACE;
};
#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"

#include <vector>

namespace Project001
{
    struct SoundData;
}

class TestScene051 : public Project001::Scene
{
public:
    TestScene051(Project001::Application* applicationPtr);
    ~TestScene051();

    TestScene051(TestScene051& other) = delete;
    void operator=(const TestScene051&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorPositionEvent);
    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void UpdatePreviousWorldCursorPosition(float xPosition, float yPosition);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    Project001::Window* windowPtr_;

    Project001::Renderer* rendererPtr_;

    Project001::SoundPlayer* soundPlayerPtr_;

    Project001::ComponentStores* componentStoresPtr_;

    // SoundData ---------------------------------------------------------------

    Project001::SoundData* sound01_SoundDataPtr_;
    unsigned int sound01_SoundBufferId_;
    std::vector<unsigned int> buttonSoundSourceIds_;

    // Font Data ---------------------------------------------------------------

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* rectangleMeshDataPtr_;
    unsigned int rectangularMeshId_;

    Project001::MeshData* circleMeshDataPtr_;

    const float buttonFontPixelSize_ = 0.0048f;
    std::vector<Project001::MeshData*> buttonTextMeshDataPtrs_;

    // Entity Ids --------------------------------------------------------------

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

    std::vector<unsigned int> buttonEntityIds_;
    static const glm::vec4 s_buttonColor_;
    static const glm::vec4 s_buttonColor2_;

    // -------------------------------------------------------------------------

    glm::vec2 previousWorldCursorPosition_;
    glm::vec2 previousWorldCursorPress_;
    glm::vec2 previousWorldCursorRelease_;

    static const Project001::KeyCode s_keyCode_toggleInstructions_ = Project001::KeyCode::KEY_CODE_TAB;

private:
};
// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-12

#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"

#include <vector>



class TestScene060 : public Project001::Scene
{
public:
    TestScene060(Project001::Application* applicationPtr);
    ~TestScene060();

    TestScene060(TestScene060& other) = delete;
    void operator=(const TestScene060&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // FontUtils Data ---------------------------------------------------------------

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    // Mesh Data ---------------------------------------------------------------

    Project001::MeshData* backgroundRectangleMeshDataPtr_;
    unsigned int backgroundRectangleMeshId_;

    const float joystickAxisAndButtonText_pixelSize = 0.005f;
    Project001::MeshData* joystickAxisTextMeshDataPtr_;
    Project001::MeshData* joystickButton_01_TextMeshDataPtr_;
    Project001::MeshData* joystickButton_02_TextMeshDataPtr_;

    Project001::MeshData* triangleMeshDataPtr_;
    unsigned int triangleMeshId_;

    Project001::MeshData* circleMeshDataPtr_;
    unsigned int circleMeshId_;

    // Entity Ids --------------------------------------------------------------

    static const uint32_t s_mainCameraMask_ = 0b00000000000000000000000000000001;
    unsigned int mainCameraEntityId_;
    static const uint32_t s_uiCameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCameraEntityId_;

    unsigned int backgroundEntityId_;

    unsigned int joystickAxisTextEntityId_;
    unsigned int joystickButton_01_TextEntityId_;
    unsigned int joystickButton_02_TextEntityId_;

    unsigned int leftStickEntityId_;
    size_t leftStickTriangleMeshIndex_;
    size_t leftStickCircleMeshIndex_;

    unsigned int rightStickEntityId_;
    size_t rightStickTriangleMeshIndex_;
    size_t rightStickCircleMeshIndex_;

    // -------------------------------------------------------------------------

    static const Project001::KeyCode s_keyCode_toggleInstructions_ = Project001::KeyCode::KEY_CODE_TAB;
};
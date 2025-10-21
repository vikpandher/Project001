// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-21

#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"

#include <vector>



class TestScene070 : public Project001::Scene
{
public:
    TestScene070(Project001::Application* applicationPtr);
    ~TestScene070();

    TestScene070(TestScene070& other) = delete;
    void operator=(const TestScene070&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void CreateText01();

    void UpdateMainCameraEntityPosition(float timestep_s);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Font Data ---------------------------------------------------------------

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    // Entity Ids --------------------------------------------------------------

    static const uint32_t s_mainCameraMask_ = 0b00000000000000000000000000000001;
    unsigned int mainCamera_EntityId_;
    static const uint32_t s_uiCameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCamera_EntityId_;

    unsigned int text01_EntityId_;
    unsigned int pixel_TextureId_;
    Project001::MeshData* text01_MeshDataPtr_;
    const float text01_pixelSize = 0.02f;

    // -------------------------------------------------------------------------

    static const Project001::KeyCode s_keyCode_toggleInstructions_ = Project001::KeyCode::KEY_CODE_TAB;
};
#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"

#include <vector>


class TestScene100 : public Project001::Scene
{
public:
    TestScene100(Project001::Application* applicationPtr);
    ~TestScene100();

    TestScene100(TestScene100& other) = delete;
    void operator=(const TestScene100&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void Test001();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    Project001::Window* windowPtr_;
    Project001::Renderer* rendererPtr_;
    Project001::ComponentStores* componentStoresPtr_;

    // Font Data ---------------------------------------------------------------

    Project001::FontData* font01_FontDataPtr_;
    Project001::TextureData* font01_TextureDataPtr_;
    unsigned int font01_TextureId_;

    // Entity Ids --------------------------------------------------------------

    unsigned int mainCameraEntityId_;
    static const uint32_t s_uiCameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCameraEntityId_;

    // -------------------------------------------------------------------------

    static const Project001::KeyCode s_keyCode_toggleInstructions_ = Project001::KeyCode::KEY_CODE_TAB;
};
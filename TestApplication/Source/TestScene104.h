// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-10

#pragma once

#include "TestInstructionScene001.h"

#include "glm/glm.hpp"

#include <vector>



class TestScene104 : public Project001::Scene
{
public:
    TestScene104(Project001::Application* applicationPtr);
    ~TestScene104();

    TestScene104(TestScene104& other) = delete;
    void operator=(const TestScene104&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void InitializeInstructionScene();

    void CreateMainCameraEntity();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

    // Entities ----------------------------------------------------------------

    static const uint32_t s_mainCamera_Mask_ = 0b00000000000000000000000000000001;
    unsigned int mainCamera_EntityId_;
};
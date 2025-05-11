// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene009 : public TestSceneBase001
{
public:
    TestScene009(Project001::Application* applicationPtr);
    ~TestScene009();

    TestScene009(TestScene009& other) = delete;
    void operator=(const TestScene009&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};
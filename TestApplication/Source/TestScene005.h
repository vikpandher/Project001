// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene005 : public TestSceneBase001
{
public:
    TestScene005(Project001::Application* applicationPtr);
    ~TestScene005();

    TestScene005(TestScene005& other) = delete;
    void operator=(const TestScene005&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};
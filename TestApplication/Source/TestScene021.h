// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



class TestScene021 : public TestSceneBase002
{
public:
    TestScene021(Project001::Application* applicationPtr);
    ~TestScene021();

    TestScene021(TestScene021& other) = delete;
    void operator=(const TestScene021&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};
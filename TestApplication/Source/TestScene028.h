// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene028 : public TestSceneBase001
{
public:
    TestScene028(Project001::Application* applicationPtr);
    ~TestScene028();

    TestScene028(TestScene028& other) = delete;
    void operator=(const TestScene028&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

private:
    void Run_UnitTests() const;
};
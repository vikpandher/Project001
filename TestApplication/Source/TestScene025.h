// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



class TestScene025 : public TestSceneBase002
{
public:
    TestScene025(Project001::Application* applicationPtr);
    ~TestScene025();

    TestScene025(TestScene025& other) = delete;
    void operator=(const TestScene025&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void CreateEntitiesForTest01();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};
// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



class TestScene015 : public TestSceneBase002
{
public:
    TestScene015(Project001::Application* applicationPtr);
    ~TestScene015();

    TestScene015(TestScene015& other) = delete;
    void operator=(const TestScene015&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void CreateEntitiesForTest01();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};
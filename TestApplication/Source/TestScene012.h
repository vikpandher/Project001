// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



class TestScene012 : public TestSceneBase002
{
public:
    TestScene012(Project001::Application* applicationPtr);
    ~TestScene012();

    TestScene012(TestScene012& other) = delete;
    void operator=(const TestScene012&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void GenerateCombinationShape(glm::vec3& position);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};
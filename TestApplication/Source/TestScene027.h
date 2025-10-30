// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase002.h"



namespace Project001
{
    struct CollisionBody2DCreationInfo;
}

class TestScene027 : public TestSceneBase002
{
public:
    TestScene027(Project001::Application* applicationPtr);
    ~TestScene027();

    TestScene027(TestScene027& other) = delete;
    void operator=(const TestScene027&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void CreateCollisionBodyEntities();

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;
};